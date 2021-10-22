//********************************************************************************
//arm_usart.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdbool.h>
#include "common.h"
#include "RTE_Device.h"
#include "RTE_Components.h"
#include "Driver_USART.h"
#include "arm_gpio.h"
#include "arm_usart.h"
#include "arm_clock.h"
#include "gpio.h"
#include "usart.h"


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

// ARM USART Transfer Information (Run-Time)
typedef struct {
    uint32_t                rx_num;        // Total number of data to be received
    uint32_t                tx_num;        // Total number of data to be send
    uint8_t                *p_rx_buf;      // Pointer to in data buffer
    uint8_t                *p_tx_buf;      // Pointer to out data buffer
    uint32_t                rx_cnt;        // Number of data received
    uint32_t                tx_cnt;        // Number of data sent
    uint8_t                 tx_def_val;    // Transmit default value (used in USART_SYNC_MASTER_MODE_RX)
    uint8_t                 rx_dump_val;   // Receive dump value (used in USART_SYNC_MASTER_MODE_TX)
    uint8_t                 sync_mode;     // Synchronous mode
} ARM_USART_TransferInfo_t;

// ARM USART Information (Run-Time)
typedef struct {
    ARM_USART_SignalEvent_t  cb_event;        // Event callback
    uint16_t                 drv_status;      // USART driver flags
    uint8_t                  mode;            // USART mode
    uint32_t                 baudrate;        // Baudrate
    ARM_USART_STATUS         xfer_status;     // USART transfer status
    ARM_USART_TransferInfo_t xfer_info;       // Transfer information
} ARM_USART_Info_t;

// ARM USART Resources definitions
typedef struct {
    ARM_USART_CAPABILITIES capabilities;  // Capabilities
    ePeriphTypes           usart_name;
    USART_TypeDef          *p_reg;        // Pointer to USART peripheral registers
    USART_PinCfg_t         *p_pin;        // Pointer to USART pins configuration
    IRQn_Type              irq_num;       // USART IRQ Number
//    uint32_t                trig_lvl;      // FIFO Trigger level
//    USART_DMA              *p_dma_tx;
//    USART_DMA              *p_dma_rx;
    ARM_USART_Info_t    *p_info;          // Run-Time Information
} ARM_USART_Resources_t;

//********************************************************************************
//Variables
//********************************************************************************

// Driver Version
static const ARM_DRIVER_VERSION ARM_USART_Driver_Version =
{ ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

#if (RTE_USART1 == 1)
static ARM_USART_Resources_t ARM_USART1_Resources;
static ARM_USART_Info_t USART1_Info = {0};
#endif //(RTE_USART1 == 1)

#if (RTE_UART4 == 1)
static ARM_USART_Resources_t ARM_UART4_Resources;
static ARM_USART_Info_t UART4_Info = {0};
#endif //(RTE_UART4 == 1)

//********************************************************************************
//Prototypes
//********************************************************************************

static ARM_DRIVER_VERSION ARM_USARTx_GetVersion(void);
static ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(ARM_USART_Resources_t *usart);
static int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t  cb_event,
                                    ARM_USART_Resources_t *usart);
static int32_t ARM_USART_Uninitialize(ARM_USART_Resources_t *usart);
static int32_t ARM_USART_PowerControl(ARM_POWER_STATE  state,
                                      ARM_USART_Resources_t *usart);
static int32_t ARM_USART_Send(const void *data, uint32_t num, ARM_USART_Resources_t *usart);
static int32_t ARM_USART_Receive(void *data, uint32_t num, ARM_USART_Resources_t *usart);
static int32_t ARM_USART_Transfer(const void *data_out, void *data_in, uint32_t num, ARM_USART_Resources_t *usart);
static uint32_t ARM_USART_GetTxCount(ARM_USART_Resources_t *usart);
static uint32_t ARM_USART_GetRxCount(ARM_USART_Resources_t *usart);
static int32_t ARM_USART_Control(uint32_t control, uint32_t arg,
                                 ARM_USART_Resources_t *usart);
