//********************************************************************************
//button.c
//********************************************************************************
#include "stm32f4xx.h"
#include "button.h"
#include "discovery-kit.h"
#include "gpio.h"
#include "arm_gpio.h"
#include "arm_clock.h"
#include "exti.h"
#include "led.h"

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

//********************************************************************************
//Prototypes
//********************************************************************************
static void Button_Init(void);
//================================================================================
//Public
//================================================================================

void Button_Test(void)
{
    Button_Init();
}

void Button_CB(void)
{
    LED_Test();
}

//================================================================================
//Private
//================================================================================

void Button_Init(void)
{
    ARM_RCC_GPIO_ClockCmd(GPIO_PORT_A, ENABLE_CMD);
    GPIO_SetCfg(GPIOA, GPIO_IO_0, GPIO_IO_MODE_INPUT, GPIO_IO_TYPE_NO,
                GPIO_IO_PULL_UP, GPIO_IO_SPEED_FREQ_LOW, GPIO_IO_AF_0);
    ARM_GPIO_Config();
    EXTI_Init();
}
