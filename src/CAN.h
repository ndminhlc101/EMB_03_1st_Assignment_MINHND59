#ifndef _CAN_H_
#define _CAN_H_

#include "S32K144.h"
#include "s32_core_cm4.h"
#include "interrupt_manager.h"
//#define NODE_A /* If using 2 boards as 2 nodes, NODE A & B use different CAN IDs */

#define CAN_RX_ISR_CHANNEL 1
#define CAN_TX_ISR_CHANNEL 0
#define CAN_ISR_FREQUENCY 1000
#define CAN_NUM_BUFF 16
#define CAN_MAX_DATA 8
#define CAN0_RX_PCR 4
#define CAN0_TX_PCR 5
#define MSG_BUF_SIZE 4 /* Msg Buffer Size. (CAN 2.0AB: 2 hdr + 2 data= 4 words) */
#define CAN_MB_HD1_CODE_MASK 0x0F000000u
#define CAN_MB_HD1_CODE_SHIFT 24u
#define CAN_MB_HD1_CODE_WIDTH 4u
#define CAN_MB_HD1_CODE(x) (((uint32_t)(((uint32_t)(x))<<CAN_MB_HD1_CODE_SHIFT))&CAN_MB_HD1_CODE_MASK)
#define CAN_MB_HD1_IDE_MASK 0x00200000u
#define CAN_MB_HD1_IDE_SHIFT 21u
#define CAN_MB_HD1_IDE_WIDTH 1u
#define CAN_MB_HD1_IDE(x) (((uint32_t)(((uint32_t)(x))<<CAN_MB_HD1_IDE_SHIFT))&CAN_MB_HD1_IDE_MASK)
#define CAN_MB_HD1_RTR_MASK 0x00100000u
#define CAN_MB_HD1_RTR_SHIFT 20u
#define CAN_MB_HD1_RTR_WIDTH 1u
#define CAN_MB_HD1_RTR(x) (((uint32_t)(((uint32_t)(x))<<CAN_MB_HD1_RTR_SHIFT))&CAN_MB_HD1_RTR_MASK)
#define CAN_MB_HD1_DLC_MASK 0x000F0000u
#define CAN_MB_HD1_DLC_SHIFT 16u
#define CAN_MB_HD1_DLC_WIDTH 4u
#define CAN_MB_HD1_DLC(x) (((uint32_t)(((uint32_t)(x))<<CAN_MB_HD1_DLC_SHIFT))&CAN_MB_HD1_DLC_MASK)
#define CAN_MB_HD2_ID_MASK 0x1FFC0000u
#define CAN_MB_HD2_ID_SHIFT 18u
#define CAN_MB_HD2_ID_WIDTH 11u
#define CAN_MB_HD2_ID(x) (((uint32_t)(((uint32_t)(x))<<CAN_MB_HD2_ID_SHIFT))&CAN_MB_HD2_ID_MASK)

typedef struct{
	volatile uint32_t CAN_MB_HD1; /* control and status word */
	volatile uint32_t CAN_MB_HD2; /* identifier */
	volatile uint8_t CAN_MB_DATA[8]; /* will only need 2 data registers = 2word = 8bytes*/
}CAN_MB;

typedef struct txinfo
{
	uint8_t buff_num; /* msg buffer to use for TX */
	uint32_t id; /* message id */
	uint8_t length; /* length of data (bytes) */
	uint8_t data[8]; /* data for TX */
} CAN_TXINFO;

typedef struct rxinfo
{
	uint8_t buff_num; /* buffer to be filled ~ msg buffer to use for RX*/
	uint8_t length; /* length of data (bytes) */
	uint32_t id; /* id registers */
	uint8_t data[8]; /* data read from msg buffer */
	uint16_t frt; /* free running-timer */
} CAN_RXINFO;

volatile void * volatile_memcpy(volatile void * destination, const volatile void *source, const int num);

void initFlexcan(void);
int can_txmsg(CAN_TXINFO *txinfo);
void send_CAN_message(uint32_t id, uint8_t data);
int can_rxmsg(CAN_RXINFO *rxinfo);
int can_rxbuff_init(int buff_num, int id);
void can_set_rxisr(void);
int can_get_buff_flag(int buff);
int can_clear_buff_flag(int buff);


#endif
