#ifndef _ARM_USART_H_
#define _ARM_USART_H_

// USART Baudrate
#define ARM_USART_BAUDRATE_9600    (uint32_t)(9600)
#define ARM_USART_BAUDRATE_19200   (uint32_t)(19200)
#define ARM_USART_BAUDRATE_38400   (uint32_t)(38400)
#define ARM_USART_BAUDRATE_57600   (uint32_t)(57600)
#define ARM_USART_BAUDRATE_115200  (uint32_t)(115200)

#define ARM_USART_BAUDRATE_DEFAULT ARM_USART_BAUDRATE_57600

int32_t ARM_USART_Init(void);
bool ARM_USART_isReady(int32_t status);
void ARM_USART_Test(void);

#endif //_ARM_USART_H_
