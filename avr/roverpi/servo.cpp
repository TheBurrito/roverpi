#include <roverpi/base.h>
#include <roverpi/servo.h>

#include <avr/interrupt.h>

namespace roverpi {

#define SERVO_COUNT 12

typedef struct {
	volatile uint8_t * port;
	unsigned int ticks;
	uint8_t clearmask, setmask;
	uint16_t min_ticks, max_ticks;
} servo_t;

servo_t _servo[SERVO_COUNT];

static volatile int8_t _channel;

ISR(TIMER5_COMPA_vect) 
{
	if (_channel < 0) {
		TCNT5 = 0; // channel set to -1 indicated that refresh interval completed so reset the timer 
	} else {
		if (_servo[_channel].ticks > 0) { 
			//digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,LOW); // pulse this channel low if activated
			*(_servo[_channel].port) &= _servo[_channel].clearmask;
		}
	}

	_channel++;    // increment to the next channel
	
	if (_channel < SERVO_COUNT) {
		if (_servo[_channel].ticks > 0) {
			OCR5A = TCNT5 + _servo[_channel].ticks;
			*(_servo[_channel].port) |= _servo[_channel].setmask;
		} else {
			OCR5A = TCNT5 + 2000;
		}
	} else { 
		// finished all channels so wait for the refresh period to expire before starting over 
		if (((unsigned)TCNT5) + 4 < 40000) {  // allow a few ticks to ensure the next OCR1A not missed
			OCR5A = 40000;  
		} else  {
			OCR5A = TCNT5 + 4;  // at least REFRESH_INTERVAL has elapsed
		}
		
		_channel = -1; // this will get incremented at the end of the refresh period to start again at the first channel
	}
}

void initISR() {
	TCCR5A = 0;             // normal counting mode 
    TCCR5B = _BV(CS51);     // set prescaler of 8  
    TCNT5 = 0;              // clear the timer count 
    TIFR5 = _BV(OCF5A);     // clear any pending interrupts; 
    TIMSK5 =  _BV(OCIE5A) ; // enable the output compare interrupt
}

void servo_init() {
	volatile uint8_t *ports[] = {&PORTL, &PORTD};
	
	for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
		_servo[i].port = ports[i / 8];
		_servo[i].ticks = 0;
		_servo[i].min_ticks = 2000;
		_servo[i].max_ticks = 4000;
		_servo[i].setmask = 1<<(i % 8);
		
		if (i > 7) {
			_servo[i].setmask = _servo[i].setmask << 4;
		}
		
		_servo[i].clearmask = ~_servo[i].setmask;
	}
	
	_channel = -1;
	
	PORTL = 0;
	DDRL = 0xFF;
	
	PORTD &= 0xF;
	DDRD |= 0x0F;
	
	initISR();
}

inline
void servo_ticks(uint8_t num, uint16_t ticks) {
	if (ticks < _servo[num].min_ticks) ticks = _servo[num].min_ticks;
	if (ticks > _servo[num].max_ticks) ticks = _servo[num].max_ticks;
	
	cli();
	_servo[num].ticks = ticks;
	sei();
}

void servo_angle(uint8_t num, uint8_t angle) {
	servo_ticks(num, (uint16_t)map(angle, 0, 180, _servo[num].min_ticks, _servo[num].max_ticks));
}

void servo_angle_x10(uint8_t num, uint16_t angx10) {
	servo_ticks(num, (uint16_t)map(angx10, 0, 1800, _servo[num].min_ticks, _servo[num].max_ticks));
}

void servo_us(uint8_t num, uint16_t us) {
	servo_ticks(num, us * 2);
}

void servo_coast(uint8_t num) {
_servo[num].ticks = 0;
}

void servo_range_us(uint8_t num, uint16_t min, uint16_t max) {
	_servo[num].min_ticks = min * 2;
	_servo[num].max_ticks = max * 2;
}

} //namespace ropi

