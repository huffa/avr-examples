/************************************************************************
* FILENAME: pin_interrput_flash.c
*
* DESCRIPTION:
* 	On external interrupt on INT0 (pin 4) toggles LED connected to PD7 (pin 13).
*	On external interrupt on INT1 (pin 5) toggles LED connected to PB0 (pin 14).
*
* NOTES:
* 	AVR C Runtime Library - http://savannah.nongnu.org/projects/avr-libc/
*
* AUTHOR:
*	Andrew R. Huff	
* 	andrew [at] andrewhuff [dot] me
*
*/

#include <avr/io.h>			// Defines pins, ports, etc.
#include <avr/interrupt.h>	// Interrupt Macros, ISR attributes, etc.
#include <avr/sleep.h>		// Power management and sleep modes

volatile int PD7_led_on;	// Status bit for led
volatile int PB0_led_on;	// Status bit for led

// Define interrupt vector for INT0 (pin 4)
ISR(INT0_vect)
{
	if (PD7_led_on) {
		PORTD = (0 << PD7);
		PD7_led_on = 0;
	} else {
		PORTD = (1 << PD7);
		PD7_led_on = 1;
	}
}

// Define interrupt vector for INT1 (pin 5)
ISR(INT1_vect)
{
	if (PB0_led_on) {
		PORTB = (0 << PB0);
		PB0_led_on = 0;
	} else {
		PORTB = (1 << PB0);
		PB0_led_on = 1;
	}
}

int main(void) {
	// Set Data Direction Register for pins PB0 and PD7 as outputs.
	// 1 sets the pin as an output, 0 (default) sets the port as an input
	DDRB |= (1 << PB0);
	DDRD |= (1 << PD7);

  	PORTD |= (1<<PD2) | (1<<PD3);		// Enable PD2, PD3 pin pull up resistor
  	EIMSK |= (1<<INT0) | (1<<INT1);		// Enable external interrupt for INT0 and INT1
  	EICRA |= (1<<ISC01) | (1<<ISC11);	// Set interrupt on falling edge

	sei(); // Enable global interrupts

	set_sleep_mode(SLEEP_MODE_STANDBY); // Set sleep mode as standby
	sleep_enable();						// Enable sleep

	while (1) {
		sleep_mode();	// Put CPU in standby mode
	}

	return (0);
}