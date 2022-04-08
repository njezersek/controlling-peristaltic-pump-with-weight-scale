#include <Arduino.h>

#include "display.h"
#include "controller.h"
#include "knob.h"
#include "scale.h"
#include "settings.h"
#include "buzzer.h"


void setup() {
	display::init();
	settings::load(true);
	knob::setup();
	buzzer::init();

	Serial.begin(9600);

	buzzer::beep(50, 0xae);

	scale::init();
}

void loop() {
	unsigned long start = micros();
	controller::render();
	for(uint8_t r = 0; r < 32; r++){ // rows
		display::setGRAMpointer(0, r);
		knob::update();
		buzzer::update();
		if (scale::is_ready()) {
			scale::update();
		}
		for(uint8_t c=0; c<32; c++){ // columns

			// map LCD to Buffer
			uint8_t C = c;
			uint8_t R = r;
			if(c < 16){
				C = 15 - c;
				R = 63 - r;
			}
			else{
				C = 31 - c;
				R = 31 - r;
			}

			display::sendCommand(1,0, display::data[R*16 + C], true); // write to ram
			delayMicroseconds(10);
		}
	}
	unsigned long end = micros();
	// Serial.println(end-start);
}