//********************************************************************************
//usart.c
//********************************************************************************
#include "stm32f4xx.h"
#include "Driver_USART.h"
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

// for multiple UARTx outputs options
typedef enum {
    USART_PIN_ID0 = 0,
    USART_PIN_ID1,
    USART_PIN_ID2,
    MAX_USART_PIN_ID_NUMBERS
} eUSART_PinIDNumbers; //

//********************************************************************************
//Variables
//********************************************************************************

#ifdef USART1_ENABLE
static USART_PinCfg_t USART1_PinConfig[MAX_USART_PIN_NAMES];
#endif //USART1_ENABLE

#ifdef USART2_ENABLE
static USART_PinCfg_t USART2_PinConfig[MAX_USART_PIN_NAMES];
#endif //USART2_ENABLE

#ifdef USART3_ENABLE
static USART_PinCfg_t USART3_PinConfig[MAX_USART_PIN_NAMES];
#endif //USART3_ENABLE

#ifdef UART4_ENABLE
static USART_PinCfg_t UART4_PinConfig[MAX_USART_PIN_NAMES];
#endif //UART4_ENABLE

#ifdef UART5_ENABLE
static USART_PinCfg_t UART5_PinConfig[MAX_USART_PIN_NAMES];
#endif //UART5_ENABLE

#ifdef USART6_ENABLE
static USART_PinCfg_t USART6_PinConfig[MAX_UART_PIN_NAMES];
#endif //USART6_ENABLE

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

USART_PinCfg_t *USART_GetPinConfig(eUSART_InterfaceNames usart_name)
{
    switch(usart_name) {
#ifdef USART1_ENABLE
        case USART_1: {
            return USART1_PinConfig;
        }
#endif //USART1_ENABLE

#ifdef USART2_ENABLE
        case USART_2: {
            return USART2_PinConfig;
        }
#endif //USART2_ENABLE

#ifdef USART3_ENABLE
        case USART_3: {
            return USART3_PinConfig;
        }
#endif //USART3_ENABLE

#ifdef UART4_ENABLE
        case UART_4: {
            return UART4_PinConfig;
        }
#endif //UART4_ENABLE

#ifdef USART5_ENABLE
        case UART_5: {
            return USART5_PinConfig;
        }
#endif //USART5_ENABLE

#ifdef UART6_ENABLE
        case USART_6: {
            return UART6_PinConfig;
        }
#endif //UART6_ENABLE

    }
}

#ifdef USART1_ENABLE
void USART1_IRQHandler(void)
{
//    USART_IRQHandler(&USART1_Resources);
}
void USART1_cb(uint32_t event)
{

}
#endif //USART1_ENABLE

#ifdef UART4_ENABLE
void UART4_IRQHandler(void)
{
//    USART_IRQHandler(&UART4_Resources);
}
void UART4_cb(uint32_t event)
{

}
#endif //UART4_ENABLE

//================================================================================
//Private
//================================================================================

