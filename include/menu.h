#pragma once
#include <Arduino.h>

namespace menu{
	extern uint8_t selected_menu;
	extern uint8_t selected_item;
	extern bool selected;

	extern int8_t speed;
	extern int16_t weight;
	extern int8_t drip;

	void onKnobRotate(int direction);
	void onButtonPress();
	void render();
	void renderMain();
	void renderSettings();
}