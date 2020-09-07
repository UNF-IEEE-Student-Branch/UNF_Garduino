// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// pin connected to DH22 data line
#define DATA_PIN 23

// create DHT22 instance
DHT_Unified dht(DATA_PIN, DHT22);

// The rest of the sensors
#define solenoid_valve 22
#define soil_moisture A7
#define light_sensor A2

// Threshold values
#define soil_not_moist 500
#define water_time 10000
int clr_inst = 0; // clear the instruction value

// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *light_level = io.feed("light_level");
AdafruitIO_Feed *moisture = io.feed("moisture");
AdafruitIO_Feed *instruction = io.feed("instruction");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // initialize sensors and relays
  dht.begin();
  pinMode(solenoid_valve, OUTPUT);
  digitalWrite(solenoid_valve, LOW);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  
  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  instruction->onMessage(handleMessage);
  
  // wait for a connection
  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  instruction->get();


  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
}

// this function checks if the garden bed needs watering
// and triggers the solenoid valve if so
void water_bed (){
  if(analogRead(soil_moisture)<= soil_not_moist){
    Serial.println("soil is not moist");
    digitalWrite(solenoid_valve, HIGH);
    delay(water_time);
    digitalWrite(solenoid_valve, LOW);
    delay(water_time);
    water_bed();
  }
  else {
    Serial.println("soil is moist");
    instruction->save(clr_inst);
  }
  return;
}

// This function collects data from all the sensors and publishes the 
// information to Adafruit.io
void collect_data(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;
  float fahrenheit = (celsius * 1.8) + 32;

  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.print("fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("F");

  // save fahrenheit (or celsius) to Adafruit IO
  temperature->save(fahrenheit);

  dht.humidity().getEvent(&event);

  Serial.print("humidity: ");
  Serial.print(event.relative_humidity);
  Serial.println("%");

  // save humidity to Adafruit IO
  humidity->save(event.relative_humidity);
 
  light_level->save(analogRead(light_sensor));
  Serial.println(analogRead(light_sensor));
  moisture->save(analogRead(soil_moisture));
  Serial.println(analogRead(soil_moisture));
  
  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(5000);  
  instruction->save(clr_inst);
  return;
}

// this function toggles the solenoid on.
void toggle_water(){
  digitalWrite(solenoid_valve, HIGH);
  delay(water_time);
  digitalWrite(solenoid_valve, LOW);
  instruction->save(clr_inst);
  return;
}
// this function is called whenever a 'instruction' message
// is received from Adafruit IO. it was attached to
// the instruction feed in the setup() function above.

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  int var = data->toInt();
  /*
  if(data->toInt() == 1) toggle_water();
  else if(data->toInt() == 2) collect_data();
  else if(data->toInt() == 3) water_bed();
  */

  switch (var) {
    case 1:  toggle_water(); break;
    case 2:  collect_data(); break;
    case 3:  water_bed(); break; 
    //default: instruction->save(clr_inst); break;
  }
  return;
}
