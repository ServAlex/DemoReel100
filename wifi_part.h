#ifndef wifi_part_h
#define wifi_part_h

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
// create this file with 2 const char* variables: ssid and password
#include "networkCredentials.h"

String ip = "0.0.0.0";
WebServer server(80);

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: " + server.uri();
  message += "\nMethod: " + (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: " + server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void wifiSetup(void)
{
  WiFi.mode(WIFI_STA);

  int current = -1;
  int atemptsRemaining = ssids_count*6;

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED && atemptsRemaining-->0) {
    current = (current+1)%ssids_count;
    WiFi.begin(ssids[current], passwords[current]);
    Serial.print("\nAttempting ");
    Serial.println(ssids[current]);
    delay(2000);
  }

  Serial.print("\nConnected to ");
  Serial.println(ssids[current]);
  Serial.print("IP address: ");
  ip = WiFi.localIP().toString();
  Serial.println(ip);

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](){
    //String message = "endpoints: [temp, humid, lux]\n";
    String message = "<html><body>";
    message += "<a href=\"http://192.168.0.248/all\">All</a><br>";
    message += "<a href=\"http://192.168.0.248/temp\">Temperature</a><br>";
    message += "<a href=\"http://192.168.0.248/humid\">Humidity</a><br>";
    message += "<a href=\"http://192.168.0.248/lux\">Lux</a>";
    message += "</body></html>";
    server.send(200, "text/html", message);
  });

  server.on("/all", [](){
    server.send(200, "text/plain", "Temperature: \t"+String(temperature)+"\nHumidity: \t"+String(humidity)+"\nLux: \t\t"+String(lux));
  });
  server.on("/temp", [](){
    server.send(200, "text/plain", String(temperature));
  });
  server.on("/humid", [](){
    server.send(200, "text/plain", String(humidity));
  });
  server.on("/lux", [](){
    server.send(200, "text/plain", String(lux));
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void wifiLoop(void) 
{
  if(WiFi.status() == WL_CONNECTED)
    server.handleClient();
}

#endif