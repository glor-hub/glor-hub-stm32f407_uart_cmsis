//********************************************************************************
//assert.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdio.h>
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

void assert_failed(uint8_t *func, uint8_t *file, uint32_t line)
{
    printf(" Wrong parameters value: in function %s, file %s on line %d\r\n", func, file, line);

    while(1);
}

//================================================================================
//Private
//================================================================================
