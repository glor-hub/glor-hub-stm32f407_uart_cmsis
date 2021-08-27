//********************************************************************************
//arm_usart.c
//********************************************************************************
#include "stm32f4xx.h"
#include "discovery-kit.h"
#include "Driver_Common.h"
#include "Driver_USART.h"
#include "arm_usart.h"

//********************************************************************************
//Macros
//********************************************************************************

/* History:
 *  Version 1.0
 *    - Initial release
 */
#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)

//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************

//********************************************************************************
//Variables
//********************************************************************************

// Driver Version
static const ARM_DRIVER_VERSION ARM_USART_Driver_Version =
{ ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

#ifdef ARM_USART1_ENABLE
static ARM_DRIVER_USART ARM_USART1_Driver;
#endif//UART1_ENABLE

//********************************************************************************
//Prototypes
//********************************************************************************

#ifdef ARM_USART1_ENABLE

static void ARM_USART1_Driver_Init(void);

#endif//ARM_USART1_ENABLE

static ARM_DRIVER_VERSION ARM_USARTx_GetVersion(void);

//================================================================================
//Public
//================================================================================

void ARM_USART_Init(void)
{
#ifdef ARM_USART1_ENABLE

    ARM_USART1_Driver_Init();

#endif//ARM_USART1_ENABLE
}



//================================================================================
//Private
//================================================================================
#ifdef ARM_USART1_ENABLE

static void ARM_USART1_Driver_Init(void)
{
    ARM_USART1_Driver = {
        ARM_USARTx_GetVersion,
        /*
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
          */
    };

    /*
        void UART1_IRQHandler(void) {
            USART_IRQHandler(&USART1_Resources);
        }

    */
    static ARM_DRIVER_VERSION ARM_USARTx_GetVersion(void) {
        return ARM_USART_Driver_Version;
    }
    /*
       static ARM_USART_CAPABILITIES ARM_USART1_GetCapabilities(void) {
           return ARM_USART_GetCapabilities(&USART1_Resources);
       }
       static int32_t ARM_USART1_Initialize(ARM_USART_SignalEvent_t cb_event) {
           return ARM_USART_Initialize(cb_event, &USART1_Resources);
       }
       static int32_t ARM_USART1_Uninitialize(void) {
           return ARM_USART_Uninitialize(&USART1_Resources);
       }
       static int32_t ARM_USART1_PowerControl(ARM_POWER_STATE state) {
           return ARM_USART_PowerControl(state, &USART1_Resources);
       }
       static int32_t ARM_USART1_Send(const void *data, uint32_t num) {
           return ARM_USART_Send(data, num, &USART1_Resources);
       }
       static int32_t ARM_USART1_Receive(void *data, uint32_t num) {
           return ARM_USART_Receive(data, num, &USART1_Resources);
       }
       static int32_t ARM_USART1_Transfer(const void      *data_out,
                                          void      *data_in,
                                          uint32_t   num) {
           return ARM_USART_Transfer(data_out, data_in, num, &USART1_Resources);
       }
       static uint32_t ARM_USART1_GetTxCount(void) {
           return ARM_USART_GetTxCount(&USART1_Resources);
       }
       static uint32_t ARM_USART1_GetRxCount(void) {
           return ARM_USART_GetRxCount(&USART1_Resources);
       }
       static int32_t ARM_USART1_Control(uint32_t control, uint32_t arg) {
           return ARM_USART_Control(control, arg, &USART1_Resources);
       }
       static ARM_USART_STATUS ARM_USART1_GetStatus(void) {
           return ARM_USART_GetStatus(&USART1_Resources);
       }
       static int32_t ARM_USART1_SetModemControl(ARM_USART_MODEM_CONTROL control) {
           return ARM_USART_SetModemControl(control, &USART1_Resources);
       }
       static ARM_USART_MODEM_STATUS ARM_USART1_GetModemStatus(void) {
           return ARM_USART_GetModemStatus(&USART1_Resources);
       }
    */

#ifdef ARM_USART1_DMA_TX_ENABLE
    void ARM_USART1_GPDMA_Tx_Event(uint32_t event) {
        ARM_USART_GPDMA_Tx_Event(event, &USART1_Resources);
    }
#endif//USART1_DMA_TX_ENABLE

#ifdef ARM_USART1_DMA_RX_ENABLE
    void ARM_USART1_GPDMA_Rx_Event(uint32_t event) {
        ARM_USART_GPDMA_Rx_Event(event, &USART1_Resources);
    }
#endif//USART1_DMA_RX_ENABLE

#endif//ARM_USART1_ENABLE

