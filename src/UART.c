#include "UART.h"

void LPUART1_Init(lpuart_config_t* config){
	PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC(1U);
	PORTC->PCR[6]|=PORT_PCR_MUX(2); /* Port C6: MUX = ALT2,UART1 TX */
	PORTC->PCR[7]|=PORT_PCR_MUX(2); /* Port C7: MUX = ALT2,UART1 RX */
	PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_CGC_MASK; /* Ensure clk disabled for config */
	PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(1) /* Clock Src= 1 (SOSCDIV2_CLK) */
								 | PCC_PCCn_CGC_MASK; /* Enable clock for LPUART1 regs */

	LPUART1->CTRL&=~LPUART_CTRL_TE_MASK;/* TE=0: Transmitter disabled */
	LPUART1->CTRL&=~LPUART_CTRL_RE_MASK;/* RE=0: Receiver disabled */

	LPUART1->BAUD |= (LPUART1->BAUD&~LPUART_BAUD_OSR_MASK)|LPUART_BAUD_OSR(15U);/* OSR=15: Over sampling ratio = 15+1=16 */
	LPUART1->BAUD |= (LPUART1->BAUD&~LPUART_BAUD_SBR_MASK)|LPUART_BAUD_SBR(config->baudRate);/* SBR=52 (0x34): baud divisor = 8M/9600/16 = ~52 */
	if(config->parityMode!=Parity_Disabled)
		{
			if(config->parityMode==Parity_Odd)
			{
				LPUART1->CTRL &=~LPUART_CTRL_PT_MASK;
			}
			if(config->parityMode==Parity_Even)
			{
				LPUART1->CTRL |=LPUART_CTRL_PT_MASK;
			}
			LPUART1->CTRL |= LPUART_CTRL_PE_MASK;
		}

	if(config->bitCountPerChar==LPUART_8_BITS)
	{
		LPUART1->BAUD &=~LPUART_BAUD_M10_MASK;/* M10=0: Rx and Tx use 7 to 9 bit data characters */
		LPUART1->CTRL &=~ LPUART_CTRL_M_MASK;/* M=0: Rx and Tx use 8 bit data characters */
	}else if(config->bitCountPerChar==LPUART_9_BITS){
		LPUART1->BAUD &=~LPUART_BAUD_M10_MASK;/* M10=0: Rx and Tx use 7 to 9 bit data characters */
		LPUART1->CTRL |= LPUART_CTRL_M_MASK;/* M=1: Rx and Tx use 9 bit data characters */
	}else{
		LPUART1->BAUD |=LPUART_BAUD_M10_MASK;/* M10=1: Rx and Tx use 10 bit data characters */
	}

	if(config->stopBitCount==ONE_STOP_BIT){
		LPUART1->BAUD &=~LPUART_BAUD_SBNS_MASK;/* SBNS=0: One stop bit */
	}else{
		LPUART1->BAUD |=LPUART_BAUD_SBNS_MASK;/* SBNS=1: 2 stop bit */
	}

	LPUART1->BAUD |=LPUART_BAUD_BOTHEDGE(1);/* BOTHEDGE=1: receiver samples on both edge */

	//Default LPUART CTRL : no parity, 8 bit char
	LPUART1->CTRL|=LPUART_CTRL_TE_MASK;/* TE=1: Transmitter enabled */
	LPUART1->CTRL|=LPUART_CTRL_RE_MASK;/* RE=1: Receiver enabled */
}

void LPUART1_Send_char(char ch) { /* Function to Transmit single Char */
	LPUART1->CTRL|=LPUART_CTRL_TE_MASK;/* TE=1: Transmitter enabled */
//	while((LPUART1->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT==0);
	while(0U==(LPUART1->STAT & LPUART_STAT_TDRE_MASK));/* Wait for transmit buffer to be empty TDRE=1*/
	LPUART1->DATA=ch; /* Send data */
	while(0U==(LPUART1->STAT & LPUART_STAT_TC_MASK)); /* Wait for transmit complete flag TC=1*/
	LPUART1->CTRL&=~LPUART_CTRL_TE_MASK;/* TE=0: Transmitter disabled */
}

void LPUART1_Send_string(char data_string[]) { /* Function to Transmit whole string */
	uint32_t i=0;
	while(data_string[i] != '\0') { /* Send chars one at a time */
		LPUART1_Send_char(data_string[i]);
		i++;
	}
}

char LPUART1_receive_char(void) { /* Function to Receive single Char */
	char receive;
	while((LPUART1->STAT & LPUART_STAT_RDRF_MASK)>>LPUART_STAT_RDRF_SHIFT==0);
	/* Wait for received buffer to be full */
	receive= LPUART1->DATA; /* Read received data*/
	return receive;
}

void LPUART1_ReceiveChar_NonBlocking(void) {
	// Enable interrupt
//	LPUART1->STAT |= LPUART_STAT_RDRF_MASK;
	LPUART1->CTRL |= LPUART_CTRL_RIE_MASK;
	NVIC_EnableIRQ(LPUART1_RxTx_IRQn);
	// Enable RE
	LPUART1->CTRL|=LPUART_CTRL_RE_MASK;/* RE=1: Receiver enabled */
}

