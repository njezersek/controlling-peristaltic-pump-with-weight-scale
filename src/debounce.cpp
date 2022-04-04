#include "debounce.h"

#define DEBOUNCE_DELAY 10

void DebounceInput::setup(){
	pinMode(pin, INPUT_PULLUP);
}

void DebounceInput::update(){
	last_state = state;
	bool state_tmp = !digitalRead(pin);
	unsigned long now = millis();
	if(last_reading != state_tmp){
		last_time = now;
	}
	if(now - last_reading > DEBOUNCE_DELAY){
		state = state_tmp;
	}
	last_reading = state_tmp;
}