static ARM_USART_STATUS ARM_USART_GetStatus(ARM_USART_Resources_t *usart);
static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control, ARM_USART_Resources_t *usart);
static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(ARM_USART_Resources_t *usart);
static void USART_IRQHandler(ARM_USART_Resources_t *usart);

#if (RTE_USART1 == 1)
static void ARM_USART1_Resources_Struct_Init(void);
static ARM_USART_CAPABILITIES ARM_USART1_GetCapabilities(void);
static int32_t ARM_USART1_Initialize(ARM_USART_SignalEvent_t cb_event);
static int32_t ARM_USART1_Uninitialize(void);
static int32_t ARM_USART1_PowerControl(ARM_POWER_STATE state);
static int32_t ARM_USART1_Send(const void *data, uint32_t num);
static int32_t ARM_USART1_Receive(void *data, uint32_t num);
static int32_t ARM_USART1_Transfer(const void *data_out, void *data_in, uint32_t num);
static uint32_t ARM_USART1_GetTxCount(void);
static uint32_t ARM_USART1_GetRxCount(void);
static int32_t ARM_USART1_Control(uint32_t control, uint32_t arg);;
static ARM_USART_STATUS ARM_USART1_GetStatus(void);
static int32_t ARM_USART1_SetModemControl(ARM_USART_MODEM_CONTROL control);
static ARM_USART_MODEM_STATUS ARM_USART1_GetModemStatus(void);
#endif //(RTE_USART1 == 1)

#if (RTE_UART4 == 1)
static void ARM_USART4_Resources_Struct_Init(void)
#endif //(RTE_UART4 == 1)

//================================================================================
//Private
//================================================================================

static ARM_DRIVER_VERSION ARM_USARTx_GetVersion(void)
{
    return ARM_USART_Driver_Version;
}

static ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(ARM_USART_Resources_t *usart)
{
    return usart->capabilities;
}

