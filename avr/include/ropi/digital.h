#ifndef digital_h_
#define digital_h_

#include <ropi/base.h>

namespace ropi {

inline void digitalWrite(uint8_t pin, value_t val) {
	volatile uint8_t * port = out_port(pin);
	uint8_t port_pin = gpio_port_pin(pin);

	if (val) {
		*port |= (1<<port_pin);
	} else {
		*port &= ~(1<<port_pin);
	}
}

inline value_t digitalRead(uint8_t pin) {
	volatile uint8_t * port = in_port(pin);
	unsigned char port_pin = gpio_port_pin(pin);

	if (*port & (1<<port_pin)) {
		return HIGH;
	} else {
		return LOW;
	}
}

} //namespace ropi

#endif //digital_h_
