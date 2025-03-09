#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

// External references 
extern ESP8266WebServer server;
extern String startDate;
extern int brewDays;

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Kombucha Tracker</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; }";
  html += ".container { max-width: 500px; margin: 0 auto; }";
  html += "h1 { color: #5c8d89; }";
  html += ".info { background-color: #f0f0f0; padding: 15px; border-radius: 5px; margin: 15px 0; }";
  html += "form { margin-top: 20px; }";
  html += "label, input { display: block; margin-bottom: 10px; }";
  html += "input[type='submit'] { background-color: #5c8d89; color: white; border: none; padding: 10px 15px; border-radius: 4px; cursor: pointer; }";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>Kombucha Brewing Tracker</h1>";
  
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
  } else {
    html += "<p>No active batch. Start a new one below.</p>";
  }
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

#endif
