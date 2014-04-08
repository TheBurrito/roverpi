/**

**/

#ifndef motor_h_
#define motor_h_

#include "base.h"
//#include <stdint.h>

#define DIV_OFF  0
#define DIV_1    1
#define DIV_8    2
#define DIV_64   3
#define DIV_256  4
#define DIV_1024 5

#ifndef MOTOR_PRESCALE
#define MOTOR_PRESCALE DIV_1
#endif

#ifndef MOTOR_FREQ
#define MOTOR_FREQ 10000
#endif

#if MOTOR_PRESCALE == DIV_1
#define MOTOR_N 1
#elif MOTOR_PRESCALE == DIV_8
#define MOTOR_N 8
#elif MOTOR_PRESCALE == DIV_64
#define MOTOR_N 64
#elif MOTOR_PRESCALE == DIV_256
#define MOTOR_N 256
#elif MOTOR_PRESCALE == DIV_1024
#define MOTOR_N 1024
#elif MOTOR_PRESCALE == DIV_OFF
#define MOTOR_N 0
#endif

#ifndef MOTOR_N
#error MOTOR_N could not be set. Ensure a proper value is set for MOTOR_PRESCALE
#define MOTOR_N 0
#endif

#if MOTOR_N == 0
#define MOTOR_TOP 0
#define MOTOR_TRUE_FREQ 0
#else
#define MOTOR_TOP ( F_CPU / (2 * MOTOR_N * MOTOR_FREQ) )
#define MOTOR_TRUE_FREQ ( F_CPU / (2 * MOTOR_N * MOTOR_TOP) )
#endif

#define MOTOR_TCCRnA 43
#define MOTOR_TCCRnB ( 16 | MOTOR_PRESCALE )

#define MOTOR1_PORT PORTE
#define MOTOR1_DDR DDRE
#define MOTOR1_PA PE2
#define MOTOR1_PB PE3
#define MOTOR1_PPWM PE4
#define MOTOR1_VAL OCR3B

#define MOTOR2_PORT PORTE
#define MOTOR2_DDR DDRE
#define MOTOR2_PA PE6
#define MOTOR2_PB PE7
#define MOTOR2_PPWM PE5
#define MOTOR2_VAL OCR3C

#define MOTOR3_PORT PORTH
#define MOTOR3_DDR DDRH
#define MOTOR3_PA PH2
#define MOTOR3_PB PH3
#define MOTOR3_PPWM PH4
#define MOTOR3_VAL OCR4B

#define MOTOR4_PORT PORTH
#define MOTOR4_DDR DDRH
#define MOTOR4_PA PH6
#define MOTOR4_PB PH7
#define MOTOR4_PPWM PH5
#define MOTOR4_VAL OCR4C

typedef enum {
	COAST,
	FORWARD,
	REVERSE,
	BRAKE
} MotorMode;

void motor_init();

void set_motor(uint8_t num, MotorMode mode, uint16_t val);

#endif
