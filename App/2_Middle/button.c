//********************************************************************************
//button.c
//********************************************************************************
#include "stm32f4xx.h"
#include "button.h"
#include "discovery-kit.h"
#include "gpio.h"
#include "Driver_USART.h"
#include "usart.h"
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

//================================================================================
//Public
//================================================================================

void Button_Test(void)
{
    uint8_t flag;
    flag = EXTI_GetFlag();
    if(flag) {
        LED_Test();
        EXTI_ClearFlag();
    }
}

void Button_Init(void)
{
    ARM_RCC_GPIO_ClockCmd(GPIO_PORT_A, ENABLE_CMD);
    GPIO_SetCfg(GPIOA, GPIO_IO_0, GPIO_IO_MODE_INPUT, GPIO_IO_TYPE_NO,
                GPIO_IO_HI_Z, GPIO_IO_SPEED_FREQ_LOW, GPIO_IO_AF_0);
    ARM_GPIO_Config();
    EXTI_Init();
}

//================================================================================
//Private
//================================================================================


