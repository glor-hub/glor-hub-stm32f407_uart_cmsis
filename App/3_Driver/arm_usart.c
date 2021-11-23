//********************************************************************************
//arm_usart.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "RTE_Device.h"
#include "RTE_Components.h"
#include "Driver_USART.h"
#include "arm_gpio.h"
#include "arm_usart.h"
#include "arm_clock.h"
#include "gpio.h"
#include "usart.h"
#include "ringbuffer.h"


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

// USART Baudrate Register Divider Masks
#define ARM_USART_DIVIDER_MANTISSA_FOR_OVER16_MASK USART_BRR_DIV_Mantissa_Msk
#define ARM_USART_DIVIDER_FRACTION_FOR_OVER16_MASK USART_BRR_DIV_Fraction_Msk
#define ARM_USART_DIVIDER_MANTISSA_FOR_OVER8_MASK  (uint32_t)(ARM_USART_DIVIDER_MANTISSA_FOR_OVER16_MASK >> 1)
#define ARM_USART_DIVIDER_FRACTION_FOR_OVER8_MASK  (uint32_t)(ARM_USART_DIVIDER_FRACTION_FOR_OVER16_MASK >> 1)

#define _ARM_USART_DEBUG_

#ifdef _ARM_USART_DEBUG_
#include "assert.h"
#endif//_ARM_USART_DEBUG_

//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************

// ARM USART Transfer Information (Run-Time)
typedef struct {
    volatile uint32_t      rx_num;        // Total number of data to be received
    volatile uint32_t      tx_num;        // Total number of data to be send
    uint8_t                *p_rx_buf;      // Pointer to in data buffer
    uint8_t                *p_tx_buf;      // Pointer to out data buffer
    volatile uint32_t      rx_cnt;        // Number of data received
    volatile uint32_t      tx_cnt;        // Number of data sent
    uint8_t                tx_def_val;    // Transmit default value (used in USART_SYNC_MASTER_MODE_RX)
    uint8_t                rx_dump_val;   // Receive dump value (used in USART_SYNC_MASTER_MODE_TX)
} ARM_USART_TransferInfo_t;

// ARM USART Information (Run-Time)
typedef struct {
    ARM_USART_SignalEvent_t  cb_event;        // Event callback
    uint16_t                 flags;          // USART driver flags
    uint8_t                  mode;            // USART mode
    uint32_t                 baudrate;        // Baudrate
    ARM_USART_STATUS         xfer_status;     // USART transfer status
    ARM_USART_MODEM_STATUS   modem_status;     // USART modem status
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

#if (RTE_USART1)
static ARM_USART_Resources_t ARM_USART1_Resources;
static ARM_USART_Info_t USART1_Info = {0};
static RingBuffer_t USART1_TxRBuff, USART1_RxRBuff;
static uint8_t USART1_TxRingBuffer[RING_BUFF_SIZE] = {0};
static uint8_t USART1_RxRingBuffer[RING_BUFF_SIZE] = {0};
#endif //(RTE_USART1)

#if (RTE_UART4)
static ARM_USART_Resources_t ARM_UART4_Resources;
static ARM_USART_Info_t UART4_Info = {0};
#endif //(RTE_UART4)

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
static int32_t ARM_USART_SetBaudrate(uint32_t arg, ARM_USART_Resources_t *usart);
static ARM_USART_STATUS ARM_USART_GetStatus(ARM_USART_Resources_t *usart);
static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control, ARM_USART_Resources_t *usart);
static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(ARM_USART_Resources_t *usart);
static void USART_IRQHandler(ARM_USART_Resources_t *usart);
static void USART_cb(uint32_t event, ARM_USART_Resources_t *usart);
static int32_t ARM_USART_PutChar(uint8_t ch, ARM_USART_Resources_t *usart);
static uint8_t ARM_USART_GetChar(ARM_USART_Resources_t *usart);

#if (RTE_USART1)
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
static void USART1_cb(uint32_t event);

#endif //(RTE_USART1)

#if (RTE_UART4)

static void ARM_UART4_Resources_Struct_Init(void);

#endif //(RTE_UART4)

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
    if(usart->p_info->flags & ARM_USART_FLAG_INITIALIZED) {
        // Driver is already initialized
        return ARM_DRIVER_OK;
    }
    // Initialize USART Run-time Resources
    usart->p_info->cb_event = cb_event;

    // Clear driver variables
    usart->p_info->flags                        = 0U;
    usart->p_info->mode                         = 0U;
    usart->p_info->baudrate                     = 0U;
    usart->p_info->xfer_status.tx_busy          = 0U;
    usart->p_info->xfer_status.rx_busy          = 0U;
    usart->p_info->xfer_status.tx_underflow     = 0U;
    usart->p_info->xfer_status.rx_overflow      = 0U;
    usart->p_info->xfer_status.rx_break         = 0U;
    usart->p_info->xfer_status.rx_framing_error = 0U;
    usart->p_info->xfer_status.rx_parity_error  = 0U;
    usart->p_info->xfer_info.tx_def_val         = 0U;
    usart->p_info->xfer_info.rx_dump_val        = 0U;

// Configure CTS pin
    if(usart->capabilities.cts) {
        ARM_RCC_Periph_ClockCmd(usart-> p_pin[CTS_PIN].port, ENABLE_CMD);
        GPIO_SetData(usart-> p_pin[CTS_PIN].GPIOx, usart-> p_pin[CTS_PIN].pin,
                     ARM_GPIO_IO_MODE_ALT_FUNC, ARM_GPIO_IO_TYPE_OPEN_DRAIN , ARM_GPIO_IO_HI_Z, ARM_GPIO_IO_SPEED_FREQ_LOW,
                     usart-> p_pin[CTS_PIN].alt_func);
    }
