#ifndef _USART_H_
#define _USART_H_

#define USART1_ENABLE
//#define USART2_ENABLE
//#define USART3_ENABLE
#define UART4_ENABLE
//#define UART5_ENABLE
//#define USART6_ENABLE

typedef enum {
    USART_1 = 0,
    USART_2,
    USART_3,
    UART_4,
    UART_5,
    USART_6,
    MAX_USART_INTERFACE_NAMES
} eUSART_InterfaceNames;

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
    eGPIO_PortNames port;
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
    uint8_t rx_busy;                       // Receiver busy flag или флаг RXNE
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
    uint16_t                flags;         //надо USART driver flags
    uint32_t                baudrate;      // Baudrate
} USART_INFO;

#ifdef USART1_ENABLE

/***************************************************
configure USART1:
****************************************************/

/***************************************************
choise pin ID
****************************************************/
#define USART1_PIN_ID USART_PIN_ID0
//#define USART1_PIN_ID USART_PIN_ID1

/***************************************************
Transmitter clock output for synchronous transmission
(unsupported in Version 1.0)
****************************************************/
//#define USART1_SCLK_PIN_ENABLE //only for ID0 option

/***************************************************
RTS Flow Control
(unsupported in Version 1.0)
****************************************************/
//#define USART1_RTS_PIN_ENABLE //only for ID0 option

/***************************************************
CTS Flow Control
(unsupported in Version 1.0)
****************************************************/
//#define USART1_CTS_PIN_ENABLE //only for ID0 option


/***************************************************
Multibuffer communication using DMA
(unsupported in Version 1.0)
****************************************************/
//#define USART1_DMA_TX_ENABLE //DMA for transmission
//#define USART1_DMA_RX_ENABLE //DMA for reception

#endif //USART1_ENABLE

#ifdef USART2_ENABLE
/***************************************************
//configure USART2:
****************************************************/

/***************************************************
choise pin ID
****************************************************/
//#define USART2_PIN_ID USART_PIN_ID0
//#define USART2_PIN_ID USART_PIN_ID1

/***************************************************
Transmitter clock output for synchronous transmission
(unsupported in Version 1.0)
****************************************************/
//#define USART2_SCLK_PIN_ENABLE

/***************************************************
RTS Flow Control
(unsupported in Version 1.0)
****************************************************/
//#define USART2_RTS_PIN_ENABLE

/***************************************************
CTS Flow Control
(unsupported in Version 1.0)
****************************************************/
//#define USART2_CTS_PIN_ENABLE


/***************************************************
Multibuffer communication using DMA
(unsupported in Version 1.0)
****************************************************/
//#define USART2_DMA_TX_ENABLE //DMA for transmission
//#define USART2_DMA_RX_ENABLE //DMA for reception

#endif //USART2_ENABLE

#ifdef USART3_ENABLE

/***************************************************
configure USART3:
****************************************************/

/***************************************************
choise pin ID
****************************************************/
//#define USART3_PIN_ID USART_PIN_ID0
//#define USART3_PIN_ID USART_PIN_ID1
//#define USART3_PIN_ID USART_PIN_ID2

/***************************************************
Transmitter clock output for synchronous transmission
(unsupported in Version 1.0)
****************************************************/
//#define USART3_SCLK_PIN_ENABLE

/***************************************************
RTS Flow Control
(unsupported in Version 1.0)
****************************************************/
//#define USART3_RTS_PIN_ENABLE //only for ID0 and ID2 options

/***************************************************
CTS Flow Control
(unsupported in Version 1.0)
****************************************************/
//#define USART3_CTS_PIN_ENABLE //only for ID0 and ID2 options

/***************************************************
Multibuffer communication using DMA
(unsupported in Version 1.0)
****************************************************/
//#define USART3_DMA_TX_ENABLE //DMA for transmission
//#define USART3_DMA_RX_ENABLE //DMA for reception

#endif //USART3_ENABLE

#ifdef UART4_ENABLE

/***************************************************
configure UART4:
****************************************************/

/***************************************************
choise pin ID
****************************************************/
//#define UART4_PIN_ID USART_PIN_ID0
//#define UART4_PIN_ID USART_PIN_ID1

/***************************************************
Multibuffer communication using DMA
(unsupported in Version 1.0)
****************************************************/
//#define UART4_DMA_TX_ENABLE //DMA for transmission
//#define UART4_DMA_RX_ENABLE //DMA for reception

#endif //UART4_ENABLE

#ifdef UART5_ENABLE

/***************************************************
configure UART5:
****************************************************/

/***************************************************
uncomment pin ID0 for UART5 using
****************************************************/
//#define UART5_PIN_ID USART_PIN_ID0

/***************************************************
Multibuffer communication using DMA
(unsupported in Version 1.0)
****************************************************/
//#define UART5_DMA_TX_ENABLE //DMA for transmission
//#define UART5_DMA_RX_ENABLE //DMA for reception

#endif //UART5_ENABLE

#ifdef USART6_ENABLE
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

#endif //USART6_ENABLE


void USART_Init(void);
USART_PinCfg_t *USART_GetPinConfig(eUSART_InterfaceNames usart_name);

#ifdef USART1_ENABLE
void USART1_cb(uint32_t event);
#endif //USART1_ENABLE

#ifdef UART4_ENABLE
void UART4_cb(uint32_t event);
#endif //UART4_ENABLE
#endif //_USART_H_
