#ifndef _EXTI_H_
#define _EXTI_H_

#define EXTI_FALLING_TRIGGER_MODE  ((uint8_t)0x01)
#define EXTI_RISING_TRIGGER_MODE  ((uint8_t)0x02)
#define EXTI_FALLING_RISING_TRIGGER_MODE  ((uint8_t)0x03)
uint8_t EXTI_GetFlag(void);
void EXTI_ClearFlag(void);
void EXTI_Init(void);

#endif //_EXTI_H_ 