// Configure RTS pin
    if(usart->capabilities.rts) {
        ARM_RCC_Periph_ClockCmd(usart-> p_pin[RTS_PIN].port, ENABLE_CMD);
        GPIO_SetData(usart-> p_pin[RTS_PIN].GPIOx, usart-> p_pin[RTS_PIN].pin,
                     ARM_GPIO_IO_MODE_ALT_FUNC, ARM_GPIO_IO_TYPE_PUSH_PULL, ARM_GPIO_IO_HI_Z, ARM_GPIO_IO_SPEED_FREQ_LOW,
                     usart-> p_pin[RTS_PIN].alt_func);
    }

// TX, RX, CK pins are configurated in function ARM_USART_Control.

// DMA Initialize
    //unsupported in Version 1.0
    usart->p_info->flags |= ARM_USART_FLAG_INITIALIZED;
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
            // Clear driver variables
            usart->p_info->flags                        = 0U;
            usart->p_info->mode                         = 0U;
            usart->p_info->baudrate                     = 0U;
            usart->p_info->xfer_status.tx_busy          = 0U;
            usart->p_info->xfer_status.rx_busy          = 0U;
            usart->p_info->xfer_status.tx_underflow     = 0U;
            usart->p_info->xfer_status.rx_overflow      = 0U;
            usart->p_info->xfer_status.rx_break         = 0U;
            usart->p_info->xfer_status.rx_framing_error = 0U;
            usart->p_info->xfer_status.rx_parity_error  = 0U;
            usart->p_info->xfer_info.tx_def_val         = 0U;
            usart->p_info->xfer_info.rx_dump_val        = 0U;
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
            usart->p_info->flags |= ARM_USART_FLAG_POWERED;
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

static int32_t ARM_USART_PutChar(uint8_t ch, ARM_USART_Resources_t *usart)
{
    usart->p_reg->DR = ch;
    return ARM_DRIVER_OK;
}

static uint8_t ARM_USART_GetChar(ARM_USART_Resources_t *usart)
{
    uint8_t ch = 0;
    ch = usart->p_reg->DR;
    return ch;
}

