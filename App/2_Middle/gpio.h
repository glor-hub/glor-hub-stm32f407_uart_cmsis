#ifndef _GPIO_H_
#define _GPIO_H_

typedef enum {
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_G,
    GPIO_PORT_H,
    GPIO_PORT_I,
    NUM_GPIO
} eGPIO_PortNames;


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

typedef enum {
    GPIO_IO_AF_0 = 0,
    GPIO_IO_AF_1,
    GPIO_IO_AF_2,
    GPIO_IO_AF_3,
    GPIO_IO_AF_4,
    GPIO_IO_AF_5,
    GPIO_IO_AF_6,
    GPIO_IO_AF_7,
    GPIO_IO_AF_8,
    GPIO_IO_AF_9,
    GPIO_IO_AF_10,
    GPIO_IO_AF_11,
    GPIO_IO_AF_12,
    GPIO_IO_AF_13,
    GPIO_IO_AF_14,
    GPIO_IO_AF_15,
    NUM_GPIO_IO_AF
} eGPIO_IOAltFunc;


#define GPIO_IO_RESET  ((uint32_t)0x00)
#define GPIO_IO_SET  ((uint32_t)0x01)

#define GPIO_IO_0_MASK  ((uint32_t)0x00000001)
#define GPIO_IO_1_MASK  ((uint32_t)0x00000002)
#define GPIO_IO_2_MASK  ((uint32_t)0x00000004)
#define GPIO_IO_3_MASK  ((uint32_t)0x00000008)
#define GPIO_IO_4_MASK  ((uint32_t)0x00000010)
#define GPIO_IO_5_MASK  ((uint32_t)0x00000020)
#define GPIO_IO_6_MASK  ((uint32_t)0x00000040)
#define GPIO_IO_7_MASK  ((uint32_t)0x00000080)
#define GPIO_IO_8_MASK  ((uint32_t)0x00000100)
#define GPIO_IO_9_MASK  ((uint32_t)0x00000200)
#define GPIO_IO_10_MASK  ((uint32_t)0x00000400)
#define GPIO_IO_11_MASK  ((uint32_t)0x00000800)
#define GPIO_IO_12_MASK  ((uint32_t)0x00001000)
#define GPIO_IO_13_MASK  ((uint32_t)0x00002000)
#define GPIO_IO_14_MASK  ((uint32_t)0x00004000)
#define GPIO_IO_15_MASK  ((uint32_t)0x00008000)

//during and just after reset the IOs configuration is input floating (Hi-Z Input).

#define GPIO_IO_MODE_OUTPUT  ((uint32_t)0x01)
#define GPIO_IO_MODE_INPUT  ((uint32_t)0x00)
#define GPIO_IO_MODE_ANALOG  ((uint32_t)0x03) // for DAC/ADC
#define GPIO_IO_MODE_ALT_FUNC  ((uint32_t)0x02)

#define GPIO_IO_TYPE_PUSH_PULL ((uint32_t)0x00)
#define GPIO_IO_TYPE_OPEN_DRAIN ((uint32_t)0x01)
#define GPIO_IO_TYPE_NO ((uint32_t)0x00)

#define GPIO_IO_HI_Z ((uint32_t)0x00)
#define GPIO_IO_PULL_UP ((uint32_t)0x01)
#define GPIO_IO_PULL_DOWN ((uint32_t)0x02)

#define GPIO_IO_SPEED_FREQ_LOW ((uint32_t)0x00)
#define GPIO_IO_SPEED_FREQ_MEDIUM ((uint32_t)0x01)
#define GPIO_IO_SPEED_FREQ_HIGH ((uint32_t)0x02)
#define GPIO_IO_SPEED_FREQ_VERY_HIGH ((uint32_t)0x03)

typedef struct {
    GPIO_TypeDef *pReg;
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Type;
    uint32_t Pull;
    uint32_t Speed; //use for output and alt_func IOs
    uint32_t AltFunc;
} GPIO_Cfg_t;

GPIO_Cfg_t *GPIO_GetConfig(void);
void GPIO_SetCfg(GPIO_TypeDef *GPIOx, eGPIO_IONumbers io_num, uint32_t io_mode, uint32_t io_type,
                 uint32_t io_pull, uint32_t io_speed, uint32_t io_alt);

void GPIO_Init(void);

#endif //_GPIO_H_ 
