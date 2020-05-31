/*
* FILENAME: timer_interrput_flash.c
*
* DESCRIPTION:
* 	Interrupt counter counts to 65,536 and triggers
*	timer interrupt vector which toggles LED connected
* 	to PB0 (pin 14).
*
* NOTES:
* 	AVR C Runtime Library - http://savannah.nongnu.org/projects/avr-libc/
*
* AUTHOR:
*	Andrew R. Huff	
* 	andrew [at] andrewhuff [dot] me
*
*/

#include <avr/io.h> 		// Defines pins, ports, etc.
#include <avr/interrupt.h>	// Interrupt Macros, ISR attributes, etc.

volatile int led_on;

ISR (TIMER2_OVF_vect) 
{
	if (led_on) {
		PORTB = (0 << PB0);
		led_on = 0;
	} else {
		PORTB = (1 << PB0);
		led_on = 1;
	}
}

int main(void) {
	DDRB |= (1 << PB0);

	/*
	* Set the source of the clock counter to 
	* the prescaler clkT2S/1024 output. The output
	* of prescaler clkT2S/1024 will tick every
	* (clock frequency) / (1024) clock cycles. 
	* 
	* For example my AVR328P is running at 8Mhz or
	* 8,000,000 cycles per second. The output of
	* clkT2S/1024 will tick (8,000,000)/(1024) = 7,812.5
	* per second
	*/
	TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22);

	/*
	* Set the counter interrupt behavior to trigger on
	* counter overflow. Counter2 is a 16 bit counter
	* which means is counts up to 2^16 = 65,535.
	*
	* A counter overflow is triggered when 65,535 + 1 is
	* counted in this regiseter. In this case the counter
	* regesiter is a 16 bit register meaning 65,536 will result 
	* in the register being set to 0x0000.
	* 
	* 63,535 = 0b1111 1111 1111 1111
	* 63,536 = 0b0001 0000 0000 0000 0000
	*
	* This effectively means that the interrupt is tiggered
	* every time the counter resets to 0.
	*
	* For example my clock counter is incrementing
	* 7,812 per second, this means that my counter
	* will overrun 65,536 / 7,812 = 8.38 per seconds
	*/
  	TIMSK2 |= (1<<TOIE2);

	sei(); // Enable global interrupts
	while (1) {}

	return (0);
}