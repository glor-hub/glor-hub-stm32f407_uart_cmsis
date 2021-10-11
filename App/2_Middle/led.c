//********************************************************************************
//.c
//********************************************************************************
#include "stm32f4xx.h"
#include "RTE_Device.h"
#include "common.h"
#include "led.h"
#include "gpio.h"
#include "Driver_USART.h"
#include "usart.h"
#include "arm_gpio.h"
#include "arm_clock.h"
#include "timer.h"

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
static void LED_SetData(void);
//================================================================================
//Public
//================================================================================
void LED_Test(void)
{
    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led_color, TRUE);
    }
    Delay(ONE_SECOND);

    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led_color, FALSE);
    }
    Delay(ONE_SECOND);

    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        LED_Set(led_color, TRUE);
        Delay(ONE_SECOND);
        LED_Set(led_color, FALSE);
        Delay(ONE_SECOND);
    }
}

void LED_Init(void)
{
    LED_SetData();
    for(eLED_Colors led_color = ORANGE; led_color < MAX_LED_COLORS; led_color++) {
        ARM_RCC_Periph_ClockCmd(LED_Data[led_color].port, ENABLE_CMD);
        GPIO_SetCfg(LED_Data[led_color].GPIOx, LED_Data[led_color].pin,
                    GPIO_IO_MODE_OUTPUT, GPIO_IO_TYPE_PUSH_PULL, GPIO_IO_HI_Z, GPIO_IO_SPEED_FREQ_LOW, GPIO_IO_AF_0);
        ARM_GPIO_Config();
    }
}

//================================================================================
//Private
//================================================================================
static void LED_SetData(void)
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
