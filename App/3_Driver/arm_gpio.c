//********************************************************************************
//gpio.c
//********************************************************************************

#include "stm32f4xx.h"
#include "arm_gpio.h"

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
void ARM_GPIO_SetCfg(ARM_GPIO_Cfg_t *pGPIO_Cfg)
{
    uint32_t position, tmp;
    GPIO_TypeDef *p_reg;
    p_reg = pGPIO_Cfg->pReg;
    position = pGPIO_Cfg->Pin;
    p_reg->MODER &= ~(GPIO_MODER_MODER0 << (position * 2U));
    p_reg->MODER |= pGPIO_Cfg->Mode << (position * 2U);
    p_reg->OTYPER &= ~GPIO_OTYPER_OT0_Msk;
    p_reg->OTYPER |= pGPIO_Cfg->Type << position;
    p_reg->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
    p_reg->PUPDR |= pGPIO_Cfg->Pull << (position * 2U);
    p_reg->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED0_Msk;
    p_reg->OSPEEDR |= pGPIO_Cfg->Speed << (position * 2U);
    if(pGPIO_Cfg->Mode == ARM_GPIO_IO_MODE_ALT_FUNC) {
        tmp = p_reg->AFR[position >> 3U];
        tmp &= ~(GPIO_AFRL_AFSEL0_Msk << ((position & 0x07U) * 4U));
        tmp |= pGPIO_Cfg->AltFunc << ((position & 0x07U) * 4U);
        p_reg->AFR[position >> 3U] = tmp;
    }
}

uint32_t ARM_GPIO_ReadIO(GPIO_TypeDef *GPIOx, uint32_t io_mask)
{
    uint32_t bitstate;
    if((GPIOx->IDR & io_mask) != ARM_GPIO_IO_RESET) {
        bitstate = ARM_GPIO_IO_SET;
    } else {
        bitstate = ARM_GPIO_IO_RESET;
    }
    return bitstate;
}

void ARM_GPIO_SetIO(GPIO_TypeDef *GPIOx, uint32_t io_mask)
{
    GPIOx->BSRR = io_mask;
}
void ARM_GPIO_ResetIO(GPIO_TypeDef *GPIOx, uint32_t io_mask)
{
    GPIOx->BSRR = io_mask << 16U;
}
//================================================================================
//Private
//================================================================================


