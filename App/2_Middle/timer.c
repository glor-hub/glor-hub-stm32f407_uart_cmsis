//********************************************************************************
//timer.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdbool.h>
#include "RTE_Device.h"
#include "common.h"
#include <stdio.h>
#include "assert.h"
#include "timer.h"
#include <string.h>

//********************************************************************************
//Macros
//********************************************************************************
#define TIMER_STA_READY ((uint32_t)0UL)
#define TIMER_STA_INIT_ERR ((uint32_t)1UL)

//********************************************************************************
//Enums
//********************************************************************************



//********************************************************************************
//Typedefs
//********************************************************************************

//********************************************************************************
//Variables
//********************************************************************************

typedef struct {
    uint8_t state;
    uint32_t time;
    uint32_t counter;
    uint8_t flag;
} Timer_Data_t;

static Timer_Data_t Timer_Data[NUM_TIMERS];

//********************************************************************************
//Prototypes
//********************************************************************************

//================================================================================
//Public
//================================================================================

void SysTick_Handler(void)
{
    for(eTimer_Types timer = TIMER_DELAY; timer < NUM_TIMERS; timer++) {
        if(Timer_Data[timer].state) {
            Timer_Data[timer].counter++;
        }
        if(Timer_Data[timer].counter == Timer_Data[timer].time) {
            Timer_Data[timer].flag = TRUE;
        }
    }
}

bool Timer_is_Ready(uint32_t status)
{
    return (status == TIMER_STA_READY);
}

uint32_t Timer_Init(void)
{
    uint32_t status = TIMER_STA_READY;
    memset(&Timer_Data, 0, sizeof(Timer_Data_t) * NUM_TIMERS);
    if(SysTick_Config(SystemCoreClock / 1000 - 1)) {
        status = TIMER_STA_INIT_ERR;
    }
    ASSERT(status == TIMER_STA_READY);
    return Timer_is_Ready(status) ? PASSED : FAILED;
}

void Timer_Enable(eTimer_Types timer, uint32_t time)
{
    Timer_Data[timer].state = TRUE;
    Timer_Data[timer].time = time;
    Timer_Data[timer].counter = 0UL;
    Timer_Data[timer].flag = FALSE;
}

void Timer_Disable(eTimer_Types timer)
{
    Timer_Data[timer].state = FALSE;
    Timer_Data[timer].time = 0UL;
    Timer_Data[timer].counter = 0UL;
    Timer_Data[timer].flag = FALSE;
}

uint8_t Timer_GetFlag(eTimer_Types timer)
{
    return Timer_Data[timer].flag;
}

void Delay(uint32_t time)
{
    Timer_Enable(TIMER_DELAY, time);
    while(!Timer_Data[TIMER_DELAY].flag);
    Timer_Disable(TIMER_DELAY);
}
//================================================================================
//Private
//================================================================================

