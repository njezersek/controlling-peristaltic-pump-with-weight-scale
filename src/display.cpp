#include <avr/pgmspace.h>

#include "display.h"
#include "pinout.h"

// display buffer
uint8_t display::data[1024];

// draw mode
uint8_t display::draw_mode = 2;

// font 5x5 uppercase letters
const PROGMEM uint8_t display::font5x5[] = {0x6e,0x99,0xfe,0x99,0x9e,0x6e,0x99,0x89,0x99,0x6e,0xee,0x88,0xcc,0x88,0xe8,0x69,0x89,0xff,0x99,0x69,0x82,0x82,0x82,0x8a,0x84,0x98,0xa8,0xc8,0xa8,0x9e,0x89,0xdd,0xab,0x89,0x89,0x6e,0x99,0x9e,0x98,0x68,0x6e,0x99,0x9e,0xa9,0x59,0x6e,0x84,0x44,0x24,0xc4,0x9a,0x9a,0x9a,0x9a,0x64,0x89,0x89,0xa6,0xd9,0x89,0x9f,0x92,0x64,0x28,0x4f};
const PROGMEM uint8_t display::width_font5x5[] = {0xff,0xaf,0x2e,0x7f,0xfa,0xe7,0xf0};

// font 5x11 numbers
const PROGMEM uint8_t display::font5x11_n[] = {0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x20,0x60,0xa0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xf8,0x70,0x88,0x8,0x8,0x8,0x8,0x10,0x20,0x40,0x80,0xf8,0x70,0x88,0x8,0x8,0x8,0x30,0x8,0x8,0x8,0x88,0x70,0x10,0x10,0x20,0x20,0x40,0x40,0x90,0xf8,0x10,0x10,0x10,0xf8,0x80,0x80,0x80,0x80,0xf0,0x8,0x8,0x8,0x88,0x70,0x70,0x88,0x80,0x80,0x80,0xf0,0x88,0x88,0x88,0x88,0x70,0xf8,0x8,0x8,0x10,0x10,0x20,0x20,0x20,0x40,0x40,0x40,0x70,0x88,0x88,0x88,0x88,0x70,0x88,0x88,0x88,0x88,0x70,0x70,0x88,0x88,0x88,0x88,0x78,0x8,0x8,0x8,0x88,0x70};

// icons
const PROGMEM uint8_t display::settings_icon[] = {0x1,0xe0,0x0,0x11,0xe2,0x0,0x3b,0xf7,0x0,0x7f,0xff,0x80,0x3f,0xff,0x0,0x1f,0xfe,0x0,0x3f,0x3f,0x0,0xfe,0x1f,0xc0,0xfc,0xf,0xc0};
const PROGMEM uint8_t display::gram_icon[] = {0x3a,0x46,0x82,0x82,0x82,0x82,0x46,0x3a,0x2,0x82,0x44,0x38};
const PROGMEM uint8_t display::small_gram_icon[] = {0x1c,0x24,0x24,0x24,0x1c,0x4,0x24,0x18};
const PROGMEM uint8_t display::small_percent_icon[] = {0xc8,0xc8,0x10,0x10,0x20,0x20,0x4c,0x4c};
const PROGMEM uint8_t display::cancle_icon[] = {0x88,0x50,0x20,0x50,0x88};
const PROGMEM uint8_t display::confirm_icon[] = {0x4,0x8,0x10,0xa0,0x40};
const PROGMEM uint8_t display::speaker_icon[] = {0x4,0xc,0xdc,0xfc,0xfc,0xfc,0xdc,0xc,0x4};
const PROGMEM uint8_t display::sound_icon[] = {0x20,0x10,0x48,0x28,0xa8,0xa8,0xa8,0x28,0x48,0x10,0x20};

// seven segment display
/*
	 -A-
	F	B
	 -G-
	E	C
	 -D-
*/
const PROGMEM uint8_t seven_segment[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; // xgfedcba


void display::init(){
	pinMode(CLK, OUTPUT);
	pinMode(CS, OUTPUT);
	pinMode(SID, OUTPUT);

	CS_PORT |= CS_MASK; // set CS high
	SID_PORT &= ~SID_MASK; // set SID low
	CLK_PORT &= ~CLK_MASK; // set CLK low

	delay(40); // lcd power on delay
	
	// initialize display buffer
	display::clear();

	sendCommand(0,0, 0b00110110, false); // function set (001D_XRG0: D=8/4b interface, R=exdended instruction set, G=graphic mode)
	delayMicroseconds(72);
}

inline void display::pulseCLK(){
	CLK_PORT |= CLK_MASK; // set CLK high
	CLK_PORT &= ~CLK_MASK; // set CLK low
}

void display::sendCommand(bool rs, bool rw, uint8_t command, bool reverse = false) {
	
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

void display::setGRAMpointer(uint8_t x, uint8_t y){
	sendCommand(0,0, 0b10000000 | y, false); // set GRAM row addres
	sendCommand(0,0, 0b10000000 | x, false); // set GRAM column addres
}

void display::printBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool reverse){
	uint8_t start = x/8;
	uint8_t bytes = (w+7) / 8;
	uint8_t offset = x%8;

	for(uint8_t row = 0; row < h; row++) {
		for(uint8_t col = start; col <= start+bytes; col++){
			uint8_t mask = 0;
			if(col == start){
				mask |= pgm_read_byte(bitmap + (row * bytes)) >> offset;
			}
			else if(col == start + bytes){
				mask |= pgm_read_byte(bitmap + (row * bytes) + col - start - 1) << (8 - offset);
			}
			else{
				mask |= pgm_read_byte(bitmap + (row * bytes) + col - start) >> offset;
				mask |= pgm_read_byte(bitmap + (row * bytes) + col - start - 1) << (8 - offset);
			}
			data[col + (y+(reverse ? h-1-row : row)) * 16] ^= mask;
		}
	}
}


uint8_t display::printChar(char c, uint8_t x, uint8_t y) {
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
		data[b + (y+row) * 16] ^= row_data >> byte_offset;

		if(byte_offset + width > 8) {
			data[b+1 + (y+row) * 16] ^= row_data << (8 - byte_offset);
		}
	}
	if(add_I) printChar('I', x+width, y);

	return width + add_I;
}

