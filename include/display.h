#pragma once
#include <Arduino.h>

namespace display{
	extern uint8_t data[];
	extern const uint8_t font5x5[];
	extern const uint8_t width_font5x5[];
	extern const uint8_t font5x11_n[];
	extern const uint8_t settings_icon[];
	extern const uint8_t gram_icon[];
	extern const uint8_t small_gram_icon[];
	extern const uint8_t small_percent_icon[];
	extern const uint8_t cancle_icon[];
	extern const uint8_t confirm_icon[];
	extern const uint8_t speaker_icon[];
	extern const uint8_t sound_icon[];
	extern uint8_t draw_mode; // 0: AND, 1: OR, 2: XOR 

	void init();
	inline void pulseCLK();
	void sendCommand(bool rs, bool rw, uint8_t command, bool reverse);
	void setGRAMpointer(uint8_t x, uint8_t y);
	uint8_t printChar(char c, uint8_t x, uint8_t y);
	uint8_t print(const char *str, uint8_t x, uint8_t y);
	uint8_t printNumberChar(uint8_t num, uint8_t x, uint8_t y);
	uint8_t printNumber(uint16_t num, uint8_t x, uint8_t y);
	void horizontalLine(uint8_t x, uint8_t y, uint8_t w);
	void verticalLine(uint8_t x, uint8_t y, uint8_t h);
	void rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
	void printSegment(uint8_t x, uint8_t y, bool vertical);
	void print7Segment(uint8_t num, uint8_t x, uint8_t y);
	void cursor(uint8_t x, uint8_t y, bool vertical);
	void printBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool reverse);
	void clear();
}