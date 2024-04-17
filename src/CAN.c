#include "CAN.h"


/****************************************************
* Function: volatile_memcpy()
* Description: memcpy() discards the volatile keyword. Custom version of memcpy when accessing
* volatile registers
* Param: destination: pointer to destination memory
* Param: source: pointer to source memory
* Param: num: number of bytes to copy from source to destination
****************************************************/
volatile void * volatile_memcpy(volatile void * destination, const volatile void *source, const int num)
{
	volatile uint8_t * destByte = (uint8_t *) destination;
	const volatile uint8_t * srcByte = (uint8_t *)source;
	int i;
	for(i = 0; i < num; i++){
		destByte[i] = srcByte[i];
	}
	return destination;
}
	/******************************************************************************
	* Function: initFlexcan()
	* Description: Initializes the CAN module
	See manual section 53.6.1 for step-by-step instructions
	******************************************************************************/
void initFlexcan(void){
	uint16_t x,i;
	volatile uint16_t j;
	/* Enable the clock to the FlexCAN0 module */
	PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC_MASK;
	/* Tx and Rx pin PCR muxing */
	PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;
	PORTE->PCR[4] |= PORT_PCR_MUX(5); /* Port E4: MUX = ALT5, CAN0_RX */
	PORTE->PCR[5] |= PORT_PCR_MUX(5); /* Port E5: MUX = ALT5, CAN0_TX */
	/* Enable the CAN module */
	CAN0->MCR &= ~CAN_MCR_MDIS(1);
	/* Reset FlexCAN Module */
	CAN0->MCR |= CAN_MCR_SOFTRST(1);
	while((CAN0->MCR & CAN_MCR_SOFTRST_MASK) == CAN_MCR_SOFTRST_MASK){}
	/*Disable the CAN module */
	CAN0->MCR |= CAN_MCR_MDIS(1);
	/* Set CAN clock source 1: bus , 0: oscillator */
	/* Oscillator frequency = 8 MHz */
	CAN0->CTRL1 &= ~CAN_CTRL1_CLKSRC(1);
	/* Enable the CAN module. CAN automatically goes into freeze after this */
	CAN0->MCR &= ~CAN_MCR_MDIS(1);
	while((!(CAN0->MCR & CAN_MCR_FRZACK_MASK)) || !(CAN0->MCR & CAN_MCR_NOTRDY_MASK))
	{}
	CAN0->MCR &= ~CAN_MCR_MAXMB_MASK;
	CAN0->MCR |= CAN_MCR_MAXMB(CAN_NUM_BUFF - 1);
	/* Not sure why but a delay is required before writing CTRL1 */
	for(j=1; j< 255; j++){}
	/* Initialize the Control 1 register */
	CAN0->CTRL1 = CAN_CTRL1_PRESDIV(0) /* PRESDIV+1 = Fclksrc/Ftq = 8Mhz */
	| CAN_CTRL1_RJW(3)
	| CAN_CTRL1_PSEG1(5)
	| CAN_CTRL1_PSEG2(5)
	| CAN_CTRL1_PROPSEG(2)
	| CAN_CTRL1_SMP(0) /* Use 3 bits per CAN sample */
	| CAN_CTRL1_CLKSRC(0); /* Keep the oscillator clock source */
	/** # of time quanta = SYNC_SEG + (PROPSEG + PSEG1 + 2) + (PSEG2 + 1)
	* 1 + (2 + 5 + 2) + (5 + 1) = 16 *
	*
	* Bit rate = (CAN clock frequency )/(# of time quanta) = 8MHz/16 = 500 kbps
	*/
	/* clear memory from message buffer 0 to 15 */
	for(x = 0; x < (CAN_NUM_BUFF * MSG_BUF_SIZE); x += MSG_BUF_SIZE){
		/* Clearing control and status(CS) field of message buffer */
		CAN0->RAMn[x] = 0;
		/* Clearing identifier field(ID) of message buffer */
		CAN0->RAMn[x + 1] = 0;
		/* Clearing data field of message buffer */
		CAN0->RAMn[x + 2] = 0;
		CAN0->RAMn[x + 3] = 0;
	}
	for(i=0; i<16; i++ ) { /* In FRZ mode, init CAN0 16 msg buf filters */
		CAN0->RXIMR[i] = 0xFFFFFFFF; /* Check all ID bits for incoming messages */
	}
	CAN0->RXMGMASK = 0x1FFFFFFF; /* Global acceptance mask: check all ID bits */
	/* Disable all buffer interrupts */
	CAN0->IMASK1 = 0;
	/* Get out of halt and freeze */
	CAN0->MCR &= ~CAN_MCR_HALT(1);
	CAN0->MCR &= ~CAN_MCR_FRZ(1);
	while (CAN0->MCR & CAN_MCR_FRZACK_MASK){}
	/* Good practice: wait for FRZACK to clear (not in freeze mode) */
	while (CAN0->MCR & CAN_MCR_NOTRDY_MASK){}
	/* await synchronization (delay) */
	for(j=1; j < 255; j++){}
}
/******************************************************************************
* Function: can_txmsg()
* Description: Initializes message buffer for TX and sends message
******************************************************************************/
int can_txmsg(CAN_TXINFO *txinfo){
	CAN_MB * mailbox = (CAN_MB *)&(CAN0->RAMn[MSG_BUF_SIZE * txinfo->buff_num]);
	// disable buffer from tx first
	// frame format is standard, not extended
	// remote transmission request, 0, has a data frame to be transmitted
	mailbox->CAN_MB_HD1 = CAN_MB_HD1_CODE(8)
						| CAN_MB_HD1_IDE(0)
						| CAN_MB_HD1_RTR(0)
						| CAN_MB_HD1_DLC(txinfo->length);
	// buffer identifier
	mailbox->CAN_MB_HD2 = CAN_MB_HD2_ID(txinfo->id);
	CAN0->IFLAG1 = 1U << txinfo->buff_num;
	// receive data, length is in bytes
	volatile_memcpy(mailbox->CAN_MB_DATA, txinfo->data, txinfo->length % (CAN_MAX_DATA+1));
	mailbox->CAN_MB_HD1 |= CAN_MB_HD1_CODE(12);//active tx buffer
	return 0;
}
/******************************************************************************
* Function: send_CAN_message(uint32_t id, uint8_t data)
* Description: send can message by MB0 with 1 word data
******************************************************************************/
void send_CAN_message(uint32_t id, uint8_t data) {
    CAN_TXINFO txinfo = {
        .buff_num = 0,
        .id = id,
        .length = 4,
        .data = {data}
    };
    can_txmsg(&txinfo);
}
/******************************************************************************
* Function: can_set_rxisr()
* Description: installs handler for ISR to be called when message is received
******************************************************************************/
void can_set_rxisr(void){
//	INT_SYS_SetPriority(CAN0_ORed_0_15_MB_IRQn,2);
//	INT_SYS_EnableIRQ(CAN0_ORed_0_15_MB_IRQn);
	S32_NVIC->ICPR[2] = 1 << (CAN0_ORed_0_15_MB_IRQn % 32); //clear pending IRQ
	S32_NVIC->IP[CAN0_ORed_0_15_MB_IRQn] =0x02;/* IRQ48-LPIT0 ch0: priority 1 of 0-15*/
	S32_NVIC->ISER[2U] = (uint32_t)(1UL << (((uint32_t)(CAN0_ORed_0_15_MB_IRQn-32*2)) & 0x1FUL));
}
/*ELSE*/
//void can_set_rxisr(volatile isr_t can_isr){
//	IRQn_Type canIrqNum = CAN0_ORed_0_15_MB_IRQn;
//	INT_SYS_InstallHandler(canIrqNum, can_isr, (isr_t*) 0);
//	INT_SYS_SetPriority(canIrqNum,0xB);
//	INT_SYS_EnableIRQ(canIrqNum);
//}
/*AFTER SETTING CALL can_isr() INSTEAD OF
void CAN0_ORed_0_15_MB_IRQHandler(void){}*/
/******************************************************************************
* Function: can_rxbuff_init()
* Description: Initializes message buffer for RX
******************************************************************************/
int can_rxbuff_init(int buff_num, int id){
	CAN_MB * mailbox = (CAN_MB *)&(CAN0->RAMn[4 * buff_num]); /* CAN msg buffer */
	mailbox->CAN_MB_HD1 = CAN_MB_HD1_CODE(0) /* buffer inactive */
						| CAN_MB_HD1_IDE(0); /* use standard 11-bit ID */
	mailbox->CAN_MB_HD2 = CAN_MB_HD2_ID(id); /* specify message identifier */
	mailbox->CAN_MB_HD1 |= CAN_MB_HD1_CODE(4); /* activate RX buffer */
	CAN0->IMASK1 |= 1<<buff_num; /* set IMASK1 bit for the buffer to enable ISR	when message is received */
	return 0;
}
/******************************************************************************
* Function: can_rxmsg()
* Description: Reads CAN message from RX message buffer
******************************************************************************/
int can_get_buff_flag(int buff_num){
	/* Return the appropriate IFLAG bit. */
	return (((CAN0->IFLAG1) >> buff_num) & 1);
}
int can_clear_buff_flag(int buff_num){
	/* Clear the appropriate IFLAG bit. */
	CAN0->IFLAG1 |= (1 << buff_num);
	return 0;
}

