#include <Arduino.h>

namespace settings{
	extern int8_t speed;
	extern int16_t weight;
	extern int8_t drip;
	extern int16_t stopping_weight;
	extern int8_t start_acceleration;
	extern int8_t sound;
	extern long scale_offset;
	extern float scale_factor;

	void load();
	void save();
}
