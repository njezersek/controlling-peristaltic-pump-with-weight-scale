#pragma once
#include <Arduino.h>
#include "pinout.h"
#include "debounce.h"

namespace knob{
	void init();
	void update();
}