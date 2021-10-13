//********************************************************************************
//usart.c
//********************************************************************************
#include "stm32f4xx.h"
#include "common.h"
#include "RTE_Device.h"
#include "Driver_USART.h"
#include "arm_gpio.h"
#include "arm_usart.h"
#include "usart.h"

//********************************************************************************
//Macros
//********************************************************************************

//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************

//********************************************************************************
//Variables
//********************************************************************************

#if (RTE_USART1==1)
static USART_PinCfg_t USART1_PinCfg[MAX_USART_PIN_NAMES];
#endif //(RTE_USART1==1)

#if (RTE_USART2==1)
static USART_PinCfg_t USART2_PinCfg[MAX_USART_PIN_NAMES];
#endif //(RTE_USART2==1)

#if (RTE_USART3==1)
static USART_PinCfg_t USART3_PinCfg[MAX_USART_PIN_NAMES];
#endif //(RTE_USART3==1)

#if (RTE_UART4==1)
static USART_PinCfg_t UART4_PinCfg[MAX_USART_PIN_NAMES];
#endif //(RTE_UART4==1)

#if (RTE_UART5==1)
static USART_PinCfg_t UART5_PinCfg[MAX_USART_PIN_NAMES];
#endif //(RTE_UART5==1)

#if (RTE_USART6==1)
static USART_PinCfg_t USART6_PinCfg[MAX_USART_PIN_NAMES];
#endif //(RTE_USART6==1)

//********************************************************************************
//Prototypes
//********************************************************************************
static void USART_SetPinCfg(void);
//================================================================================
//Public
//================================================================================



void USART_Init(void)
{
    USART_SetPinCfg();
    ARM_USART_Init();
}

USART_PinCfg_t *USART_GetPinCfg(ePeriphTypes usart_name)
{
    USART_PinCfg_t *pUSART_PinCfg;

    switch(usart_name) {
#if (RTE_USART1==1)
        case USART_1: {
            pUSART_PinCfg = USART1_PinCfg;
        }
#endif //(RTE_USART1==1)

#if (RTE_USART2==1)
        case USART_2: {
            pUSART_PinCfg = USART2_PinCfg;
        }
#endif //(RTE_USART2==1)

#if (RTE_USART3==1)
        case USART_3: {
            pUSART_PinCfg = USART3_PinCfg;
        }
#endif //(RTE_USART3==1)

#if (RTE_UART4==1)
        case UART_4: {
            pUSART_PinCfg = UART4_PinCfg;
        }
#endif //(RTE_UART4==1)

#ifdef USART5_ENABLE
        case UART_5: {
            pUSART_PinCfg = UART5_PinCfg;
        }
#endif //USART5_ENABLE

#ifdef UART6_ENABLE
        case USART_6: {
            pUSART_PinCfg = USART6_PinCfg;
        }
#endif //UART6_ENABLE

        default: {
            break;
        }

    }

    return pUSART_PinCfg;
}

#if (RTE_USART1==1)
void USART1_IRQHandler(void)
{
//    USART_IRQHandler(&USART1_Resources);
}
void USART1_cb(uint32_t event)
{

}
#endif //(RTE_USART1==1)

#if (RTE_UART4==1)
void UART4_IRQHandler(void)
{
//    USART_IRQHandler(&UART4_Resources);
}
void UART4_cb(uint32_t event)
{

}
#endif //(RTE_UART4==1)

//================================================================================
//Private
//================================================================================

static void USART_SetPinCfg(void)
{
#if (RTE_USART1==1)

#if (RTE_USART1_TX_ID == 0)
//if ID==0, then the USART1_TX Pin not used"
#elif (RTE_USART1_TX_ID == 1)
    USART1_PinCfg[TX_PIN].GPIOx = RTE_USART1_TX_PORT;
    USART1_PinCfg[TX_PIN].port = GPIO_PORT_A;
    USART1_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART1_TX_BIT;
    USART1_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART1_TX_ID == 2)
