#include <roverpi.h>

#include <util/delay.h>

using namespace roverpi;

#define PIN 0

int main(void) {

	init();

	pinMode(PIN, OUTPUT);
	
	while (1) {
		digitalWrite(PIN, HIGH);
		_delay_ms(30);
		digitalWrite(PIN, LOW);
		_delay_ms(70);
	}
	
	return 0;
}
