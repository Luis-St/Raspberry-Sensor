#include <iostream>
#include <getopt.h>
#include <wiringPi.h>
#include "DHT.hpp"
#include "sensor.h"

#define DHT11_Pin 17

void printHelp();

int main(const int argc, char *argv[]) {
	if (argc == 1) {
		printHelp();
		return 0;
	}

	static struct option options[] = {
		{"temperature", no_argument, nullptr, 't'},
		{"humidity", no_argument, nullptr, 'h'},
		{"pin", required_argument, nullptr, 'p'},
		{"number", required_argument, nullptr, 'n'},
		{"delay", required_argument, nullptr, 'd'},
		{"infinite", no_argument, nullptr, 'i'},
		{"help", no_argument, nullptr, 0},
		{nullptr, 0, nullptr, 0}
	};

	bool temperature = false;
	bool humidity = false;
	int measurementPin = DHT11_Pin;
	int numberOfMeasurements = 1;
	int measurementDelay = 5;
	bool infinite = false;

	int opt;
	while ((opt = getopt_long(argc, argv, "thp:n:d:i", options, nullptr)) != -1) {
		if (opt == -1) {
			break;
		}
		if (opt == 't') {
			temperature = true;
		} else if (opt == 'h') {
			humidity = true;
		} else if (opt == 'p') {
			char *endptr;
			measurementPin = static_cast<int>(strtol(optarg, &endptr, 10));
			if (*endptr != '\0') {
				std::cerr << "Invalid pin number: " << optarg << std::endl;
				return 1;
			}
		} else if (opt == 'n') {
			char *endptr;
			numberOfMeasurements = static_cast<int>(strtol(optarg, &endptr, 10));
			if (*endptr != '\0') {
				std::cerr << "Invalid number of measurements: " << optarg << std::endl;
				return 1;
			}
		} else if (opt == 'd') {
			char *endptr;
			measurementDelay = static_cast<int>(strtol(optarg, &endptr, 10));
			if (*endptr != '\0') {
				std::cerr << "Invalid delay: " << optarg << std::endl;
				return 1;
			}
		} else if (opt == 'i') {
			infinite = true;
		} else {
			printHelp();
			return 1;
		}
	}

	sensor sensor;
	int i = 0;
	while (infinite || i < numberOfMeasurements) {
		if (temperature && humidity) {
			auto [humidity, temperature] = sensor.getMeasurement(measurementPin);
			std::cout << "Temperature: " << temperature << "*C" << std::endl;
			std::cout << "Humidity: " << humidity << "%" << std::endl;
		} else if (temperature) {
			std::cout << "Temperature: " << sensor.getTemperature(measurementPin) << "*C" << std::endl;
		} else if (humidity) {
			std::cout << "Humidity: " << sensor.getHumidity(measurementPin) << "%" << std::endl;
		}
		if (!infinite) {
			++i;
		}
		if (i < numberOfMeasurements) {
			delay(measurementDelay * 1000);
		}
	}
	return 0;
}

void printHelp() {
	std::cout << "Usage: myapp [options]\n"
			<< "Options:\n"
			<< "  -t, --temperature  If set, the temperature will be read\n"
			<< "  -h, --humidity     If set, the humidity will be read\n"
			<< "  -p, --pin          The pin number of the DHT11 sensor, default is 17\n"
			<< "  -n, --number       The number of measurements to take, default is 1\n"
			<< "  -d, --delay        The delay between measurements in seconds, default is 5\n"
			<< "  -i, --infinite 	 If set, the measurements will be taken infinitely\n"
			<< "      --help         Display this help message\n";
}