static int32_t ARM_USART_Send(const void *pdata, uint32_t num,
                              ARM_USART_Resources_t *usart)
{
    ARM_USART_TransferInfo_t *p_str = &(usart->p_info->xfer_info);
    if(num == 0U) {
        // Invalid parameters
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    if((usart->p_info->flags & ARM_USART_FLAG_CONFIGURED) == 0U) {
        // USART is not configured (mode not selected)
        return ARM_DRIVER_ERROR;
    }
    if(usart->p_info->xfer_status.tx_busy != 0U) {
        // Send is not completed yet
        return ARM_DRIVER_ERROR_BUSY;
    }
    usart->p_info->xfer_status.tx_busy = 1U;
    p_str->p_tx_buf = (uint8_t *)pdata;
    p_str->tx_num = num;
    p_str->tx_cnt = 0U;
// Transmit data register empty and Transmission complete interrupts enable
    usart->p_reg->CR1 |= (USART_CR1_TXEIE);
    return ARM_DRIVER_OK;
}


// Used in asynchronous mode

// Can also be used in synchronous mode when receiving data only
//(transmits the default value ARM_USART_SET_DEFAULT_TX_VALUE as control parameter)
static int32_t ARM_USART_Receive(void *pdata, uint32_t num, ARM_USART_Resources_t *usart)
{
    if(num == 0U) {
        // Invalid parameters
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    if((usart->p_info->flags & ARM_USART_FLAG_CONFIGURED) == 0U) {
        // USART is not configured (mode not selected)
        return ARM_DRIVER_ERROR;
    }

    // Check reciver buzy
    if(usart->p_info->xfer_status.rx_busy != 0U) {
        return ARM_DRIVER_ERROR_BUSY;
    }

    // Set RX busy flag
    usart->p_info->xfer_status.rx_busy = 1U;

    usart->p_info->xfer_status.rx_overflow = 0U;
    usart->p_info->xfer_status.rx_break = 0U;
    usart->p_info->xfer_status.rx_framing_error = 0U;
    usart->p_info->xfer_status.rx_parity_error = 0U;

    usart->p_info->xfer_info.p_rx_buf = (uint8_t *)pdata;
    usart->p_info->xfer_info.rx_num = num;
    usart->p_info->xfer_info.rx_cnt = 0U;

    // Read data register not empty interrupt enable
    usart->p_reg->CR1 |= USART_CR1_RXNEIE;

    return ARM_DRIVER_OK;
}

// Only in synchronous mode
static int32_t ARM_USART_Transfer(const void *data_out, void *data_in,
                                  uint32_t num, ARM_USART_Resources_t *usart)
{
    if(num == 0U) {
        // Invalid parameters
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if((usart->p_info->flags & ARM_USART_FLAG_CONFIGURED) == 0U) {
        // USART is not configured
        return ARM_DRIVER_ERROR;
    }

    if((usart->p_info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
       (usart->p_info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE)) {
        //the mode is unsupported in Version 1.0
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    } else {
        // Only in synchronous mode
        return ARM_DRIVER_ERROR;
    }
}

static uint32_t ARM_USART_GetTxCount(ARM_USART_Resources_t *usart)
{
    uint32_t cnt = 0UL;
    cnt = usart->p_info->xfer_info.tx_cnt;
    return cnt;
}

static uint32_t ARM_USART_GetRxCount(ARM_USART_Resources_t *usart)

{
    uint32_t cnt = 0UL;
    cnt = usart->p_info->xfer_info.rx_cnt;
    return cnt;
}

static int32_t ARM_USART_Control(uint32_t control, uint32_t arg,
                                 ARM_USART_Resources_t *usart)
{
    if((usart->p_info->flags & ARM_USART_FLAG_POWERED) == 0U) {
        // USART not powered
        return ARM_DRIVER_ERROR;
    }

// Check if Receiver/Transmitter is busy
    if((usart->p_info->xfer_status.tx_busy != 0U) ||
       (usart->p_info->xfer_status.rx_busy != 0U)) {
        return ARM_DRIVER_ERROR_BUSY;
    }

    /*************************************************************************
    Miscellaneous Controls Section

    Attention!!! - Miscellaneous Controls Operations cannot be ORed!

    **************************************************************************/

    switch(control & ARM_USART_CONTROL_Msk) {
        //Synchronous Receive only
        case ARM_USART_SET_DEFAULT_TX_VALUE: {
            if((usart->p_info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
               (usart->p_info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE)) {
                usart->p_info->xfer_info.tx_def_val = arg;
                return ARM_DRIVER_OK;
            } else {
                return ARM_DRIVER_ERROR;
            }
        }
        //IrDA Pulse
        case ARM_USART_SET_IRDA_PULSE: {
            if(usart->p_info->mode == ARM_USART_MODE_IRDA)  {
                //the mode is unsupported in Version 1.0
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            } else {
                return ARM_DRIVER_ERROR;
            }
        }
        //Smart Card Guard Time
        case ARM_USART_SET_SMART_CARD_GUARD_TIME: {
            if(usart->p_info->mode == ARM_USART_MODE_SMART_CARD)  {
                //the mode is unsupported in Version 1.0
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            } else {
                return ARM_DRIVER_ERROR;
            }
        }
        //Smart Card Clock
        case ARM_USART_SET_SMART_CARD_CLOCK: {
            if(usart->p_info->mode == ARM_USART_MODE_SMART_CARD)  {
                //the mode is unsupported in Version 1.0
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            } else {
                return ARM_DRIVER_ERROR;
            }
        }
        //Smart Card NACK generation
        case ARM_USART_CONTROL_SMART_CARD_NACK: {
            if(usart->p_info->mode == ARM_USART_MODE_SMART_CARD)  {
                //the mode is unsupported in Version 1.0
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            } else {
                return ARM_DRIVER_ERROR;
            }
        }
        //Control TX
        case ARM_USART_CONTROL_TX: {
            if(usart->p_info->mode != ARM_USART_MODE_SMART_CARD) {
                if(arg) {
                    if(usart->p_info->mode != ARM_USART_MODE_SMART_CARD) {
                        ARM_RCC_Periph_ClockCmd(usart-> p_pin[TX_PIN].port, ENABLE_CMD);
                        GPIO_SetData(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin,
                                     ARM_GPIO_IO_MODE_ALT_FUNC, ARM_GPIO_IO_TYPE_PUSH_PULL ,
                                     ARM_GPIO_IO_HI_Z, ARM_GPIO_IO_SPEED_FREQ_LOW,
                                     usart-> p_pin[TX_PIN].alt_func);
                    } else {
                        ARM_RCC_Periph_ClockCmd(usart-> p_pin[TX_PIN].port, ENABLE_CMD);
                        GPIO_SetData(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin,
                                     ARM_GPIO_IO_MODE_ALT_FUNC, ARM_GPIO_IO_TYPE_OPEN_DRAIN ,
                                     ARM_GPIO_IO_PULL_UP, ARM_GPIO_IO_SPEED_FREQ_LOW,
                                     usart-> p_pin[TX_PIN].alt_func);
                    }
                    usart->p_reg->CR1 |= USART_CR1_TE;
                    usart->p_info->flags |= ARM_USART_FLAG_TX_ENABLED;
                } else {
                    usart->p_reg->CR1 &= ~USART_CR1_TE;
                    usart->p_info->flags &= ~ARM_USART_FLAG_TX_ENABLED;
                    ARM_RCC_Periph_ClockCmd(usart-> p_pin[TX_PIN].port, ENABLE_CMD);
                    GPIO_SetData(usart-> p_pin[TX_PIN].GPIOx, usart-> p_pin[TX_PIN].pin,
                                 0U, 0U, 0U, 0U, 0U);
                }
            }
            return ARM_DRIVER_OK;
        }

        //Control RX
        case ARM_USART_CONTROL_RX: {
            if(arg) {
                if((usart->p_info->mode != ARM_USART_MODE_SMART_CARD) &&
                   (usart->p_info->mode != ARM_USART_MODE_SINGLE_WIRE)) {
                    ARM_RCC_Periph_ClockCmd(usart-> p_pin[RX_PIN].port, ENABLE_CMD);
                    GPIO_SetData(usart-> p_pin[RX_PIN].GPIOx, usart-> p_pin[RX_PIN].pin,
                                 ARM_GPIO_IO_MODE_ALT_FUNC, ARM_GPIO_IO_TYPE_OPEN_DRAIN ,
                                 ARM_GPIO_IO_HI_Z, ARM_GPIO_IO_SPEED_FREQ_LOW,
                                 usart-> p_pin[RX_PIN].alt_func);
                } else {
                    ARM_RCC_Periph_ClockCmd(usart-> p_pin[RX_PIN].port, ENABLE_CMD);
                    GPIO_SetData(usart-> p_pin[RX_PIN].GPIOx, usart-> p_pin[RX_PIN].pin,
                                 0U, 0U, 0U, 0U, 0U);
                }
                usart->p_reg->CR1 |= USART_CR1_RE;
                usart->p_info->flags |= ARM_USART_FLAG_RX_ENABLED;
            } else {
                usart->p_reg->CR1 &= ~USART_CR1_RE;
                usart->p_info->flags &= ~ARM_USART_FLAG_RX_ENABLED;
                ARM_RCC_Periph_ClockCmd(usart-> p_pin[RX_PIN].port, ENABLE_CMD);
                GPIO_SetData(usart-> p_pin[RX_PIN].GPIOx, usart-> p_pin[RX_PIN].pin,
                             0U, 0U, 0U, 0U, 0U);
            }
            return ARM_DRIVER_OK;
        }

        //Control break
        case ARM_USART_CONTROL_BREAK: {
            if(arg) {
                if(usart->p_info->xfer_status.tx_busy) {
                    return ARM_DRIVER_ERROR_BUSY;
                }
                usart->p_info->xfer_status.tx_busy = 1UL;
                usart->p_reg->CR1 |= USART_CR1_SBK;
            } else {
                usart->p_reg->CR1 &= ~USART_CR1_SBK;
                usart->p_info->xfer_status.tx_busy = 0UL;
            }
            return ARM_DRIVER_OK;
        }

        //Abort Send
        case ARM_USART_ABORT_SEND: {
            //Disable TXE and TC interrupts
            usart->p_reg->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE);
            usart->p_info->xfer_info.tx_cnt = 0UL;
            usart->p_info->xfer_status.tx_busy = 0UL;
            return ARM_DRIVER_OK;
        }
        case ARM_USART_ABORT_RECEIVE: {
            //Disable RXNE interrupt
            usart->p_reg->CR1 &= ~USART_CR1_RXNEIE;
            //Disable PE interrupt
            usart->p_reg->SR &= ~USART_SR_PE;
            //Disable FE, ORE and NF interrupts
            usart->p_reg->CR3 &= ~USART_CR3_EIE;
            usart->p_info->xfer_info.rx_cnt = 0UL;
            usart->p_info->xfer_status.rx_busy = 0UL;
            return ARM_DRIVER_OK;
        }
        case ARM_USART_ABORT_TRANSFER: {
            //Disable TXE and TC interrupts
            usart->p_reg->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE);
            //Disable RXNE interrupt
            usart->p_reg->CR1 &= ~USART_CR1_RXNEIE;
            //Disable PE interrupt
            usart->p_reg->SR &= ~USART_SR_PE;
            //Disable FE, ORE and NF interrupts
            usart->p_reg->CR3 &= ~USART_CR3_EIE;
            usart->p_info->xfer_info.tx_cnt = 0UL;
            usart->p_info->xfer_info.rx_cnt = 0UL;
            usart->p_info->xfer_status.tx_busy = 0UL;
            usart->p_info->xfer_status.rx_busy = 0UL;
            return ARM_DRIVER_OK;
        }
        default: {
            break;
        }
    }

    /*************************************************************************

      End of Miscellaneous Controls Section

    **************************************************************************/

    //USART Mode
    uint8_t mode = 0U;
    switch(control & ARM_USART_CONTROL_Msk) {
        case ARM_USART_MODE_ASYNCHRONOUS: {
            if(usart->capabilities.asynchronous) {
                mode = ARM_USART_MODE_ASYNCHRONOUS;
            } else {
                return ARM_USART_ERROR_MODE;
            }
            break;
        }
        case ARM_USART_MODE_SYNCHRONOUS_MASTER: {
            if(usart->capabilities.synchronous_master) {
                //the mode is unsupported in Version 1.0
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            } else {
                return ARM_USART_ERROR_MODE;
            }
        }
        case ARM_USART_MODE_SYNCHRONOUS_SLAVE: {
            //the mode is unsupported in STM32F407
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        }
        case ARM_USART_MODE_SINGLE_WIRE: {
            if(usart->capabilities.single_wire) {
                //the mode is unsupported in Version 1.0
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            } else {
                return ARM_USART_ERROR_MODE;
            }
        }
        case ARM_USART_MODE_IRDA: {
            if(usart->capabilities.irda) {
                //the mode is unsupported in Version 1.0
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            } else {
                return ARM_USART_ERROR_MODE;
            }
        }
        case ARM_USART_MODE_SMART_CARD: {
            if(usart->capabilities.smart_card) {
                //the mode is unsupported in Version 1.0
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            } else {
                return ARM_USART_ERROR_MODE;
            }
        }
        //unsupported commands
        default: {
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        }
    }

    //USART Data Bits
    switch(control & ARM_USART_DATA_BITS_Msk) {
        case ARM_USART_DATA_BITS_5: {
            return ARM_USART_ERROR_DATA_BITS;
        }
        case ARM_USART_DATA_BITS_6: {
            return ARM_USART_ERROR_DATA_BITS;
        }
        case ARM_USART_DATA_BITS_7: {
            return ARM_USART_ERROR_DATA_BITS;
        }
        case ARM_USART_DATA_BITS_8: {
            usart->p_reg->CR1 &= ~USART_CR1_M_Msk;
            usart->p_reg->CR1 &= ~USART_CR1_M;
            break;
        }
        case ARM_USART_DATA_BITS_9: {
            usart->p_reg->CR1 &= ~USART_CR1_M_Msk;
            usart->p_reg->CR1 |= USART_CR1_M;
            break;
        }
        default: {
            return ARM_USART_ERROR_DATA_BITS;
        }
    }

    //USART Parity
    switch(control & ARM_USART_PARITY_Msk) {
        case ARM_USART_PARITY_NONE: {
            usart->p_reg->CR1 &= ~USART_CR1_PCE_Msk;
            usart->p_reg->CR1 &= ~USART_CR1_PS_Msk;
            usart->p_reg->CR1 &= ~USART_CR1_PCE;
            break;
        }
        case ARM_USART_PARITY_EVEN: {
            usart->p_reg->CR1 &= ~USART_CR1_PCE_Msk;
            usart->p_reg->CR1 &= ~USART_CR1_PS_Msk;
            usart->p_reg->CR1 |= USART_CR1_PCE;
            usart->p_reg->CR1 &= ~USART_CR1_PS;
            break;
        }
        case ARM_USART_PARITY_ODD: {
            usart->p_reg->CR1 &= ~USART_CR1_PCE_Msk;
            usart->p_reg->CR1 &= ~USART_CR1_PS_Msk;
            usart->p_reg->CR1 |= USART_CR1_PCE;
            usart->p_reg->CR1 |= USART_CR1_PS;
            break;
        }
        default: {
            return ARM_USART_ERROR_PARITY;
        }
    }

    //USART Stop Bits
    switch(control & ARM_USART_STOP_BITS_Msk) {
        case ARM_USART_STOP_BITS_1: {
            usart->p_reg->CR2 &= ~USART_CR2_STOP_Msk;
            usart->p_reg->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1);
            break;
        }
        case ARM_USART_STOP_BITS_2: {
            usart->p_reg->CR2 &= ~USART_CR2_STOP_Msk;
            usart->p_reg->CR2 &= ~USART_CR2_STOP_0;
            usart->p_reg->CR2 |= USART_CR2_STOP_1;
            break;
        }
        case ARM_USART_STOP_BITS_1_5: {
            usart->p_reg->CR2 &= ~USART_CR2_STOP_Msk;
            usart->p_reg->CR2 |= (USART_CR2_STOP_0 | USART_CR2_STOP_1);
            break;
        }
        case ARM_USART_STOP_BITS_0_5: {
            usart->p_reg->CR2 &= ~USART_CR2_STOP_Msk;
            usart->p_reg->CR2 |= USART_CR2_STOP_0;
            usart->p_reg->CR2 &= ~USART_CR2_STOP_1;
            break;
        }
        default: {
            return ARM_USART_ERROR_STOP_BITS;
        }
    }

    //USART Flow Control
    switch(control & ARM_USART_FLOW_CONTROL_Msk) {
        case ARM_USART_FLOW_CONTROL_NONE: {
            usart->p_reg->CR3 &= ~(USART_CR3_RTSE_Msk | USART_CR3_CTSE_Msk);
            break;
        }
        case ARM_USART_FLOW_CONTROL_RTS: {
            if(usart->capabilities.flow_control_rts) {
                usart->p_reg->CR3 &= ~(USART_CR3_RTSE_Msk | USART_CR3_CTSE_Msk);
                usart->p_reg->CR3 |= USART_CR3_RTSE;
            } else {
                return ARM_USART_ERROR_FLOW_CONTROL;
            }
            break;
        }
        case ARM_USART_FLOW_CONTROL_CTS: {
            if(usart->capabilities.flow_control_cts) {
                usart->p_reg->CR3 &= ~(USART_CR3_RTSE_Msk | USART_CR3_CTSE_Msk);
                usart->p_reg->CR3 |= USART_CR3_CTSE;
            } else {
                return ARM_USART_ERROR_FLOW_CONTROL;
            }
            break;
        }
        case ARM_USART_FLOW_CONTROL_RTS_CTS: {
            if(usart->capabilities.flow_control_rts &&
               usart->capabilities.flow_control_cts) {
                usart->p_reg->CR3 &= ~(USART_CR3_RTSE_Msk | USART_CR3_CTSE_Msk);
                usart->p_reg->CR3 |= (USART_CR3_RTSE | USART_CR3_CTSE);
            } else {
                return ARM_USART_ERROR_FLOW_CONTROL;
            }
            break;
        }
        default: {
            return ARM_USART_ERROR_FLOW_CONTROL;
        }
    }

    // Clock setting for synchronous mode
    //USART Clock Polarity (Synchronous mode)
    if((mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
       (mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE)) {
        switch(control & ARM_USART_CPOL_Msk) {
            case ARM_USART_CPOL0:
                usart->p_reg->CR2 &= ~(USART_CR2_CPOL_Msk);
                usart->p_reg->CR2 &= ~(USART_CR2_CPOL);
                break;
            case ARM_USART_CPOL1: {
                usart->p_reg->CR2 &= ~(USART_CR2_CPOL_Msk);
                usart->p_reg->CR2 |= USART_CR2_CPOL;
                break;
            }

            default: {
                return ARM_USART_ERROR_CPOL;
            }
        }
    }

//USART Clock Phase (Synchronous mode)
    if((mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
       (mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE)) {
        switch(control & ARM_USART_CPHA_Msk) {
            case ARM_USART_CPHA0: {
                usart->p_reg->CR2 &= ~(USART_CR2_CPHA_Msk);
                usart->p_reg->CR2 &= ~(USART_CR2_CPHA);
            }
            break;
            case ARM_USART_CPHA1: {
                usart->p_reg->CR2 &= ~(USART_CR2_CPHA_Msk);
                usart->p_reg->CR2 |= USART_CR2_CPHA;
                break;
            }
            default: {
                return ARM_USART_ERROR_CPHA;
            }
        }
    }

// Configuration is successful - mode id valid
    usart->p_info->mode = mode;
    if(ARM_USART_SetBaudrate(arg, usart) == -1) {
        return ARM_USART_ERROR_BAUDRATE;
    } else {
        usart->p_info->baudrate = arg;
    }
    // Enable USART block
    usart->p_reg->CR1 |= USART_CR1_UE;
// Set configured flag
    usart->p_info->flags |= ARM_USART_FLAG_CONFIGURED;
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_SetBaudrate(uint32_t arg, ARM_USART_Resources_t *usart)
{
    /*
    baudrate = clock_freq/(8*(2-oversampl8)*div)
    */
    uint32_t div = 0UL;
    uint32_t div_mantissa = 0UL;
    uint32_t div_fraction = 0UL;
    uint32_t oversampl8 = 0UL;
    uint32_t baudrate = arg;

    uint32_t clock_freq = ARM_RCC_GetPeriphClock(usart->usart_name);
    if(clock_freq == 0) {
        return ARM_USART_ERROR_BAUDRATE;
    }
    if(arg == 0) {
        return ARM_USART_ERROR_BAUDRATE;
    }

    uint32_t mode = usart->p_info->mode;
    switch(mode) {
        case ARM_USART_MODE_ASYNCHRONOUS:
        case ARM_USART_MODE_SYNCHRONOUS_MASTER:
        case ARM_USART_MODE_SYNCHRONOUS_SLAVE: {
            oversampl8 = (usart->p_reg->CR1 & USART_CR1_OVER8_Msk) ? 1UL : 0UL;
            break;
        }
        case ARM_USART_MODE_SINGLE_WIRE:
        case ARM_USART_MODE_IRDA:
        case ARM_USART_MODE_SMART_CARD: {
            oversampl8 = 0;
            break;
        }
        default: {
            return ARM_USART_ERROR_BAUDRATE;
        }
    }
    if(oversampl8) {
        /*
        for oversampling by 8
        */
        //for rounding in the least significant bit, multiply by 2,
        //add 1 and shift to the right by 1
        div = (clock_freq * 8 * 2 / (baudrate * 8 * (2 - oversampl8)));
        div |= 1;
        div >>= 1;
        div_mantissa = (uint32_t)((div & ARM_USART_DIVIDER_MANTISSA_FOR_OVER8_MASK) << 1);
        div_fraction = (uint32_t)(div & ARM_USART_DIVIDER_FRACTION_FOR_OVER8_MASK);
    } else {
        /*
        for oversampling by 16
        */
        //for rounding in the least significant bit, multiply by 2,
        //add 1 and shift to the right by 1
        div = (clock_freq * 16 * 2 / (baudrate * 8 * (2 - oversampl8)));
        div |= 1;
        div >>= 1;
        div_mantissa = (uint32_t)(div & ARM_USART_DIVIDER_MANTISSA_FOR_OVER16_MASK);
        div_fraction = (uint32_t)(div & ARM_USART_DIVIDER_FRACTION_FOR_OVER16_MASK);
    }
    usart->p_reg->BRR = (div_mantissa | div_fraction);
    return ARM_DRIVER_OK;
}

static ARM_USART_STATUS ARM_USART_GetStatus(ARM_USART_Resources_t *usart)
{
    ARM_USART_STATUS status;
    status.tx_busy = usart->p_info->xfer_status.tx_busy; //Transmitter busy flag
    status.rx_busy = usart->p_info->xfer_status.rx_busy; //Receiver busy flag
    status.tx_underflow = usart->p_info->xfer_status.tx_underflow; //Transmit data underflow detected (cleared on start of next send operation)
    status.rx_overflow = usart->p_info->xfer_status.rx_overflow; //Receive data overflow detected (cleared on start of next receive operation)
    status.rx_break = usart->p_info->xfer_status.rx_break; //Break detected on receive (cleared on start of next receive operation)
    status.rx_framing_error = usart->p_info->xfer_status.rx_framing_error; //Framing error detected on receive (cleared on start of next receive operation)
    status.rx_parity_error = usart->p_info->xfer_status.rx_parity_error; //Parity error detected on receive (cleared on start of next receive operation)
    status.reserved = usart->p_info->xfer_status.reserved;
    return status;
}

static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control, ARM_USART_Resources_t *usart)
{
//to do
    return ARM_DRIVER_OK;
}

static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(ARM_USART_Resources_t *usart)
{
    ARM_USART_MODEM_STATUS status;
    status.cts = usart->p_info->modem_status.cts; //CTS state: 1=Active, 0=Inactive
    status.dsr = usart->p_info->modem_status.dsr; //DSR state: 1=Active, 0=Inactive
    status.dcd = usart->p_info->modem_status.dcd; //DCD state: 1=Active, 0=Inactive
    status.ri = usart->p_info->modem_status.ri; //RI  state: 1=Active, 0=Inactive
    status.reserved = usart->p_info->modem_status.reserved;
    return status;
}

static void USART_IRQHandler(ARM_USART_Resources_t *usart)
{
    uint32_t flag = usart->p_reg->SR;
    uint32_t event = 0U;
    uint8_t ch = 0U;
    ARM_USART_TransferInfo_t *p_str = &(usart->p_info->xfer_info);

// Transmit Data Register Empty
    if((flag & USART_SR_TXE) && (usart->p_info->xfer_status.tx_busy == 1)) {
        if(RingBuffer_ReadChar(&USART1_TxRBuff, &ch)) {
            event |= ARM_USART_EVENT_RING_BUFFER_UNDERFLOW;
        } else {
            if(ARM_USART_PutChar(ch, usart) == ARM_DRIVER_OK) {
                p_str->tx_cnt++;
                if(ARM_USART_GetTxCount(usart) == p_str->tx_num) {
                    // Wait for transmission complete
                    while((usart->p_reg->SR & USART_SR_TC) == 0U);
                    usart->p_reg->CR1 &= ~USART_CR1_TXEIE;
                    usart->p_info->xfer_status.tx_busy = 0U;
                    p_str->tx_num = 0U;
                    p_str->tx_cnt = 0U;
                    event |= ARM_USART_EVENT_TX_COMPLETE;
                }
            }
        }
    }
// OverRun Error
    if(flag & USART_SR_ORE) {
        usart->p_info->xfer_status.rx_overflow = 1U;
        event |= ARM_USART_EVENT_RX_OVERFLOW;
    }

    // Noise Error or
    // Framing Error (de-synchronization, excessive noise
    // a break character is detected)
    if(flag & (USART_SR_NE | USART_SR_FE)) {
        usart->p_info->xfer_status.rx_framing_error = 1U;
        event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
    }

// Parity Error
    if(flag & USART_SR_PE) {
        if(flag & USART_SR_RXNE) {
            usart->p_info->xfer_status.rx_parity_error = 1U;
            event |= ARM_USART_EVENT_RX_PARITY_ERROR;
        }
    }

// IDLE line detected
    if(flag & ARM_USART_EVENT_RX_IDLE) {
        event |= ARM_USART_EVENT_RX_OVERFLOW;
    }

// Read Data Register Not Empty
    if(flag & USART_SR_RXNE) {
        ch = ARM_USART_GetChar(usart);
        if(RingBuffer_WriteChar(&USART1_RxRBuff, &ch)) {
            event |= ARM_USART_EVENT_RING_BUFFER_OVERFLOW;
        } else {
            p_str->rx_cnt++;
            usart->p_reg ->SR &= ~USART_SR_RXNE;
            if(ARM_USART_GetRxCount(usart) == p_str->rx_num) {
                usart->p_reg->CR1 &= ~USART_CR1_RXNEIE;
                usart->p_info->xfer_status.rx_busy = 0U;
                p_str->rx_num = 0U;
                p_str->rx_cnt = 0U;
                event |= ARM_USART_EVENT_RECEIVE_COMPLETE;
            }
        }
    }

    if(event != 0U) {
        usart->p_info->cb_event(event);
    }

}

static void USART_cb(uint32_t event, ARM_USART_Resources_t *usart)
{
    if(event & ARM_USART_EVENT_TX_COMPLETE) {

    }

    if(event & ARM_USART_EVENT_RECEIVE_COMPLETE) {

    }

    if(event & ARM_USART_EVENT_RING_BUFFER_OVERFLOW) {
#ifdef _ARM_USART_DEBUG_
        LOG("Warning! Ring buffer overflow");
#endif //_ARM_USART_DEBUG_
    }

    if(event & ARM_USART_EVENT_RING_BUFFER_UNDERFLOW) {
#ifdef _ARM_USART_DEBUG_
        LOG("Warning! Ring buffer underflow");
#endif //_ARM_USART_DEBUG_
    }

    if(event & ARM_USART_EVENT_RX_OVERFLOW) {
        // to do
#ifdef _ARM_USART_DEBUG_
        LOG("Warning! At least 1 data has been lost in shift register.");
#endif //_ARM_USART_DEBUG_
        usart->p_info->xfer_status.rx_overflow = 0U;
    }

    if(event & ARM_USART_EVENT_RX_FRAMING_ERROR) {
        // to do
#ifdef _ARM_USART_DEBUG_
        LOG("Warning! The invalid data or a break character is transferred from the Shift register to the USART_DR register.");
#endif //_ARM_USART_DEBUG_
        usart->p_info->xfer_status.rx_framing_error = 0U;
    }

    if(event & ARM_USART_EVENT_RX_PARITY_ERROR) {
        // to do
#ifdef _ARM_USART_DEBUG_
        LOG("Warning! The parity check failed.");
#endif //_ARM_USART_DEBUG_
        usart->p_info->xfer_status.rx_parity_error = 0U;
    }
}

/*************************************************
Driver capabilities of USART2, USART3, USART6 are completely
similar to USART1 driver capabilities.
***************************************************/
#if (RTE_USART1)

static void ARM_USART1_Resources_Struct_Init(void)
{
    ARM_USART_Resources_t *p_str = &ARM_USART1_Resources;
    p_str->capabilities.asynchronous = 1;               // supports UART (Asynchronous) mode

#if (RTE_USART1_CK_ID > 0)
    p_str->capabilities.synchronous_master = 1;         // supports Synchronous Master mode
#else
    p_str->capabilities.synchronous_master = 0;         // supports Synchronous Master mode
#endif //(RTE_USART1_CK_ID > 0)

    p_str->capabilities.synchronous_slave = 0;          // supports Synchronous Slave mode
    p_str->capabilities.single_wire = 1;                // supports UART Single-wire mode
    p_str->capabilities.irda = 1;                       // supports UART IrDA mode

#if (RTE_USART1_CK_ID > 0)
    p_str->capabilities.smart_card =  1;                // supports UART Smart Card mode
    p_str->capabilities.smart_card_clock =  1;          // Smart Card Clock generator available
#else
    p_str->capabilities.smart_card =  0;                // supports UART Smart Card mode
    p_str->capabilities.smart_card_clock =  0;          // Smart Card Clock generator available
#endif //(RTE_USART1_CK_ID > 0)

#if (RTE_USART1_CTS_ID > 0)
    p_str->capabilities.flow_control_rts =  1;          // RTS Flow Control available
#else
    p_str->capabilities.flow_control_rts =  0;          // RTS Flow Control available
#endif //(RTE_USART1_CTS_ID > 0) 

#if RTE_USART1_CTS_ID > 0
    p_str->capabilities.flow_control_cts =  1;          // CTS Flow Control available
#else
    p_str->capabilities.flow_control_cts =  0;          // CTS Flow Control available
#endif //(RTE_USART1_CTS_ID > 0)               

    p_str->capabilities.event_tx_complete =  1;         // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    p_str->capabilities.event_rx_timeout =  0;          // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT

#if (RTE_USART1_CTS_ID > 0)
    p_str->capabilities.rts =  1;                       // RTS Line: 0=not available, 1=available
#else
    p_str->capabilities.rts =  0;                       // RTS Line: 0=not available, 1=available
#endif //(RTE_USART1_CTS_ID > 0) 

#if (RTE_USART1_CTS_ID > 0)
    p_str->capabilities.cts =  1;                       // CTS Line: 0=not available, 1=available
#else
    p_str->capabilities.cts =  0;                       // CTS Line: 0=not available, 1=available
#endif //(RTE_USART1_CTS_ID > 0)       

    p_str->capabilities.dtr =  0;                       // DTR Line: 0=not available, 1=available
    p_str->capabilities.dsr =  0;                       // DSR Line: 0=not available, 1=available
    p_str->capabilities.dcd =  0;                       // DCD Line: 0=not available, 1=available
    p_str->capabilities.ri =  0;                        // RI Line: 0=not available, 1=available

#if (RTE_USART1_CTS_ID > 0)
    p_str->capabilities.event_cts = 1;                  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    p_str->capabilities.event_cts =  0;                 // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif //(RTE_USART1_CTS_ID > 0) 

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

void USART1_IRQHandler(void)
{
    USART_IRQHandler(&ARM_USART1_Resources);
}

static void USART1_cb(uint32_t event)
{
    USART_cb(event, &ARM_USART1_Resources);
}

//********************************************************************************
//Variables (continuation)
//********************************************************************************

ARM_DRIVER_USART ARM_USART1_Driver = {
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

#endif //(RTE_USART1)

/*************************************************
Driver capabilities of UART5 are completely
similar to UART4 driver capabilities.
***************************************************/
#if (RTE_UART4)

static void ARM_UART4_Resources_Struct_Init(void)
{
    ARM_USART_Resources_t *p_str = &ARM_UART4_Resources;
    p_str->capabilities.asynchronous = 1;               // supports UART (Asynchronous) mode
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
#endif //(RTE_UART4)

//================================================================================
//Public
//================================================================================

bool ARM_USART_isReady(int32_t status)
{
    return(status == ARM_DRIVER_OK);
}

int32_t ARM_USART_Init(void)
{

#if (RTE_USART1)

    ARM_DRIVER_USART *p_drv = &ARM_USART1_Driver;
    ARM_USART_Resources_t *usart = &ARM_USART1_Resources;
    ARM_USART1_Resources_Struct_Init();
    int32_t status = ARM_DRIVER_OK;
    status |= p_drv->Initialize(&USART1_cb);
    status |= p_drv->PowerControl(ARM_POWER_FULL);
    status |= p_drv->Control(ARM_USART_MODE_ASYNCHRONOUS |
                             ARM_USART_DATA_BITS_8 |
                             ARM_USART_PARITY_NONE |
                             ARM_USART_STOP_BITS_1 |
                             ARM_USART_FLOW_CONTROL_NONE,
                             57600);
    status |= p_drv->Control(ARM_USART_CONTROL_TX, TRUE);
    status |= p_drv->Control(ARM_USART_CONTROL_RX, TRUE);
    RingBuffer_Init(&USART1_TxRBuff, USART1_TxRingBuffer);
    RingBuffer_Init(&USART1_RxRBuff, USART1_RxRingBuffer);
    usart->p_reg->CR1 |= USART_CR1_RXNEIE;
    return status;

#endif //(RTE_USART1)
}

void ARM_USART_Test(void)
{

#if (RTE_USART1)

    ARM_DRIVER_USART *p_drv = &ARM_USART1_Driver;
    volatile uint8_t num_char = 16;
    uint8_t ch = 0;
    p_drv->Receive(USART1_RxRBuff.p_buff, num_char);
    while(p_drv->GetStatus().rx_busy);
    num_char = RingBuffer_GetCount(&USART1_RxRBuff);
    while(RingBuffer_GetCount(&USART1_RxRBuff)) {
        RingBuffer_ReadChar(&USART1_RxRBuff, &ch);
        RingBuffer_WriteChar(&USART1_TxRBuff, &ch);
    }
    p_drv->Send(USART1_TxRBuff.p_buff, num_char);
    while(1);

#endif //(RTE_USART1)
}
