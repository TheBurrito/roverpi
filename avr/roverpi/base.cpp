#include <roverpi/base.h>
#include <roverpi/servo.h>
#include <roverpi/motor.h>

namespace roverpi {

volatile uint8_t * out_ports[4];
volatile uint8_t * in_ports[4];
volatile uint8_t * dir_ports[4];

void init() {
	/**
	 * Setup default pin config:
	 *
	 * 1-16 : Digital Output
	 * 17-32: Digital Input
	 *
	 * Servos on S1 - S12
	 *
	 * Motor control on associated pins
	 */

	out_ports[0] = &PORTF;
	out_ports[1] = &PORTK;
	out_ports[2] = &PORTA;
	out_ports[3] = &PORTC;

	in_ports[0] = &PINF;
	in_ports[1] = &PINK;
	in_ports[2] = &PINA;
	in_ports[3] = &PINC;

	dir_ports[0] = &DDRF;
	dir_ports[1] = &DDRK;
	dir_ports[2] = &DDRA;
	dir_ports[3] = &DDRC;
	
	servo_init();
	motor_init();
}

void pinMode(uint8_t pin, pinmode_t mode) {
	volatile uint8_t * port = dir_port(pin);
	uint8_t port_pin = gpio_port_pin(pin);

	switch (mode) {
	case OUTPUT:
		*port |= (1<<port_pin);
		break;

	case INPUT:
		*port &= ~(1<<port_pin);
		break;
	}
}

}
