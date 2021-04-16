//********************************************************************************
//clock.c
//********************************************************************************
#include "stm32f4xx.h"
#include "clock.h"
#include "arm_clock.h"


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

void CLOCK_Init(void)
{
    ARM_RCC_Reset();
    ARM_RCC_SetSysClockTo168();
    SystemCoreClockUpdate();
}

void CLOCK_Test(void)
{

}

//================================================================================
//Private
//================================================================================
