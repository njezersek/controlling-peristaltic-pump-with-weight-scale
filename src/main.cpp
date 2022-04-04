#include <Arduino.h>
#include <avr/pgmspace.h>

#define CLK 10  // CLK
#define CS 2    // RS
#define SID 3    // RW

#define CLK_MASK 0b01000000 // port B
#define CS_MASK 0b10000010 // port D
#define SID_MASK 0b00000001 // port D

uint8_t data[1024];

// font 5x5 uppercase letters
const PROGMEM uint8_t font5x5[] = {0x6e,0x99,0xfe,0x99,0x9e,0x6e,0x99,0x89,0x99,0x6e,0xee,0x88,0xcc,0x88,0xe8,0x69,0x89,0xff,0x99,0x69,0x82,0x82,0x82,0x8a,0x84,0x98,0xa8,0xc8,0xa8,0x9e,0x89,0xdd,0xab,0x89,0x89,0x6e,0x99,0x9e,0x98,0x68,0x6e,0x99,0x9e,0xa9,0x59,0x6e,0x84,0x44,0x24,0xc4,0x9a,0x9a,0x9a,0x9a,0xf4,0x89,0x89,0xa6,0xd9,0x89,0x9f,0x92,0x64,0x28,0x4f};
const PROGMEM uint8_t width_font5x5[] = {0xff,0xaf,0x2e,0x7f,0xfa,0xe7,0xf0};

// font 5x11 numbers
const PROGMEM uint8_t font5x11_n[] = {0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x20,0x60,0xa0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xf8,0x70,0x88,0x8,0x8,0x8,0x8,0x10,0x20,0x40,0x80,0xf8,0x70,0x88,0x8,0x8,0x8,0x30,0x8,0x8,0x8,0x88,0x70,0x10,0x10,0x20,0x20,0x40,0x40,0x90,0xf8,0x10,0x10,0x10,0xf8,0x80,0x80,0x80,0x80,0xf0,0x8,0x8,0x8,0x88,0x70,0x70,0x88,0x80,0x80,0x80,0xf0,0x88,0x88,0x88,0x88,0x70,0xf8,0x8,0x8,0x10,0x10,0x20,0x20,0x20,0x40,0x40,0x40,0x70,0x88,0x88,0x88,0x88,0x70,0x88,0x88,0x88,0x88,0x70,0x70,0x88,0x88,0x88,0x88,0x78,0x8,0x8,0x8,0x88,0x70};


inline void pulseCLK() {
	PORTB |= CLK_MASK; // set CLK high
	PORTB &= ~CLK_MASK; // set CLK low
}

uint8_t printChar(char c, uint8_t x, uint8_t y) {
	uint8_t i = c - 'A';
	if(i > 25) {
		i = c - 'a';
		if(i > 25) {
			return 3;
		}
	}
	uint8_t b = x / 8;
	uint8_t byte_offset = x % 8;
	uint8_t width = pgm_read_byte(&width_font5x5[i/4]) >> (3-i%4)*2 & 0b11;
	width++;
	bool add_I = width == 2;
	if(width == 2) width = 4;
	for(uint8_t row = 0; row < 5; row++) {
		// uint8_t data = pgm_read_byte(&font5x4[(c - 32) * 5 + row]);
		uint8_t row_data = ((pgm_read_byte(&font5x5[i/2 * 5 + row]) << (i%2 * 4)) & 0xf0);
		data[b + (y+row) * 16] |= row_data >> byte_offset;

		if(byte_offset + width > 8) {
			data[b+1 + (y+row) * 16] |= row_data << (8 - byte_offset);
		}
	}
	if(add_I) printChar('I', x+width, y);

	return width + add_I;
}

uint8_t print(char *str, uint8_t x, uint8_t y) {
	uint8_t width = 0;
	while(*str) {
		width += printChar(*str, x + width, y) + 1;
		str++;
	}
	return width;
}

uint8_t printNumberChar(uint8_t num, uint8_t x, uint8_t y) {
	uint8_t i = num;
	uint8_t b = x / 8;
	uint8_t byte_offset = x % 8;
	uint8_t width = 5;

	for(uint8_t row = 0; row < 11; row++) {
		uint8_t row_data = pgm_read_byte(&font5x11_n[i * 11 + row]);
		data[b + (y+row) * 16] |= row_data >> byte_offset;

		if(byte_offset + width > 8) {
			data[b+1 + (y+row) * 16] |= row_data << (8 - byte_offset);
		}
	}

	return width;
}

uint8_t printNumber(uint16_t num, uint8_t x, uint8_t y) {
	uint8_t width = 0;
	x -= 5;
	while(num) {
		width -= printNumberChar(num % 10, x + width, y) + 1;
		num /= 10;
	}
	return width;
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

	// for(uint8_t y=0; y<64; y++){
	// 	data[y * 16] = y%2 == 0 ? 0xff : 0xaa;
	// }

	// uint8_t cursor = 0;
	// for(char c = 'A'; c <= 'Z'; c++){
	// 	cursor += printChar(c, cursor, 0) + 1;
	// }

	print("HELLO WORLD", 0, 0);
	print("ABCDEFGHIJKLMNOPQRSTUVWZ", 0, 6);

	for(uint8_t n = 0; n < 10; n++){
		printNumberChar(n, n*6, 12);
	}

	printNumber(12345, 127, 40);

	delay(40); // lcd power on delay
	sendCommand(0,0, 0b00110110); // function set (001D_XRG0: D=8/4b interface, R=exdended instruction set, G=graphic mode)
	delayMicroseconds(72);

}

void loop() {
	delayMicroseconds(72);
	unsigned long start = micros();
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