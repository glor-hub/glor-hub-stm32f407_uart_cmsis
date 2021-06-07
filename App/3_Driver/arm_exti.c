//********************************************************************************
//arm_exti.c
//********************************************************************************
#include "stm32f4xx.h"
#include "discovery-kit.h"
#include "gpio.h"
#include "arm_exti.h"
#include "exti.h"

//********************************************************************************
//Macros
//********************************************************************************

//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************

//********************************************************************************
//Variables
//********************************************************************************

//********************************************************************************
//Prototypes
//********************************************************************************

//================================================================================
//Public
//================================================================================
void ARM_EXTI_SetPinCfg(eGPIO_PortNames port_name, eGPIO_IONumbers pin_num,
                        uint8_t trigger_mode)
{
    SYSCFG_TypeDef *p_cfgReg = SYSCFG;
    EXTI_TypeDef *p_reg = EXTI;
    switch(pin_num) {
        case GPIO_IO_0:
        case GPIO_IO_1:
        case GPIO_IO_2:
        case GPIO_IO_3: {
            p_cfgReg->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0_Msk << (4U * (uint32_t)pin_num));
            p_cfgReg->EXTICR[0] |= (uint32_t)port_name <<
                                   (4U * ((uint32_t)pin_num >> SYSCFG_EXTICR1_EXTI0_Pos));
            break;
        }
        case GPIO_IO_4:
        case GPIO_IO_5:
        case GPIO_IO_6:
        case GPIO_IO_7: {
            p_cfgReg->EXTICR[1] &= ~(SYSCFG_EXTICR1_EXTI0_Msk << (4U * (uint32_t)pin_num));
            p_cfgReg->EXTICR[1] |= (uint32_t)port_name <<
                                   (4U * ((uint32_t)pin_num >> SYSCFG_EXTICR1_EXTI1_Pos));
            break;
        }
        case GPIO_IO_8:
        case GPIO_IO_9:
        case GPIO_IO_10:
        case GPIO_IO_11: {
            p_cfgReg->EXTICR[2] &= ~(SYSCFG_EXTICR1_EXTI0_Msk << (4U * (uint32_t)pin_num));
            p_cfgReg->EXTICR[2] |= (uint32_t)port_name <<
                                   (4U * ((uint32_t)pin_num >> SYSCFG_EXTICR1_EXTI2_Pos));
            break;
        }
        case GPIO_IO_12:
        case GPIO_IO_13:
        case GPIO_IO_14:
        case GPIO_IO_15: {
            p_cfgReg->EXTICR[3] &= ~(SYSCFG_EXTICR1_EXTI0_Msk << (4U * (uint32_t)pin_num));
            p_cfgReg->EXTICR[3] |= (uint32_t)port_name <<
                                   (4U * ((uint32_t)pin_num >> SYSCFG_EXTICR1_EXTI3_Pos));
            break;
        }

    }
    switch(trigger_mode) {
        case EXTI_FALLING_TRIGGER_MODE: {
            p_reg->RTSR &= ~(EXTI_RTSR_TR0_Msk << (uint32_t)pin_num);
            p_reg->FTSR |= EXTI_FTSR_TR0_Msk << (uint32_t)pin_num;
            break;
        }
        case EXTI_RISING_TRIGGER_MODE: {
            p_reg->RTSR |= EXTI_RTSR_TR0_Msk << (uint32_t)pin_num;
            p_reg->FTSR &= ~(EXTI_FTSR_TR0_Msk << (uint32_t)pin_num);
            break;
        }
        case EXTI_FALLING_RISING_TRIGGER_MODE: {
            p_reg->RTSR |= EXTI_RTSR_TR0_Msk << (uint32_t)pin_num;
            p_reg->FTSR |= EXTI_FTSR_TR0_Msk << (uint32_t)pin_num;
            break;
        }
    }
}

void ARM_EXTI_EventEnable(eGPIO_IONumbers pin_num,
                          ePeriphCmd cmd)
{
    EXTI_TypeDef *p_reg = EXTI;
    if(cmd == ENABLE_CMD) {
        p_reg->EMR |= EXTI_EMR_MR0_Msk << (uint32_t)pin_num;
    } else {

        p_reg->EMR &= ~(EXTI_EMR_MR0_Msk << (uint32_t)pin_num);
    }
}
void ARM_EXTI_IRQEnable(eGPIO_IONumbers pin_num, ePeriphCmd cmd)
{
    EXTI_TypeDef *p_reg = EXTI;
    if(cmd == ENABLE_CMD) {
        p_reg->IMR |= EXTI_IMR_MR0_Msk << (uint32_t)pin_num;
    } else {

        p_reg->IMR &= ~(EXTI_IMR_MR0_Msk << (uint32_t)pin_num);
    }
}

void ARM_EXTI_ClearPendingIRQ(eGPIO_IONumbers pin_num)
{
    EXTI_TypeDef *p_reg = EXTI;
    p_reg->PR |= EXTI_PR_PR0_Msk << (uint32_t)pin_num;
}

//================================================================================
//Private
//================================================================================

