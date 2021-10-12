//********************************************************************************
//arm_exti.c
//********************************************************************************
#include "stm32f4xx.h"
#include "common.h"
#include "arm_gpio.h"
//#include "gpio.h"
#include "arm_exti.h"

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

static void ARM_EXTI_EventEnable(eARM_GPIO_IONumbers pin_num, ePeriphCmd cmd);

//================================================================================
//Public
//================================================================================
void ARM_EXTI_SetCfg(ARM_EXTI_Cfg_t *pEXTI_Cfg)
{
    ePeriphTypes port_name;
    eARM_GPIO_IONumbers pin_num;
    eARM_EXTI_TriggerModes mode;
    SYSCFG_TypeDef *p_cfgReg = SYSCFG;
    EXTI_TypeDef *p_reg = EXTI;
    port_name = pEXTI_Cfg->Port;
    pin_num = pEXTI_Cfg->Pin;
    mode = pEXTI_Cfg->Trigger_Mode;
    switch(pin_num) {
        case ARM_GPIO_IO_0:
        case ARM_GPIO_IO_1:
        case ARM_GPIO_IO_2:
        case ARM_GPIO_IO_3: {
            p_cfgReg->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0_Msk << (4U * (uint32_t)pin_num));
            p_cfgReg->EXTICR[0] |= (uint32_t)port_name <<
                                   (4U * ((uint32_t)pin_num >> SYSCFG_EXTICR1_EXTI0_Pos));
            break;
        }
        case ARM_GPIO_IO_4:
        case ARM_GPIO_IO_5:
        case ARM_GPIO_IO_6:
        case ARM_GPIO_IO_7: {
            p_cfgReg->EXTICR[1] &= ~(SYSCFG_EXTICR1_EXTI0_Msk << (4U * (uint32_t)pin_num));
            p_cfgReg->EXTICR[1] |= (uint32_t)port_name <<
                                   (4U * ((uint32_t)pin_num >> SYSCFG_EXTICR1_EXTI1_Pos));
            break;
        }
        case ARM_GPIO_IO_8:
        case ARM_GPIO_IO_9:
        case ARM_GPIO_IO_10:
        case ARM_GPIO_IO_11: {
            p_cfgReg->EXTICR[2] &= ~(SYSCFG_EXTICR1_EXTI0_Msk << (4U * (uint32_t)pin_num));
            p_cfgReg->EXTICR[2] |= (uint32_t)port_name <<
                                   (4U * ((uint32_t)pin_num >> SYSCFG_EXTICR1_EXTI2_Pos));
            break;
        }
        case ARM_GPIO_IO_12:
        case ARM_GPIO_IO_13:
        case ARM_GPIO_IO_14:
        case ARM_GPIO_IO_15: {
            p_cfgReg->EXTICR[3] &= ~(SYSCFG_EXTICR1_EXTI0_Msk << (4U * (uint32_t)pin_num));
            p_cfgReg->EXTICR[3] |= (uint32_t)port_name <<
                                   (4U * ((uint32_t)pin_num >> SYSCFG_EXTICR1_EXTI3_Pos));
            break;
        }

    }
    switch(mode) {
        case ARM_EXTI_FALLING_TRIGGER_MODE: {
            p_reg->RTSR &= ~(EXTI_RTSR_TR0_Msk << (uint32_t)pin_num);
            p_reg->FTSR |= EXTI_FTSR_TR0_Msk << (uint32_t)pin_num;
            break;
        }
        case ARM_EXTI_RISING_TRIGGER_MODE: {
            p_reg->RTSR |= EXTI_RTSR_TR0_Msk << (uint32_t)pin_num;
            p_reg->FTSR &= ~(EXTI_FTSR_TR0_Msk << (uint32_t)pin_num);
            break;
        }
        case ARM_EXTI_FALLING_RISING_TRIGGER_MODE: {
            p_reg->RTSR |= EXTI_RTSR_TR0_Msk << (uint32_t)pin_num;
            p_reg->FTSR |= EXTI_FTSR_TR0_Msk << (uint32_t)pin_num;
            break;
        }
    }
}

void ARM_EXTI_IRQEnable(eARM_GPIO_IONumbers pin_num, ePeriphCmd cmd)
{
    EXTI_TypeDef *p_reg = EXTI;
    if(cmd == ENABLE_CMD) {
        p_reg->IMR |= EXTI_IMR_MR0_Msk << (uint32_t)pin_num;
    } else {

        p_reg->IMR &= ~(EXTI_IMR_MR0_Msk << (uint32_t)pin_num);
    }
}

void ARM_EXTI_ClearPendingIRQ(eARM_GPIO_IONumbers pin_num)
{
    EXTI_TypeDef *p_reg = EXTI;
    p_reg->PR |= EXTI_PR_PR0_Msk << (uint32_t)pin_num;
}

//================================================================================
//Private
//================================================================================

void ARM_EXTI_EventEnable(eARM_GPIO_IONumbers pin_num,
                          ePeriphCmd cmd)
{
    EXTI_TypeDef *p_reg = EXTI;
    if(cmd == ENABLE_CMD) {
        p_reg->EMR |= EXTI_EMR_MR0_Msk << (uint32_t)pin_num;
    } else {

        p_reg->EMR &= ~(EXTI_EMR_MR0_Msk << (uint32_t)pin_num);
    }
}
