//********************************************************************************
//app_process.c
//********************************************************************************
#include "stm32f4xx.h"
#include "app.h"
#include "discovery-kit.h"
#include "clock.h"
#include "led.h"
#include "gpio.h"

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
static void App_IntPeripherialTest(void);
static void App_ExtPeripherialTest(void);

//================================================================================
//Public
//================================================================================
void App_IdleTask(void)
{
    //Some IDLE task implemenation here. This is the lowest priority task
}

void App_Init(void)
{
    Clock_Init();

    GPIO_Init();

    App_PeripherialTest();
}

//================================================================================
//Private
//================================================================================
static void App_PeripherialTest(void)
{
    App_CorePeripherialTest();
    App_IntPeripherialTest();
    App_ExtPeripherialTest();
}
static void App_CorePeripherialTest(void)
{
#ifdef HARDWARE_TESTING_MODE
//SYSCLOCK testing
    Clock_Test();
#endif//HARDWARE_TESTING_MODE
}

static void App_IntPeripherialTest(void)
{
    GPIO_Test();
}

static void App_ExtPeripherialTest(void)
{
    LED_Test();
}
