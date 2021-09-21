//********************************************************************************
//gpio.c
//********************************************************************************
#include "stm32f4xx.h"
#include "discovery-kit.h"
#include "gpio.h"
#include "Driver_USART.h"
#include "usart.h"
#include "arm_gpio.h"
#include "arm_clock.h"

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
static GPIO_Cfg_t GPIO_Config;


//********************************************************************************
//Prototypes
//********************************************************************************

//================================================================================
//Public
//================================================================================

void GPIO_SetCfg(GPIO_TypeDef *GPIOx, eGPIO_IONumbers io_num, uint32_t io_mode, uint32_t io_type,
                 uint32_t io_pull, uint32_t io_speed, uint32_t io_alt)

{

    GPIO_Config.pReg = GPIOx;
    GPIO_Config.Pin = io_num;
    GPIO_Config.Mode = io_mode;
    GPIO_Config.Type = io_type;
    GPIO_Config.Pull = io_pull;
    GPIO_Config.Speed = io_speed;
    GPIO_Config.AltFunc = io_alt; //used only for alternate mode
}

void GPIO_Init(void)
{

}

GPIO_Cfg_t *GPIO_GetConfig(void)
{
    return &GPIO_Config;
}

//================================================================================
//Private
//================================================================================
