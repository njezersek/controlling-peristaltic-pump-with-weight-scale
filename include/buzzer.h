#include <Arduino.h>

namespace buzzer{
	extern unsigned long start_time;
	extern uint8_t beep_duration;
	extern uint8_t beep_patern;

	void init();
	void beep(uint8_t duration, uint8_t patern);
	void update();
}