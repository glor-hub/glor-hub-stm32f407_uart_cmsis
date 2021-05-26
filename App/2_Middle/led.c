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
    ORANGE = 0,
    RED,
    BLUE,
    GREEN,
    MAX_LED_COLORS
} eLED_Colors;

typedef struct {
    eLED_Colors color;
    GPIO_TypeDef *GPIOx;
    eGPIO_PortNames port;
    eGPIO_IONumbers pin;
    uint32_t pin_mask;
    uint32_t pin_active_state;
} LED_Data_t;

//********************************************************************************
//Variables
//********************************************************************************
static LED_Data_t LED_Data[MAX_LED_COLORS];
//********************************************************************************
//Prototypes
//********************************************************************************
static void LED_Set(eLED_Colors led_color, uint32_t state);
static void LED_Set_Cfg(void);
static void Delay(uint32_t tick);
//================================================================================
//Public
//================================================================================
void LED_Test(void)
{
    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led, TRUE);
    }
    Delay(32000000);

    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led, FALSE);
    }
    Delay(32000000);

    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led, TRUE);
        Delay(32000000);
        LED_Set(led, FALSE);
        Delay(32000000);
    }
}

void LED_Init(void)
{
    LED_Set_Cfg();
    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        ARM_RCC_GPIO_ClockCmd(LED_Data[led_color].port, ENABLE_CMD);
        GPIO_SetCfg(LED_Data[led_color].GPIOx, LED_Data[led_color].pin,
                    GPIO_IO_MODE_OUTPUT, GPIO_IO_TYPE_PUSH_PULL, GPIO_IO_HI_Z, GPIO_IO_SPEED_FREQ_LOW, GPIO_IO_AF_0);
        ARM_GPIO_Config();
    }
}

//================================================================================
//Private
//================================================================================
static void LED_Set_Cfg(void)
{
    LED_Data[ORANGE].color = ORANGE;
    LED_Data[ORANGE].GPIOx = GPIOD;
    LED_Data[ORANGE].port = GPIO_PORT_D;
    LED_Data[ORANGE].pin = GPIO_IO_13;
    LED_Data[ORANGE].pin_mask = GPIO_IO_13_MASK;
    LED_Data[ORANGE].pin_active_state = GPIO_IO_SET;

    LED_Data[RED].color = RED;
    LED_Data[RED].GPIOx = GPIOD;
    LED_Data[RED].port = GPIO_PORT_D;
    LED_Data[RED].pin = GPIO_IO_14;
    LED_Data[RED].pin_mask = GPIO_IO_14_MASK;
    LED_Data[RED].pin_active_state = GPIO_IO_SET;

    LED_Data[BLUE].color = BLUE;
    LED_Data[BLUE].GPIOx = GPIOD;
    LED_Data[BLUE].port = GPIO_PORT_D;
    LED_Data[BLUE].pin = GPIO_IO_15;
    LED_Data[BLUE].pin_mask = GPIO_IO_15_MASK;
    LED_Data[BLUE].pin_active_state = GPIO_IO_SET;

    LED_Data[GREEN].color = GREEN;
    LED_Data[GREEN].GPIOx = GPIOD;
    LED_Data[GREEN].port = GPIO_PORT_D;
    LED_Data[GREEN].pin = GPIO_IO_12;
    LED_Data[GREEN].pin_mask = GPIO_IO_12_MASK;
    LED_Data[GREEN].pin_active_state = GPIO_IO_SET;
}

static void LED_Set(eLED_Colors led_color, uint32_t state)
{
    if(state) {
        if(LED_Data[led_color].pin_active_state == GPIO_IO_SET) {
            ARM_GPIO_SetIO(LED_Data[led_color].GPIOx, LED_Data[led_color].pin_mask);
        } else {
            ARM_GPIO_ResetIO(LED_Data[led_color].GPIOx, LED_Data[led_color].pin_mask);
        }
    } else {
        if(LED_Data[led_color].pin_active_state == GPIO_IO_SET) {
            ARM_GPIO_ResetIO(LED_Data[led_color].GPIOx, LED_Data[led_color].pin_mask);
        } else {
            ARM_GPIO_SetIO(LED_Data[led_color].GPIOx, LED_Data[led_color].pin_mask);
        }
    }
}

static void Delay(uint32_t tick)
{
    while(tick--);
    return;
}
