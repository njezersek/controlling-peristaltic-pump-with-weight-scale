#include <Arduino.h>

#define CLK 10  // CLK
#define CS 2    // RS
#define SID 3    // RW

#define CLK_MASK 0b01000000 // port B
#define CS_MASK 0b10000010 // port D
#define SID_MASK 0b00000001 // port D

uint8_t data[1024];

inline void pulseCLK() {
	PORTB |= CLK_MASK; // set CLK high
	PORTB &= ~CLK_MASK; // set CLK low
}

void sendCommand(bool rs, bool rw, uint8_t command, bool reverse = false) {
	
	// sync wiht 5 consecutive 1
	for(int i = 0; i < 5; i++){
		PORTD |= SID_MASK; // set SID high
		pulseCLK();
	}

	// send rw
	if(rw) PORTD |= SID_MASK; // set SID high
	else PORTD &= ~SID_MASK; // set SID low
	pulseCLK();
	
	// send rs
	if(rs) PORTD |= SID_MASK; // set SID high
	else PORTD &= ~SID_MASK; // set SID low
	pulseCLK();

	// send a zero
	PORTD &= ~SID_MASK; // set SID low
	pulseCLK();

	// send hight 4 bits of command
	for(int i = 0; i < 4; i++){
		if(reverse){
			if(command & 0b1) PORTD |= SID_MASK; // set SID high
			else PORTD &= ~SID_MASK; // set SID low
			command >>= 1;
		}
		else{
			if(command & 0b10000000) PORTD |= SID_MASK; // set SID high
			else PORTD &= ~SID_MASK; // set SID low
			command <<= 1;
		}
		pulseCLK();
	}
	// send 4 consecutive 0
	for(int i = 0; i < 4; i++){
		PORTD &= ~SID_MASK; // set SID low
		pulseCLK();
	}

	// send low 4 bits of command
	// send hight 4 bits of command
	for(int i = 0; i < 4; i++){
		if(reverse){
			if(command & 0b1) PORTD |= SID_MASK; // set SID high
			else PORTD &= ~SID_MASK; // set SID low
			command >>= 1;
		}
		else{
			if(command & 0b10000000) PORTD |= SID_MASK; // set SID high
			else PORTD &= ~SID_MASK; // set SID low
			command <<= 1;
		}
		pulseCLK();
	}

	// send 4 consecutive 0
	for(int i = 0; i < 4; i++){
		PORTD &= ~SID_MASK; // set SID low
		pulseCLK();
	}
}

void setup() {
	// put your setup code here, to run once:
	pinMode(13, OUTPUT);
	pinMode(CLK, OUTPUT);
	pinMode(CS, OUTPUT);
	pinMode(SID, OUTPUT);

	digitalWrite(CS, HIGH);
	digitalWrite(SID, LOW);
	digitalWrite(CLK, LOW);

	Serial.begin(9600);

	for(uint16_t i = 0; i < sizeof(data); i++){
		data[i] = 0;
	}

	for(uint8_t y=0; y<64; y++){
		data[y * 16] = y%2 == 0 ? 0xff : 0xaa;
	}

	delay(40); // lcd power on delay
	sendCommand(0,0, 0b00110110); // function set (001D_XRG0: D=8/4b interface, R=exdended instruction set, G=graphic mode)
	delayMicroseconds(72);
	sendCommand(0,0, 0b00000111);
	delayMicroseconds(72);
	sendCommand(0,0, 0b00000111);

}

void loop() {
	delayMicroseconds(72);
	// sendCommand(0,0, 0b00000001); // clear display
	// delay(10);
	// sendCommand(0,0, 0b00001111); // show cursor
	// sendCommand(0,0, 0b00011000); // cursor and display control
	unsigned long start = micros();
	uint16_t k = 0;
	for(uint8_t r = 0; r < 32; r++){ // rows
		sendCommand(0,0, 0b10000000 | r); // set GRAM row addres
		sendCommand(0,0, 0b10000000); // set GRAM column addres
		delayMicroseconds(72);
		for(uint8_t c=0; c<32; c++){ // columns
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
			sendCommand(1,0, data[R*16 + C], true); // write to ram
			delayMicroseconds(10);
		}
	}
	unsigned long end = micros();
	Serial.print("time: ");
	Serial.println(end - start);

}