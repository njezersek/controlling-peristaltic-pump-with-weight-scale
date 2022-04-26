#include <Arduino.h>

#define PUMP_DRRECTION_FORWARD 0
#define PUMP_DRRECTION_BACKWARD 1

namespace pump{
	extern bool running;
	extern unsigned long start_time;
	void init();
	void update();

	void setSpeed(uint16_t speed);
	void setDirection(bool direction);
	void start();
	void stop();
}