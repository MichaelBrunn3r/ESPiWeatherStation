#include <Arduino.h>

#define SERIAL_BAUD     115200

// ESP-Now settings
#define WIFI_CHANNEL    1
#define STATION_ROLE	ESP_NOW_ROLE_CONTROLLER 				// ESP-NOW role of the weather station
#define RECEIVER_ROLE   ESP_NOW_ROLE_SLAVE      				// ESP-NOW role of the receiver on the Pi

struct __attribute__((packed)) MeasurementPacket {
  float temp;   // Temperature
  float hum;    // Humidity
  float press;  // Pressure
  float batv;   // Battery voltage
};