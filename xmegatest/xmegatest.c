#include "xmegatest.h"

int main(void)
{
	MCU_Init();

	FILE USB_stdout = FDEV_SETUP_STREAM (USB_Putchar, NULL, _FDEV_SETUP_WRITE);
	fprintf(&USB_stdout,"I'm alive...\r\n");

	for(;;) {
		if(newrx) {
			newrx = 0;
			fprintf(&USB_stdout,"Sine Frequency = %u\r\n",sine_frequency);
		}
	}
}

//Setup MCU pins and peripheral systems
void MCU_Init( void )
{
	////////////////////////////////////////////////////////
	//                  Setup GPIO Pins                   //
	////////////////////////////////////////////////////////
	/*    PORTA    */
	//Set inputs and outputs

	/*    PORTB    */
	//Set inputs and outputs

	/*    PORTC    */
	//Set inputs and outputs
	PORTC.OUTSET = USB_UART_RXD;
	PORTC.DIRSET = MB_MOSI | MB_SCK | USB_UART_RXD;

	/*    PORTD    */
	//Set inputs and outputs
	PORTD.OUTSET = FLASH_CS | LED0 | LED1;
	PORTD.DIRSET = FLASH_CS | LED0 | LED1;

	/*    PORTE    */
	//Set inputs and outputs

	/*    PORTR    */
	//Set inputs and outputs

	////////////////////////////////////////////////////////
	//                 Setup Peripherals                  //
	////////////////////////////////////////////////////////

	//CLOCK SYSTEM
	OSC.CTRL |= OSC_RC32MEN_bm; // Enable 32Mhz internal RC oscillator
	CLK.PSCTRL = CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc; // Set prescalers (1,1,1)
	while( (OSC.STATUS & OSC_RC32MRDY_bm) == 0); // Wait for the clock to be ready
	CCP_CLK(CLK_SCLKSEL_RC32M_gc); // Enable new clock source, timing critical (4 clock cycles)

	//PORTC USART
	USARTC0.CTRLA = USART_RXCINTLVL_LO_gc; // RX interrupt is enabled and low priority
	USARTC0.CTRLC = USART_CHSIZE_8BIT_gc; // 8 data bits, no parity, 1 stop bit
	USARTC0.BAUDCTRLA = BSELH;
	USARTC0.BAUDCTRLB = BSCALE | BSELL;
	USARTC0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // RX and TX enabled

	//PORTD Timer0 - Channel C & D = LED0 & 1 respectively
	TCD0.PER = 488; //Count to 488 before overflowing and reseting the counter
	TCD0.CTRLA = TC_CLKSEL_DIV1024_gc; // Enable timer, clk = F_CPU/1024
	//32MHz / 1024 = 31.25KHz => 31.25KHz / 488 = 64 interrupts/sec
	TCD0.CTRLB = TC0_CCCEN_bm | TC0_CCDEN_bm | TC0_WGMODE0_bm | TC0_WGMODE1_bm; // Enable Channel B SS PWM
	TCD0.INTCTRLA = TC0_OVFINTLVL0_bm; // Enable Timer 0 Overflow Interrupt

	//Globals
	sine_frequency = 256;

	//INTERRUPT SYSTEM	
	PMIC.CTRL |= PMIC_LOLVLEN_bm; //Low, medium, and high interrupts enabled
	sei(); //Enable Interrupts
}

int USB_Putchar (char c, FILE *stream)
{
	// Wait for the transmit buffer to be empty
	while ( !( USARTC0.STATUS & USART_DREIF_bm) );
 
	// Put the character into the transmit buffer
	USARTC0.DATA = c; 

	return 0;
}

//Data must be read to clear the interrupt flag.
//Otherwise, the interrupt will be executed continuously.
ISR(USARTC0_RXC_vect)
{
	uint8_t temp = USARTC0.DATA;

	switch(temp)
	{
		case '0': sine_frequency = 16;
		          break;

		case '1': sine_frequency = 32;
		          break;

		case '2': sine_frequency = 64;
				  break;

		case '3': sine_frequency = 128;
				  break;

		case '4': sine_frequency = 256;
				  break;

		case '5': sine_frequency = 512;
				  break;

		case '6': sine_frequency = 1024;
				  break;

		case 'R': CCP_RST();
				  break;
	}
	
	newrx = 1;
}

//TCD0 overflow interrupt. Timer frequency / Timer period = overflows per second.
ISR(TCD0_OVF_vect)
{
	static uint16_t i;

	//Generate a scaled sine wave value. This is crazy slow, but it works.
	//The value will be between 0 and 255 and centered at 127.5.
	uint16_t x = (uint16_t)(sin(2*M_PI*i/sine_frequency)*243.5+243.5);

	TCD0.CCCBUF = x;
	TCD0.CCDBUF = 488-x;

	i++;
}