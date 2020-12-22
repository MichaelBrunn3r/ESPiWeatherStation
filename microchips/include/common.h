#include <Arduino.h>

#define STATION_ROLE	ESP_NOW_ROLE_CONTROLLER 				// ESP-NOW role of the weather station
#define BME280_IC2_ADDR	0x76									// The IC2 Address of the BME280 sensor
#define BME280_TEMP_OFFSET -6.0
#define BAT_ADC_RES     1024 									// Resolution of the Analog pin reading the battery level
#define BAT_MAX_V 		4.2f									// Maximum Battery Voltage
#define BAT_PIN			A0

#define RECEIVER_ROLE   ESP_NOW_ROLE_SLAVE      				// ESP-NOW role of the receiver on the Pi
#define RECEIVER_MAC    {0x48, 0x3F, 0xDA, 0x01, 0xF9, 0xE3}

#define WIFI_CHANNEL    1
#define SERIAL_BAUD     115200

struct __attribute__((packed)) MeasurementPacket {
  float temp;   // Temperature
  float hum;    // Humidity
  float press;  // Pressure
  float batv;   // Battery voltage
};