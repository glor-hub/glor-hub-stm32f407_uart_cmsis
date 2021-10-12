//********************************************************************************
//.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdbool.h>
#include "common.h"
#include "arm_clock.h"
#include "arm_gpio.h"
#include "gpio.h"
#include "timer.h"
#include "led.h"

//********************************************************************************
//Macros
//********************************************************************************

//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************

typedef enum {
    ORANGE = 0,
    RED,
    BLUE,
    GREEN,
    MAX_LED_COLORS
} eLED_Colors;

typedef struct {
    eLED_Colors color;
    GPIO_TypeDef *GPIOx;
    ePeriphTypes port;
    eARM_GPIO_IONumbers pin;
    uint32_t pin_mask;
    uint32_t pin_active_state;
} LED_Cfg_t;

//********************************************************************************
//Variables
//********************************************************************************
static LED_Cfg_t LED_Cfg[MAX_LED_COLORS];
//********************************************************************************
//Prototypes
//********************************************************************************
static void LED_Set(eLED_Colors led_color, uint32_t state);
static void LED_SetCfg(void);
//================================================================================
//Public
//================================================================================
void LED_Test(void)
{
    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led_color, TRUE);
    }
    Timer_Delay(ONE_SECOND);

    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led_color, FALSE);
    }
    Timer_Delay(ONE_SECOND);

    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led_color, TRUE);
        Timer_Delay(ONE_SECOND);
        LED_Set(led_color, FALSE);
        Timer_Delay(ONE_SECOND);
    }
}

void LED_Init(void)
{
    LED_SetCfg();
    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        ARM_RCC_Periph_ClockCmd(LED_Cfg[led_color].port, ENABLE_CMD);
        GPIO_SetData(LED_Cfg[led_color].GPIOx, LED_Cfg[led_color].pin,
                     ARM_GPIO_IO_MODE_OUTPUT, ARM_GPIO_IO_TYPE_PUSH_PULL,
                     ARM_GPIO_IO_HI_Z, ARM_GPIO_IO_SPEED_FREQ_LOW, ARM_GPIO_IO_AF_0);
    }
}

//================================================================================
//Private
//================================================================================
static void LED_SetCfg(void)
{
    LED_Cfg[ORANGE].color = ORANGE;
    LED_Cfg[ORANGE].GPIOx = GPIOD;
    LED_Cfg[ORANGE].port = GPIO_PORT_D;
    LED_Cfg[ORANGE].pin = ARM_GPIO_IO_13;
    LED_Cfg[ORANGE].pin_mask = ARM_GPIO_IO_13_MASK;
    LED_Cfg[ORANGE].pin_active_state = ARM_GPIO_IO_SET;

    LED_Cfg[RED].color = RED;
    LED_Cfg[RED].GPIOx = GPIOD;
    LED_Cfg[RED].port = GPIO_PORT_D;
    LED_Cfg[RED].pin = ARM_GPIO_IO_14;
    LED_Cfg[RED].pin_mask = ARM_GPIO_IO_14_MASK;
    LED_Cfg[RED].pin_active_state = ARM_GPIO_IO_SET;

    LED_Cfg[BLUE].color = BLUE;
    LED_Cfg[BLUE].GPIOx = GPIOD;
    LED_Cfg[BLUE].port = GPIO_PORT_D;
    LED_Cfg[BLUE].pin = ARM_GPIO_IO_15;
    LED_Cfg[BLUE].pin_mask = ARM_GPIO_IO_15_MASK;
    LED_Cfg[BLUE].pin_active_state = ARM_GPIO_IO_SET;

    LED_Cfg[GREEN].color = GREEN;
    LED_Cfg[GREEN].GPIOx = GPIOD;
    LED_Cfg[GREEN].port = GPIO_PORT_D;
    LED_Cfg[GREEN].pin = ARM_GPIO_IO_12;
    LED_Cfg[GREEN].pin_mask = ARM_GPIO_IO_12_MASK;
    LED_Cfg[GREEN].pin_active_state = ARM_GPIO_IO_SET;
}

static void LED_Set(eLED_Colors led_color, uint32_t state)
{
    if(state) {
        if(LED_Cfg[led_color].pin_active_state == ARM_GPIO_IO_SET) {
            ARM_GPIO_SetIO(LED_Cfg[led_color].GPIOx, LED_Cfg[led_color].pin_mask);
        } else {
            ARM_GPIO_ResetIO(LED_Cfg[led_color].GPIOx, LED_Cfg[led_color].pin_mask);
        }
    } else {
        if(LED_Cfg[led_color].pin_active_state == ARM_GPIO_IO_SET) {
            ARM_GPIO_ResetIO(LED_Cfg[led_color].GPIOx, LED_Cfg[led_color].pin_mask);
        } else {
            ARM_GPIO_SetIO(LED_Cfg[led_color].GPIOx, LED_Cfg[led_color].pin_mask);
        }
    }
}
