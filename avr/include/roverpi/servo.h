#ifndef servo_h_
#define servo_h_

#include <inttypes.h>

namespace roverpi {

void servo_init();

void servo_angle(uint8_t num, uint8_t angle);
void servo_angle_x10(uint8_t num, uint16_t angx10);
void servo_us(uint8_t num, uint16_t us);
void servo_ticks(uint8_t num, uint8_t ticks);

void servo_coast(uint8_t num);

void servo_range_us(uint8_t num, uint16_t min, uint16_t max);

} //namespace ropi

#endif //servo_h_
