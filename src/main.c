#include "Middleware.h"
#include "stdio.h"

volatile uint8_t duty_cycle = 50;
#define  MIN_VALUE_DECREASE 0      //[SRS_4]
#define  MAX_VALUE_INCREASE 100    //[SRS_4]
char str_data[128];


void PORTC_IRQHandler(void)
{
	// Button 1 interrupt
	if (PORTC->ISFR & (1 << 12)) {
		if(duty_cycle<MAX_VALUE_INCREASE){
			stop_PWM(0);
			duty_cycle +=10;// increase duty cycle for 10% [SRS_2]
			set_PWM(0,0,2000,duty_cycle);
			start_PWM(0);
		}
		PORTC->ISFR |= (1 << 12); // Clear the interrupt flag
	}
	// Button 2 interrupt
	if (PORTC->ISFR & (1 << 13)) {
		if(duty_cycle>MIN_VALUE_DECREASE){
			stop_PWM(0);
			duty_cycle -=10;// decrease duty cycle for 10% [SRS_3]
			set_PWM(0,0,2000,duty_cycle);
			start_PWM(0);
		}
		PORTC->ISFR |= (1 << 13); // Clear the interrupt flag
	}
	// Button 3 interrupt
		if (PORTC->ISFR & (1 << 14)) {
				stop_PWM(0);
				duty_cycle =50;// duty cycle = 50% [SRS_7]
				set_PWM(0,0,4000,duty_cycle);// set frequency by 4000 [SRS_6]
				start_PWM(0);
			PORTC->ISFR |= (1 << 14); // Clear the interrupt flag
		}
}

void LPIT0_Ch1_IRQHandler(void) {
    LPIT0->MSR |= LPIT_MSR_TIF1_MASK; // Clear LPIT0 Timer 1 interrupt flag
    sprintf(str_data, "Duty cycle = %d\r\n", duty_cycle);
    LPUART1_Send_string(str_data);//[SRS_5]  Send duty cycle to UART
    send_CAN_message(0xA0, duty_cycle);//[SRS_8] Send LED STATE by CAN message every second
}

void CAN0_ORed_0_15_MB_IRQHandler(void){
	CAN_RXINFO bn4 ={.buff_num = 4};
	CAN_RXINFO bn5 ={.buff_num = 5};
	CAN_RXINFO bn6 ={.buff_num = 6};
	// 	Check if a message has been received in buffer 4
	if (can_get_buff_flag(4)) { /* If CAN 0 MB 4 flag is set (received msg), read MB4 */
		can_rxmsg(&bn4);
		can_clear_buff_flag(4);
	 }
	//	Check if a message has been received in buffer 5
		if (can_get_buff_flag(5)) {/* If CAN 0 MB 5 flag is set (received msg), read MB5 */
			can_rxmsg(&bn5);
			if (bn5.id == 0xC0 && bn5.data[0] == 0x01) {//receive CAN message with ID: 0xC0 and Data = 0x01
				// [SRS_09] Execute Stop generating PWM
				stop_PWM(0);
			}
			can_clear_buff_flag(5);  // Clear the flag after handling
		}
	//	Check if a message has been received in buffer 6
		if (can_get_buff_flag(6)) {/* If CAN 0 MB 6 flag is set (received msg), read MB6 */
			can_rxmsg(&bn6);
			if (bn6.id == 0xD0 && bn6.data[0] == 0x01) {
				// [SRS_10] receive CAN message with ID: 0xD0 and Data = 0x01 do SRS_1
				duty_cycle =50;// set duty cycle by 50%
				set_PWM(0,0,2000,duty_cycle);
				start_PWM (0);
			}
			can_clear_buff_flag(6);  // Clear the flag after handling
		}
}

int main(void)
{
	Clock_init();
	Button_Init();
	myUART_Init();
	LPIT0_Init();
	myFTM_PWM_Init();//[SRS_1]
	myCAN_Init();

	while(1)
	{
		 if (duty_cycle == 100) {
			 send_CAN_message(0xC0, 0x01);//[SRS_09]
		 } else if (duty_cycle == 0) {
			 send_CAN_message(0xD0, 0x01);//[SRS_10]
		 }
	}
	return 0;
}