static void USART_SetPinConfig(void)
{
#ifdef USART1_ENABLE

#if (USART1_PIN_ID == USART_PIN_ID0)
    USART1_PinConfig[TX_PIN].GPIOx = GPIOA;
    USART1_PinConfig[TX_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[TX_PIN].pin = GPIO_IO_9;
    USART1_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_7;
    USART1_PinConfig[RX_PIN].GPIOx = GPIOA;
    USART1_PinConfig[RX_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[RX_PIN].pin = GPIO_IO_10;
    USART1_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_7;
#ifdef USART1_SCLK_PIN_ENABLE
    USART1_PinConfig[SCLK_PIN].GPIOx = GPIOA;
    USART1_PinConfig[SCLK_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[SCLK_PIN].pin = GPIO_IO_8;
    USART1_PinConfig[SCLK_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART1_SCLK_PIN_ENABLE
#ifdef USART1_CTS_PIN_ENABLE
    USART1_PinConfig[CTS_PIN].GPIOx = GPIOA;
    USART1_PinConfig[CTS_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[CTS_PIN].pin = GPIO_IO_11;
    USART1_PinConfig[CTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART1_CTS_PIN_ENABLE
#ifdef USART1_RTS_PIN_ENABLE
    USART1_PinConfig[RTS_PIN].GPIOx = GPIOA;
    USART1_PinConfig[RTS_PIN].port = GPIO_PORT_A;
    USART1_PinConfig[RTS_PIN].pin = GPIO_IO_12;
    USART1_PinConfig[RTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART1_RTS_PIN_ENABLE
#else
    USART1_PinConfig[TX_PIN].GPIOx = GPIOB;
    USART1_PinConfig[TX_PIN].port = GPIO_PORT_B;
    USART1_PinConfig[TX_PIN].pin = GPIO_IO_6;
    USART1_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_7;
    USART1_PinConfig[RX_PIN].GPIOx = GPIOB;
    USART1_PinConfig[RX_PIN].port = GPIO_PORT_B;
    USART1_PinConfig[RX_PIN].pin = GPIO_IO_7;
    USART1_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_7;
#endif //(USART1_PIN_ID == USART_PIN_ID0)

#endif //USART1_ENABLE

#ifdef USART2_ENABLE

#if (USART2_PIN_ID == USART_PIN_ID0)
    USART2_PinConfig[TX_PIN].GPIOx = GPIOA;
    USART2_PinConfig[TX_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[TX_PIN].pin = GPIO_IO_2;
    USART2_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_7;
    USART2_PinConfig[RX_PIN].GPIOx = GPIOA;
    USART2_PinConfig[RX_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[RX_PIN].pin = GPIO_IO_3;
    USART2_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_7;
#ifdef USART2_SCLK_PIN_ENABLE
    USART2_PinConfig[SCLK_PIN].GPIOx = GPIOA;
    USART2_PinConfig[SCLK_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[SCLK_PIN].pin = GPIO_IO_4;
    USART2_PinConfig[SCLK_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART2_SCLK_PIN_ENABLE
#ifdef USART2_CTS_PIN_ENABLE
    USART2_PinConfig[CTS_PIN].GPIOx = GPIOA;
    USART2_PinConfig[CTS_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[CTS_PIN].pin = GPIO_IO_0;
    USART2_PinConfig[CTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART2_CTS_PIN_ENABLE
#ifdef USART2_RTS_PIN_ENABLE
    USART2_PinConfig[RTS_PIN].GPIOx = GPIOA;
    USART2_PinConfig[RTS_PIN].port = GPIO_PORT_A;
    USART2_PinConfig[RTS_PIN].pin = GPIO_IO_1;
    USART2_PinConfig[RTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART2_RTS_PIN_ENABLE
#else
    USART2_PinConfig[TX_PIN].GPIOx = GPIOD;
    USART2_PinConfig[TX_PIN].port = GPIO_PORT_D;
    USART2_PinConfig[TX_PIN].pin = GPIO_IO_5;
    USART2_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_7;
    USART2_PinConfig[RX_PIN].GPIOx = GPIOD;
    USART2_PinConfig[RX_PIN].port = GPIO_PORT_D;
    USART2_PinConfig[RX_PIN].pin = GPIO_IO_6;
    USART2_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_7;
#ifdef USART2_SCLK_PIN_ENABLE
    USART2_PinConfig[SCLK_PIN].GPIOx = GPIOD;
    USART2_PinConfig[SCLK_PIN].port = GPIO_PORT_D;
    USART2_PinConfig[SCLK_PIN].pin = GPIO_IO_7;
    USART2_PinConfig[SCLK_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART2_SCLK_PIN_ENABLE
#ifdef USART2_CTS_PIN_ENABLE
    USART2_PinConfig[CTS_PIN].GPIOx = GPIOD;
    USART2_PinConfig[CTS_PIN].port = GPIO_PORT_D;
    USART2_PinConfig[CTS_PIN].pin = GPIO_IO_3;
    USART2_PinConfig[CTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART2_CTS_PIN_ENABLE
#ifdef USART2_RTS_PIN_ENABLE
    USART2_PinConfig[RTS_PIN].GPIOx = GPIOD;
    USART2_PinConfig[RTS_PIN].port = GPIO_PORT_D;
    USART2_PinConfig[RTS_PIN].pin = GPIO_IO_4;
    USART2_PinConfig[RTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART2_RTS_PIN_ENABLE
#endif //(USART2_PIN_ID == USART_PIN_ID0)

#endif //USART2_ENABLE

#ifdef USART3_ENABLE

#if (USART3_PIN_ID == USART_PIN_ID0)
    USART3_PinConfig[TX_PIN].GPIOx = GPIOB;
    USART3_PinConfig[TX_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[TX_PIN].pin = GPIO_IO_10;
    USART3_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_7;
    USART3_PinConfig[RX_PIN].GPIOx = GPIOB;
    USART3_PinConfig[RX_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[RX_PIN].pin = GPIO_IO_11;
    USART3_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_7;
#ifdef USART3_SCLK_PIN_ENABLE
    USART3_PinConfig[SCLK_PIN].GPIOx = GPIOB;
    USART3_PinConfig[SCLK_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[SCLK_PIN].pin = GPIO_IO_12;
    USART3_PinConfig[SCLK_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART3_SCLK_PIN_ENABLE
#ifdef USART3_CTS_PIN_ENABLE
    USART3_PinConfig[CTS_PIN].GPIOx = GPIOB;
    USART3_PinConfig[CTS_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[CTS_PIN].pin = GPIO_IO_13;
    USART3_PinConfig[CTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART3_CTS_PIN_ENABLE
#ifdef USART3_RTS_PIN_ENABLE
    USART3_PinConfig[RTS_PIN].GPIOx = GPIOB;
    USART3_PinConfig[RTS_PIN].port = GPIO_PORT_B;
    USART3_PinConfig[RTS_PIN].pin = GPIO_IO_14;
    USART3_PinConfig[RTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART3_RTS_PIN_ENABLE
#elif (USART3_PIN_ID == USART_PIN_ID1)
    USART3_PinConfig[TX_PIN].GPIOx = GPIOC;
    USART3_PinConfig[TX_PIN].port = GPIO_PORT_C;
    USART3_PinConfig[TX_PIN].pin = GPIO_IO_10;
    USART3_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_7;
    USART3_PinConfig[RX_PIN].GPIOx = GPIOC;
    USART3_PinConfig[RX_PIN].port = GPIO_PORT_C;
    USART3_PinConfig[RX_PIN].pin = GPIO_IO_11;
    USART3_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_7;
#ifdef USART3_SCLK_PIN_ENABLE
    USART3_PinConfig[SCLK_PIN].GPIOx = GPIOC;
    USART3_PinConfig[SCLK_PIN].port = GPIO_PORT_C;
    USART3_PinConfig[SCLK_PIN].pin = GPIO_IO_12;
    USART3_PinConfig[SCLK_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART3_SCLK_PIN_ENABLE
#else
    USART3_PinConfig[TX_PIN].GPIOx = GPIOD;
    USART3_PinConfig[TX_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[TX_PIN].pin = GPIO_IO_8;
    USART3_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_7;
    USART3_PinConfig[RX_PIN].GPIOx = GPIOD;
    USART3_PinConfig[RX_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[RX_PIN].pin = GPIO_IO_9;
    USART3_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_7;
#ifdef USART3_SCLK_PIN_ENABLE
    USART3_PinConfig[SCLK_PIN].GPIOx = GPIOD;
    USART3_PinConfig[SCLK_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[SCLK_PIN].pin = GPIO_IO_10;
    USART3_PinConfig[SCLK_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART3_SCLK_PIN_ENABLE
#ifdef USART3_CTS_PIN_ENABLE
    USART3_PinConfig[CTS_PIN].GPIOx = GPIOD;
    USART3_PinConfig[CTS_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[CTS_PIN].pin = GPIO_IO_11;
    USART3_PinConfig[CTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART3_CTS_PIN_ENABLE
#ifdef USART3_RTS_PIN_ENABLE
    USART3_PinConfig[RTS_PIN].GPIOx = GPIOD;
    USART3_PinConfig[RTS_PIN].port = GPIO_PORT_D;
    USART3_PinConfig[RTS_PIN].pin = GPIO_IO_12;
    USART3_PinConfig[RTS_PIN].alt_func = GPIO_IO_AF_7;
#endif //USART3_RTS_PIN_ENABLE
#endif //(USART3_PIN_ID == USART_PIN_ID0)

#endif //USART3_ENABLE

#ifdef UART4_ENABLE

#if (UART4_PIN_ID == USART_PIN_ID0)
    UART4_PinConfig[TX_PIN].GPIOx = GPIOA;
    UART4_PinConfig[TX_PIN].port = GPIO_PORT_A;
    UART4_PinConfig[TX_PIN].pin = GPIO_IO_0;
    UART4_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_8;
    UART4_PinConfig[RX_PIN].GPIOx = GPIOA;
    UART4_PinConfig[RX_PIN].port = GPIO_PORT_A;
    UART4_PinConfig[RX_PIN].pin = GPIO_IO_1;
    UART4_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_8;
#else
    UART4_PinConfig[TX_PIN].GPIOx = GPIOC;
    UART4_PinConfig[TX_PIN].port = GPIO_PORT_C;
    UART4_PinConfig[TX_PIN].pin = GPIO_IO_10;
    UART4_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_8;
    UART4_PinConfig[RX_PIN].GPIOx = GPIOC;
    UART4_PinConfig[RX_PIN].port = GPIO_PORT_C;
    UART4_PinConfig[RX_PIN].pin = GPIO_IO_11;
    UART4_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_8;
#endif //(UART4_PIN_ID == USART_PIN_ID0)

#endif //UART4_ENABLE

#ifdef UART5_ENABLE

#if (UART5_PIN_ID == USART_PIN_ID0)
    UART5_PinConfig[TX_PIN].GPIOx = GPIOC;
    UART5_PinConfig[TX_PIN].port = GPIO_PORT_C;
    UART5_PinConfig[TX_PIN].pin = GPIO_IO_12;
    UART5_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_8;
    UART5_PinConfig[RX_PIN].GPIOx = GPIOD;
    UART5_PinConfig[RX_PIN].port = GPIO_PORT_D;
    UART5_PinConfig[RX_PIN].pin = GPIO_IO_2;
    UART5_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_8;
#endif //(UART5_PIN_ID == USART_PIN_ID0)

#endif //UART5_ENABLE

#ifdef USART6_ENABLE

#if (USART6_PIN_ID == USART_PIN_ID0)
    USART6_PinConfig[TX_PIN].GPIOx = GPIOC;
    USART6_PinConfig[TX_PIN].port = GPIO_PORT_C;
    USART6_PinConfig[TX_PIN].pin = GPIO_IO_6;
    USART6_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_8;
    USART6_PinConfig[RX_PIN].GPIOx = GPIOC;
    USART6_PinConfig[RX_PIN].port = GPIO_PORT_C;
    USART6_PinConfig[RX_PIN].pin = GPIO_IO_7;
    USART6_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_8;
#ifdef USART6_SCLK_PIN_ENABLE
    USART6_PinConfig[SCLK_PIN].GPIOx = GPIOC;
    USART6_PinConfig[SCLK_PIN].port = GPIO_PORT_C;
    USART6_PinConfig[SCLK_PIN].pin = GPIO_IO_8;
    USART6_PinConfig[SCLK_PIN].alt_func = GPIO_IO_AF_8;
#endif //USART6_SCLK_PIN_ENABLE
#ifdef USART6_CTS_PIN_ENABLE
    USART6_PinConfig[CTS_PIN].GPIOx = GPIOG;
    USART6_PinConfig[CTS_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[CTS_PIN].pin = GPIO_IO_13;
    USART6_PinConfig[CTS_PIN].alt_func = GPIO_IO_AF_8;
#endif //USART6_CTS_PIN_ENABLE
#ifdef USART6_RTS_PIN_ENABLE
    USART6_PinConfig[RTS_PIN].GPIOx = GPIOG;
    USART6_PinConfig[RTS_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[RTS_PIN].pin = GPIO_IO_8;
    USART6_PinConfig[RTS_PIN].alt_func = GPIO_IO_AF_8;
#endif //USART6_RTS_PIN_ENABLE
#else
    USART6_PinConfig[TX_PIN].GPIOx = GPIOG;
    USART6_PinConfig[TX_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[TX_PIN].pin = GPIO_IO_14;
    USART6_PinConfig[TX_PIN].alt_func = GPIO_IO_AF_8;
    USART6_PinConfig[RX_PIN].GPIOx = GPIOG;
    USART6_PinConfig[RX_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[RX_PIN].pin = GPIO_IO_9;
    USART6_PinConfig[RX_PIN].alt_func = GPIO_IO_AF_8;
#ifdef USART6_SCLK_PIN_ENABLE
    USART6_PinConfig[SCLK_PIN].GPIOx = GPIOG;
    USART6_PinConfig[SCLK_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[SCLK_PIN].pin = GPIO_IO_7;
    USART6_PinConfig[SCLK_PIN].alt_func = GPIO_IO_AF_8;
#endif //USART6_SCLK_PIN_ENABLE
#ifdef USART6_CTS_PIN_ENABLE
    USART6_PinConfig[CTS_PIN].GPIOx = GPIOG;
    USART6_PinConfig[CTS_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[CTS_PIN].pin = GPIO_IO_15;
    USART6_PinConfig[CTS_PIN].alt_func = GPIO_IO_AF_8;
#endif //USART6_CTS_PIN_ENABLE
#ifdef USART6_RTS_PIN_ENABLE
    USART6_PinConfig[RTS_PIN].GPIOx = GPIOG;
    USART6_PinConfig[RTS_PIN].port = GPIO_PORT_G;
    USART6_PinConfig[RTS_PIN].pin = GPIO_IO_12;
    USART6_PinConfig[RTS_PIN].alt_func = GPIO_IO_AF_8;
#endif //USART6_RTS_PIN_ENABLE
#endif //(USART6_PIN_ID == USART_PIN_ID0)

#endif //USART6_ENABLE
}

