//********************************************************************************
//gpio.c
//********************************************************************************
#include "stm32f4xx.h"
#include "gpio.h"
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
void ARM_GPIO_Config(void)
{
    uint32_t position, tmp;
    GPIO_TypeDef *p_reg;
    GPIO_Cfg_t *pCfgstruct = GPIO_GetConfig();
    p_reg = pCfgstruct->pReg;
    position = pCfgstruct->Pin;
    p_reg->MODER &= ~(GPIO_MODER_MODER0 << (position * 2U));
    p_reg->MODER |= pCfgstruct->Mode << (position * 2U);
    p_reg->OTYPER &= ~GPIO_OTYPER_OT0_Msk;
    p_reg->OTYPER |= pCfgstruct->Type << position;
    p_reg->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
    p_reg->PUPDR |= pCfgstruct->Pull << (position * 2U);
    p_reg->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED0_Msk;
    p_reg->OSPEEDR |= pCfgstruct->Speed << (position * 2U);
    if(pCfgstruct->Mode == GPIO_IO_MODE_ALT_FUNC) {
        tmp = p_reg->AFR[position >> 3U];
        tmp &= ~(GPIO_AFRL_AFSEL0_Msk << ((position & 0x07U) * 4U));
        tmp |= pCfgstruct->AltFunc << ((position & 0x07U) * 4U);
        p_reg->AFR[position >> 3U] = tmp;
    }
}

uint32_t ARM_GPIO_ReadIO(GPIO_TypeDef *GPIOx, uint32_t io_mask)
{
    uint32_t bitstate;
    if((GPIOx->IDR & io_mask) != GPIO_IO_RESET) {
        bitstate = GPIO_IO_SET;
    } else {
        bitstate = GPIO_IO_RESET;
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


