#include <ropi.h>

#include <util/delay.h>

using namespace ropi;

int main(void) {

	init();

	pinMode(0, OUTPUT);
	
	while (1) {
		digitalWrite(0, HIGH);
		_delay_ms(30);
		digitalWrite(0, LOW);
		_delay_ms(70);
	}
	
	return 0;
}
