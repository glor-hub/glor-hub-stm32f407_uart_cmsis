#ifndef _ARM_GPIO_H_
#define _ARM_GPIO_H_

#define ARM_GPIO_IO_RESET    ((uint32_t)0UL)
#define ARM_GPIO_IO_SET      ((uint32_t)1UL)

#define ARM_GPIO_IO_0_MASK   ((uint32_t)1UL << 0)
#define ARM_GPIO_IO_1_MASK   ((uint32_t)1UL << 1)
#define ARM_GPIO_IO_2_MASK   ((uint32_t)1UL << 2)
#define ARM_GPIO_IO_3_MASK   ((uint32_t)1UL << 3)
#define ARM_GPIO_IO_4_MASK   ((uint32_t)1UL << 4)
#define ARM_GPIO_IO_5_MASK   ((uint32_t)1UL << 5)
#define ARM_GPIO_IO_6_MASK   ((uint32_t)1UL << 6)
#define ARM_GPIO_IO_7_MASK   ((uint32_t)1UL << 7)
#define ARM_GPIO_IO_8_MASK   ((uint32_t)1UL << 8)
#define ARM_GPIO_IO_9_MASK   ((uint32_t)1UL << 9)
#define ARM_GPIO_IO_10_MASK  ((uint32_t)1UL << 10)
#define ARM_GPIO_IO_11_MASK  ((uint32_t)1UL << 11)
#define ARM_GPIO_IO_12_MASK  ((uint32_t)1UL << 12)
#define ARM_GPIO_IO_13_MASK  ((uint32_t)1UL << 13)
#define ARM_GPIO_IO_14_MASK  ((uint32_t)1UL << 14)
#define ARM_GPIO_IO_15_MASK  ((uint32_t)1UL << 15)

//during and just after reset the IOs configuration is input floating (Hi-Z Input).

#define ARM_GPIO_IO_MODE_OUTPUT          ((uint32_t)0x01)
#define ARM_GPIO_IO_MODE_INPUT           ((uint32_t)0x00)
#define ARM_GPIO_IO_MODE_ANALOG          ((uint32_t)0x03) // for DAC/ADC
#define ARM_GPIO_IO_MODE_ALT_FUNC        ((uint32_t)0x02)

#define ARM_GPIO_IO_TYPE_PUSH_PULL       ((uint32_t)0x00)
#define ARM_GPIO_IO_TYPE_OPEN_DRAIN      ((uint32_t)0x01)
#define ARM_GPIO_IO_TYPE_NO              ((uint32_t)0x00)

#define ARM_GPIO_IO_HI_Z                 ((uint32_t)0x00)
#define ARM_GPIO_IO_PULL_UP              ((uint32_t)0x01)
#define ARM_GPIO_IO_PULL_DOWN            ((uint32_t)0x02)

#define ARM_GPIO_IO_SPEED_FREQ_LOW       ((uint32_t)0x00) // max 8 MHz depending on load
#define ARM_GPIO_IO_SPEED_FREQ_MEDIUM    ((uint32_t)0x01) // max 25-50 MHz
#define ARM_GPIO_IO_SPEED_FREQ_HIGH      ((uint32_t)0x02) // max 50-100 MHz
#define ARM_GPIO_IO_SPEED_FREQ_VERY_HIGH ((uint32_t)0x03) // max 100-180 MHz

typedef enum {
    ARM_GPIO_IO_0 = 0,
    ARM_GPIO_IO_1,
    ARM_GPIO_IO_2,
    ARM_GPIO_IO_3,
    ARM_GPIO_IO_4,
    ARM_GPIO_IO_5,
    ARM_GPIO_IO_6,
    ARM_GPIO_IO_7,
    ARM_GPIO_IO_8,
    ARM_GPIO_IO_9,
    ARM_GPIO_IO_10,
    ARM_GPIO_IO_11,
    ARM_GPIO_IO_12,
    ARM_GPIO_IO_13,
    ARM_GPIO_IO_14,
    ARM_GPIO_IO_15,
    NUM_ARM_GPIO_IO
} eARM_GPIO_IONumbers;

typedef enum {
    ARM_GPIO_IO_AF_0 = 0,
    ARM_GPIO_IO_AF_1,
    ARM_GPIO_IO_AF_2,
    ARM_GPIO_IO_AF_3,
    ARM_GPIO_IO_AF_4,
    ARM_GPIO_IO_AF_5,
    ARM_GPIO_IO_AF_6,
    ARM_GPIO_IO_AF_7,
    ARM_GPIO_IO_AF_8,
    ARM_GPIO_IO_AF_9,
    ARM_GPIO_IO_AF_10,
    ARM_GPIO_IO_AF_11,
    ARM_GPIO_IO_AF_12,
    ARM_GPIO_IO_AF_13,
    ARM_GPIO_IO_AF_14,
    ARM_GPIO_IO_AF_15,
    NUM_ARM_GPIO_IO_AF
} eARM_GPIO_IOAltFunc;

typedef struct {
    GPIO_TypeDef *pReg;
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Type;
    uint32_t Pull;
    uint32_t Speed; //use for output and alt_func IOs
    uint32_t AltFunc; // ARM_GPIO_IO_AF_0 after reset
} ARM_GPIO_Cfg_t;

void ARM_GPIO_SetCfg(ARM_GPIO_Cfg_t *pGPIO_Cfg);
uint32_t ARM_GPIO_ReadIO(GPIO_TypeDef *GPIOx, uint32_t io_mask);
void ARM_GPIO_ResetIO(GPIO_TypeDef *GPIOx, uint32_t io_mask);
void ARM_GPIO_SetIO(GPIO_TypeDef *GPIOx, uint32_t io_mask);

#endif //_ARM_GPIO_H_ 