#error "Invalid USART1_TX Pin Configuration"
#elif (RTE_USART1_TX_ID == 3)
    USART1_PinCfg[TX_PIN].GPIOx = RTE_USART1_TX_PORT;
    USART1_PinCfg[TX_PIN].port = GPIO_PORT_B;
    USART1_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART1_TX_BIT;
    USART1_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_TX Pin Configuration"
#endif //(RTE_USART1_TX_ID == 0)

#if (RTE_USART1_RX_ID == 0)
//if ID==0, then the USART1_RX Pin not used"
#elif (RTE_USART1_RX_ID == 1)
    USART1_PinCfg[RX_PIN].GPIOx = RTE_USART1_RX_PORT;
    USART1_PinCfg[RX_PIN].port = GPIO_PORT_A;
    USART1_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART1_RX_BIT;
    USART1_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART1_RX_ID == 2)
#error "Invalid USART1_RX Pin Configuration"
#elif (RTE_USART1_RX_ID == 3)
    USART1_PinCfg[RX_PIN].GPIOx = RTE_USART1_RX_PORT;
    USART1_PinCfg[RX_PIN].port = GPIO_PORT_B;
    USART1_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART1_RX_BIT;
    USART1_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_RX Pin Configuration"
#endif //(RTE_USART1_RX_ID == 0)

