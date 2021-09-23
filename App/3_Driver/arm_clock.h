#ifndef _ARM_CLOCK_H_
#define _ARM_CLOCK_H_

bool ARM_RCC_isReady(void);
void ARM_RCC_Reset(void);
void ARM_RCC_SetSysClockTo168(void);
uint32_t *ARM_RCC_GetStatus(void);
void ARM_RCC_OutputSysClocktoMCO2(void);
void ARM_RCC_ConfigMCO2(void);
void ARM_RCC_USART_ResetCmd(eUSART_InterfaceNames usart, ePeriphCmd cmd);
void ARM_RCC_USART_ClockCmd(eUSART_InterfaceNames usart, ePeriphCmd cmd);
void ARM_RCC_GPIO_ClockCmd(eGPIO_PortNames port_name, ePeriphCmd cmd);


#endif //_ARM_CLOCK_H_
