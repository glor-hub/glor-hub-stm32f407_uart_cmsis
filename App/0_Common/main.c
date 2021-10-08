
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
#include "assert.h"
#include "app.h"


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
    bool app_init_result = App_Init();
    ASSERT(app_init_result == PASSED);

    while(1) {
        App_IdleTask();
    }
}
