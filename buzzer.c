/*
 * buzzer.c
 *
 * Created: 13/09/2021 10:04:53 pm
 *  Author: Lily Z
 */ 
#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "buzzer.h"

volatile uint16_t buzzer_ticks = 0;
volatile uint16_t buzzer_duration_ticks = 0;

//// use timer 2 for buzzer operation - this currently doesn't work
//isr(timer2_compa_vect){
	//// toggle buzzer pin
	//toggle_bit(portc, pc0);
//
	//// count ticks to stop after duration
	//if (++buzzer_ticks >= buzzer_duration_ticks){
		//timsk2 &= ~(1 << ocie2a);  // disable interrupt
		//clear_bit(portc, pc0);     // ensure buzzer is off
	//}
//}
//
//void buzzer_play(uint16_t frequency_hz, uint16_t duration_ms){
	//buzzer_ticks = 0;
	//buzzer_duration_ticks = (duration_ms * frequency_hz) / 1000;
//
	//uint16_t ocr_value = (f_cpu / (2ul * 256ul * frequency_hz)) - 1;
	//tccr2a = (1 << wgm21);
	//tccr2b = (1 << cs22) | (1 << cs21);
	//ocr2a = ocr_value;
	//timsk2 = (1 << ocie2a);
//
	//while (buzzer_ticks < buzzer_duration_ticks); // hold it
//
	//timsk2 &= ~(1 << ocie2a);
	//clear_bit(portc, pc0);
//}
//
//
//// time up tone 
//void play_timeup_tone(void){
	//buzzer_play(500, 2000);
	//buzzer_play(550, 2000); 
	//buzzer_play(600, 2000);
	//buzzer_play(650, 2000); 
//}


// blocking delay method 
 
// toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms.
// pwm frequency = 1/(2*buzz_delay_us) * 1000 khz, 50% duty cycle
void buzz_sound(uint16_t tone_duration_ms, uint16_t half_cycle_delay_us)
{
	uint32_t total_duration_us;
	uint16_t delay_counter;

	total_duration_us = tone_duration_ms * (uint32_t)1000;

	while (total_duration_us > half_cycle_delay_us * 2) {
		total_duration_us -= half_cycle_delay_us * 2;

		// Generate one full cycle of the square wave
		CLEAR_BIT(PORTC, PC0);
		delay_counter = half_cycle_delay_us;
		while (--delay_counter) {};

		SET_BIT(PORTC, PC0);
		delay_counter = half_cycle_delay_us;
		while (--delay_counter) {};
	}
}


void play_timeup_tone(void){ // four notes going up sounds  
		buzz_sound(500,3000); // 0.5s of 166.7hz 
		_delay_ms(50);
		buzz_sound(550,2500); // 0.55s of 200hz 
		_delay_ms(50);  
		buzz_sound(600,2000); // 0.6s of 250hz
		_delay_ms(50);
		buzz_sound(650,1500); // 0.65s of 330hz 
		_delay_ms(50);
}

// windows shutdown tone
void play_shutdown_tone(void) {
	buzz_sound(400, 1910);   // C4
	_delay_ms(100);
	buzz_sound(500, 2272);  // A3
	_delay_ms(100);
	buzz_sound(700, 2862);  // F3
	_delay_ms(100);
	buzz_sound(1000, 2552);  // G3
	_delay_ms(100);
}
