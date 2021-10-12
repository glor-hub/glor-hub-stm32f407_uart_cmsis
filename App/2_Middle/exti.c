//********************************************************************************
//exti.c
//********************************************************************************
#include "stm32f4xx.h"
#include "common.h"
#include "arm_gpio.h"
#include "arm_exti.h"
#include "gpio.h"
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

static ARM_EXTI_Cfg_t EXTI_Config;
static uint8_t ARM_EXTI_Flag;

//********************************************************************************
//Prototypes
//********************************************************************************

static void EXTI_SetData(ePeriphTypes port_name, eARM_GPIO_IONumbers pin_num,
                         eARM_EXTI_TriggerModes mode);

//================================================================================
//Public
//================================================================================

void EXTI_Init(void)
{
    EXTI_SetData(GPIO_PORT_A, ARM_GPIO_IO_0, ARM_EXTI_FALLING_RISING_TRIGGER_MODE);
    ARM_EXTI_ClearPendingIRQ(ARM_GPIO_IO_0);
    ARM_EXTI_Flag = FALSE;
    ARM_EXTI_IRQEnable(ARM_GPIO_IO_0, ENABLE_CMD);
    NVIC_EnableIRQ(EXTI0_IRQn);//EXTI Line0 Interrupt for pin 0
}

void EXTI0_IRQHandler(void)
{
    ARM_EXTI_ClearPendingIRQ(ARM_GPIO_IO_0);
    ARM_EXTI_Flag = TRUE;

}

uint8_t EXTI_GetFlag(void)
{
    return ARM_EXTI_Flag;
}

void EXTI_ClearFlag(void)
{
    ARM_EXTI_Flag = FALSE;
}

//================================================================================
//Private
//================================================================================

void EXTI_SetData(ePeriphTypes port_name, eARM_GPIO_IONumbers pin_num,
                  eARM_EXTI_TriggerModes mode)
{
    EXTI_Config.Port = port_name;
    EXTI_Config.Pin = pin_num;
    EXTI_Config.Trigger_Mode = mode;
    ARM_EXTI_SetCfg(&EXTI_Config);
}