uint8_t display::print(const char *str, uint8_t x, uint8_t y) {
	uint8_t width = 0;
	while(*str) {
		width += printChar(*str, x + width, y) + 1;
		str++;
	}
	return width;
}

uint8_t display::printNumberChar(uint8_t num, uint8_t x, uint8_t y) {
	uint8_t i = num;
	uint8_t b = x / 8;
	uint8_t byte_offset = x % 8;
	uint8_t width = 5;

	for(uint8_t row = 0; row < 11; row++) {
		uint8_t row_data = pgm_read_byte(&font5x11_n[i * 11 + row]);
		data[b + (y+row) * 16] ^= row_data >> byte_offset;

		if(byte_offset + width > 8) {
			data[b+1 + (y+row) * 16] ^= row_data << (8 - byte_offset);
		}
	}

	return width;
}

uint8_t display::printNumber(uint16_t num, uint8_t x, uint8_t y) {
	uint8_t width = 0;
	x -= 5;
	if(num == 0){
		width -= printNumberChar(0, x + width, y) + 1;
	}
	while(num) {
		width -= printNumberChar(num % 10, x + width, y) + 1;
		num /= 10;
	}
	return width;
}

void display::horizontalLine(uint8_t x, uint8_t y, uint8_t w){
	uint8_t start = x/8;
	uint8_t end = (x+w)/8;
	for(uint8_t i = start; i <= end; i++){
		uint8_t mask = 0xff;
		if(i == start){
			mask &= 0xff >> (x % 8);
		}
		if(i == end){
			mask &= 0xff << (8 - (x+w) % 8);
		}

		if(draw_mode == 0){
			data[i + y * 16] &= ~mask;
		}
		else if(draw_mode == 1){
			data[i + y * 16] |= mask;
		}
		else if(draw_mode == 2){
			data[i + y * 16] ^= mask;
		}
	}
}

void display::verticalLine(uint8_t x, uint8_t y, uint8_t h){
	uint8_t mask = 0x80 >> (x % 8);
	for(uint8_t i = y; i < y+h; i++){
		if(draw_mode == 0){
			data[x/8 + i * 16] &= ~mask;
		}
		else if(draw_mode == 1){
			data[x/8 + i * 16] |= mask;
		}
		else if(draw_mode == 2){
			data[x/8 + i * 16] ^= mask;
		}
	}
}

void display::rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h){
	for(uint8_t i = 0; i < h; i++){
		horizontalLine(x, y+i, w);
	}
}

void display::printSegment(uint8_t x, uint8_t y, bool vertical){
	if(vertical){
		horizontalLine(x+1, y, 1);
		for(uint8_t i = 1; i < 12; i++){
			horizontalLine(x, y+i, 3);
		}
		horizontalLine(x+1, y+12, 1);
	}
	else{
		horizontalLine(x+1, y, 11);
		horizontalLine(x, y+1, 13);
		horizontalLine(x+1, y+2, 11);
	}
}

void display::cursor(uint8_t x, uint8_t y, bool vertical){
	if(vertical){
		horizontalLine(x-3, y-2, 1);
		horizontalLine(x-3, y-1, 2);
		horizontalLine(x-3, y, 3);
		horizontalLine(x-3, y+1, 2);
		horizontalLine(x-3, y+2, 1);

	}
	else{
		horizontalLine(x-2, y-2, 5);
		horizontalLine(x-1, y-1, 3);
		horizontalLine(x, y, 1);
	}
}

void display::print7Segment(uint8_t num, uint8_t x, uint8_t y){
	uint8_t conf = pgm_read_byte(&seven_segment[num]);
	if(conf & 0b1) display::printSegment(x+2, y, false); // A
	if(conf & 0b10) display::printSegment(x+14, y+2, true); // B
	if(conf & 0b100) display::printSegment(x+14, y+2+14, true); // C
	if(conf & 0b1000) display::printSegment(x+2, y+14+14, false); // D
	if(conf & 0b10000) display::printSegment(x+0, y+2+14, true); // E
	if(conf & 0b100000) display::printSegment(x+0, y+2, true); // F
	if(conf & 0b1000000) display::printSegment(x+2, y+14, false); // G
}

void display::clear(){
	for(uint16_t i = 0; i < sizeof(data); i++){
		data[i] = 0;
	}
}