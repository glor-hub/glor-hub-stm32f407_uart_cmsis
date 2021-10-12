#ifndef _ARM_EXTI_H_
#define _ARM_EXTI_H_

typedef enum {
    ARM_EXTI_NO_TRIGGER_MODE = 0,
    ARM_EXTI_FALLING_TRIGGER_MODE,
    ARM_EXTI_RISING_TRIGGER_MODE,
    ARM_EXTI_FALLING_RISING_TRIGGER_MODE,
    NUM_ARM_EXTI_TRIGGER_MODE
} eARM_EXTI_TriggerModes;

typedef struct {
    ePeriphTypes           Port;
    eARM_GPIO_IONumbers    Pin;
    eARM_EXTI_TriggerModes Trigger_Mode;
} ARM_EXTI_Cfg_t;

void ARM_EXTI_SetCfg(ARM_EXTI_Cfg_t *pEXTI_Cfg);
void ARM_EXTI_IRQEnable(eARM_GPIO_IONumbers pin_num, ePeriphCmd cmd);
void ARM_EXTI_ClearPendingIRQ(eARM_GPIO_IONumbers pin_num);

#endif //_ARM_EXTI_H_