#if (RTE_USART1_CK_ID == 0)
//if ID==0, then the USART1_CK Pin not used"
#elif (RTE_USART1_CK_ID == 1)
    USART1_PinCfg[CK_PIN].GPIOx = RTE_USART1_CK_PORT;
    USART1_PinCfg[CK_PIN].port = GPIO_PORT_A;
    USART1_PinCfg[CK_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART1_CK_BIT;
    USART1_PinCfg[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_CK Pin Configuration"
#endif //(RTE_USART1_CK_ID == 0)

#if (RTE_USART1_CTS_ID == 0)
//if ID==0, then the USART1_CTS Pin not used"
#elif (RTE_USART1_CTS_ID == 1)
    USART1_PinCfg[CTS_PIN].GPIOx = RTE_USART1_CTS_PORT;
    USART1_PinCfg[CTS_PIN].port = GPIO_PORT_A;
    USART1_PinCfg[CTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART1_CTS_BIT;
    USART1_PinCfg[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_CTS Pin Configuration"
#endif //(RTE_USART1_CTS_ID == 0)

#if (RTE_USART1_RTS_ID == 0)
//if ID==0, then the USART1_RTS Pin not used"
#elif (RTE_USART1_RTS_ID == 1)
    USART1_PinCfg[RTS_PIN].GPIOx = RTE_USART1_RTS_PORT;
    USART1_PinCfg[RTS_PIN].port = GPIO_PORT_A;
    USART1_PinCfg[RTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART1_RTS_BIT;
    USART1_PinCfg[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_RTS Pin Configuration"
#endif //(RTE_USART1_RTS_ID == 0)

#endif //(RTE_USART1 == 1)

#if (RTE_USART2 == 1)

#if (RTE_USART2_TX_ID == 0)
//if ID==0, then the USART2_TX Pin not used"
#elif (RTE_USART2_TX_ID == 1)
    USART2_PinCfg[TX_PIN].GPIOx = RTE_USART2_TX_PORT;
    USART2_PinCfg[TX_PIN].port = GPIO_PORT_A;
    USART2_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_TX_BIT;
    USART2_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_TX_ID == 2)
    USART2_PinCfg[TX_PIN].GPIOx = RTE_USART2_TX_PORT;
    USART2_PinCfg[TX_PIN].port = GPIO_PORT_B;
    USART2_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_TX_BIT;
    USART2_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_TX Pin Configuration"
#endif //(RTE_USART2_TX_ID == 0)

#if (RTE_USART2_RX_ID == 0)
//if ID==0, then the USART2_RX Pin not used"
#elif (RTE_USART2_RX_ID == 1)
    USART2_PinCfg[RX_PIN].GPIOx = RTE_USART2_RX_PORT;
    USART2_PinCfg[RX_PIN].port = GPIO_PORT_A;
    USART2_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_RX_BIT;
    USART2_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_RX_ID == 2)
    USART2_PinCfg[RX_PIN].GPIOx = RTE_USART2_RX_PORT;
    USART2_PinCfg[RX_PIN].port = GPIO_PORT_B;
    USART2_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_RX_BIT;
    USART2_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_RX Pin Configuration"
#endif //(RTE_USART2_RX_ID == 0)

#if (RTE_USART2_CK_ID == 0)
//if ID==0, then the USART2_CK Pin not used"
#elif (RTE_USART2_CK_ID == 1)
    USART2_PinCfg[CK_PIN].GPIOx = RTE_USART2_CK_PORT;
    USART2_PinCfg[CK_PIN].port = GPIO_PORT_A;
    USART2_PinCfg[CK_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_CK_BIT;
    USART2_PinCfg[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_CK_ID == 2)
    USART2_PinCfg[CK_PIN].GPIOx = RTE_USART2_CK_PORT;
    USART2_PinCfg[CK_PIN].port = GPIO_PORT_D;
    USART2_PinCfg[CK_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_CK_BIT;
    USART2_PinCfg[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_CK Pin Configuration"
#endif //(RTE_USART2_CK_ID == 0)

#if (RTE_USART2_CTS_ID == 0)
//if ID==0, then the USART2_CTS Pin not used"
#elif (RTE_USART2_CTS_ID == 1)
    USART2_PinCfg[CTS_PIN].GPIOx = RTE_USART2_CTS_PORT;
    USART2_PinCfg[CTS_PIN].port = GPIO_PORT_A;
    USART2_PinCfg[CTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_CTS_BIT;
    USART2_PinCfg[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_CTS_ID == 2)
    USART2_PinCfg[CTS_PIN].GPIOx = RTE_USART2_CTS_PORT;
    USART2_PinCfg[CTS_PIN].port = GPIO_PORT_D;
    USART2_PinCfg[CTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_CTS_BIT;
    USART2_PinCfg[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_CTS Pin Configuration"
#endif //(RTE_USART2_CTS_ID == 0)

#if (RTE_USART2_RTS_ID == 0)
//if ID==0, then the USART2_RTS Pin not used"
#elif (RTE_USART2_RTS_ID == 1)
    USART2_PinCfg[RTS_PIN].GPIOx = RTE_USART2_RTS_PORT;
    USART2_PinCfg[RTS_PIN].port = GPIO_PORT_A;
    USART2_PinCfg[RTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_RTS_BIT;
    USART2_PinCfg[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_RTS_ID == 2)
    USART2_PinCfg[RTS_PIN].GPIOx = RTE_USART2_RTS_PORT;
    USART2_PinCfg[RTS_PIN].port = GPIO_PORT_D;
    USART2_PinCfg[RTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART2_RTS_BIT;
    USART2_PinCfg[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_RTS Pin Configuration"
#endif //(RTE_USART2_RTS_ID == 0)

#endif //(RTE_USART2==1)

#if (RTE_USART3 == 1)

#if (RTE_USART3_TX_ID == 0)
//if ID==0, then the USART3_TX Pin not used"
#elif (RTE_USART3_TX_ID == 1)
    USART3_PinCfg[TX_PIN].GPIOx = RTE_USART3_TX_PORT;
    USART3_PinCfg[TX_PIN].port = GPIO_PORT_A;
    USART3_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_TX_BIT;
    USART3_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_TX_ID == 2)
    USART3_PinCfg[TX_PIN].GPIOx = RTE_USART3_TX_PORT;
    USART3_PinCfg[TX_PIN].port = GPIO_PORT_C;
    USART3_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_TX_BIT;
    USART3_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_TX_ID == 3)
    USART3_PinCfg[TX_PIN].GPIOx = RTE_USART3_TX_PORT;
    USART3_PinCfg[TX_PIN].port = GPIO_PORT_D;
    USART3_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_TX_BIT;
    USART3_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART3_TX Pin Configuration"
#endif //(RTE_USART3_TX_ID == 0)

#if (RTE_USART3_RX_ID == 0)
//if ID==0, then the USART3_RX Pin not used"
#elif (RTE_USART3_RX_ID == 1)
    USART3_PinCfg[RX_PIN].GPIOx = RTE_USART3_RX_PORT;
    USART3_PinCfg[RX_PIN].port = GPIO_PORT_B;
    USART3_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_RX_BIT;
    USART3_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_RX_ID == 2)
    USART3_PinCfg[RX_PIN].GPIOx = RTE_USART3_RX_PORT;
    USART3_PinCfg[RX_PIN].port = GPIO_PORT_C;
    USART3_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_RX_BIT;
    USART3_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_RX_ID == 3)
    USART3_PinCfg[RX_PIN].GPIOx = RTE_USART3_RX_PORT;
    USART3_PinCfg[RX_PIN].port = GPIO_PORT_D;
    USART3_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_RX_BIT;
    USART3_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_RX_ID == 4)
#error "Invalid USART3_RX Pin Configuration"
#else
#error "Invalid USART3_RX Pin Configuration"
#endif // (RTE_USART3_RX_ID == 0)

#if (RTE_USART3_CK_ID == 0)
//if ID==0, then the USART3_CK Pin not used"
#elif (RTE_USART3_CK_ID == 1)
    USART3_PinCfg[CK_PIN].GPIOx = RTE_USART3_CK_PORT;
    USART3_PinCfg[CK_PIN].port = GPIO_PORT_B;
    USART3_PinCfg[CK_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_CK_BIT;
    USART3_PinCfg[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_CK_ID == 2)
    USART3_PinCfg[CK_PIN].GPIOx = RTE_USART3_CK_PORT;
    USART3_PinCfg[CK_PIN].port = GPIO_PORT_C;
    USART3_PinCfg[CK_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_CK_BIT;
    USART3_PinCfg[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_CK_ID == 3)
    USART3_PinCfg[CK_PIN].GPIOx = RTE_USART3_CK_PORT;
    USART3_PinCfg[CK_PIN].port = GPIO_PORT_D;
    USART3_PinCfg[CK_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_CK_BIT;
    USART3_PinCfg[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART3_CK Pin Configuration"
#endif //(RTE_USART3_CK_ID == 0)

#if (RTE_USART3_CTS_ID == 0)
//if ID==0, then the USART3_CTS Pin not used"
#elif (RTE_USART3_CTS_ID == 1)
    USART3_PinCfg[CTS_PIN].GPIOx = RTE_USART3_CTS_PORT;
    USART3_PinCfg[CTS_PIN].port = GPIO_PORT_B;
    USART3_PinCfg[CTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_CTS_BIT;
    USART3_PinCfg[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_CTS_ID == 2)
    USART3_PinCfg[CTS_PIN].GPIOx = RTE_USART3_CTS_PORT;
    USART3_PinCfg[CTS_PIN].port = GPIO_PORT_D;
    USART3_PinCfg[CTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_CTS_BIT;
    USART3_PinCfg[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART3_CTS Pin Configuration"
#endif //(RTE_USART3_CTS_ID == 0)

#if (RTE_USART3_RTS_ID == 0)
//if ID==0, then the USART3_RTS Pin not used"
#elif (RTE_USART3_RTS_ID == 1)
    USART3_PinCfg[RTS_PIN].GPIOx = RTE_USART3_RTS_PORT;
    USART3_PinCfg[RTS_PIN].port = GPIO_PORT_B;
    USART3_PinCfg[RTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_RTS_BIT;
    USART3_PinCfg[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_RTS_ID == 2)
    USART3_PinCfg[RTS_PIN].GPIOx = RTE_USART3_RTS_PORT;
    USART3_PinCfg[RTS_PIN].port = GPIO_PORT_D;
    USART3_PinCfg[RTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART3_RTS_BIT;
    USART3_PinCfg[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART3_RTS Pin Configuration"
#endif //(RTE_USART3_RTS_ID == 0)

#endif //(RTE_USART3==1)

#if (RTE_UART4==1)

#if (RTE_UART4_TX_ID == 0)
#error "The UART4_TX Pin not used"
#elif (RTE_UART4_TX_ID == 1)
    UART4_PinCfg[TX_PIN].GPIOx = RTE_UART4_TX_PORT;
    UART4_PinCfg[TX_PIN].port = GPIO_PORT_A;
    UART4_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_UART4_TX_BIT;
    UART4_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART4_TX_ID == 2)
    UART4_PinCfg[TX_PIN].GPIOx = RTE_UART4_TX_PORT;
    UART4_PinCfg[TX_PIN].port = GPIO_PORT_C;
    UART4_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_UART4_TX_BIT;
    UART4_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART4_TX_ID == 3)
#error "Invalid UART4_TX Pin Configuration"
#elif (RTE_UART4_TX_ID == 4)
#error "Invalid UART4_TX Pin Configuration"
#elif (RTE_UART4_TX_ID == 5)
#error "Invalid UART4_TX Pin Configuration"
#else
#error "Invalid UART4_TX Pin Configuration"
#endif //(RTE_UART4_TX_ID == 0)

#if (RTE_UART4_RX_ID == 0)
#error "The UART4_RX Pin not used"
#elif (RTE_UART4_RX_ID == 1)
    UART4_PinCfg[RX_PIN].GPIOx = RTE_UART4_RX_PORT;
    UART4_PinCfg[RX_PIN].port = GPIO_PORT_A;
    UART4_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_UART4_RX_BIT;
    UART4_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART4_RX_ID == 2)
    UART4_PinCfg[RX_PIN].GPIOx = RTE_UART4_RX_PORT;
    UART4_PinCfg[RX_PIN].port = GPIO_PORT_C;
    UART4_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_UART4_RX_BIT;
    UART4_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART4_RX_ID == 3)
#error "Invalid UART4_RX Pin Configuration"
#elif (RTE_UART4_RX_ID == 4)
#error "Invalid UART4_RX Pin Configuration"
#else
#error "Invalid UART4_RX Pin Configuration"
#endif //(RTE_UART4_RX_ID == 0)

#endif //(RTE_UART4==1)

#if (RTE_UART5==1)

#if (RTE_UART5_TX_ID == 0)
#error "The UART5_TX Pin not used"
#elif (RTE_UART5_TX_ID == 1)
    UART5_PinCfg[TX_PIN].GPIOx = RTE_UART5_TX_PORT;
    UART5_PinCfg[TX_PIN].port = GPIO_PORT_C;
    UART5_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_UART5_TX_BIT;
    UART5_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART5_TX_ID == 2)
#error "Invalid UART5_TX Pin Configuration"
#elif (RTE_UART5_TX_ID == 3)
#error "Invalid UART5_TX Pin Configuration"
#elif (RTE_UART5_TX_ID == 4)
#error "Invalid UART5_TX Pin Configuration"
#else
#error "Invalid UART5_TX Pin Configuration"
#endif // (RTE_UART5_TX_ID == 0)

#if (RTE_UART5_RX_ID == 0)
#error "The UART5_RX Pin not used"
#elif (RTE_UART5_RX_ID == 1)
    UART5_PinCfg[RX_PIN].GPIOx = RTE_UART5_RX_PORT;
    UART5_PinCfg[RX_PIN].port = GPIO_PORT_D;
    UART5_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_UART5_RX_BIT;
    UART5_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART5_RX_ID == 2)
#error "Invalid UART5_RX Pin Configuration"
#elif (RTE_UART5_RX_ID == 3)
#error "Invalid UART5_RX Pin Configuration"
#elif (RTE_UART5_RX_ID == 4)
#error "Invalid UART5_RX Pin Configuration"
#else
#error "Invalid UART5_RX Pin Configuration"
#endif // (RTE_UART5_RX_ID == 0)

#endif //(RTE_UART5==1)

#if (RTE_USART6 == 1)

#if (RTE_USART6_TX_ID == 0)
//if ID==0, then the USART6_TX Pin not used"
#elif (RTE_USART6_TX_ID == 1)
#error "Invalid USART6_TX Pin Configuration"
#elif (RTE_USART6_TX_ID == 2)
    USART6_PinCfg[TX_PIN].GPIOx = RTE_USART6_TX_PORT;
    USART6_PinCfg[TX_PIN].port = GPIO_PORT_C;
    USART6_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_TX_BIT;
    USART6_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_TX_ID == 3)
    USART6_PinCfg[TX_PIN].GPIOx = RTE_USART6_TX_PORT;
    USART6_PinCfg[TX_PIN].port = GPIO_PORT_G;
    USART6_PinCfg[TX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_TX_BIT;
    USART6_PinCfg[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_TX Pin Configuration"
#endif //(RTE_USART6_TX_ID == 0)

#if (RTE_USART6_RX_ID == 0)
//if ID==0, then the USART6_RX Pin not used"
#elif (RTE_USART6_RX_ID == 1)
#error "Invalid USART6_RX Pin Configuration"
#elif (RTE_USART6_RX_ID == 2)
    USART6_PinCfg[RX_PIN].GPIOx = RTE_USART6_RX_PORT;
    USART6_PinCfg[RX_PIN].port = GPIO_PORT_C;
    USART6_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_RX_BIT;
    USART6_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_RX_ID == 3)
    USART6_PinCfg[RX_PIN].GPIOx = RTE_USART6_RX_PORT;
    USART6_PinCfg[RX_PIN].port = GPIO_PORT_G;
    USART6_PinCfg[RX_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_RX_BIT;
    USART6_PinCfg[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_RX Pin Configuration"
#endif //(RTE_USART6_RX_ID == 0)

#if (RTE_USART6_CK_ID == 0)
//if ID==0, then the USART6_CK Pin not used"
#elif (RTE_USART6_CK_ID == 1)
    USART6_PinCfg[CK_PIN].GPIOx = RTE_USART6_CK_PORT;
    USART6_PinCfg[CK_PIN].port = GPIO_PORT_Ñ;
    USART6_PinCfg[CK_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_CK_BIT;
    USART6_PinCfg[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_CK_ID == 2)
    USART6_PinCfg[CK_PIN].GPIOx = RTE_USART6_CK_PORT;
    USART6_PinCfg[CK_PIN].port = GPIO_PORT_G;
    USART6_PinCfg[CK_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_CK_BIT;
    USART6_PinCfg[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_CK Pin Configuration"
#endif //(RTE_USART6_CK_ID == 0)

#if (RTE_USART6_CTS_ID == 0)
//if ID==0, then the USART6_CTS Pin not used"
#elif (RTE_USART6_CTS_ID == 1)
    USART6_PinCfg[CTS_PIN].GPIOx = RTE_USART6_CTS_PORT;
    USART6_PinCfg[CTS_PIN].port = GPIO_PORT_G;
    USART6_PinCfg[CTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_CTS_BIT;
    USART6_PinCfg[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_CTS_ID == 2)
    USART6_PinCfg[CTS_PIN].GPIOx = RTE_USART6_CTS_PORT;
    USART6_PinCfg[CTS_PIN].port = GPIO_PORT_G;
    USART6_PinCfg[CTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_CTS_BIT;
    USART6_PinCfg[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_CTS Pin Configuration"
#endif //(RTE_USART6_CTS_ID == 0)

#if (RTE_USART6_RTS_ID == 0)
//if ID==0 the USART6_RTS Pin not used"
#elif (RTE_USART6_RTS_ID == 1)
    USART6_PinCfg[RTS_PIN].GPIOx = RTE_USART6_RTS_PORT;
    USART6_PinCfg[RTS_PIN].port = GPIO_PORT_G;
    USART6_PinCfg[RTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_RTS_BIT;
    USART6_PinCfg[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_RTS_ID == 2)
    USART6_PinCfg[RTS_PIN].GPIOx = RTE_USART6_RTS_PORT;
    USART6_PinCfg[RTS_PIN].port = GPIO_PORT_G;
    USART6_PinCfg[RTS_PIN].pin = (eARM_GPIO_IONumbers)RTE_USART6_RTS_BIT;
    USART6_PinCfg[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_RTS Pin Configuration"
#endif //(RTE_USART6_RTS_ID == 0)

#endif //(RTE_USART6==1)

}
