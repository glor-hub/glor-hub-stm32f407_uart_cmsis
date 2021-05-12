#ifndef _ARM_GPIO_H_
#define _ARM_GPIO_H_

void ARM_GPIO_Config(void);
uint32_t ARM_GPIO_ReadIO(GPIO_TypeDef *GPIOx, uint32_t io_mask);
void ARM_GPIO_ResetIO(GPIO_TypeDef *GPIOx, uint32_t io_mask);
void ARM_GPIO_SetIO(GPIO_TypeDef *GPIOx, uint32_t io_mask);

#endif //_ARM_GPIO_H_ 
