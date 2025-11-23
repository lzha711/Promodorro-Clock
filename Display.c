/*
 * Display.c
 *
 * Created: 13/09/2021 9:53:34 pm
 *  Author: Lily Z
 */ 

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#include "Display.h"

extern uint8_t minutes;
extern uint8_t seconds;

//a-g 7 segment values for each number
uint8_t seven_seg_digits[10][7] = {
	{ 1,1,1,1,1,1,0 },  // = 0
	{ 0,1,1,0,0,0,0 },  // = 1
	{ 1,1,0,1,1,0,1 },  // = 2
	{ 1,1,1,1,0,0,1 },  // = 3
	{ 0,1,1,0,0,1,1 },  // = 4
	{ 1,0,1,1,0,1,1 },  // = 5
	{ 1,0,1,1,1,1,1 },  // = 6
	{ 1,1,1,0,0,0,0 },  // = 7
	{ 1,1,1,1,1,1,1 },  // = 8
	{ 1,1,1,1,0,1,1 }   // = 9
};

//a-g 7 segment values for word "rest"
uint8_t seven_seg_REST[4][7] = {
	{ 1,1,1,0,1,1,1 },  // = R
	{ 1,0,0,1,1,1,1 },  // = E
	{ 1,0,1,1,0,1,1 },  // = S
	{ 1,0,0,0,1,1,0 },  // = T
};

// more options here
uint8_t seven_seg_STOP[4][7] = {
	{ 1,0,1,1,0,1,1 },  // = S
	{ 1,0,0,0,1,1,0 },  // = T
	{ 1,1,1,1,1,1,0 },  // = O
	{ 1,1,0,0,1,1,1 },  // = P
};

//a-g 7 segment values for word "redy"
uint8_t seven_seg_REDY[4][7] = {
	{ 1,1,1,0,1,1,1 },  // = R
	{ 1,0,0,1,1,1,1 },  // = E
	{ 1,1,1,1,1,1,0 },  // = D
	{ 0,1,1,0,0,1,1 },  // = Y
};


uint8_t seven_seg_GO[2][7] = {
	{ 1,0,1,1,1,1,1 },  // = G
	{ 1,1,1,1,1,1,0 },  // = O
};


void WriteDisplays(uint8_t digit, uint8_t displaynum){
	uint8_t PD_pin; 
	PORTD &= 0b00000000; //clear port D
	
	// display select
	if (displaynum == 0){
		// select digit 4
		 SET_BIT(PORTC, PC2); 
		 CLEAR_BIT(PORTC,PC3);
		 CLEAR_BIT(PORTC,PC4);
		 CLEAR_BIT(PORTC,PC5);
	
	}else if (displaynum == 1){
		// select digit 3
		CLEAR_BIT(PORTC, PC2);
		SET_BIT(PORTC, PC3); 
		CLEAR_BIT(PORTC, PC4);
		CLEAR_BIT(PORTC, PC5);
		
	}else if (displaynum == 2){
		// select digit 2
		CLEAR_BIT(PORTC,PC2);
		CLEAR_BIT(PORTC, PC3);
		SET_BIT(PORTC, PC4);
		CLEAR_BIT(PORTC, PC5);		
	
	}else if (displaynum == 3){
		// select digit 1
		CLEAR_BIT(PORTC, PC2);
		CLEAR_BIT(PORTC, PC3);
		CLEAR_BIT(PORTC, PC4);
		SET_BIT(PORTC, PC5); 	
	}
	
	// assign display digit
	for (int segCount = 0; segCount <7; ++segCount){
		PD_pin = segCount + 1;
		PORTD |= (seven_seg_digits[digit][segCount]<<PD_pin); //assign [digit][a-g] to PD1-PD7
	}
	
}

