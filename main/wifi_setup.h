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
  #if USE_STATIC_IP
    // Configure static IP
    IPAddress staticIP(STATIC_IP);
    IPAddress gateway(GATEWAY);
    IPAddress subnet(SUBNET);
    IPAddress dns1(PRIMARY_DNS);
    IPAddress dns2(SECONDARY_DNS);
    
    // Apply the static IP configuration
    WiFi.config(staticIP, gateway, subnet, dns1, dns2);
    Serial.println("Using static IP: " + staticIP.toString());
  #endif
  
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
