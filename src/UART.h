#ifndef UART_H_
#define UART_H_
#include "S32K144.h"
#include "PORT.h"

typedef enum
{
    LPUART_DMA         = 0,    /*!< The driver will use DMA to perform UART transfer */
    LPUART_INTERRUPTS          /*!< The driver will use interrupts to perform UART transfer */
} uart_transfer_type_t;

typedef enum{
	BaudRate2400=208U,
	BaudRate4800=104U,
	BaudRate9600=52U,
	BaudRate19200=26U,
	BaudRate38400=13U,
	BaudRate56000=9U,
	BaudRate115200=4U,
}BaudRate;

typedef enum
{
    LPUART_8_BITS  = 0x0U, /*!< 8-bit data characters */
    LPUART_9_BITS  = 0x1U, /*!< 9-bit data characters */
    LPUART_10_BITS = 0x2U  /*!< 10-bit data characters */
} uart_bit_count_per_char_t;

typedef enum
{
    ONE_STOP_BIT = 0x0U, /*!< one stop bit */
    TWO_STOP_BIT = 0x1U  /*!< two stop bits */
} uart_stop_bit_count_t;

/*UART Parity Mode*/
typedef enum {
    Parity_Disabled = 0x0U,
    Parity_Even = 0x2U,
    Parity_Odd = 0x3U,
} UART_Parity_Mode;

typedef enum {
	SEND_SUCCESS = 0U,
	SEND_FAIL = 1U,
	RECEIVE_SUCCESS = 2U,
	RECEIVE_FAIL = 3U
}UART_status;

typedef enum{
	CLOCK_UART_SOURCE_DISABLED=0U,
	CLOCK_UART_SOURCE_SOSCDIV2=1U,
	CLOCK_UART_SOURCE_SIRCDIV2=2U,
	CLOCK_UART_SOURCE_FIRCDIV2=3U,
	CLOCK_UART_SOURCE_SPLLDIV2=6U,
}ClockSourceUart;

typedef struct
{
	BaudRate baudRate;                           /*!< LPUART baud rate */
    UART_Parity_Mode parityMode;             /*!< parity mode, disabled (default), even, odd */
    uart_stop_bit_count_t stopBitCount;        /*!< number of stop bits, 1 stop bit (default) or 2 stop bits */
    uart_bit_count_per_char_t bitCountPerChar; /*!< number of bits in a character (8-default, 9 or 10);
                                                      for 9/10 bits chars, users must provide appropriate buffers
                                                      to the send/receive functions (bits 8/9 in subsequent bytes);
                                                      for DMA transmission only 8-bit char is supported. */
    uart_transfer_type_t tranfertype;/*!< The driver will use DMA/interrupts to perform UART transfer */
} lpuart_config_t;

extern const lpuart_config_t lpUartInitConfig1;

void LPUART1_Init(lpuart_config_t* config);
void LPUART1_Send_char(char ch);
void LPUART1_Send_string(char data_string[]);
char LPUART1_receive_char(void);
void LPUART1_ReceiveChar_NonBlocking(void);


static inline void LPUART1_Receiver_Enable(void) {
	LPUART1->CTRL|=LPUART_CTRL_RE_MASK;/* RE=1: Receiver enabled */
}
static inline void LPUART1_Receiver_Disable(void) {
	LPUART1->CTRL&=~LPUART_CTRL_RE_MASK;/* RE=0: Receiver disabled */
}
static inline void LPUART1_Transmitter_Enable(void) {
	LPUART1->CTRL|=LPUART_CTRL_TE_MASK;/* TE=1: Transmitter enabled */
}
static inline void LPUART1_Transmitter_Disable(void) {
	LPUART1->CTRL&=~LPUART_CTRL_TE_MASK;/* TE=0: Transmitter disabled */
}

#endif /* UART_H_ */
