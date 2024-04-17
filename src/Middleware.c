#include "Middleware.h"
volatile uint8_t current_led = 0; // 0: Red, 1: Green, 2: Blue

void Button_Init(void){
	//BTN1 SW3 	 : PTC13
	//BTN0 SW2 	 : PTC12
	//BTN3 SW4	 : PTC14
	PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC(1U);
	//define BTN0 SW2 	 : PTC12
	PortConfigType SW2cfg = {
				.PORTx=PORTC,
				.pin=12,
				.mux=PORT_MUX_GPIO,
				.pull=PORT_PULL_DISABLED,
				.interrupt= PORT_INT_FALLING,
				.IRQ=PORTC_IRQn,
//				.callback =&ToggleModeValue,//if write Interrupt handler function in driver
		};
	GPIOConfigType SW2GPIO ={
					.PTx=PTC,
					.pin=12,
					.pinDirection=input,
			};
	myPORT_PinInit(&SW2cfg);
	myGPIO_Init(&SW2GPIO);
	PORT_EXTI_Config(&SW2cfg);

	//define BTN1 SW3 	 : PTC13
	PortConfigType SW3cfg = {
				.PORTx=PORTC,
				.pin=13,
				.mux=PORT_MUX_GPIO,
				.pull=PORT_PULL_DISABLED,
				.interrupt= PORT_INT_FALLING,
				.IRQ=PORTC_IRQn,
//				.callback =&ToggleModeValue,//if write Interrupt handler function in driver
		};
	GPIOConfigType SW3GPIO ={
					.PTx=PTC,
					.pin=13,
					.pinDirection=input,
			};
	myPORT_PinInit(&SW3cfg);
	myGPIO_Init(&SW3GPIO);
	PORT_EXTI_Config(&SW3cfg);

	//define BTN3 SW4	 : PTC14
	PortConfigType SW4cfg = {
					.PORTx=PORTC,
					.pin=14,
					.mux=PORT_MUX_GPIO,
					.pull=PORT_PULL_DISABLED,
					.interrupt= PORT_INT_FALLING,
					.IRQ=PORTC_IRQn,
	//				.callback =&ToggleModeValue,//if write Interrupt handler function in driver
			};
		GPIOConfigType SW4GPIO ={
						.PTx=PTC,
						.pin=14,
						.pinDirection=input,
				};
		myPORT_PinInit(&SW4cfg);
		myGPIO_Init(&SW4GPIO);
		PORT_EXTI_Config(&SW4cfg);
}

void myUART_Init(void){
	lpuart_config_t uart1cfg = {
		.baudRate = BaudRate9600,
		.parityMode = Parity_Disabled,
		.stopBitCount=  ONE_STOP_BIT,
		.bitCountPerChar=LPUART_8_BITS,
	};
	LPUART1_Init(&uart1cfg);
}

void myCAN_Init(void){
	initFlexcan();
    // Set up CAN receive for SRS5 and SRS6
	can_rxbuff_init(4, 0xA0);  // Buffer 4 for ID 0xA0
    can_rxbuff_init(5, 0xC0);  // Buffer 5 for ID 0xC0
    can_rxbuff_init(6, 0xD0);  // Buffer 6 for ID 0xD0
    can_set_rxisr();
}

void myFTM_PWM_Init(void){
	PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTD */
	PORTD->PCR[15]|=PORT_PCR_MUX(2); /* Port D15: MUX = ALT2, FTM0CH0 */
//	PORTD->PCR[16]|=PORT_PCR_MUX(2); /* Port D16: MUX = ALT2, FTM0CH1 */
	init_PWM(0, 0);
	set_PWM(0,0,2000,50);
	start_PWM (0);
}

//void LPIT0_Ch0_IRQHandler(void){
//	LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */
////	RGB_BlinkLed();
//}

void LED_Init(void)
{
	//RGB BLUE	 : PTD0
	//RGB RED 	 : PTD15
	//RGB GREEN  : PTD16

	//Clock for portD
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC(1U);

	// Init RED LED PTD15
		PortConfigType RedLed = {
				.PORTx=PORTD,
				.pin=15,
				.mux=PORT_MUX_GPIO,
			};
		GPIOConfigType RedLedGPIO ={
				.PTx=PTD,
				.pin=15,
				.pinDirection=output,
		};
		myPORT_PinInit(&RedLed);
		myGPIO_Init(&RedLedGPIO);

	// Init Green LED PTD16
		PortConfigType GreenLed = {
					.PORTx=PORTD,
					.pin=16,
					.mux=PORT_MUX_GPIO,
				};
		GPIOConfigType GreenLedGPIO ={
					.PTx=PTD,
					.pin=16,
					.pinDirection=output,
			};
		myPORT_PinInit(&GreenLed);
		myGPIO_Init(&GreenLedGPIO);

		// Init Blue LED PTD0
		PortConfigType BlueLed = {
				.PORTx=PORTD,
				.pin=0,
				.mux=PORT_MUX_GPIO,
			};
		GPIOConfigType BlueLedGPIO ={
				.PTx=PTD,
				.pin=0,
				.pinDirection=output,
		};
		myPORT_PinInit(&BlueLed);
		myGPIO_Init(&BlueLedGPIO);
}

void RGB_BlinkLed(void){
	GPIO_ClearPin(PTD, 15);
	GPIO_ClearPin(PTD, 16);
	GPIO_ClearPin(PTD, 0);
	switch (current_led) {
			case 0:
				GPIO_TogglePin(PTD, 15);
				break;
			case 1:
				GPIO_TogglePin(PTD, 16);
				break;
			case 2:
				GPIO_TogglePin(PTD, 0);
				break;
	   }
	// Move to the next LED
	current_led = (current_led + 1) % 3;
}
