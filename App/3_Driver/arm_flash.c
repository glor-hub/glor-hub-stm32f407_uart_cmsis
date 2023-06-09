//********************************************************************************
//arm_flash.c
//********************************************************************************
#include "stm32f4xx.h"
#include "arm_flash.h"

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
void ARM_FLASH_Init(void)
{
//enable prefetch buffer
    FLASH->ACR |= FLASH_ACR_PRFTEN;
//for system clock from 150 to 168 MHz a latency of 6 cycles is recommended
//set the latency
    FLASH->ACR &= FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_6WS;
}

//================================================================================
//Private
//================================================================================

