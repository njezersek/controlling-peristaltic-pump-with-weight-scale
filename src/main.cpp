#include <Arduino.h>

#define CLK 10  // CLK
#define CS 2    // RS
#define SID 3    // RW

#define CLK_MASK 0b01000000 // port B
#define CS_MASK 0b10000010 // port D
#define SID_MASK 0b00000001 // port D

inline void pulseCLK() {
	PORTB |= CLK_MASK; // set CLK high
	PORTB &= ~CLK_MASK; // set CLK low
}

void sendCommand(bool rs, bool rw, uint8_t command){
	
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
		if(command & 0b10000000) PORTD |= SID_MASK; // set SID high
		else PORTD &= ~SID_MASK; // set SID low
		pulseCLK();
		command <<= 1;
	}
	// send 4 consecutive 0
	for(int i = 0; i < 4; i++){
		PORTD &= ~SID_MASK; // set SID low
		pulseCLK();
	}

	// send low 4 bits of command
	for(int i = 0; i < 4; i++){
		if(command & 0b10000000) PORTD |= SID_MASK; // set SID high
		else PORTD &= ~SID_MASK; // set SID low
		pulseCLK();
		command <<= 1;
	}

	// send 4 consecutive 0
	for(int i = 0; i < 4; i++){
		PORTD &= ~SID_MASK; // set SID low
		pulseCLK();
	}
}

void sendCommand8(bool rs, bool rw, uint8_t command){
	
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
	for(int i = 0; i < 8; i++){
		if(command & 0b10000000) PORTD |= SID_MASK; // set SID high
		else PORTD &= ~SID_MASK; // set SID low
		pulseCLK();
		command <<= 1;
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

	delay(40); // lcd power on delay
}

void loop() {
	sendCommand(0,0, 0b00110110); // function set (001D_XRG0: D=8/4b interface, R=exdended instruction set, G=graphic mode)
	delayMicroseconds(72);
	// sendCommand(0,0, 0b00000001); // clear display
	// delay(10);
	// sendCommand(0,0, 0b00001111); // show cursor
	// sendCommand(0,0, 0b00011000); // cursor and display control
	unsigned long start = micros();
	for(uint8_t j = 0; j < 32; j++){ // rows
		sendCommand(0,0, 0b10000000 | j); // set GRAM row addres
		sendCommand(0,0, 0b10000000); // set GRAM column addres
		delayMicroseconds(72);
		for(uint8_t i=0; i<32; i++){ // columns
			if((i+(j/8)) % 2 == 0){
				if(i == 0 || i == 16){
					sendCommand(1,0, 0xbf); // write to ram
				}
				else{
					sendCommand(1,0, 0xff); // write to ram
				}
			}
			else{
				if(i == 0 || i == 16){
					sendCommand(1,0, 0x80); // write to ram
				}
				else{
					sendCommand(1,0, 0x00); // write to ram
				}
			}
			delayMicroseconds(10);
		}
	}
	unsigned long end = micros();
	Serial.print("time: ");
	Serial.println(end - start);

	delay(1000);

}