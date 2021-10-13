
/*
This project is to test  the possibility of STM32F407VGT6 microcontroller peripherals
using STM32F4DISCOVERY kit.
*/

//********************************************************************************
//main.c
//********************************************************************************

#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdio.h>
#include "common.h"
#include "app.h"
#include "assert.h"

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


//================================================================================
//Private
//================================================================================

int main(void)
{
    bool init_result = App_Init();

#ifdef _APP_DEBUG_
    ASSERT(init_result == PASSED);
#endif//_APP_DEBUG_

    if(init_result != PASSED) {
        while(1);
    }

    while(1) {
        App_IdleTask();
    }
}