static int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t  cb_event,
                                    ARM_USART_Resources_t         *usart)
{

    if(usart->p_info->drv_status & ARM_USART_FLAG_INITIALIZED) {
        // Driver is already initialized
        return ARM_DRIVER_OK;
    }
    // Initialize USART Run-time Resources
    usart->p_info->cb_event = cb_event;


    // Clear driver variables
    usart->p_info->drv_status                 = 0U;
    usart->p_info->mode                       = 0U;
    usart->p_info->baudrate                   = 0U;
    usart->p_info->xfer_status.tx_busy          = 0U;
    usart->p_info->xfer_status.rx_busy          = 0U;
    usart->p_info->xfer_status.tx_underflow     = 0U;
    usart->p_info->xfer_status.rx_overflow      = 0U;
    usart->p_info->xfer_status.rx_break         = 0U;
    usart->p_info->xfer_status.rx_framing_error = 0U;
    usart->p_info->xfer_status.rx_parity_error  = 0U;
    usart->p_info->xfer_info.tx_def_val         = 0U;
    usart->p_info->xfer_info.rx_dump_val        = 0U;
    usart->p_info->xfer_info.sync_mode          = 0U;

// Configure CTS pin
    if(usart->capabilities.cts) {
        GPIO_SetData(usart-> p_pin[CTS_PIN].GPIOx, usart-> p_pin[CTS_PIN].pin,
                     ARM_GPIO_IO_MODE_INPUT, ARM_GPIO_IO_TYPE_OPEN_DRAIN , ARM_GPIO_IO_HI_Z, ARM_GPIO_IO_SPEED_FREQ_LOW,
                     usart-> p_pin[CTS_PIN].alt_func);
    }
// Configure RTS pin
    if(usart->capabilities.rts) {
        GPIO_SetData(usart-> p_pin[RTS_PIN].GPIOx, usart-> p_pin[RTS_PIN].pin,
                     ARM_GPIO_IO_MODE_OUTPUT, ARM_GPIO_IO_TYPE_PUSH_PULL, ARM_GPIO_IO_HI_Z, ARM_GPIO_IO_SPEED_FREQ_LOW,
                     usart-> p_pin[RTS_PIN].alt_func);
    }
// DMA Initialize
    //unsupported in Version 1.0
    usart->p_info->drv_status = ARM_USART_FLAG_INITIALIZED;
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Uninitialize(ARM_USART_Resources_t *usart)
{

// Reset TX pin configuration
    GPIO_SetData(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin, 0U, 0U, 0U, 0U, 0U);
// Reset RX pin configuration
    GPIO_SetData(usart-> p_pin[RX_PIN].GPIOx, usart-> p_pin[RX_PIN].pin, 0U, 0U, 0U, 0U, 0U);
// Unconfigure CTS pin
    if(usart->capabilities.cts) {
        GPIO_SetData(usart-> p_pin[CTS_PIN].GPIOx, usart-> p_pin[CTS_PIN].pin, 0U, 0U, 0U, 0U, 0U);
    }
// Unconfigure RTS pin
    if(usart->capabilities.rts) {
        GPIO_SetData(usart-> p_pin[RTS_PIN].GPIOx, usart-> p_pin[RTS_PIN].pin, 0U, 0U, 0U, 0U, 0U);
    }
// DMA Uninitialize function
    //unsupported in Version 1.0

// Reset USART status flags
    usart->p_info->drv_status = 0UL;
    return ARM_DRIVER_OK;
}

/***********************************************************************************
To stop working with a peripheral the functions PowerControl and Uninitialize need
to be called in this order:
drv->PowerControl (ARM_POWER_OFF);     // Terminate any pending transfers, reset IRQ/DMA, power off peripheral
drv->Uninitialize (...);               // Release I/O pins
************************************************************************************/

static int32_t ARM_USART_PowerControl(ARM_POWER_STATE  state,
                                      ARM_USART_Resources_t *usart)
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
            usart->p_info->drv_status &= ~ARM_USART_FLAG_POWERED;
            break;
        }
        case ARM_POWER_LOW: {
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        }
        case ARM_POWER_FULL: {
            if((usart->p_info->drv_status & ARM_USART_FLAG_INITIALIZED) == 0U) {
                return ARM_DRIVER_ERROR;
            }
            if((usart->p_info->drv_status & ARM_USART_FLAG_POWERED)     != 0U) {
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

            usart->p_info->drv_status |= ARM_USART_FLAG_POWERED;

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
                              ARM_USART_Resources_t *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Receive(void *data, uint32_t num,
                                 ARM_USART_Resources_t *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Transfer(const void *data_out, void *data_in,
                                  uint32_t num, ARM_USART_Resources_t *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static uint32_t ARM_USART_GetTxCount(ARM_USART_Resources_t *usart)
{
    uint32_t cnt;
// to do
    return cnt;
}

static uint32_t ARM_USART_GetRxCount(ARM_USART_Resources_t *usart)

{
    uint32_t cnt;
// to do
    return cnt;
}

static int32_t ARM_USART_Control(uint32_t control, uint32_t arg,
                                 ARM_USART_Resources_t *usart)
{
    if((usart->p_info->drv_status & ARM_USART_FLAG_POWERED) == 0U) {
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
                    GPIO_SetData(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin,
                                 ARM_GPIO_IO_MODE_INPUT, ARM_GPIO_IO_TYPE_OPEN_DRAIN , ARM_GPIO_IO_HI_Z, ARM_GPIO_IO_SPEED_FREQ_LOW,
                                 usart-> p_pin[TX_PIN].alt_func);
                    usart->p_reg->CR1 |= USART_CR1_TE;
                    usart->p_info->drv_status |= ARM_USART_FLAG_TX_ENABLED;
                }
            } else {
                usart->p_reg->CR1 &= ~USART_CR1_TE;
                if(usart->p_info->mode != ARM_USART_MODE_SMART_CARD) {
                    // SPIO default pin function selected
                    GPIO_SetData(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin,
                                 0U, 0U, 0U, 0U, 0U);
                }
                usart->p_info->drv_status &= ~ARM_USART_FLAG_TX_ENABLED;

            }
        }
    }
    //to do
    return ARM_DRIVER_OK;
}

static ARM_USART_STATUS ARM_USART_GetStatus(ARM_USART_Resources_t *usart)
{
    ARM_USART_STATUS status;
// to do
    return status;
}

static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control, ARM_USART_Resources_t *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(ARM_USART_Resources_t *usart)
{
    ARM_USART_MODEM_STATUS modem_status;
//to do
    return modem_status;
}

static void USART_IRQHandler(ARM_USART_Resources_t *usart)
{
//to do
}

/*************************************************
Driver capabilities of USART2, USART3, USART6 are completely
similar to USART1 driver capabilities.
***************************************************/
#if (RTE_USART1 == 1)

static void ARM_USART1_Resources_Struct_Init(void)
{
    ARM_USART_Resources_t *p_str = &ARM_USART1_Resources;
    ARM_USART1_Resources.capabilities.asynchronous = 1;// supports UART (Asynchronous) mode

#if (RTE_USART1_CK_ID != 0)
    p_str->capabilities.synchronous_master = 1;         // supports Synchronous Master mode
#else
    p_str->capabilities.synchronous_master = 0;         // supports Synchronous Master mode
#endif //(RTE_USART1_CK_ID != 0)

    p_str->capabilities.synchronous_slave = 0;          // supports Synchronous Slave mode
    p_str->capabilities.single_wire = 1;                // supports UART Single-wire mode
    p_str->capabilities.irda = 1;                       // supports UART IrDA mode

#if (RTE_USART1_CK_ID != 0)
    p_str->capabilities.smart_card =  1;                // supports UART Smart Card mode
    p_str->capabilities.smart_card_clock =  1;          // Smart Card Clock generator available
#else
    p_str->capabilities.smart_card =  0;                // supports UART Smart Card mode
    p_str->capabilities.smart_card_clock =  0;          // Smart Card Clock generator available
#endif //(RTE_USART1_CK_ID != 0)

#if (RTE_USART1_CTS_ID !=0)
    p_str->capabilities.flow_control_rts =  1;          // RTS Flow Control available
#else
    p_str->capabilities.flow_control_rts =  0;          // RTS Flow Control available
#endif //(RTE_USART1_CTS_ID !=0) 

#if RTE_USART1_CTS_ID
    p_str->capabilities.flow_control_cts =  1;          // CTS Flow Control available
#else
    p_str->capabilities.flow_control_cts =  0;          // CTS Flow Control available
#endif //(RTE_USART1_CTS_ID != 0)               

    p_str->capabilities.event_tx_complete =  1;         // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    p_str->capabilities.event_rx_timeout =  0;          // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT

#if (RTE_USART1_CTS_ID !=0)
    p_str->capabilities.rts =  1;                       // RTS Line: 0=not available, 1=available
#else
    p_str->capabilities.rts =  0;                       // RTS Line: 0=not available, 1=available
#endif //(RTE_USART1_CTS_ID !=0) 

#if (RTE_USART1_CTS_ID != 0)
    p_str->capabilities.cts =  1;                       // CTS Line: 0=not available, 1=available
#else
    p_str->capabilities.cts =  0;                       // CTS Line: 0=not available, 1=available
#endif //(RTE_USART1_CTS_ID != 0)       

    p_str->capabilities.dtr =  0;                       // DTR Line: 0=not available, 1=available
    p_str->capabilities.dsr =  0;                       // DSR Line: 0=not available, 1=available
    p_str->capabilities.dcd =  0;                       // DCD Line: 0=not available, 1=available
    p_str->capabilities.ri =  0;                        // RI Line: 0=not available, 1=available

#if (RTE_USART1_CTS_ID != 0)
    p_str->capabilities.event_cts = 1;                  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    p_str->capabilities.event_cts =  0;                 // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif //(RTE_USART1_CTS_ID != 0) 

    p_str->capabilities.event_dsr =  0;                 // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    p_str->capabilities.event_dcd =  0;                 // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    p_str->capabilities.event_ri =  0;                  // Signal RI change event: \ref ARM_USART_EVENT_RI
    p_str->capabilities.reserved =  0;                  // Reserved (must be zero)
    p_str->usart_name = USART_1;                        //Interface name
    p_str->p_reg = USART1;                              // Pointer to USART peripheral registers
    p_str->p_pin = USART_GetPinCfg(USART_1);         // Pointer to USART pins configuration
    p_str->irq_num = USART1_IRQn;
    p_str->p_info = &USART1_Info;
}

/*
    void USART1_IRQHandler(void) {
        USART_IRQHandler(&ARM_USART1_Resources);
    }

*/

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
#endif //(RTE_USART1 == 1)

/*************************************************
Driver capabilities of UART5 are completely
similar to UART4 driver capabilities.
***************************************************/
#if (RTE_UART4 == 1)

static void ARM_USART4_Resources_Struct_Init(void)
{
    ARM_USART_Resources_t *p_str = &ARM_USART4_Resources;
    ARM_USART1_Resources.capabilities.asynchronous = 1;// supports UART (Asynchronous) mode
    p_str->capabilities.synchronous_master = 0;         // supports Synchronous Master mode
    p_str->capabilities.synchronous_slave = 0;          // supports Synchronous Slave mode
    p_str->capabilities.single_wire = 1;                // supports UART Single-wire mode
    p_str->capabilities.irda = 1;                       // supports UART IrDA mode
    p_str->capabilities.smart_card =  0;                // supports UART Smart Card mode
    p_str->capabilities.smart_card_clock =  0;          // Smart Card Clock generator available
    p_str->capabilities.flow_control_rts =  0;          // RTS Flow Control available
    p_str->capabilities.flow_control_cts =  0;          // CTS Flow Control available
    p_str->capabilities.event_tx_complete =  1;         // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    p_str->capabilities.event_rx_timeout =  0;          // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    p_str->capabilities.rts =  0;                       // RTS Line: 0=not available, 1=available
    p_str->capabilities.cts =  0;                       // CTS Line: 0=not available, 1=available
    p_str->capabilities.dtr =  0;                       // DTR Line: 0=not available, 1=available
    p_str->capabilities.dsr =  0;                       // DSR Line: 0=not available, 1=available
    p_str->capabilities.dcd =  0;                       // DCD Line: 0=not available, 1=available
    p_str->capabilities.ri =  0;                        // RI Line: 0=not available, 1=available
    p_str->capabilities.event_cts =  0;                 // Signal CTS change event: \ref ARM_USART_EVENT_CTS
    p_str->capabilities.event_dsr =  0;                 // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    p_str->capabilities.event_dcd =  0;                 // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    p_str->capabilities.event_ri =  0;                  // Signal RI change event: \ref ARM_USART_EVENT_RI
    p_str->capabilities.reserved =  0;                  // Reserved (must be zero)
    p_str->usart_name = UART_4;                        //Interface name
    p_str->p_reg = UART4;                              // Pointer to USART peripheral registers
    p_str->p_pin = USART_GetPinCfg(UART_4);         // Pointer to USART pins configuration
    p_str->irq_num = UART4_IRQn;
    p_str->p_info = &UART4_Info;
}
#endif //(RTE_UART4 == 1)

//================================================================================
//Public
//================================================================================

bool ARM_USART_isReady(int32_t status)
{
    return(status == ARM_DRIVER_OK);
}

int32_t ARM_USART_Init(void)
{

#if (RTE_USART1 == 1)

    ARM_DRIVER_USART *p_drv = &ARM_USART1_Driver;
    ARM_USART1_Resources_Struct_Init();
    int32_t status = ARM_DRIVER_OK;
    status |= p_drv->Initialize(&USART1_cb);
    status |= p_drv->PowerControl(ARM_POWER_FULL);
    return status;

#endif //(RTE_USART1 == 1)

}
