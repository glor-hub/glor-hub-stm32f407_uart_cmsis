#ifndef _EXTI_H_
#define _EXTI_H_

typedef enum {
    EXTI_NO_TRIGGER_MODE = 0,
    EXTI_FALLING_TRIGGER_MODE,
    EXTI_RISING_TRIGGER_MODE,
    EXTI_FALLING_RISING_TRIGGER_MODE,
    NUM_EXTI_TRIGGER_MODE
} eEXTI_TriggerModes;

uint8_t EXTI_GetFlag(void);
void EXTI_ClearFlag(void);
void EXTI_Init(void);

#endif //_EXTI_H_ 
