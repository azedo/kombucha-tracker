#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include "config.h"

// External references
extern ESP8266WebServer server;
extern String startDate;
extern int brewDays;
extern float temperature;
extern float humidity;
extern int lightLevel;

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Kombucha Tracker</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; }";
  html += ".container { max-width: 500px; margin: 0 auto; }";
  html += "h1 { color: #5c8d89; }";
  html += ".info { background-color: #f0f0f0; padding: 15px; border-radius: 5px; margin: 15px 0; }";
  html += "form { margin-top: 20px; }";
  html += "label, input { display: block; margin-bottom: 10px; }";
  html += "input[type='submit'] { background-color: #5c8d89; color: white; border: none; padding: 10px 15px; border-radius: 4px; cursor: pointer; }";
  html += ".environment {background-color: #e8f5e9;padding: 15px;border-radius: 5px;margin: 15px 0;}";
  html += ".environment h2 {margin-top: 0;color: #2e7d32;}";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>Kombucha Brewing Tracker</h1>";

  html += "<p><small>Device IP: " + WiFi.localIP().toString() + "</small></p>";

  // Add temperature and humidity information
  html += "<div class='environment'>";
  html += "<h2>Environment</h2>";
  html += "<p>Temperature: <strong>" + String(temperature, 1) + "°C</strong>";

  // Add temperature status indicator with color coding
  if (temperature < OPTIMAL_TEMP_MIN) {
    html += " <span style='color: blue;'>&#10052; Too cold</span>";
  } else if (temperature > OPTIMAL_TEMP_MAX) {
    html += " <span style='color: orange;'>&#9728; Too warm</span>";
  } else {
    html += " <span style='color: green;'>&#10004; Optimal</span>";
  }
  html += "</p>";

  // Add temperature explanation
  html += "<p><small>Optimal kombucha brewing temperature: " + String(OPTIMAL_TEMP_MIN) + "-" + String(OPTIMAL_TEMP_MAX) + "°C. Outside this range, fermentation may be too slow or too fast.</small></p>";

  html += "<p>Humidity: <strong>" + String(humidity, 1) + "%</strong></p>";

  // Add light level information
  html += "<p>Light Level: <strong>" + String(lightLevel) + "%</strong> ";
  if (lightLevel < LIGHT_LOW_THRESHOLD) {
    html += "<span style='color: green;'>&#10004; Good for kombucha</span>";
  } else if (lightLevel < LIGHT_HIGH_THRESHOLD) {
    html += "<span style='color: orange;'>&#9888; Moderate - consider reducing</span>";
  } else {
    html += "<span style='color: red;'>&#9888; Too bright - protect your brew</span>";
  }
  html += "</p>";

  // Add a light level meter
  html += "<div class='light-meter' style='background: linear-gradient(to right, #003366, #ffcc00, #ffffff); height: 15px; border-radius: 7px; position: relative;'>";
  html += "<div style='position: absolute; left: " + String(lightLevel) + "%; top: -7px; transform: translateX(-50%);'>▼</div>";
  html += "</div>";

  // Add light explanation
  html += "<p><small>Kombucha ferments best in low to moderate light. Direct sunlight can harm the SCOBY.</small></p>";

  html += "</div>";

  // Display current brewing information
  html += "<div class='info'>";
  html += "<p><small>System time: " + getCurrentTimeString() + "</small></p>";
  if (startDate != "") {
    // Calculate days elapsed and remaining
    int daysPassed = getDaysPassed(startDate);
    int daysRemaining = brewDays - daysPassed;

    html += "<p>Batch started on: " + startDate + "</p>";
    html += "<p>Days brewing: " + String(daysPassed) + "</p>";

    if (daysRemaining > 0) {
      html += "<p>Ready in approximately " + String(daysRemaining) + " days</p>";
      html += "<p>Expected ready date: " + getReadyDate(startDate, brewDays) + "</p>";
    } else {
      html += "<p><strong>Your kombucha is ready!</strong> (Ready since " + String(-daysRemaining) + " days)</p>";
    }

    // Add cancel button when there's an active batch
    html += "<form action='/cancel' method='post' style='margin-top: 15px;'>";
    html += "<input type='submit' value='Cancel This Batch' style='background-color: #f44336; padding: 8px 12px;'>";
    html += "</form>";
  } else {
    html += "<p>No active batch. Start a new one below.</p>";
  }
  html += "</div>";


  // Determine status for display
  String statusText = "NO ACTIVE BATCH";
  String statusColor = "red";
  int progressPercent = 0;

  if (startDate != "") {
    int daysPassed = getDaysPassed(startDate);
    progressPercent = (daysPassed * 100) / brewDays;
    progressPercent = min(progressPercent, 100);  // Cap at 100%

    if (daysPassed >= brewDays) {
      statusText = "READY TO ENJOY!";
      statusColor = "green";
    } else {
      statusText = "BREWING IN PROGRESS";
      statusColor = "orange";  // for yellow
    }
  }

  html += "<p>Status: <span style='color: " + statusColor + "; font-weight: bold;'>" + statusText + "</span></p>";

  // Only show progress bar if there's an active batch
  if (startDate != "") {
    html += "<div class='progress-bar' style='background-color: #eee; height: 20px; border-radius: 10px; margin: 10px 0;'>";
    html += "<div style='background-color: " + statusColor + "; width: " + String(progressPercent) + "%; height: 100%; border-radius: 10px;'></div>";
    html += "</div>";
  }

  // Add a simple color legend
  html += "<div class='led-legend' style='margin-top: 15px;'>";
  html += "<h3>LED Color Guide:</h3>";
  html += "<ul>";
  html += "<li><span style='color: red;'>●</span> <strong>Red:</strong> No active batch</li>";
  html += "<li><span style='color: green;'>●</span> <strong>Green:</strong> Ready to enjoy!</li>";
  html += "</ul>";
  html += "</div>";

  // Form for setting new brewing batch
  html += "<form action='/set' method='post'>";
  html += "<label for='startDate'>Start Date (YYYY-MM-DD):</label>";
  html += "<input type='date' id='startDate' name='startDate' value='" + startDate + "'>";
  html += "<label for='brewDays'>Brewing Duration (days):</label>";
  html += "<input type='number' id='brewDays' name='brewDays' value='" + String(brewDays) + "' min='1' max='30'>";
  html += "<input type='submit' value='Set Brewing Time'>";
  html += "</form>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void handleSet() {
  if (server.hasArg("startDate") && server.hasArg("brewDays")) {
    startDate = server.arg("startDate");
    brewDays = server.arg("brewDays").toInt();

    // Save data to file system
    saveBrewingData();

    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleCancel() {
  // Clear the batch data
  startDate = "";

  // Save the updated (empty) data
  saveBrewingData();

  // Redirect back to the main page
  server.sendHeader("Location", "/");
  server.send(303);
}

#endif
