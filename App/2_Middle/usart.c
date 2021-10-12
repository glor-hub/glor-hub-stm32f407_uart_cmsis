//********************************************************************************
//usart.c
//********************************************************************************
#include "stm32f4xx.h"
#include "common.h"
#include "RTE_Device.h"
#include "Driver_USART.h"
#include "arm_gpio.h"
#include "gpio.h"
#include "usart.h"
#include "arm_usart.h"

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
static USART_PinCfg_t USART1_PinConfig[MAX_USART_PIN_NAMES];
#endif //(RTE_USART1==1)

#if (RTE_USART2==1)
static USART_PinCfg_t USART2_PinConfig[MAX_USART_PIN_NAMES];
#endif //(RTE_USART2==1)

#if (RTE_USART3==1)
static USART_PinCfg_t USART3_PinConfig[MAX_USART_PIN_NAMES];
#endif //(RTE_USART3==1)

#if (RTE_UART4==1)
static USART_PinCfg_t UART4_PinConfig[MAX_USART_PIN_NAMES];
#endif //(RTE_UART4==1)

#if (RTE_UART5==1)
static USART_PinCfg_t UART5_PinConfig[MAX_USART_PIN_NAMES];
#endif //(RTE_UART5==1)

#if (RTE_USART6==1)
static USART_PinCfg_t USART6_PinConfig[MAX_UART_PIN_NAMES];
#endif //(RTE_USART6==1)

//********************************************************************************
//Prototypes
//********************************************************************************
static void USART_SetPinConfig(void);
//================================================================================
//Public
//================================================================================



void USART_Init(void)
{
    USART_SetPinConfig();
    ARM_USART_Init();

}

