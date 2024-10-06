#ifndef DHT_H
#define DHT_H

#include <wiringPi.h>
#include <cstdint>

#define DHTLIB_OK 0
#define DHTLIB_ERROR_CHECKSUM (-1)
#define DHTLIB_ERROR_TIMEOUT (-2)
#define DHTLIB_INVALID_VALUE (-999)

#define DHTLIB_DHT11_WAKEUP 20
#define DHTLIB_DHT_WAKEUP 1

#define DHTLIB_TIMEOUT 100

class DHT {
public:
	DHT();
	double humidity, temperature;
	int readDHT11Once(int pin);
	int readDHT11(int pin);

private:
	uint8_t bits[5];
	int readSensor(int pin, int wakeupDelay);
};

#endif
