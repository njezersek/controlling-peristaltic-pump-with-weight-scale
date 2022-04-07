#pragma once
#include <Arduino.h>

namespace controller{
	extern uint8_t selected_menu;

	void onKnobRotate(int8_t direction, int8_t multiplier);
	void onButtonPress();
	void render();
}