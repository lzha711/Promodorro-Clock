/*
 * common.h
 *
 * Created: 13/09/2021 9:48:29 pm
 *  Author: yuanf
 */ 


#ifndef COMMON_H_
#define COMMON_H_

// state machine 
typedef enum {
	STATE_COUNTING,
	STATE_IDLE,
	STATE_BUZZING
} SystemState;

extern SystemState current_state;


#define F_CPU 16000000UL //16Mhz crystal
#define true 1
#define false 0
#define POV_delay 500 // persistence of vision delay in us
#define initial_min 45
#define initial_sec 0

#define SET_BIT(byte, bit) (byte |= (1 << bit))
#define CLEAR_BIT(byte, bit) (byte &= ~(1 << bit))
#define BIT_IS_SET(byte, bit) (byte & (1<<bit)) //if bit is set
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1<<bit))) // if bit is clear
#define TOGGLE_BIT(port, bit) ((port) ^= (1 << (bit)))

#endif /* COMMON_H_ */