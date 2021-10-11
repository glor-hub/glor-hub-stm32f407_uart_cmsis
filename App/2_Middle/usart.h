#ifndef _USART_H_
#define _USART_H_

typedef enum {
    TX_PIN = 0,
    RX_PIN,
    SCLK_PIN, // CK pin in datasheet
    CTS_PIN,
    RTS_PIN,
    MAX_USART_PIN_NAMES
} eUSART_PinNames;

// USART pins configuration
typedef struct {
    GPIO_TypeDef *GPIOx;
    ePeriphTypes port;
    eGPIO_IONumbers pin;
    eGPIO_IOAltFunc alt_func; //used only for alternate mode
} USART_PinCfg_t;

// USART Transfer Information (Run-Time)
typedef struct _USART_TRANSFER_INFO {
    uint32_t                rx_num;        // Total number of data to be received
    uint32_t                tx_num;        // Total number of data to be send
    uint8_t                *rx_buf;        // Pointer to in data buffer
    uint8_t                *tx_buf;        // Pointer to out data buffer
    uint32_t                rx_cnt;        // Number of data received
    uint32_t                tx_cnt;        // Number of data sent
    uint8_t                 tx_def_val;    // Transmit default value (used in USART_SYNC_MASTER_MODE_RX)
    uint8_t                 rx_dump_val;   // Receive dump value (used in USART_SYNC_MASTER_MODE_TX)
    uint8_t                 send_active;   // Send active flag
    uint8_t                 sync_mode;     // Synchronous mode
} USART_TRANSFER_INFO;

typedef struct _USART_RX_STATUS {
    uint8_t rx_busy;                       // Receiver busy flag ��� ���� RXNE
    uint8_t rx_overflow;                   // Receive data overflow detected (cleared on start of next receive operation)
    uint8_t rx_break;                      // Break detected on receive (cleared on start of next receive operation)
    uint8_t rx_framing_error;         // Framing error detected on receive (cleared on start of next receive operation)
    uint8_t rx_parity_error;          // Parity error detected on receive (cleared on start of next receive operation)
} USART_RX_STATUS;

// USART Information (Run-Time)
typedef struct _USART_INFO {
    ARM_USART_SignalEvent_t cb_event;      // Event callback
    USART_RX_STATUS         rx_status;     // Receive status flags
    USART_TRANSFER_INFO     xfer;          // Transfer information
    uint8_t                 mode;          // USART mode
    uint16_t                flags;         //���� USART driver flags
    uint32_t                baudrate;      // Baudrate
} USART_INFO;

#if (RTE_USART6==1)
/***************************************************
//configure USART6:
****************************************************/

/***************************************************
for multiple USARTx outputs options
****************************************************/
//#define USART6_PIN_ID USART_PIN_ID0
//#define USART6_PIN_ID USART_PIN_ID1

/***************************************************
Transmitter clock output for synchronous transmission
(unsupported in Version 1.0)
****************************************************/
//#define USART6_SCLK_PIN_ENABLE

/***************************************************
RTS Flow Control
(unsupported in Version 1.0)
****************************************************/
//#define USART6_RTS_PIN_ENABLE

/***************************************************
CTS Flow Control
(unsupported in Version 1.0)
****************************************************/
//#define USART6_CTS_PIN_ENABLE

/***************************************************
Multibuffer communication using DMA
(unsupported in Version 1.0)
****************************************************/
//#define USART6_DMA_TX_ENABLE //DMA for transmission
//#define USART6_DMA_RX_ENABLE //DMA for reception

#endif //(RTE_USART6==1)


void USART_Init(void);
USART_PinCfg_t *USART_GetPinConfig(ePeriphTypes usart_name);

#if (RTE_USART1==1)
void USART1_cb(uint32_t event);
#endif //(RTE_USART1==1)

#if (RTE_UART4==1)
void UART4_cb(uint32_t event);
#endif //(RTE_UART4==1)
#endif //_USART_H_
