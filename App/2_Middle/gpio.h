#ifndef _GPIO_H_
#define _GPIO_H_

void GPIO_SetData(GPIO_TypeDef *GPIOx, eARM_GPIO_IONumbers io_num, uint32_t io_mode, uint32_t io_type,
                  uint32_t io_pull, uint32_t io_speed, uint32_t io_alt);

void GPIO_Init(void);

#endif //_GPIO_H_ 
