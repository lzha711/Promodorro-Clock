/*
 * Promo timer code
 *
 * Created: 5/28/2021 10:10:48 AM
 * Modify: 2025 for Timer_A2 project
 * Author : LZ
 */ 

// commons 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#include "common.h"
#include "Display.h"
#include "buzzer.h"
#include "timer.h"

// variables
volatile uint8_t minutes = initial_min;  // defined in common.h
volatile uint8_t seconds = initial_sec;

// Assume button not pressed initially
volatile uint8_t button1_state = 1;
volatile uint8_t button2_state = 1;

SystemState current_state = STATE_IDLE; //initialize to idle

void init(void){
	DDRD = 0b11111110; //set PD1-PD7 for 7 segment
	CLEAR_BIT(DDRB, PB1); // PB1 for switch2 
	DDRC = 0b00111101; // PC2-PC5 for digit select, PC1 for switch1, PC0 for buzzer 
	// SET_BIT(PORTC, PC1); //use internal pull-up resistor
	TIMER0_init(); // for switch
	TIMER1_init(); // for display
	sei(); //enable interrupt
}

int main(void)
{
	init();

	while (true) // main program
	{	
		switch (current_state){
			case STATE_IDLE: 
				Display_REDY(); 
				break; 
			
			case STATE_COUNTING: 
				DisplayMinutes();
				DisplaySeconds(); 
				break;
			
			case STATE_BUZZING:  // turn off display when buzzer on
				Display_OFF();
				play_shutdown_tone();
				// when buzzer is off change state back to idle
				current_state = STATE_IDLE; 
				break; 
		}	
	}
}