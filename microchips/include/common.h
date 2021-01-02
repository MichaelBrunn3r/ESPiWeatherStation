#include <Arduino.h>

#define SERIAL_BAUD     115200

// Weather station settings
#define SLEEP_DURATION     5e6
#define BME280_IC2_ADDR	0x76									// The IC2 Address of the BME280 sensor
#define BME280_TEMP_OFFSET -3.0
#define BAT_ADC_RES     1024 									// Resolution of the Analog pin reading the battery level
#define BAT_MAX_V 		4.2f									// Maximum Battery Voltage
#define BAT_PIN			A0

// ESP-Now settings
#define WIFI_CHANNEL    1
#define STATION_ROLE	ESP_NOW_ROLE_CONTROLLER 				// ESP-NOW role of the weather station
#define RECEIVER_ROLE   ESP_NOW_ROLE_SLAVE      				// ESP-NOW role of the receiver on the Pi
#define RECEIVER_MAC    {0x48, 0x3F, 0xDA, 0x01, 0xF9, 0xE3}

struct __attribute__((packed)) MeasurementPacket {
  float temp;   // Temperature
  float hum;    // Humidity
  float press;  // Pressure
  float batv;   // Battery voltage
};