//********************************************************************************
//app_process.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdbool.h>
#include "common.h"
#include "app.h"
#include "clock.h"
#include "led.h"
#include "arm_gpio.h"
#include "gpio.h"
#include "timer.h"
#include "button.h"
#include "Driver_USART.h"
#include "usart.h"

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

static void App_PeripherialTest(void);
static void App_CorePeripherialTest(void);
static void App_ExtPeripherialTest(void);

//================================================================================
//Public
//================================================================================
void App_IdleTask(void)
{
    //Some IDLE task implemenation here. This is the lowest priority task
}

bool App_Init(void)
{
    bool init_result = PASSED;
    init_result |= Clock_Init();
    GPIO_Init();
    init_result |= Timer_Init();
    LED_Init();
    Button_Init();
    init_result |= USART_Init();
    return init_result;
}

void App_Test(void)
{
    App_PeripherialTest();
    Button_Test();
}

//================================================================================
//Private
//================================================================================
static void App_PeripherialTest(void)
{
    App_CorePeripherialTest();
    App_ExtPeripherialTest();
}

static void App_CorePeripherialTest(void)
{
//SYSCLOCK testing
    Clock_Test();
}

static void App_ExtPeripherialTest(void)
{
    USART_Test();
//    LED_Test();
}
