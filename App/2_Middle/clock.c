//********************************************************************************
//clock.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdbool.h>
#include "common.h"
#include "arm_clock.h"
#include "arm_gpio.h"
#include "gpio.h"
#include "clock.h"

#define _CLOCK_DEBUG_

#ifdef _CLOCK_DEBUG_
#include "assert.h"
#endif//_CLOCK_DEBUG_

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

bool Clock_Init(void)
{
    uint32_t status = ARM_RCC_Reset();
    status |= ARM_RCC_SetSysClockTo168();
    status |= ARM_RCC_NMI_HandlerErrCheck();

    SystemCoreClockUpdate();

#ifdef _CLOCK_DEBUG_
    ASSERT(ARM_RCC_isReady(status));
#endif//_CLOCK_DEBUG_

    return ARM_RCC_isReady(status) ? PASSED : FAILED;
}

void Clock_Test(void)
{

    ARM_RCC_Periph_ClockCmd(GPIO_PORT_C, ENABLE_CMD);
//configure MCO2 pin for SYSCLOCK testing
    GPIO_SetData(GPIOC, ARM_GPIO_IO_9, ARM_GPIO_IO_MODE_ALT_FUNC, ARM_GPIO_IO_TYPE_PUSH_PULL, ARM_GPIO_IO_PULL_UP,
                 ARM_GPIO_IO_SPEED_FREQ_HIGH, ARM_GPIO_IO_AF_0);
//configure for output System clock divided by 5 (168/5=33,6 MHz) on MCO2 pin
    ARM_RCC_ConfigMCO2();
}

//================================================================================
//Private
//================================================================================

