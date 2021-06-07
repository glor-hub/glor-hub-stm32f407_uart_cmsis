#ifndef _EXTI_H_
#define _EXTI_H_

#define EXTI_FALLING_TRIGGER_MODE  ((uint8_t)0x01)
#define EXTI_RISING_TRIGGER_MODE  ((uint8_t)0x02)
#define EXTI_FALLING_RISING_TRIGGER_MODE  ((uint8_t)0x03)

void EXTI_Init(void);

#endif //_EXTI_H_ 
