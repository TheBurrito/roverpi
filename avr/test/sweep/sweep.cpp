#include <roverpi.h>

#include <util/delay.h>

#define servo_num 1

using namespace roverpi;

int main(void) {

	init();
	
	int16_t angle = 0;
	int16_t step = 1;
	
	servo_range_us(servo_num, 500, 2100);
	
	servo_angle(servo_num, 0);
	
	while (1) {
		angle += step;

		if (angle >= 180) {
			step *= -1;
			angle = 180;
		}
		
		if (angle <= 0) {
			step *= -1;
			angle = 0;
		}
		
		servo_angle(servo_num, angle);
		
		_delay_ms(100);
	}
	
	return 0;
}
