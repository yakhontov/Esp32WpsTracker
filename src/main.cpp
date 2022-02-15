#include <Arduino.h>
#include "WiFi.h"
#include "SSD1306.h" // legacy: #include "SSD1306.h"

SSD1306Wire display(0x3c, SDA, SCL); // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup Wifi");
  pinMode(2, OUTPUT);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup display");
  display.init();
  display.flipScreenVertically();

  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 20, "WPS");
  display.display();

  Serial.println("Setup done");

  delay(2000);
}

void printNet(int i)
{
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, i * 21, WiFi.SSID(i));
  display.drawString(0, i * 21 + 10, WiFi.BSSIDstr(i));
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, i * 21 + 10, String(WiFi.RSSI(i)));
}

void loop()
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  display.clear();
  if (n)
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      printNet(i);
      Serial.print(i + 1);
      Serial.print(": SSID=");
      Serial.print(WiFi.SSID(i));
      Serial.print(", RSSI=");
      Serial.print(WiFi.RSSI(i));
      Serial.print(", BSSID=");
      Serial.print(WiFi.BSSIDstr(i));
      Serial.print(", ENCR=");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "NO" : "YES");
      delay(10);
    }
  }
  else // No neworks
  {
    Serial.println("No networks found");
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 20, "WI-FI NOT");
    display.drawString(64, 36, "FOUND");
  }
  display.display();
  Serial.println("");
  digitalWrite(2, HIGH);
  delay(100);
  digitalWrite(2, LOW);
  delay(1000);
}