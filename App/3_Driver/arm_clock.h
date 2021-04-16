#ifndef _ARM_CLOCK_H_
#define _ARM_CLOCK_H_

void ARM_RCC_Reset(void);
void ARM_RCC_SetSysClockTo168(void);
uint32_t *ARM_RCC_GetStatus(void);

#endif //_ARM_CLOCK_H_
