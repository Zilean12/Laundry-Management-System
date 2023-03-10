#include <SPI.h>

#include <RF24.h>

#include <BTLE.h>

#include <DHT.h>                                                            // dht11 temperature and humidity sensor library


#define DHTPIN 4                                                           // what digital pin we're connected to

#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22

DHT dht(DHTPIN, DHTTYPE);


RF24 radio(9, 10); // CE, CSN

BTLE btle(&radio);


void setup() {

  Serial.begin(9600);

  delay(1000);

  Serial.print("BLE and DHT Starting... ");

  Serial.println("Send Temperature Data over BTLE");

  dht.begin();   // initialise DHT11 sensor

  btle.begin("CD Temp");    // 8 chars max

  Serial.println("Successfully Started");

}


void loop() {                                              

  float temp = dht.readTemperature(); 
  float h,t;  //read temperature data

  if (isnan(h) || isnan(t)) {                                                // Check if any reads failed and exit early (to try again).

    Serial.println(F("Failed to read from DHT sensor!"));

    return;

  }

  Serial.print(" Temperature: ");  Serial.print(t);  Serial.println("Â°C ");

  nrf_service_data buf;

  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;

  buf.value = BTLE::to_nRF_Float(temp);


  if (!btle.advertise(0x16, &buf, sizeof(buf))) {

    Serial.println("BTLE advertisement failed..!");

  }

  btle.hopChannel(); 

  delay(2000);

}