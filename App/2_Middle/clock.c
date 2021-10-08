//********************************************************************************
//clock.c
//********************************************************************************
#include "stm32f4xx.h"
#include "RTE_Device.h"
#include "discovery-kit.h"
#include "common.h"
#include <stdio.h>
#include "assert.h"
#include "gpio.h"
#include "Driver_USART.h"
#include "usart.h"
#include "clock.h"
#include "arm_clock.h"
#include "arm_gpio.h"

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

uint32_t Clock_Init(void)
{
    uint32_t arm_status = 0UL;
    arm_status |= ARM_RCC_Reset();
    arm_status |= ARM_RCC_SetSysClockTo168();
    arm_status |= ARM_RCC_NMI_HandlerErrCheck();
    SystemCoreClockUpdate();
    ASSERT(arm_status == ARM_RCC_STA_READY);
    return ARM_RCC_isReady(arm_status) ? PASSED : FAILED;
}

void Clock_Test(void)
{

    ARM_RCC_GPIO_ClockCmd(GPIO_PORT_C, ENABLE_CMD);
//configure MCO2 pin for SYSCLOCK testing
    GPIO_SetCfg(GPIOC, GPIO_IO_9, GPIO_IO_MODE_ALT_FUNC, GPIO_IO_TYPE_PUSH_PULL, GPIO_IO_PULL_UP,
                GPIO_IO_SPEED_FREQ_HIGH, GPIO_IO_AF_0);
    ARM_GPIO_Config();
//configure for output System clock divided by 5 (168/5=33,6 MHz) on MCO2 pin
    ARM_RCC_ConfigMCO2();
}

//================================================================================
//Private
//================================================================================

