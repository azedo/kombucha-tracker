#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include "config.h"

// WiFi credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// External references
extern ESP8266WebServer server;
// No need to declare handleRoot and handleSet as they're already declared in main.ino

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());
}

void setupServer() {
  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/set", HTTP_POST, handleSet);
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

#endif
