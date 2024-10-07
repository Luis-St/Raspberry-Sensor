#ifndef SENSOR_H
#define SENSOR_H

#include "DHT.hpp"

struct measurement {
	double humidity;
	double temperature;
};

class sensor {
private:
	DHT module;

	void read(const int pin) {
		while (module.readDHT11(pin) != DHTLIB_OK) {
			delay(100);
		}
	}

public:
	measurement getMeasurement(const int pin) {
		this->read(pin);
		return {this->module.humidity, this->module.temperature};
	}

	double getHumidity(const int pin) {
		this->read(pin);
		return this->module.humidity;
	}

	double getTemperature(const int pin) {
		this->read(pin);
		return this->module.temperature;
	}
};

#endif
