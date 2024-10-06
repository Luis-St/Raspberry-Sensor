#include <iostream>
#include <wiringPi.h>
#include "DHT.hpp"

#define DHT11_Pin 0

[[noreturn]] int main() {
	DHT module;
	int counts = 0;
	while (true) {
		counts++;
		printf("Measurement counts : %d \n", counts);
		for (int i = 0; i < 15; i++) {
			if (module.readDHT11(DHT11_Pin) == DHTLIB_OK) {
				printf("DHT11,OK! \n");
				break;
			}
			delay(100);
		}
		printf("Humidity is %.2f %%, \t Temperature is %.2f *C\n\n", module.humidity, module.temperature);
		delay(2000);
	}
}