void WriteREST(uint8_t wordnum, uint8_t displaynum){
	uint8_t PD_pin;
	PORTD &= 0b00000000; //clear port D
	
	// multiplex
	if (displaynum == 0){
		SET_BIT(PORTC, PC2); 
		CLEAR_BIT(PORTC, PC3);
		CLEAR_BIT(PORTC, PC4);
		CLEAR_BIT(PORTC, PC5);
		}else if (displaynum == 1){
		CLEAR_BIT(PORTC, PC2);
		SET_BIT(PORTC, PC3);
		CLEAR_BIT(PORTC, PC4);
		CLEAR_BIT(PORTC, PC5);
		}else if (displaynum == 2){
		CLEAR_BIT(PORTC, PC2);
		CLEAR_BIT(PORTC, PC3);
		SET_BIT(PORTC, PC4);
		CLEAR_BIT(PORTC, PC5);
		}else if (displaynum == 3){
		CLEAR_BIT(PORTC, PC2);
		CLEAR_BIT(PORTC, PC3);
		CLEAR_BIT(PORTC, PC4);
		SET_BIT(PORTC, PC5);
	}
	
	// assign display number
	for (int segCount = 0; segCount <7; ++segCount){
		PD_pin = segCount + 1;
		PORTD |= (seven_seg_REST[wordnum][segCount]<<PD_pin); //assign [word][a-g] to PD1-PD7
	}
}

// turn off display
void Display_OFF(void){
	CLEAR_BIT(PORTC, PC2);
	CLEAR_BIT(PORTC, PC3);
	CLEAR_BIT(PORTC, PC4);
	CLEAR_BIT(PORTC, PC5);
}

void WriteREDY(uint8_t wordnum, uint8_t displaynum){
	uint8_t PD_pin;
	PORTD &= 0b00000000; //clear port D
	
	// multiplex
	if (displaynum == 0){
		SET_BIT(PORTC, PC2); 
		CLEAR_BIT(PORTC, PC3);
		CLEAR_BIT(PORTC, PC4);
		CLEAR_BIT(PORTC, PC5);
		}else if (displaynum == 1){
		CLEAR_BIT(PORTC, PC2);
		SET_BIT(PORTC, PC3);
		CLEAR_BIT(PORTC, PC4);
		CLEAR_BIT(PORTC, PC5);
		}else if (displaynum == 2){
		CLEAR_BIT(PORTC, PC2);
		CLEAR_BIT(PORTC, PC3);
		SET_BIT(PORTC, PC4);
		CLEAR_BIT(PORTC, PC5);
		}else if (displaynum == 3){
		CLEAR_BIT(PORTC, PC2);
		CLEAR_BIT(PORTC, PC3);
		CLEAR_BIT(PORTC, PC4);
		SET_BIT(PORTC, PC5);
	}
	
	// assign display number
	for (int segCount = 0; segCount <7; ++segCount){
		PD_pin = segCount + 1;
		PORTD |= (seven_seg_REDY[wordnum][segCount]<<PD_pin); //assign [word][a-g] to PD1-PD7
	}
}

// display REST
void Display_REST(void){
	WriteREST(3,0); //T at sec2
	_delay_us(POV_delay);
	WriteREST(2,1); //S at sec1
	_delay_us(POV_delay);
	WriteREST(1,2); //E at min2
	_delay_us(POV_delay);
	WriteREST(0,3); //R at min1
	_delay_us(POV_delay);
}

// display REDY
void Display_REDY(void){
	WriteREDY(3,0); 
	_delay_us(POV_delay);
	WriteREDY(2,1); 
	_delay_us(POV_delay);
	WriteREDY(1,2);
	_delay_us(POV_delay);
	WriteREDY(0,3);
	_delay_us(POV_delay);
}

void DisplaySeconds(void){
	uint8_t tens = seconds / 10;
	uint8_t units = seconds % 10;

	WriteDisplays(tens, 1);   // Tens digit at position 1
	_delay_us(POV_delay);
	WriteDisplays(units, 0);  // Units digit at position 0
	_delay_us(POV_delay);
}

void DisplayMinutes(void){
	uint8_t tens = minutes / 10;
	uint8_t units = minutes % 10;

	WriteDisplays(tens, 3);   // Tens digit at position 3
	_delay_us(POV_delay);
	WriteDisplays(units, 2);  // Units digit at position 2
	_delay_us(POV_delay);
}
