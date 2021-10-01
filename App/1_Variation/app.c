//********************************************************************************
//app_process.c
//********************************************************************************
#include "stm32f4xx.h"
#include "RTE_Device.h"
#include "common.h"
#include "app.h"
#include "discovery-kit.h"
#include "clock.h"
#include "led.h"
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
    Button_Test();
}

uint32_t App_Init(void)
{
    uint32_t init_result = PASSED;

    init_result |= Clock_Init();
    GPIO_Init();
    //Sys_Status |= Timer_Init();
    LED_Init();
    Button_Init();
    USART_Init();
    App_PeripherialTest();

    return init_result;
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
#ifdef HARDWARE_TESTING_MODE
//SYSCLOCK testing
    Clock_Test();
#endif//HARDWARE_TESTING_MODE
}

static void App_ExtPeripherialTest(void)
{
    LED_Test();
}
