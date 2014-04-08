#ifndef serial_h_
#define serial_h_

#include "base.h"

#ifndef SERIAL_BUFFER_SIZE
#define SERIAL_BUFFER_SIZE 128
#endif

namespace roverpi {

class UART {
public:
	virtual ~Serial();
	
	void begin(int baud);
	
	int available() const;
	
	uint8_t read();
	void write(uint8_t b);
	
	int read(uint8_t *buf, int num);
	int write(const uint8_t *buf, int num);
	
protected:
	Serial();
	
private:
	uint8_t _buf[SERIAL_BUFFER_SIZE];
	int _head;
	int _tail;
}

extern UART Serial;
extern UART Serial1;
extern UART Serial2;

} //namespace roverpi

#endif // serial_h_
