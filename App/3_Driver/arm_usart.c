//********************************************************************************
//arm_usart.c
//********************************************************************************
#include "stm32f4xx.h"
#include "common.h"
#include "RTE_Device.h"
#include "RTE_Components.h"
#include "Driver_USART.h"
#include "gpio.h"
#include "usart.h"
#include "arm_usart.h"
#include "arm_gpio.h"
#include "arm_clock.h"

//********************************************************************************
//Macros
//********************************************************************************

/* History:
 *  Version 1.0
 *    - Initial release
 */
#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)

// USART Driver flags
#define ARM_USART_FLAG_INITIALIZED       (uint16_t)(1U << 0)
#define ARM_USART_FLAG_POWERED           (uint16_t)(1U << 1)
#define ARM_USART_FLAG_CONFIGURED        (uint16_t)(1U << 2)
#define ARM_USART_FLAG_TX_ENABLED        (uint16_t)(1U << 3)
#define ARM_USART_FLAG_RX_ENABLED        (uint16_t)(1U << 4)

//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************

// USART Resources definitions
typedef struct {
    ARM_USART_CAPABILITIES  capabilities;  // Capabilities
    ePeriphTypes   usart_name;
    USART_TypeDef           *p_reg;          // Pointer to USART peripheral registers
    USART_PinCfg_t          *p_pin;          // Pointer to USART pins configuration
//    USART_CLOCKS            clk;           // USART clocks configuration
    IRQn_Type               irq_num;       // USART IRQ Number
//    uint32_t                trig_lvl;      // FIFO Trigger level

//    USART_DMA              *dma_tx;
//    USART_DMA              *dma_rx;
    USART_INFO             *p_info;          // Run-Time Information
} ARM_USART_RESOURCES;

//********************************************************************************
//Variables
//********************************************************************************

// Driver Version
static const ARM_DRIVER_VERSION ARM_USART_Driver_Version =
{ ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

#if (RTE_USART1==1)
static ARM_USART_RESOURCES ARM_USART1_Resources;
static USART_INFO USART1_Info = {0};
#endif//(RTE_USART1==1)



//********************************************************************************
//Prototypes
//********************************************************************************

static ARM_DRIVER_VERSION ARM_USARTx_GetVersion(void);
static ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(ARM_USART_RESOURCES *usart);
static int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t  cb_event,
                                    ARM_USART_RESOURCES *usart);
static int32_t ARM_USART_Uninitialize(ARM_USART_RESOURCES *usart);
static int32_t ARM_USART_PowerControl(ARM_POWER_STATE  state,
                                      ARM_USART_RESOURCES *usart);
static int32_t ARM_USART_Send(const void *data, uint32_t num, ARM_USART_RESOURCES *usart);
static int32_t ARM_USART_Receive(void *data, uint32_t num, ARM_USART_RESOURCES *usart);
static int32_t ARM_USART_Transfer(const void *data_out, void *data_in, uint32_t num, ARM_USART_RESOURCES *usart);
static uint32_t ARM_USART_GetTxCount(ARM_USART_RESOURCES *usart);
static uint32_t ARM_USART_GetRxCount(ARM_USART_RESOURCES *usart);
static int32_t ARM_USART_Control(uint32_t control, uint32_t arg,
                                 ARM_USART_RESOURCES *usart);
static ARM_USART_STATUS ARM_USART_GetStatus(ARM_USART_RESOURCES *usart);
static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control, ARM_USART_RESOURCES *usart);
static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(ARM_USART_RESOURCES *usart);
static void USART_IRQHandler(ARM_USART_RESOURCES *usart);

#if (RTE_USART1==1)
static void ARM_USART1_Resources_Struct_Init(void);
static ARM_USART_CAPABILITIES ARM_USART1_GetCapabilities(void);
static int32_t ARM_USART1_Initialize(ARM_USART_SignalEvent_t cb_event);
static int32_t ARM_USART1_Uninitialize(void);
static int32_t ARM_USART1_PowerControl(ARM_POWER_STATE state);
static int32_t ARM_USART1_Send(const void *data, uint32_t num);
static int32_t ARM_USART1_Receive(void *data, uint32_t num);
static int32_t ARM_USART1_Transfer(const void      *data_out,
                                   void      *data_in,
                                   uint32_t   num);
