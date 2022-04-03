#include <Arduino.h>

#define CLK 10  // CLK
#define CS 2    // RS
#define SID 3    // RW

inline void toggle_clk() {
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);
}

void sendCommand(bool rs, bool rw, uint8_t command){
	digitalWrite(CS, HIGH);
	digitalWrite(SID, LOW);
	digitalWrite(CLK, LOW);
	// sync wiht 5 consecutive 1
	digitalWrite(SID, HIGH);
	for(int i = 0; i < 5; i++){
		digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
	}
	// send rw
	digitalWrite(SID, rw);
	digitalWrite(CLK, HIGH);
	digitalWrite(CLK, LOW);
	// send rs
	digitalWrite(SID, rs);
	digitalWrite(CLK, HIGH);
	digitalWrite(CLK, LOW);
	// send a zero
	digitalWrite(SID, LOW);
	digitalWrite(CLK, HIGH);
	digitalWrite(CLK, LOW);

	// send hight 4 bits of command
	for(int i = 0; i < 4; i++){
		digitalWrite(SID, (command >> 7) & 1);
		digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
		command <<= 1;
	}
	// send 4 consecutive 0
	digitalWrite(SID, LOW);
	for(int i = 0; i < 4; i++){
		digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
	}
	// send low 4 bits of command
	for(int i = 0; i < 4; i++){
		digitalWrite(SID, (command >> 7) & 1);
		digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
		command <<= 1;
	}
	// send 4 consecutive 0
	digitalWrite(SID, LOW);
	for(int i = 0; i < 4; i++){
		digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
	}
	digitalWrite(CS, LOW);
}

void setup() {
	// put your setup code here, to run once:
	pinMode(13, OUTPUT);
	pinMode(CLK, OUTPUT);
	pinMode(CS, OUTPUT);
	pinMode(SID, OUTPUT);

	Serial.begin(9600);

	delay(40); // lcd power on delay
}

void loop() {
	sendCommand(0,0, 0b00100110); // function set (001D_XRG0: D=8/4b interface, R=exdended instruction set, G=graphic mode)
	delayMicroseconds(72);
	sendCommand(0,0, 0b00000001); // clear display
	delay(10);
	// sendCommand(0,0, 0b00001111); // show cursor
	// sendCommand(0,0, 0b00011000); // cursor and display control
	unsigned long start = micros();
	for(uint8_t j = 0; j < 32; j++){
		sendCommand(0,0, 0b10000000 | j); // set GRAM row addres
		sendCommand(0,0, 0b10000000); // set GRAM column addres
		for(uint8_t i=0; i<32; i++){
			if((i+j) % 2 == 0){
				sendCommand(1,0, 0xff); // write to ram
			}
			else{
				sendCommand(1,0, 0x00); // write to ram
			}
		}
	}
	unsigned long end = micros();
	Serial.print("time: ");
	Serial.println(end - start);

	delay(1000);

}