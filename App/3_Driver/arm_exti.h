#ifndef _ARM_EXTI_H_
#define _ARM_EXTI_H_

void ARM_EXTI_SetPinCfg(eGPIO_PortNames port_name, eGPIO_IONumbers pin_num, eEXTI_TriggerModes trigger_mode);
void ARM_EXTI_EventEnable(eGPIO_IONumbers pin_num, ePeriphCmd cmd);
void ARM_EXTI_IRQEnable(eGPIO_IONumbers pin_num, ePeriphCmd cmd);
void ARM_EXTI_ClearPendingIRQ(eGPIO_IONumbers pin_num);

#endif //_ARM_EXTI_H_
