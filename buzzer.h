/*
 * buzzer.h
 *
 * Created: 13/09/2021 10:04:27 pm
 *  Author: yuanf
 */ 

#ifndef BUZZER_H_
#define BUZZER_H_

void buzz_sound(uint16_t tone_duration_ms, uint16_t half_cycle_delay_us);
void play_timeup_tone(void); 
void play_shutdown_tone(void);

#endif /* BUZZER_H_ */