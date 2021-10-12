#ifndef _GPIO_H_
#define _GPIO_H_

typedef enum {
    GPIO_IO_0 = 0,
    GPIO_IO_1,
    GPIO_IO_2,
    GPIO_IO_3,
    GPIO_IO_4,
    GPIO_IO_5,
    GPIO_IO_6,
    GPIO_IO_7,
    GPIO_IO_8,
    GPIO_IO_9,
    GPIO_IO_10,
    GPIO_IO_11,
    GPIO_IO_12,
    GPIO_IO_13,
    GPIO_IO_14,
    GPIO_IO_15,
    NUM_GPIO_IO
} eGPIO_IONumbers;

void GPIO_SetData(GPIO_TypeDef *GPIOx, eGPIO_IONumbers io_num, uint32_t io_mode, uint32_t io_type,
                  uint32_t io_pull, uint32_t io_speed, uint32_t io_alt);

void GPIO_Init(void);

#endif //_GPIO_H_ 
