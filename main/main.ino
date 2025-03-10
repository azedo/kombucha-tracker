#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <time.h>

// Include config first so its constants are available to all other files
#include "config.h"

// Global variables - must be defined before including other files
String startDate = "";
int brewDays = 7;  // Default brewing time in days
ESP8266WebServer server(80);

// Forward declarations of functions used across files
void handleRoot();
void handleSet();
String getCurrentTimeString();
int getDaysPassed(String start);
String getReadyDate(String start, int days);
void saveBrewingData();
void loadBrewingData();

// Include the temp_sensor.h file first since web_interface.h uses its variables
#include "temp_sensor.h"
// Now include the header files
#include "time_utils.h"
#include "data_manager.h"
#include "web_interface.h"
#include "wifi_setup.h"
#include "led_manager.h"
#include "light_sensor.h"


void setup() {
  Serial.begin(115200);

  // Initialize LED pins
  setupLEDs();

  // Initialize DHT sensor
  setupDHT();

  // Setup light sensor
  setupLightSensor();

  // Initialize file system
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  // Setup WiFi
  setupWiFi();

  // Initialize time with NTP
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  // Set timezone to Central European Time (CET/CEST)
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

  Serial.println("Waiting for time sync...");
  while (time(nullptr) < 8 * 3600 * 2) {  // Wait until the time is set
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Time synchronized");

  // Load saved brewing data
  loadBrewingData();

  // Setup web server
  setupServer();
}

void loop() {
  // Handle web requests
  server.handleClient();

  // Update temperature readings
  updateDHTReadings();

  // Update light sensor readings
  updateLightReading();

  // Update LED status
  bool hasBatch = (startDate != "");
  bool isReady = false;

  if (hasBatch) {
    int daysPassed = getDaysPassed(startDate);
    isReady = (daysPassed >= brewDays);
  }

  updateLEDs(hasBatch, isReady);

  // Small delay to prevent watchdog reset
  delay(10);
}