int can_rxmsg(CAN_RXINFO *rxinfo){
	CAN_MB * mailbox = (CAN_MB *)&(CAN0->RAMn[MSG_BUF_SIZE * rxinfo->buff_num]);
	/* Read Control and Status word to activate lock on the buffer (mandatory).
	Check if CODE(0) is set and wait until it is cleared. */
	while((mailbox->CAN_MB_HD1 >> CAN_MB_HD1_CODE_SHIFT) & 1);
	/* Read the ID field of the received msg. */
	rxinfo->id = (mailbox->CAN_MB_HD2 & CAN_MB_HD2_ID_MASK) >> CAN_MB_HD2_ID_SHIFT;
	/* These aren't mandatory by the processor, but populating these fields	is convenient. */
	rxinfo->length = (mailbox->CAN_MB_HD1 & CAN_MB_HD1_DLC_MASK) >>	CAN_MB_HD1_DLC_SHIFT ;
	rxinfo->length = rxinfo->length % (CAN_MAX_DATA + 1);
	/* Get the data. Mandatory step. */
	volatile_memcpy(rxinfo->data, mailbox->CAN_MB_DATA, rxinfo->length);
	/* Read the free running timer, which unlocks the buffer.Mandatory step. */
	rxinfo->frt = CAN0->TIMER;
	/* Clear IFLAG so the next receive is visible to us when it happens. */
	can_clear_buff_flag(rxinfo->buff_num);
	return 0;
}
