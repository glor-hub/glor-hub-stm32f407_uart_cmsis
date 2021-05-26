//********************************************************************************
//.c
//********************************************************************************
#include "stm32f4xx.h"
#include "discovery-kit.h"
#include "led.h"
#include "gpio.h"
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
typedef enum {
    LED_ORANGE = 0,
    LED_RED,
    LED_BLUE,
    LED_GREEN,
    MAX_LEDS
} eLED_Types;

typedef struct {
    eLED_Types color;
    GPIO_TypeDef *GPIOx;
    eGPIO_PortNames port;
    eGPIO_IONumbers pin;
    uint32_t pin_mask;
    uint32_t pin_active_state;
} LED_Cfg_t;

//********************************************************************************
//Variables
//********************************************************************************
static LED_Cfg_t LED_Cfg[MAX_LEDS];
//********************************************************************************
//Prototypes
//********************************************************************************
static void LED_Set(eLED_Types led, uint32_t state);
static void LED_Set_Cfg(void);
static void Delay(uint32_t tick);
//================================================================================
//Public
//================================================================================
void LED_Test(void)
{
    eLED_Types led;
    for(led = LED_ORANGE; led < MAX_LEDS; led++) {
        LED_Set(led, TRUE);
    }
    Delay(32000000);
    for(led = LED_ORANGE; led < MAX_LEDS; led++) {
        LED_Set(led, FALSE);
    }
    Delay(32000000);
    for(led = LED_ORANGE; led < MAX_LEDS; led++) {
        LED_Set(led, TRUE);
        Delay(32000000);
        LED_Set(led, FALSE);
        Delay(32000000);
    }
}

void LED_Init(void)
{

    eLED_Types led;
    eGPIO_PortNames port;
    eGPIO_IONumbers pin;
    GPIO_TypeDef *preg;
    LED_Set_Cfg();
    for(led = LED_ORANGE; led < MAX_LEDS; led++) {
        port = LED_Cfg[led].port;
        preg = LED_Cfg[led].GPIOx;
        pin = LED_Cfg[led].pin;
        ARM_RCC_GPIO_ClockCmd(port, ENABLE_CMD);
        GPIO_SetCfg(preg, pin, GPIO_IO_MODE_OUTPUT, GPIO_IO_TYPE_PUSH_PULL, GPIO_IO_HI_Z,
                    GPIO_IO_SPEED_FREQ_LOW, GPIO_IO_AF_0);
        ARM_GPIO_Config();
    }
}

//================================================================================
//Private
//================================================================================
static void LED_Set_Cfg(void)
{
    LED_Cfg[LED_ORANGE].color = LED_ORANGE;
    LED_Cfg[LED_ORANGE].GPIOx = GPIOD;
    LED_Cfg[LED_ORANGE].port = GPIO_PORT_D;
    LED_Cfg[LED_ORANGE].pin = GPIO_IO_13;
    LED_Cfg[LED_ORANGE].pin_mask = GPIO_IO_13_MASK;
    LED_Cfg[LED_ORANGE].pin_active_state = GPIO_IO_SET;
    LED_Cfg[LED_RED].color = LED_RED;
    LED_Cfg[LED_RED].GPIOx = GPIOD;
    LED_Cfg[LED_RED].port = GPIO_PORT_D;
    LED_Cfg[LED_RED].pin = GPIO_IO_14;
    LED_Cfg[LED_RED].pin_mask = GPIO_IO_14_MASK;
    LED_Cfg[LED_RED].pin_active_state = GPIO_IO_SET;
    LED_Cfg[LED_BLUE].color = LED_BLUE;
    LED_Cfg[LED_BLUE].GPIOx = GPIOD;
    LED_Cfg[LED_BLUE].port = GPIO_PORT_D;
    LED_Cfg[LED_BLUE].pin = GPIO_IO_15;
    LED_Cfg[LED_BLUE].pin_mask = GPIO_IO_15_MASK;
    LED_Cfg[LED_BLUE].pin_active_state = GPIO_IO_SET;
    LED_Cfg[LED_GREEN].color = LED_GREEN;
    LED_Cfg[LED_GREEN].GPIOx = GPIOD;
    LED_Cfg[LED_GREEN].port = GPIO_PORT_D;
    LED_Cfg[LED_GREEN].pin = GPIO_IO_12;
    LED_Cfg[LED_GREEN].pin_mask = GPIO_IO_12_MASK;
    LED_Cfg[LED_GREEN].pin_active_state = GPIO_IO_SET;
}

static void LED_Set(eLED_Types led, uint32_t state)
{

    if(state) {
        if(LED_Cfg[led].pin_active_state == GPIO_IO_SET) {
            ARM_GPIO_SetIO(LED_Cfg[led].GPIOx, LED_Cfg[led].pin_mask);
        } else {
            ARM_GPIO_ResetIO(LED_Cfg[led].GPIOx, LED_Cfg[led].pin_mask);
        }
    } else {
        if(LED_Cfg[led].pin_active_state == GPIO_IO_SET) {
            ARM_GPIO_ResetIO(LED_Cfg[led].GPIOx, LED_Cfg[led].pin_mask);
        } else {
            ARM_GPIO_SetIO(LED_Cfg[led].GPIOx, LED_Cfg[led].pin_mask);
        }

    }
}

static void Delay(uint32_t tick)
{
    while(tick--);
    return;
}
