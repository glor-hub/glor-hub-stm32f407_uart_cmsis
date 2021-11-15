#ifndef _USART_H_
#define _USART_H_

typedef enum {
    TX_PIN = 0,
    RX_PIN,
    CK_PIN,
    CTS_PIN,
    RTS_PIN,
    MAX_USART_PIN_NAMES
} eUSART_PinNames;

// USART pins configuration
typedef struct {
    GPIO_TypeDef *GPIOx;
    ePeriphTypes port;
    eARM_GPIO_IONumbers pin;
    eARM_GPIO_IOAltFunc alt_func; //used only for alternate mode
} USART_PinCfg_t;

bool USART_Init(void);
int32_t USART_Printf(char *fmt, ...);

#ifdef HARDWARE_TESTING_MODE
void USART_Test(void);
#endif //HARDWARE_TESTING_MODE

USART_PinCfg_t *USART_GetPinCfg(ePeriphTypes usart_name);

#endif //_USART_H_
