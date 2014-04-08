#define F_CPU 16000000UL

//MOTOR_FREQ is used to request a target frequency for PWM motor control.
//Note: based on the prescale used the actual frequency will likely be different
//The actual frequency used is stored in the MOTOR_TRUE_FREQ define.
//MOTOR_FREQ defaults to 10KHz if not defined.
#define MOTOR_FREQ 75

//MOTOR_PRESCALE can be set to one of:
//DIV_OFF, DIV_1, DIV_8, DIV_64, DIV_256, DIV_1024
//and controls the clock prescaling for the timer used for PWM. Using a higher
//prescaler allows for finer control at lower frequencies while a lower
//prescaler allows for higher frequencies.
//DIV_OFF turns off motor control.
#define MOTOR_PRESCALE DIV_1024

//The results of the above defines are stored in MOTOR_TOP. This is the max
//value that can be assigned as the motor speed.

#include <roverpi/motor.h>

#include <iostream>

//using namespace roverpi;
using namespace std;

int main(void) {
	cout << "Motor Frequency: " << MOTOR_FREQ << endl;
	cout << "Motor Prescale: " << MOTOR_N << endl;
	cout << "Motor Top: " << MOTOR_TOP << endl;
	cout << "Actual Frequency: " << MOTOR_TRUE_FREQ << endl;
	cout << "TCCRnA: " << MOTOR_TCCRnA << endl;
	cout << "TCCRnB: " << MOTOR_TCCRnB << endl;
	
	return 0;
}