static uint32_t ARM_USART1_GetTxCount(void);
static uint32_t ARM_USART1_GetRxCount(void);
static int32_t ARM_USART1_Control(uint32_t control, uint32_t arg);;
static ARM_USART_STATUS ARM_USART1_GetStatus(void);
static int32_t ARM_USART1_SetModemControl(ARM_USART_MODEM_CONTROL control);
static ARM_USART_MODEM_STATUS ARM_USART1_GetModemStatus(void);
#endif//(RTE_USART1==1)

//================================================================================
//Private
//================================================================================

static ARM_DRIVER_VERSION ARM_USARTx_GetVersion(void)
{
    return ARM_USART_Driver_Version;
}

static ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(ARM_USART_RESOURCES *usart)
{
    return usart->capabilities;
}

static int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t  cb_event,
                                    ARM_USART_RESOURCES         *usart)
{

    if(usart->p_info->flags & ARM_USART_FLAG_INITIALIZED) {
        // Driver is already initialized
        return ARM_DRIVER_OK;
    }

    // Initialize USART Run-time Resources
    usart->p_info->cb_event = cb_event;

    usart->p_info->rx_status.rx_busy          = 0U;//надо
    usart->p_info->rx_status.rx_overflow      = 0U;//надо
    usart->p_info->rx_status.rx_break         = 0U;
    usart->p_info->rx_status.rx_framing_error = 0U;//надо
    usart->p_info->rx_status.rx_parity_error  = 0U;//надо

    usart->p_info->xfer.send_active           = 0U;
    usart->p_info->xfer.tx_def_val            = 0U;

// Configure CTS pin
    if(usart->capabilities.cts) {
        GPIO_SetCfg(usart-> p_pin[CTS_PIN].GPIOx, usart-> p_pin[CTS_PIN].pin,
                    GPIO_IO_MODE_INPUT, GPIO_IO_TYPE_OPEN_DRAIN , GPIO_IO_HI_Z, GPIO_IO_SPEED_FREQ_LOW,
                    usart-> p_pin[CTS_PIN].alt_func);
        ARM_GPIO_Config();
    }
// Configure RTS pin
    if(usart->capabilities.rts) {
        GPIO_SetCfg(usart-> p_pin[RTS_PIN].GPIOx, usart-> p_pin[RTS_PIN].pin,
                    GPIO_IO_MODE_OUTPUT, GPIO_IO_TYPE_PUSH_PULL, GPIO_IO_HI_Z, GPIO_IO_SPEED_FREQ_LOW,
                    usart-> p_pin[RTS_PIN].alt_func);
        ARM_GPIO_Config();
    }
// DMA Initialize
    //unsupported in Version 1.0
    usart->p_info->flags = ARM_USART_FLAG_INITIALIZED;
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Uninitialize(ARM_USART_RESOURCES *usart)
{
// Reset TX pin configuration
    GPIO_SetCfg(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin, 0U, 0U, 0U, 0U, 0U);
    ARM_GPIO_Config();
// Reset RX pin configuration
    GPIO_SetCfg(usart-> p_pin[RX_PIN].GPIOx, usart-> p_pin[RX_PIN].pin, 0U, 0U, 0U, 0U, 0U);
    ARM_GPIO_Config();
// Unconfigure CTS pin
    if(usart->capabilities.cts) {
        GPIO_SetCfg(usart-> p_pin[CTS_PIN].GPIOx, usart-> p_pin[CTS_PIN].pin, 0U, 0U, 0U, 0U, 0U);
        ARM_GPIO_Config();
    }
// Unconfigure RTS pin
    if(usart->capabilities.rts) {
        GPIO_SetCfg(usart-> p_pin[RTS_PIN].GPIOx, usart-> p_pin[RTS_PIN].pin, 0U, 0U, 0U, 0U, 0U);
        ARM_GPIO_Config();
    }
// DMA Uninitialize function
    //unsupported in Version 1.0

// Reset USART status flags
    usart->p_info->flags = 0UL;
    return ARM_DRIVER_OK;
}

/***********************************************************************************
To stop working with a peripheral the functions PowerControl and Uninitialize need
to be called in this order:
drv->PowerControl (ARM_POWER_OFF);     // Terminate any pending transfers, reset IRQ/DMA, power off peripheral
drv->Uninitialize (...);               // Release I/O pins
************************************************************************************/

static int32_t ARM_USART_PowerControl(ARM_POWER_STATE  state,
                                      ARM_USART_RESOURCES *usart)
{
    switch(state) {
        case ARM_POWER_OFF: {
            // Disable USART IRQ
            NVIC_DisableIRQ(usart->irq_num);

            ARM_RCC_Periph_ClockCmd(usart->usart_name, ENABLE_CMD);
            // Reset USART registers
            ARM_RCC_Periph_ResetCmd(usart->usart_name, ENABLE_CMD);
            // Release reset signal from USART
            ARM_RCC_Periph_ResetCmd(usart->usart_name, DISABLE_CMD);
            // Disable clock to UARTx block
            ARM_RCC_Periph_ClockCmd(usart->usart_name, DISABLE_CMD);
            // Clear pending USART interrupts in NVIC
            NVIC_ClearPendingIRQ(usart->irq_num);

            // Clear driver flag variables
            usart->p_info->rx_status.rx_busy          = 0U;
            usart->p_info->rx_status.rx_overflow      = 0U;
            usart->p_info->rx_status.rx_break         = 0U;
            usart->p_info->rx_status.rx_framing_error = 0U;
            usart->p_info->rx_status.rx_parity_error  = 0U;
            usart->p_info->xfer.send_active           = 0U;

            usart->p_info->flags &= ~ARM_USART_FLAG_POWERED;
            break;
        }
        case ARM_POWER_LOW: {
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        }
        case ARM_POWER_FULL: {
            if((usart->p_info->flags & ARM_USART_FLAG_INITIALIZED) == 0U) {
                return ARM_DRIVER_ERROR;
            }
            if((usart->p_info->flags & ARM_USART_FLAG_POWERED)     != 0U) {
                return ARM_DRIVER_OK;
            }

            // Enable clock to UARTx block
            ARM_RCC_Periph_ClockCmd(usart->usart_name, ENABLE_CMD);
            // Reset USART registers
            ARM_RCC_Periph_ResetCmd(usart->usart_name, ENABLE_CMD);
            // Release reset signal from USART
            ARM_RCC_Periph_ResetCmd(usart->usart_name, DISABLE_CMD);
            // Disable clock to UARTx block
            ARM_RCC_Periph_ClockCmd(usart->usart_name, DISABLE_CMD);

            // Clear driver variables
            usart->p_info->rx_status.rx_busy          = 0U;
            usart->p_info->rx_status.rx_overflow      = 0U;
            usart->p_info->rx_status.rx_break         = 0U;
            usart->p_info->rx_status.rx_framing_error = 0U;
            usart->p_info->rx_status.rx_parity_error  = 0U;

            usart->p_info->xfer.send_active           = 0U;

            usart->p_info->mode                       = 0U;
            usart->p_info->flags                      = 0U;
            usart->p_info->baudrate                   = 0U;//я добавила

            usart->p_info->flags = ARM_USART_FLAG_POWERED | ARM_USART_FLAG_INITIALIZED;

            // Clear and Enable USART IRQ
            NVIC_ClearPendingIRQ(usart->irq_num);
            NVIC_EnableIRQ(usart->irq_num);
            break;
        }
        default: {
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        }
    }
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Send(const void *data, uint32_t num,
                              ARM_USART_RESOURCES *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Receive(void *data, uint32_t num,
                                 ARM_USART_RESOURCES *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Transfer(const void *data_out, void *data_in,
                                  uint32_t num, ARM_USART_RESOURCES *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static uint32_t ARM_USART_GetTxCount(ARM_USART_RESOURCES *usart)
{
    uint32_t cnt;
// to do
    return cnt;
}

static uint32_t ARM_USART_GetRxCount(ARM_USART_RESOURCES *usart)

{
    uint32_t cnt;
// to do
    return cnt;
}

static int32_t ARM_USART_Control(uint32_t control, uint32_t arg,
                                 ARM_USART_RESOURCES *usart)
{
    if((usart->p_info->flags & ARM_USART_FLAG_POWERED) == 0U) {
        // USART not powered
        return ARM_DRIVER_ERROR;
    }
    switch(control & ARM_USART_CONTROL_Msk) {
        // Control TX
        case  ARM_USART_CONTROL_TX: {
            //Check if pin configure available
            if(usart->p_pin == NULL) {
                return ARM_DRIVER_ERROR;
            }
            if(arg) {
                if(usart->p_info->mode != ARM_USART_MODE_SMART_CARD) {
                    // USART TX pin function selected
                    GPIO_SetCfg(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin,
                                GPIO_IO_MODE_INPUT, GPIO_IO_TYPE_OPEN_DRAIN , GPIO_IO_HI_Z, GPIO_IO_SPEED_FREQ_LOW,
                                usart-> p_pin[TX_PIN].alt_func);
                    ARM_GPIO_Config();
                    usart->p_reg->CR1 |= USART_CR1_TE;
                    usart->p_info->flags |= ARM_USART_FLAG_TX_ENABLED;
                }
            } else {
                usart->p_reg->CR1 &= ~USART_CR1_TE;
                if(usart->p_info->mode != ARM_USART_MODE_SMART_CARD) {
                    // SPIO default pin function selected
                    GPIO_SetCfg(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin,
                                0U, 0U, 0U, 0U, 0U);
                    ARM_GPIO_Config();
                }
                usart->p_info->flags &= ~ARM_USART_FLAG_TX_ENABLED;

            }
        }
    }
    //to do
    return ARM_DRIVER_OK;
}

static ARM_USART_STATUS ARM_USART_GetStatus(ARM_USART_RESOURCES *usart)
{
    ARM_USART_STATUS status;
// to do
    return status;
}

static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control, ARM_USART_RESOURCES *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(ARM_USART_RESOURCES *usart)
{
    ARM_USART_MODEM_STATUS modem_status;
//to do
    return modem_status;
}

static void USART_IRQHandler(ARM_USART_RESOURCES *usart)
{
//to do
}



#if (RTE_USART1==1)

static void ARM_USART1_Resources_Struct_Init(void)
{

    ARM_USART_RESOURCES *p_str = &ARM_USART1_Resources;
    ARM_USART1_Resources.capabilities.asynchronous = 1U; // supports UART (Asynchronous) mode

#ifdef USART1_SCLK_PIN_ENABLE
    p_str->capabilities.synchronous_master = 1; // supports Synchronous Master mode
#else
    p_str->capabilities.synchronous_master = 0; // supports Synchronous Master mode
#endif //USART1_SCLK_PIN_ENABLE        

    p_str->capabilities.synchronous_slave = 0;  // supports Synchronous Slave mode
    p_str->capabilities.single_wire = 1;        // supports UART Single-wire mode
    p_str->capabilities.irda = 1;               // supports UART IrDA mode

#ifdef USART1_SCLK_PIN_ENABLE
    p_str->capabilities.smart_card =  1;        // supports UART Smart Card mode
    p_str->capabilities.smart_card_clock =  1;  // Smart Card Clock generator available
#else
    p_str->capabilities.smart_card =  0;        // supports UART Smart Card mode
    p_str->capabilities.smart_card_clock =  0;  // Smart Card Clock generator available
#endif //USART1_SCLK_PIN_ENABLE

#ifdef USART1_RTS_PIN_ENABLE
    p_str->capabilities.flow_control_rts =  1;   // RTS Flow Control available
#else
    p_str->capabilities.flow_control_rts =  0;   // RTS Flow Control available
#endif //USART1_RTS_PIN_ENABLE 

#ifdef USART1_CTS_PIN_ENABLE
    p_str->capabilities.flow_control_cts =  1;   // CTS Flow Control available
#else
    p_str->capabilities.flow_control_cts =  0;   // CTS Flow Control available
#endif //USART1_CTS_PIN_ENABLE               

    p_str->capabilities.event_tx_complete =  1;  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    p_str->capabilities.event_rx_timeout =  0;   // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT

#ifdef USART1_RTS_PIN_ENABLE
    p_str->capabilities.rts =  1;                // RTS Line: 0=not available, 1=available
#else
    p_str->capabilities.rts =  0;                // RTS Line: 0=not available, 1=available
#endif //USART1_RTS_PIN_ENABLE 

#ifdef USART1_CTS_PIN_ENABLE
    p_str->capabilities.cts =  1;                // CTS Line: 0=not available, 1=available
#else
    p_str->capabilities.cts =  0;                // CTS Line: 0=not available, 1=available
#endif //USART1_CTS_PIN_ENABLE       
    p_str->capabilities.dtr =  0;      // DTR Line: 0=not available, 1=available
    p_str->capabilities.dsr =  0;      // DSR Line: 0=not available, 1=available
    p_str->capabilities.dcd =  0;      // DCD Line: 0=not available, 1=available
    p_str->capabilities.ri =  0;      // RI Line: 0=not available, 1=available

#ifdef USART1_CTS_PIN_ENABLE
    p_str->capabilities.event_cts = 1;     // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    p_str->capabilities.event_cts =  0;      // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif //USART1_CTS_PIN_ENABLE 

    p_str->capabilities.event_dsr =  0;      // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    p_str->capabilities.event_dcd =  0;      // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    p_str->capabilities.event_ri =  0;      // Signal RI change event: \ref ARM_USART_EVENT_RI
    p_str->capabilities.reserved =  0;      // Reserved (must be zero)
    p_str->usart_name = USART_1;   //Interface name
    p_str->p_reg = USART1;   // Pointer to USART peripheral registers
    p_str->p_pin = USART_GetPinConfig(USART_1); // Pointer to USART pins configuration
    p_str->irq_num = USART1_IRQn;
    p_str->p_info = &USART1_Info;
}

/*
    void USART1_IRQHandler(void) {
        USART_IRQHandler(&ARM_USART1_Resources);
    }

*/

/*************************************************
Driver capabilities of USART2, USART3, USART6 are completely
similar to USART1 driver capabilities.
***************************************************/

static ARM_USART_CAPABILITIES ARM_USART1_GetCapabilities(void)
{
    return ARM_USART_GetCapabilities(&ARM_USART1_Resources);
}

static int32_t ARM_USART1_Initialize(ARM_USART_SignalEvent_t cb_event)
{
    return ARM_USART_Initialize(cb_event, &ARM_USART1_Resources);
}
static int32_t ARM_USART1_Uninitialize(void)
{
    return ARM_USART_Uninitialize(&ARM_USART1_Resources);
}
static int32_t ARM_USART1_PowerControl(ARM_POWER_STATE state)
{
    return ARM_USART_PowerControl(state, &ARM_USART1_Resources);

}
static int32_t ARM_USART1_Send(const void *data, uint32_t num)
{
    return ARM_USART_Send(data, num, &ARM_USART1_Resources);
}

static int32_t ARM_USART1_Receive(void *data, uint32_t num)
{
    return ARM_USART_Receive(data, num, &ARM_USART1_Resources);
}
static int32_t ARM_USART1_Transfer(const void *data_out,
                                   void      *data_in,
                                   uint32_t   num)
{
    return ARM_USART_Transfer(data_out, data_in, num, &ARM_USART1_Resources);
}
static uint32_t ARM_USART1_GetTxCount(void)
{
    return ARM_USART_GetTxCount(&ARM_USART1_Resources);
}
static uint32_t ARM_USART1_GetRxCount(void)
{
    return ARM_USART_GetRxCount(&ARM_USART1_Resources);
}

static int32_t ARM_USART1_Control(uint32_t control, uint32_t arg)
{
    return ARM_USART_Control(control, arg, &ARM_USART1_Resources);
}

static ARM_USART_STATUS ARM_USART1_GetStatus(void)
{
    return ARM_USART_GetStatus(&ARM_USART1_Resources);
}
static int32_t ARM_USART1_SetModemControl(ARM_USART_MODEM_CONTROL control)
{
    return ARM_USART_SetModemControl(control, &ARM_USART1_Resources);
}
static ARM_USART_MODEM_STATUS ARM_USART1_GetModemStatus(void)
{
    return ARM_USART_GetModemStatus(&ARM_USART1_Resources);
}

void UART1_IRQHandler(void)
{
    USART_IRQHandler(&ARM_USART1_Resources);
};

//********************************************************************************
//Variables (continuation)
//********************************************************************************

static ARM_DRIVER_USART ARM_USART1_Driver = {
    ARM_USARTx_GetVersion,
    ARM_USART1_GetCapabilities,
    ARM_USART1_Initialize,
    ARM_USART1_Uninitialize,
    ARM_USART1_PowerControl,
    ARM_USART1_Send,
    ARM_USART1_Receive,
    ARM_USART1_Transfer,
    ARM_USART1_GetTxCount,
    ARM_USART1_GetRxCount,
    ARM_USART1_Control,
    ARM_USART1_GetStatus,
    ARM_USART1_SetModemControl,
    ARM_USART1_GetModemStatus

};
#endif //(RTE_USART1==1)

//================================================================================
//Public
//================================================================================

void ARM_USART_Init(void)
{
    ARM_DRIVER_USART *p_drv;
#if (RTE_USART1==1)
    p_drv = &ARM_USART1_Driver;
    ARM_USART1_Resources_Struct_Init();
    p_drv->Initialize(&USART1_cb);
    p_drv->PowerControl(ARM_POWER_FULL);
#endif//(RTE_USART1==1)

}