USART_PinCfg_t *USART_GetPinConfig(ePeriphTypes usart_name)
{
    USART_PinCfg_t *pUSART_PinCfg = NULL;

    switch(usart_name) {
#if (RTE_USART1==1)
        case USART_1: {
            pUSART_PinCfg = USART1_PinConfig;
        }
#endif //(RTE_USART1==1)

#if (RTE_USART2==1)
        case USART_2: {
            pUSART_PinCfg = USART2_PinConfig;
        }
#endif //(RTE_USART2==1)

#if (RTE_USART3==1)
        case USART_3: {
            pUSART_PinCfg = USART3_PinConfig;
        }
#endif //(RTE_USART3==1)

#if (RTE_UART4==1)
        case UART_4: {
            pUSART_PinCfg = UART4_PinConfig;
        }
#endif //(RTE_UART4==1)

#ifdef USART5_ENABLE
        case UART_5: {
            pUSART_PinCfg = USART5_PinConfig;
        }
#endif //USART5_ENABLE

#ifdef UART6_ENABLE
        case USART_6: {
            pUSART_PinCfg = UART6_PinConfig;
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

static void USART_SetPinConfig(void)
{
#if (RTE_USART1==1)

#if (RTE_USART1_TX_ID == 0)
#error "The USART1_TX Pin Configuration not used"
#elif (RTE_USART1_TX_ID == 1)
    USART1_PinConfig[TX_PIN].GPIOx = RTE_USART1_TX_PORT;
    USART1_PinConfig[TX_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[TX_PIN].pin = RTE_USART1_TX_BIT;
    USART1_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART1_TX_ID == 2)
#error "The USART1_TX Pin Configuration not used"
#elif (RTE_USART1_TX_ID == 3)
    USART1_PinConfig[TX_PIN].GPIOx = RTE_USART1_TX_PORT;
    USART1_PinConfig[TX_PIN].port = GPIO_PORT_B;
    USART1_PinConfig[TX_PIN].pin = RTE_USART1_TX_BIT;
    USART1_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_TX Pin Configuration"
#endif //(RTE_USART1_TX_ID == 0)

#if (RTE_USART1_RX_ID == 0)
#error "The USART1_RX Pin Configuration not used"
#elif (RTE_USART1_RX_ID == 1)
    USART1_PinConfig[RX_PIN].GPIOx = RTE_USART1_RX_PORT;
    USART1_PinConfig[RX_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[RX_PIN].pin = RTE_USART1_RX_BIT;
    USART1_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART1_RX_ID == 2)
#error "The USART1_RX Pin Configuration not used"
#elif (RTE_USART1_RX_ID == 3)
    USART1_PinConfig[RX_PIN].GPIOx = RTE_USART1_RX_PORT;
    USART1_PinConfig[RX_PIN].port = GPIO_PORT_B;
    USART1_PinConfig[RX_PIN].pin = RTE_USART1_RX_BIT;
    USART1_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_RX Pin Configuration"
#endif //(RTE_USART1_RX_ID == 0)

#if (RTE_USART1_CK_ID == 0)
#error "The USART1_CK Pin Configuration not used"
#elif (RTE_USART1_CK_ID == 1)
    USART1_PinConfig[CK_PIN].GPIOx = RTE_USART1_CK_PORT;
    USART1_PinConfig[CK_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[CK_PIN].pin = RTE_USART1_CK_BIT;
    USART1_PinConfig[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_CK Pin Configuration"
#endif //(RTE_USART1_CK_ID == 0)

#if (RTE_USART1_CTS_ID == 0)
#error "The USART1_CTS Pin Configuration not used"
#elif (RTE_USART1_CTS_ID == 1)
    USART1_PinConfig[CTS_PIN].GPIOx = RTE_USART1_CTS_PORT;
    USART1_PinConfig[CTS_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[CTS_PIN].pin = RTE_USART1_CTS_BIT;
    USART1_PinConfig[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_CTS Pin Configuration"
#endif //(RTE_USART1_CTS_ID == 0)

#if (RTE_USART1_RTS_ID == 0)
#error "The USART1_RTS Pin Configuration not used"
#elif (RTE_USART1_RTS_ID == 1)
    USART1_PinConfig[RTS_PIN].GPIOx = RTE_USART1_RTS_PORT;
    USART1_PinConfig[RTS_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[RTS_PIN].pin = RTE_USART1_RTS_BIT;
    USART1_PinConfig[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART1_RTS Pin Configuration"
#endif //(RTE_USART1_RTS_ID == 0)

#endif //(RTE_USART1 == 1)

#if (RTE_USART2 == 1)

#if (RTE_USART2_TX_ID == 0)
#error "The USART2_TX Pin Configuration not used"
#elif (RTE_USART2_TX_ID == 1)
    USART2_PinConfig[TX_PIN].GPIOx = RTE_USART2_TX_PORT;
    USART2_PinConfig[TX_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[TX_PIN].pin = RTE_USART2_TX_BIT;
    USART2_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_TX_ID == 2)
    USART2_PinConfig[TX_PIN].GPIOx = RTE_USART2_TX_PORT;
    USART2_PinConfig[TX_PIN].port = GPIO_PORT_B;
    USART2_PinConfig[TX_PIN].pin = RTE_USART2_TX_BIT;
    USART2_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_TX Pin Configuration"
#endif //(RTE_USART2_TX_ID == 0)

#if (RTE_USART2_RX_ID == 0)
#error "The USART2_RX Pin Configuration not used"
#elif (RTE_USART2_RX_ID == 1)
    USART2_PinConfig[RX_PIN].GPIOx = RTE_USART2_RX_PORT;
    USART2_PinConfig[RX_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[RX_PIN].pin = RTE_USART2_RX_BIT;
    USART2_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_RX_ID == 2)
    USART2_PinConfig[RX_PIN].GPIOx = RTE_USART2_RX_PORT;
    USART2_PinConfig[RX_PIN].port = GPIO_PORT_B;
    USART2_PinConfig[RX_PIN].pin = RTE_USART2_RX_BIT;
    USART2_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_RX Pin Configuration"
#endif //(RTE_USART2_RX_ID == 0)

#if (RTE_USART2_CK_ID == 0)
#error "The USART2_CK Pin Configuration not used"
#elif (RTE_USART2_CK_ID == 1)
    USART2_PinConfig[CK_PIN].GPIOx = RTE_USART2_CK_PORT;
    USART2_PinConfig[CK_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[CK_PIN].pin = RTE_USART2_CK_BIT;
    USART2_PinConfig[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_CK_ID == 2)
    USART2_PinConfig[CK_PIN].GPIOx = RTE_USART2_CK_PORT;
    USART2_PinConfig[CK_PIN].port = GPIO_PORT_D;
    USART2_PinConfig[CK_PIN].pin = RTE_USART2_CK_BIT;
    USART2_PinConfig[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_CK Pin Configuration"
#endif //(RTE_USART2_CK_ID == 0)

#if (RTE_USART2_CTS_ID == 0)
#error "The USART2_CTS Pin Configuration not used"
#elif (RTE_USART2_CTS_ID == 1)
    USART2_PinConfig[CTS_PIN].GPIOx = RTE_USART2_CTS_PORT;
    USART2_PinConfig[CTS_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[CTS_PIN].pin = RTE_USART2_CTS_BIT;
    USART2_PinConfig[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_CTS_ID == 2)
    USART2_PinConfig[CTS_PIN].GPIOx = RTE_USART2_CTS_PORT;
    USART2_PinConfig[CTS_PIN].port = GPIO_PORT_D;
    USART2_PinConfig[CTS_PIN].pin = RTE_USART2_CTS_BIT;
    USART2_PinConfig[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_CTS Pin Configuration"
#endif //(RTE_USART2_CTS_ID == 0)

#if (RTE_USART2_RTS_ID == 0)
#error "The USART2_RTS Pin Configuration not used"
#elif (RTE_USART2_RTS_ID == 1)
    USART2_PinConfig[RTS_PIN].GPIOx = RTE_USART2_RTS_PORT;
    USART2_PinConfig[RTS_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[RTS_PIN].pin = RTE_USART2_RTS_BIT;
    USART2_PinConfig[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART2_RTS_ID == 2)
    USART2_PinConfig[RTS_PIN].GPIOx = RTE_USART2_RTS_PORT;
    USART2_PinConfig[RTS_PIN].port = GPIO_PORT_D;
    USART2_PinConfig[RTS_PIN].pin = RTE_USART2_RTS_BIT;
    USART2_PinConfig[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART2_RTS Pin Configuration"
#endif //(RTE_USART2_RTS_ID == 0)

#endif //(RTE_USART2==1)

#if (RTE_USART3 == 1)

#if (RTE_USART3_TX_ID == 0)
#error "The USART3_TX Pin Configuration not used"
#elif (RTE_USART3_TX_ID == 1)
    USART3_PinConfig[TX_PIN].GPIOx = RTE_USART3_TX_PORT;
    USART3_PinConfig[TX_PIN].port = GPIO_PORT_A;
    USART3_PinConfig[TX_PIN].pin = RTE_USART3_TX_BIT;
    USART3_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_TX_ID == 2)
    USART3_PinConfig[TX_PIN].GPIOx = RTE_USART3_TX_PORT;
    USART3_PinConfig[TX_PIN].port = GPIO_PORT_C;
    USART3_PinConfig[TX_PIN].pin = RTE_USART3_TX_BIT;
    USART3_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_TX_ID == 3)
    USART3_PinConfig[TX_PIN].GPIOx = RTE_USART3_TX_PORT;
    USART3_PinConfig[TX_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[TX_PIN].pin = RTE_USART3_TX_BIT;
    USART3_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART3_TX Pin Configuration"
#endif //(RTE_USART3_TX_ID == 0)

#if (RTE_USART3_RX_ID == 0)
#error "The USART3_RX Pin Configuration not used"
#elif (RTE_USART3_RX_ID == 1)
    USART3_PinConfig[RX_PIN].GPIOx = RTE_USART3_RX_PORT;
    USART3_PinConfig[RX_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[RX_PIN].pin = RTE_USART3_RX_BIT;
    USART3_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_RX_ID == 2)
    USART3_PinConfig[RX_PIN].GPIOx = RTE_USART3_RX_PORT;
    USART3_PinConfig[RX_PIN].port = GPIO_PORT_C;
    USART3_PinConfig[RX_PIN].pin = RTE_USART3_RX_BIT;
    USART3_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_RX_ID == 3)
    USART3_PinConfig[RX_PIN].GPIOx = RTE_USART3_RX_PORT;
    USART3_PinConfig[RX_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[RX_PIN].pin = RTE_USART3_RX_BIT;
    USART3_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_RX_ID == 4)
#error "The USART3_RX Pin Configuration not used"
#else
#error "Invalid USART3_RX Pin Configuration"
#endif // (RTE_USART3_RX_ID == 0)

#if (RTE_USART3_CK_ID == 0)
#error "The USART3_CK Pin Configuration not used"
#elif (RTE_USART3_CK_ID == 1)
    USART3_PinConfig[CK_PIN].GPIOx = RTE_USART3_CK_PORT;
    USART3_PinConfig[CK_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[CK_PIN].pin = RTE_USART3_CK_BIT;
    USART3_PinConfig[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_CK_ID == 2)
    USART3_PinConfig[CK_PIN].GPIOx = RTE_USART3_CK_PORT;
    USART3_PinConfig[CK_PIN].port = GPIO_PORT_C;
    USART3_PinConfig[CK_PIN].pin = RTE_USART3_CK_BIT;
    USART3_PinConfig[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_CK_ID == 3)
    USART3_PinConfig[CK_PIN].GPIOx = RTE_USART3_CK_PORT;
    USART3_PinConfig[CK_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[CK_PIN].pin = RTE_USART3_CK_BIT;
    USART3_PinConfig[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART3_CK Pin Configuration"
#endif //(RTE_USART3_CK_ID == 0)

#if (RTE_USART3_CTS_ID == 0)
#error "The USART3_CTS Pin Configuration not used"
#elif (RTE_USART3_CTS_ID == 1)
    USART3_PinConfig[CTS_PIN].GPIOx = RTE_USART3_CTS_PORT;
    USART3_PinConfig[CTS_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[CTS_PIN].pin = RTE_USART3_CTS_BIT;
    USART3_PinConfig[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_CTS_ID == 2)
    USART3_PinConfig[CTS_PIN].GPIOx = RTE_USART3_CTS_PORT;
    USART3_PinConfig[CTS_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[CTS_PIN].pin = RTE_USART3_CTS_BIT;
    USART3_PinConfig[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART3_CTS Pin Configuration"
#endif //(RTE_USART3_CTS_ID == 0)

#if (RTE_USART3_RTS_ID == 0)
#error "The USART3_RTS Pin Configuration not used"
#elif (RTE_USART3_RTS_ID == 1)
    USART3_PinConfig[RTS_PIN].GPIOx = RTE_USART3_RTS_PORT;
    USART3_PinConfig[RTS_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[RTS_PIN].pin = RTE_USART3_RTS_BIT;
    USART3_PinConfig[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART3_RTS_ID == 2)
    USART3_PinConfig[RTS_PIN].GPIOx = RTE_USART3_RTS_PORT;
    USART3_PinConfig[RTS_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[RTS_PIN].pin = RTE_USART3_RTS_BIT;
    USART3_PinConfig[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART3_RTS Pin Configuration"
#endif //(RTE_USART3_RTS_ID == 0)

#endif //(RTE_USART3==1)

#if (RTE_UART4==1)

#if (RTE_UART4_TX_ID == 0)
#error "The UART4_TX Pin Configuration not used"
#elif (RTE_UART4_TX_ID == 1)
    UART4_PinConfig[TX_PIN].GPIOx = RTE_UART4_TX_PORT;
    UART4_PinConfig[TX_PIN].port = GPIO_PORT_A;
    UART4_PinConfig[TX_PIN].pin = RTE_UART4_TX_BIT;
    UART4_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART4_TX_ID == 2)
    UART4_PinConfig[TX_PIN].GPIOx = RTE_UART4_TX_PORT;
    UART4_PinConfig[TX_PIN].port = GPIO_PORT_C;
    UART4_PinConfig[TX_PIN].pin = RTE_UART4_TX_BIT;
    UART4_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART4_TX_ID == 3)
#error "The UART4_TX Pin Configuration not used"
#elif (RTE_UART4_TX_ID == 4)
#error "The UART4_TX Pin Configuration not used"
#elif (RTE_UART4_TX_ID == 5)
#error "The UART4_TX Pin Configuration not used"
#else
#error "Invalid UART4_TX Pin Configuration"
#endif //(RTE_UART4_TX_ID == 0)

#if (RTE_UART4_RX_ID == 0)
#error "The UART4_RX Pin Configuration not used"
#elif (RTE_UART4_RX_ID == 1)
    UART4_PinConfig[RX_PIN].GPIOx = RTE_UART4_RX_PORT;
    UART4_PinConfig[RX_PIN].port = GPIO_PORT_A;
    UART4_PinConfig[RX_PIN].pin = RTE_UART4_RX_BIT;
    UART4_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART4_RX_ID == 2)
    UART4_PinConfig[RX_PIN].GPIOx = RTE_UART4_RX_PORT;
    UART4_PinConfig[RX_PIN].port = GPIO_PORT_C;
    UART4_PinConfig[RX_PIN].pin = RTE_UART4_RX_BIT;
    UART4_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART4_RX_ID == 3)
#error "The UART4_RX Pin Configuration not used"
#elif (RTE_UART4_RX_ID == 4)
#error "The UART4_RX Pin Configuration not used"
#else
#error "Invalid UART4_RX Pin Configuration"
#endif //(RTE_UART4_RX_ID == 0)

#endif //(RTE_UART4==1)

#if (RTE_UART5==1)

#if (RTE_UART5_TX_ID == 0)
#error "The UART5_TX Pin Configuration not used"
#elif (RTE_UART5_TX_ID == 1)
    UART5_PinConfig[TX_PIN].GPIOx = RTE_UART5_TX_PORT;
    UART5_PinConfig[TX_PIN].port = GPIO_PORT_C;
    UART5_PinConfig[TX_PIN].pin = RTE_UART5_TX_BIT;
    UART5_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART5_TX_ID == 2)
#error "The UART5_TX Pin Configuration not used"
#elif (RTE_UART5_TX_ID == 3)
#error "The UART5_TX Pin Configuration not used"
#elif (RTE_UART5_TX_ID == 4)
#error "The UART5_TX Pin Configuration not used"
#else
#error "Invalid UART5_TX Pin Configuration"
#endif // (RTE_UART5_TX_ID == 0)

#if (RTE_UART5_RX_ID == 0)
#error "The UART5_RX Pin Configuration not used"
#elif (RTE_UART5_RX_ID == 1)
    UART5_PinConfig[RX_PIN].GPIOx = RTE_UART5_RX_PORT;
    UART5_PinConfig[RX_PIN].port = GPIO_PORT_D;
    UART5_PinConfig[RX_PIN].pin = RTE_UART5_RX_BIT;
    UART5_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_UART5_RX_ID == 2)
#error "The UART5_RX Pin Configuration not used"
#elif (RTE_UART5_RX_ID == 3)
#error "The UART5_RX Pin Configuration not used"
#elif (RTE_UART5_RX_ID == 4)
#error "The UART5_RX Pin Configuration not used"
#else
#error "Invalid UART5_RX Pin Configuration"
#endif // (RTE_UART5_RX_ID == 0)

#endif //(RTE_UART5==1)

#if (RTE_USART6 == 1)

#if (RTE_USART6_TX_ID == 0)
#error "The USART6_TX Pin Configuration not used"
#elif (RTE_USART6_TX_ID == 1)
#error "The USART6_TX Pin Configuration not used"
#elif (RTE_USART6_TX_ID == 2)
    USART6_PinConfig[TX_PIN].GPIOx = RTE_USART6_TX_PORT;
    USART6_PinConfig[TX_PIN].port = GPIO_PORT_C;
    USART6_PinConfig[TX_PIN].pin = RTE_USART6_TX_BIT;
    USART6_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_TX_ID == 3)
    USART6_PinConfig[TX_PIN].GPIOx = RTE_USART6_TX_PORT;
    USART6_PinConfig[TX_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[TX_PIN].pin = RTE_USART6_TX_BIT;
    USART6_PinConfig[TX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_TX Pin Configuration"
#endif //(RTE_USART6_TX_ID == 0)

#if (RTE_USART6_RX_ID == 0)
#error "The USART6_RX Pin Configuration not used"
#elif (RTE_USART6_RX_ID == 1)
#error "The USART6_RX Pin Configuration not used"
#elif (RTE_USART6_RX_ID == 2)
    USART6_PinConfig[RX_PIN].GPIOx = RTE_USART6_RX_PORT;
    USART6_PinConfig[RX_PIN].port = GPIO_PORT_C;
    USART6_PinConfig[RX_PIN].pin = RTE_USART6_RX_BIT;
    USART6_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_RX_ID == 3)
    USART6_PinConfig[RX_PIN].GPIOx = RTE_USART6_RX_PORT;
    USART6_PinConfig[RX_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[RX_PIN].pin = RTE_USART6_RX_BIT;
    USART6_PinConfig[RX_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_RX Pin Configuration"
#endif //(RTE_USART6_RX_ID == 0)

#if (RTE_USART6_CK_ID == 0)
#error "The USART6_CK Pin Configuration not used"
#elif (RTE_USART6_CK_ID == 1)
    USART6_PinConfig[CK_PIN].GPIOx = RTE_USART6_CK_PORT;
    USART6_PinConfig[CK_PIN].port = GPIO_PORT_Ñ;
    USART6_PinConfig[CK_PIN].pin = RTE_USART6_CK_BIT;
    USART6_PinConfig[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_CK_ID == 2)
    USART6_PinConfig[CK_PIN].GPIOx = RTE_USART6_CK_PORT;
    USART6_PinConfig[CK_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[CK_PIN].pin = RTE_USART6_CK_BIT;
    USART6_PinConfig[CK_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_CK Pin Configuration"
#endif //(RTE_USART6_CK_ID == 0)

#if (RTE_USART6_CTS_ID == 0)
#error "The USART6_CTS Pin Configuration not used"
#elif (RTE_USART6_CTS_ID == 1)
    USART6_PinConfig[CTS_PIN].GPIOx = RTE_USART6_CTS_PORT;
    USART6_PinConfig[CTS_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[CTS_PIN].pin = RTE_USART6_CTS_BIT;
    USART6_PinConfig[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_CTS_ID == 2)
    USART6_PinConfig[CTS_PIN].GPIOx = RTE_USART6_CTS_PORT;
    USART6_PinConfig[CTS_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[CTS_PIN].pin = RTE_USART6_CTS_BIT;
    USART6_PinConfig[CTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_CTS Pin Configuration"
#endif //(RTE_USART6_CTS_ID == 0)

#if (RTE_USART6_RTS_ID == 0)
#error "The USART6_RTS Pin Configuration not used"
#elif (RTE_USART6_RTS_ID == 1)
    USART6_PinConfig[RTS_PIN].GPIOx = RTE_USART6_RTS_PORT;
    USART6_PinConfig[RTS_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[RTS_PIN].pin = RTE_USART6_RTS_BIT;
    USART6_PinConfig[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#elif (RTE_USART6_RTS_ID == 2)
    USART6_PinConfig[RTS_PIN].GPIOx = RTE_USART6_RTS_PORT;
    USART6_PinConfig[RTS_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[RTS_PIN].pin = RTE_USART6_RTS_BIT;
    USART6_PinConfig[RTS_PIN].alt_func = ARM_GPIO_IO_AF_7;
#else
#error "Invalid USART6_RTS Pin Configuration"
#endif //(RTE_USART6_RTS_ID == 0)

#endif //(RTE_USART6==1)

}
