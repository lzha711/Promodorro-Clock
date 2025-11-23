/*
 * timer.c
 * Created: 15/09/2021 8:58:29 pm
 * Author: LZ
 */
 
#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "timer.h"

extern bool restart_flag;
extern uint8_t minutes;
extern uint8_t seconds; 

#define DEBOUNCE_TICKS 2  // 50ms switch debounce
volatile uint8_t debounce_counter1 = 0;
volatile uint8_t debounce_counter2 = 0;
extern uint8_t button1_state; 
extern uint8_t button2_state;

extern SystemState current_state; 

// timer0 for switch port listen 
void TIMER0_init(void){ 
    TCCR0A = (1 << WGM01);               // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00);  // Prescaler 64
    OCR0A = 249;                         // Compare match value for 10ms
	SET_BIT(TIMSK0, OCIE0A); //enable interrupt
}

ISR(TIMER0_COMPA_vect){
	// switch 1 logic
	if (BIT_IS_CLEAR(PINC, PC1)) { 
		if (debounce_counter1 < DEBOUNCE_TICKS) {
			debounce_counter1++;
			} else if (button1_state == 1) { // Button is pressed 
			 // toggle between idle and counting
			if (current_state == STATE_IDLE)
			{
				current_state = STATE_COUNTING; 
			}else if(current_state == STATE_COUNTING){
				current_state = STATE_IDLE;
			}
			//restart_flag ^= 1;  // toggle restart flag
			button1_state = 0;  // Mark as already toggled
		}
	} else {
		// Button released or bouncing high
		debounce_counter1 = 0;
		button1_state = 1;
	}
	
	// switch 2 logic
	if (BIT_IS_CLEAR(PINB, PB1)) {
		if (debounce_counter2 < DEBOUNCE_TICKS) {
			debounce_counter2++;
			} else if (button2_state == 1) { // Button is pressed
			// button 2 logic TBD

			button2_state = 0;  // Mark as already toggled
		}
		} else {
		// Button released or bouncing high
		debounce_counter2 = 0;
		button2_state = 1;
	}
	
}


// Timer1 for 1s counting down
void TIMER1_init(void){
	TCCR1A = 0b00000000; // OC1A disconnected, PB1 as normal port 
	TCCR1B = 0b00001101; // CTC mode, clk_t0 = clk_io/1024
	OCR1A = 0x3D08; // exact 1s
	// OCR1A = 0x0024; // to generate output at PB1, do not use in A2 version as PB1 is switch input
	SET_BIT(TIMSK1, OCIE1A); // timer output compare match A interrupt enable
}


// execute every 1s, down counting function is here
ISR(TIMER1_COMPA_vect){
	if (current_state == STATE_IDLE) { // Idle state: reset to initial values
		minutes = initial_min;
		seconds = initial_sec;
		return;
	}

	// Countdown active
	if (minutes == 0 && seconds == 0) {  // Time's up — reset and pause
		minutes = initial_min;
		seconds = initial_sec;
		current_state = STATE_BUZZING; 
		return;
	}
	
	// STATE COUNTING
	if (seconds == 0) { 
		seconds = 59;
		if (minutes > 0) {
			minutes--;
		}
	} else {
		seconds--;
	}
}

