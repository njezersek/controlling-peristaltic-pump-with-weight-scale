#pragma once
#include <Arduino.h>
#include "pinout.h"
#include "debounce.h"

namespace input{
	void init();
	void update();
}