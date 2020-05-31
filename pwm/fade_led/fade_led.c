/*
* FILENAME: fade_led.c
*
* DESCRIPTION:
* 	Simple PWM example fading an LED on PD6 (pin 12)
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
#include <util/delay.h>		// Busy wait delay functions

int main(void) {
	DDRD |= (1<<PD6);

	TCCR0A |= (1<<COM0A1) | (0<<COM0A0);	// Set non-inverting mode
	TCCR0A |= (1<<WGM01) | (1<<WGM00);		// Fast PWM mode
	TCCR0B |= (0<<WGM02);					// Fast PWM mode
	TCCR0B |= (0<<CS02) | (0<<CS01) | (1<<CS00); // Set prescaler to (clk / 1)
	OCR0A = 0;

	while (1) {
		int i = 0;
		for(i=0; i<=255; i++) {
			OCR0A = i;
			_delay_ms(10);
		}
		for(i=255; i>=0; i--) {
			OCR0A = i;
			_delay_ms(10);
		}
	}

	return (0);
}