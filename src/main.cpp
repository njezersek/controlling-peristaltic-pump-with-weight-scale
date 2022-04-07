#include <Arduino.h>

#include "display.h"
#include "controller.h"
#include "knob.h"
#include "scale.h"


void setup() {
	pinMode(13, OUTPUT);
	display::init();
	knob::setup();

	Serial.begin(9600);

	scale::init();

	display::print("HELLO WORLD", 0, 0);
	display::print("ABCDEFGHIJKLMNOPQRSTUVWZ", 0, 6);

	for(uint8_t n = 0; n < 10; n++){
		display::printNumberChar(n, n*6, 12);
	}
	// display::printNumber(9876, 127, 40);

	display::print7Segment(9, 0, 25);

	// display::horizontalLine(1, 25, 3);
}

void loop() {
	unsigned long start = micros();
	controller::render();
	for(uint8_t r = 0; r < 32; r++){ // rows
		display::setGRAMpointer(0, r);
		knob::update();
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