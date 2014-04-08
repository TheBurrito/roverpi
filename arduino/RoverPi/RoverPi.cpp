#include "RoverPi.h"

#include <Arduino.h>

uint8_t PIN[] = {
	A0,
	A1,
	A2,
	A3,
	A4,
	A5,
	A6,
	A7,
	A8,
	A9,
	A10,
	A11,
	A12,
	A13,
	A14,
	A15,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	37,
	36,
	35,
	34,
	33,
	32,
	31,
	30
};

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

void motor_init() {
	//Set all outputs to default off
	MOTOR1_PORT &= ~(1<<MOTOR1_PA);
	MOTOR1_DDR |=   (1<<MOTOR1_PA);
	MOTOR1_PORT &= ~(1<<MOTOR1_PB);
	MOTOR1_DDR |=   (1<<MOTOR1_PB);
	MOTOR1_PORT &= ~(1<<MOTOR1_PPWM);
	MOTOR1_DDR |=   (1<<MOTOR1_PPWM);
	MOTOR1_VAL = 0;
	
	MOTOR2_PORT &= ~(1<<MOTOR2_PA);
	MOTOR2_DDR |=   (1<<MOTOR2_PA);
	MOTOR2_PORT &= ~(1<<MOTOR2_PB);
	MOTOR2_DDR |=   (1<<MOTOR2_PB);
	MOTOR2_PORT &= ~(1<<MOTOR2_PPWM);
	MOTOR2_DDR |=   (1<<MOTOR2_PPWM);
	MOTOR2_VAL = 0;
	
	MOTOR3_PORT &= ~(1<<MOTOR3_PA);
	MOTOR3_DDR |=   (1<<MOTOR3_PA);
	MOTOR3_PORT &= ~(1<<MOTOR3_PB);
	MOTOR3_DDR |=   (1<<MOTOR3_PB);
	MOTOR3_PORT &= ~(1<<MOTOR3_PPWM);
	MOTOR3_DDR |=   (1<<MOTOR3_PPWM);
	MOTOR3_VAL = 0;
	
	MOTOR4_PORT &= ~(1<<MOTOR4_PA);
	MOTOR4_DDR |=   (1<<MOTOR4_PA);
	MOTOR4_PORT &= ~(1<<MOTOR4_PB);
	MOTOR4_DDR |=   (1<<MOTOR4_PB);
	MOTOR4_PORT &= ~(1<<MOTOR4_PPWM);
	MOTOR4_DDR |=   (1<<MOTOR4_PPWM);
	MOTOR4_VAL = 0;
	
	OCR3A = MOTOR_TOP;
	OCR4A = MOTOR_TOP;
	
	//establish control register values based on configuration
	TCCR3A = MOTOR_TCCRnA;
	TCCR4A = MOTOR_TCCRnA;
	
	TCCR3B = MOTOR_TCCRnB;
	TCCR4B = MOTOR_TCCRnB;
}

void set_motor(uint8_t num, MotorMode mode, uint16_t val) {
	volatile uint8_t *port;
	uint8_t pina, pinb;
	volatile uint16_t *motor_val;
	
	switch (num) {
	case 1:
		port =       &MOTOR1_PORT;
		pina =        MOTOR1_PA;
		pinb =        MOTOR1_PB;
		motor_val =  &MOTOR1_VAL;
		break;
		
	case 2:
		port =       &MOTOR2_PORT;
		pina =        MOTOR2_PA;
		pinb =        MOTOR2_PB;
		motor_val =  &MOTOR2_VAL;
		break;
		
	case 3:
		port =       &MOTOR3_PORT;
		pina =        MOTOR3_PA;
		pinb =        MOTOR3_PB;
		motor_val =  &MOTOR3_VAL;
		break;
		
	case 4:
		port =       &MOTOR4_PORT;
		pina =        MOTOR4_PA;
		pinb =        MOTOR4_PB;
		motor_val =  &MOTOR4_VAL;
		break;
		
	default:
		return;
		
	}
	
	if (val > MOTOR_TOP) {
		val = MOTOR_TOP;
	}
	
	*motor_val = val;
	
	switch (mode) {
	case COAST:
		*port &= ~((1<<pina) | (1<<pinb));
		break;
		
	case FORWARD:
		*port |= (1<<pina);
		*port &= ~(1<<pinb);
		break;
		
	case REVERSE:
		*port &= ~(1<<pina);
		*port |= (1<<pinb);
		break;
	
	case BRAKE:
		*port |= ((1<<pina) | (1<<pinb));
		break;
	}
}

RoverPiExpansion::RoverPiExpansion() {
	motor_init();
	
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

RoverPiExpansion::~RoverPiExpansion() {
}

inline
void RoverPiExpansion::servo_ticks(uint8_t num, uint8_t ticks) {
	if (ticks < _servo[num].min_ticks) ticks = _servo[num].min_ticks;
	if (ticks > _servo[num].max_ticks) ticks = _servo[num].max_ticks;
	
	cli();
	_servo[num].ticks = ticks;
	sei();
}

void RoverPiExpansion::servo_angle(uint8_t num, uint8_t angle) {
	servo_ticks(num, (uint16_t)map(angle, 0, 180, _servo[num].min_ticks, _servo[num].max_ticks));
}

void RoverPiExpansion::servo_angle_x10(uint8_t num, uint16_t angx10) {
	servo_ticks(num, (uint16_t)map(angx10, 0, 1800, _servo[num].min_ticks, _servo[num].max_ticks));
}

void RoverPiExpansion::servo_us(uint8_t num, uint16_t us) {
	servo_ticks(num, us * 2);
}

void RoverPiExpansion::servo_coast(uint8_t num) {
	_servo[num].ticks = 0;
}

void RoverPiExpansion::servo_range_us(uint8_t num, uint16_t min, uint16_t max) {
	_servo[num].min_ticks = min * 2;
	_servo[num].max_ticks = max * 2;
}

RoverPiExpansion RoverPi;
