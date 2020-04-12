#include <Arduino.h>
#include "Adafruit_CCS811.h"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "ClosedCube_HDC1080.h"
#include "DHT.h"
#include "math.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
ClosedCube_HDC1080 hdc1080;
Adafruit_CCS811 ccs;

char auth[] = "4hxZfSD1j35wmMsOYS66Ze7wEkD3GwwX";

const char ssid[] = "Psin(a)";
const char pass[] = "Hey!28R0om!";

//const char ssid[] = "24";
//const char pass[] = "zxcvbnmlkjhgf";
void setup() {
  Serial.begin(9600);
  Serial.println("CCS811 test");
  Blynk.begin(auth, ssid, pass);

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  	// Default settings: 
	//  - Heater off
	//  - 14 bit Temperature and Humidity Measurement Resolutions
	hdc1080.begin(0x40);
  pinMode(DHTPIN, INPUT);
  dht.begin();

  // Wait for the sensor to be ready
  while(!ccs.available());
}
double CO2 = 0;
double temp = 0;
double t = 0;
double h = 0;
double hum = 0;

double nf(double num){
  double out = roundf(num * 10) / 10;
  return out;
}

void loop() {
  if(ccs.available()){
    if(!ccs.readData()){

      CO2 = ccs.geteCO2();
      temp = hdc1080.readTemperature();
      hum = hdc1080.readHumidity();

      h = dht.readHumidity();
      t = dht.readTemperature();

      temp = nf(temp);
      t = nf(t);
      hum = nf(hum);
      CO2 = nf(CO2);
      h = nf(h);
  
      Blynk.run();
      Blynk.virtualWrite(V0, hum);
      Blynk.virtualWrite(V1, CO2);
      Blynk.virtualWrite(V2, temp);
      Blynk.virtualWrite(V3, h);
      Blynk.virtualWrite(V4, t);
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(500);
}