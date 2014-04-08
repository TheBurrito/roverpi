#include <roverpi/motor.h>

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
