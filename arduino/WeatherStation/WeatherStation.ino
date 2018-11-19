#include "DHT.h"
#include <Bridge.h>
#include <Process.h>

#define DHTPIN A0
#define DHTTYPE DHT22   // DHT 22  (AM2302), TempHum Pro
#define DELAY 10000
DHT dht(DHTPIN, DHTTYPE);

void printTemperature(float temp){
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" C");
    Serial.println("");
}

void printHumidity(float hum){
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print("%");
    Serial.println("");
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  Bridge.begin();
}

char cmd_buffer[100];

void loop() {
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(temp)){
      Serial.println("Failed to read temperature!");
  }
  else if(isnan(hum)){
      Serial.println("Failed to read humidity!");
  }
  else {
      printTemperature(temp);
      printHumidity(hum);
      sprintf(cmd_buffer, "python sendMeasurements.py --temp %d --hum %d", (int)temp, (int)hum);
      Process p;
      Serial.println("Sending sensor data to UDP server...");
      p.runShellCommand(cmd_buffer);
      // See if the process ran succesfully
      Serial.println("Script output: ");
      while(p.available()){
          char c = p.read();
          Serial.print(c);
      }
  }
  delay(DELAY);
}
