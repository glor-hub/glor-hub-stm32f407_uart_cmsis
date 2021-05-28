#ifndef _TIMER_H_
#define _TIMER_H_

typedef enum {
    TIMER_DELAY = 0,
    NUM_TIMERS
} eTimer_Types;

#define ONE_SECOND (1000UL)

uint32_t Timer_Init(void);
void Timer_Enable(eTimer_Types timer, uint32_t time);
void Timer_Disable(eTimer_Types timer);
uint8_t Timer_GetFlag(eTimer_Types timer);
void Delay(uint32_t time);

#endif //_TIMER_H_
