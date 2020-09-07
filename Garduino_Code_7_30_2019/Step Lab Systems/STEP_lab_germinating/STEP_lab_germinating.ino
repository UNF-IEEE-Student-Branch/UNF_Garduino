
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
/*
 * STEP Lab Germinating Station REV 1
 * ----------------------------------
 * This sketch controls 2 lights and 2 heated beds.
 * This system is used to germinate plants at the 
 * UNF STEP lab
 */

 // Define all pins used to toggle relays
 #define LIGHT1 D0
 #define LIGHT2 D1
 #define BED1 D2 
 #define BED2 D3

// Set up the feeds that will toggle the relays

AdafruitIO_Feed *instruction = io.feed("stepins");
//AdafruitIO_Feed *bed2 = io.feed("bed2");
//AdafruitIO_Feed *light1 = io.feed("light1");
//AdafruitIO_Feed *light2 = io.feed("light2");
 
void setup() {
  // initialize relays
  pinMode(BED1, OUTPUT);
  pinMode(BED2, OUTPUT);
  pinMode(LIGHT1, OUTPUT);
  pinMode(LIGHT2, OUTPUT);
    
    // start the serial connection
  Serial.begin(115200);
  // wait for serial monitor to open
  while(! Serial);

  // Do all the internet connecting stuff
  Serial.print("Connecting to Adafruit IO");

  // start MQTT connection to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  instruction->onMessage(handleMessage);

  // wait for an MQTT connection
  // NOTE: when blending the HTTP and MQTT API, always use the mqttStatus
  // method to check on MQTT connection status specifically
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

// this function is called whenever a 'instruction' message
// is received from Adafruit IO. it was attached to
// the instruction feed in the setup() function above.

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  int var = data->toInt();

// This switch statement has a case to deal with both 
// potential states for all four relays (8 cases).
// This way, the relays can individually be toggled 
// on or off.This may not be the most efficient way 
// to do it, but whatever.

  switch (var) {
    // 1 and 2 handle LIGHT1
    case 1:  digitalWrite(LIGHT1, HIGH); break;
    case 2:  digitalWrite(LIGHT1, LOW); break;
    
    // 1 and 2 handle LIGHT2
    case 3:  digitalWrite(LIGHT2, HIGH); break; 
    case 4:  digitalWrite(LIGHT2, LOW); break;
    
    // 1 and 2 handle BED1
    case 5:   digitalWrite(BED1, HIGH); break;
    case 6:   digitalWrite(BED1, LOW);break;

    // 1 and 2 handle BED2
    case 7:   digitalWrite(BED2, HIGH); break; 
    case 8:   digitalWrite(BED2, LOW);break;
    
    // Since all outcomes are accounted for, we don't
    // reeeaallyyy need a default state
    //default: instruction->save(clr_inst); break;
  }
  return;
}
