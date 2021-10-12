#ifndef _ARM_EXTI_H_
#define _ARM_EXTI_H_

typedef enum {
    ARM_EXTI_NO_TRIGGER_MODE = 0,
    ARM_EXTI_FALLING_TRIGGER_MODE,
    ARM_EXTI_RISING_TRIGGER_MODE,
    ARM_EXTI_FALLING_RISING_TRIGGER_MODE,
    NUM_ARM_EXTI_TRIGGER_MODE
} eARM_EXTI_TriggerModes;

void ARM_EXTI_SetPinCfg(ePeriphTypes port_name, eGPIO_IONumbers pin_num, eARM_EXTI_TriggerModes trigger_mode);
void ARM_EXTI_EventEnable(eGPIO_IONumbers pin_num, ePeriphCmd cmd);
void ARM_EXTI_IRQEnable(eGPIO_IONumbers pin_num, ePeriphCmd cmd);
void ARM_EXTI_ClearPendingIRQ(eGPIO_IONumbers pin_num);

#endif //_ARM_EXTI_H_
