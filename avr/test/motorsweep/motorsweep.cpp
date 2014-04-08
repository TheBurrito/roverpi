#define MOTOR_FREQ 1000
#define MOTOR_PRESCALE DIV_8

#include <roverpi.h>

#include <util/delay.h>

using namespace roverpi;

uint16_t val = 0;
uint16_t delta = 1;

void setup() {
	init();
}

void loop() {
	val += delta;
	if (val == MOTOR_TOP) {
		delta *= -1;
	}
	
	set_motor(1, COAST, val);
	_delay_ms(1);
}

int main(void) {
	setup();
	
	while (1) {
		loop();
	}
	
	return 0;	
}
