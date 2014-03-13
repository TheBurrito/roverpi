#ifndef base_h_
#define base_h_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/io.h>

namespace ropi {

inline long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void init();

extern volatile uint8_t * out_ports[4];
extern volatile uint8_t * in_ports[4];
extern volatile uint8_t * dir_ports[4];

typedef enum { INPUT, OUTPUT } pinmode_t;
typedef enum { LOW, HIGH } value_t;

inline uint8_t gpio_port_pin(uint8_t g) {
	return (g % 8);
}

inline volatile uint8_t * out_port(uint8_t g) {
	return (out_ports[g / 8]);
}

inline volatile uint8_t * in_port(uint8_t g) {
	return (in_ports[g / 8]);
}

inline volatile uint8_t * dir_port(uint8_t g) {
	return (dir_ports[g / 8]);
}

void pinMode(uint8_t pin, pinmode_t mode);

} //namespace ropi

#endif //base_h_
