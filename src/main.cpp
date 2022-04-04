#include <Arduino.h>

#include "display.h"

void setup() {
	pinMode(13, OUTPUT);
	display::init();

	Serial.begin(9600);

	display::print("HELLO WORLD", 0, 0);
	display::print("ABCDEFGHIJKLMNOPQRSTUVWZ", 0, 6);

	for(uint8_t n = 0; n < 10; n++){
		display::printNumberChar(n, n*6, 12);
	}

	display::printNumber(9876, 127, 40);
}

void loop() {
	delayMicroseconds(72);
	unsigned long start = micros();
	for(uint8_t r = 0; r < 32; r++){ // rows
		display::setGRAMpointer(0, r);
		delayMicroseconds(10);
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
	Serial.print("time: ");
	Serial.println(end - start);
}