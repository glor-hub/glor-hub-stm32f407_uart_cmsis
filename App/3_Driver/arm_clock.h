#ifndef _ARM_CLOCK_H_
#define _ARM_CLOCK_H_

void ARM_RCC_Reset(void);
void ARM_RCC_SetSysClockTo168(void);
uint32_t *ARM_RCC_GetStatus(void);
void ARM_RCC_OutputSysClocktoMCO2(void);
void ARM_RCC_GPIO_ClockCmd(eGPIO_PortNames port_name, ePeriphCmd cmd);
void ARM_RCC_ConfigMCO2(void);

#endif //_ARM_CLOCK_H_
