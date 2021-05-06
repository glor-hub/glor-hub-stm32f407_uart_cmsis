#ifndef _GPIO_H_
#define _GPIO_H_

typedef enum {
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_G,
    GPIO_PORT_H,
    GPIO_PORT_I,
    NUM_GPIO
} GPIO_PortNames_enum;


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
} GPIO_IONumbers_enum;

//during and just after reset the IOs configuration is input floating (Hi-Z Input).

typedef enum {
    GPIO_IO_MODE_OUTPUT = (uint32_t)0x01,
    GPIO_IO_MODE_INPUT = (uint32_t)0x00,
    GPIO_IO_MODE_ANALOG = (uint32_t)0x03, // for DAC/ADC
    GPIO_IO_MODE_ALT_FUNC = (uint32_t)0x02,
} GPIO_IOModes_enum;

typedef enum {
    GPIO_IO_TYPE_PUSH_PULL = (uint32_t)0x00,
    GPIO_IO_TYPE_OPEN_DRAIN = (uint32_t)0x01,
    GPIO_IO_NO_TYPE = (uint32_t)0x00
} GPIO_IOTypes_enum;

typedef enum {
    GPIO_IO_HI_Z = (uint32_t)0x00,
    GPIO_IO_PULL_UP = (uint32_t)0x01,
    GPIO_IO_PULL_DOWN = (uint32_t)0x02
} GPIO_IOPulls_enum;


typedef enum {
    GPIO_IO_SPEED_FREQ_LOW = (uint32_t)0x00,
    GPIO_IO_SPEED_FREQ_MEDIUM = (uint32_t)0x01,
    GPIO_IO_SPEED_FREQ_HIGH = (uint32_t)0x02,
    GPIO_IO_SPEED_FREQ_VERY_HIGH = (uint32_t)0x03
} GPIO_IOSpeedValues_enum;

typedef struct {
    GPIO_TypeDef *pReg;
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Type;
    uint32_t Pull;
    uint32_t Speed; //use for output and alt_func IOs
} GPIO_Cfg_struct;

GPIO_Cfg_struct *GPIO_GetConfig(void);
void GPIO_SetCfg(GPIO_TypeDef *GPIOx, GPIO_IONumbers_enum io_num, GPIO_IOModes_enum io_mode, GPIO_IOTypes_enum io_type,
                 GPIO_IOPulls_enum io_pull, GPIO_IOSpeedValues_enum io_speed);

void GPIO_Init(void);
void GPIO_Test(void);

#endif //_GPIO_H_ 
