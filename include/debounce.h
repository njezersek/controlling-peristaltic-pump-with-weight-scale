#pragma once
#include <Arduino.h>

class DebounceInput{
private:
	bool last_reading = 0;
	unsigned long last_time = 0;
	uint8_t pin;
public:
	bool last_state = 0;
	bool state = 0;
	DebounceInput(uint8_t pin) : pin(pin) {}

	void setup();
	void update();
};