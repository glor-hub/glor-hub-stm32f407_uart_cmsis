#ifndef _ARM_CLOCK_H_
#define _ARM_CLOCK_H_

#define ARM_RCC_STA_READY                ((uint32_t)0UL)
#define ARM_RCC_STA_HSE_READY_ERR        ((uint32_t)1UL << 0)
#define ARM_RCC_STA_HSI_READY_ERR        ((uint32_t)1UL << 1)
#define ARM_RCC_STA_PLL_READY_ERR        ((uint32_t)1UL << 2)
#define ARM_RCC_STA_PLL_CLOCK_SWITCH_ERR ((uint32_t)1UL << 3)

bool ARM_RCC_isReady(uint32_t drv_status);
uint32_t ARM_RCC_Reset(void);
uint32_t ARM_RCC_SetSysClockTo168(void);
uint32_t ARM_RCC_NMI_HandlerErrCheck(void);
void ARM_RCC_OutputSysClocktoMCO2(void);
void ARM_RCC_ConfigMCO2(void);
void ARM_RCC_Periph_ResetCmd(ePeriphTypes periph, ePeriphCmd cmd);
void ARM_RCC_Periph_ClockCmd(ePeriphTypes periph, ePeriphCmd cmd);
uint32_t ARM_RCC_AHBClockConfig(void);
uint32_t ARM_RCC_APBxClockConfig(void);

#endif //_ARM_CLOCK_H_
