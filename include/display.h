#pragma once
#include <Arduino.h>

namespace display{
	extern uint8_t data[];
	extern const uint8_t font5x5[];
	extern const uint8_t width_font5x5[];
	extern const uint8_t font5x11_n[];

	void init();
	inline void pulseCLK();
	void sendCommand(bool rs, bool rw, uint8_t command, bool reverse);
	void setGRAMpointer(uint8_t x, uint8_t y);
	uint8_t printChar(char c, uint8_t x, uint8_t y);
	uint8_t print(const char *str, uint8_t x, uint8_t y);
	uint8_t printNumberChar(uint8_t num, uint8_t x, uint8_t y);
	uint8_t printNumber(uint16_t num, uint8_t x, uint8_t y);
	void clear();
}