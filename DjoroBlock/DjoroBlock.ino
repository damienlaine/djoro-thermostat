#include <WiFi.h>

#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif

#include <VirtualWire.h>
#include <aJSON.h>
#include <HttpClient.h>
#include <ArduinoJson.h>

#include "DjoroMessage.h"
#include "RadioReceiver.h"
#include "BackendServer.h"
#include "HeatingController.h"
#include "Network.h"

//Radio
#define RADIO_RX_PIN 18
#define RADIO_TX_PIN 29
#define RADIO_PTT_PIN 28
#define RADIO_SPEED 2000
#define RELAY_PIN 19


//Backend server config
//IPAddress server(192,168,0,35);  // numeric IP (no DNS)
char server[] = "dev.thermlabs.com";    // name address (using DNS)
uint16_t port = 80 ;
#define THERMOSTAT_UID "uid"
#define API_KEY "apikey"

#define DEFAULT_TEMP 21.0
#define DEFAULT_HYSTERESIS 0.3

typedef struct {
  float measuredTemp;
  float targetTemp;
  float hysteresisThreshold;
  boolean manual;
} block_t;

RadioReceiver rf;
HeatingController heating(RELAY_PIN);
BackendServer backend(server, port, THERMOSTAT_UID, API_KEY, &heating);
Network network;
block_t block;


void setup()
{
  Serial.begin(9600);
  Serial.println("Setup starting ...");
  Serial.println("Init the network ...");  
  network.start();
  block.measuredTemp = DEFAULT_TEMP;
  block.targetTemp = DEFAULT_TEMP;
  block.hysteresisThreshold = DEFAULT_HYSTERESIS;
  block.manual = false;
  Serial.println("Init the radio receiver ...");  
  rf.init(RADIO_RX_PIN, RADIO_TX_PIN, RADIO_PTT_PIN, RADIO_SPEED);
  Serial.println("Init heating controller ...");
  heating.init();
  Serial.println("Setup done.");
}


unsigned long rf_count = 0;
unsigned long data_count = 0;
unsigned long iter = 0;


void loop()
{
  iter++;
  
  backend_data_t backendData;
  if(backend.checkDataAvailable(&backendData)) {
    data_count++;
    if (!block.manual) {
      block.targetTemp = backendData.targetTemp;
      block.hysteresisThreshold = backendData.hysteresisThreshold;  
    }
    printCounters();
  }
  djoro_message_t msg;
  if (rf.getMessage(&msg)) {
    rf_count++;
    djoro_println("MAIN - Post the measurement to the REST server");
    djoro_println(msg.temperature);
    block.measuredTemp = msg.temperature;
    
    //If block.command is greater than 0, it means that the user has switch to manual mode
    // using the potentiometer on the sensor module. Thus we have to apply the target temp as 
    // set by the potentiometer. Value received is between 0 and 10. 1 means 16°C, 10 means 25°C i.e. Maximum
    if (msg.command > 0) {      
      block.manual = true;
      block.targetTemp = map(msg.command, 1, 10, 16, 25);
      djoro_print("switch to manual mode t°C: ");
      djoro_println(block.targetTemp);
    } else if (msg.command == 0){
      //This means we are in automatic mode
      block.manual = false; 
    }
    backend.postThermostatStatus(block.measuredTemp, block.targetTemp, block.manual);
    printCounters();
  }
  heating.boilerControl(block.measuredTemp, block.targetTemp, block.hysteresisThreshold);
  delay(1000);
}



void printCounters() {
  djoro_println("--------------------------------");  
  djoro_print("ITERATION ");
  djoro_println(iter);
  djoro_print("rf_count : ");
  djoro_println(rf_count);
  djoro_print("data_count = ");
  djoro_println(data_count);
  djoro_println("--------------------------------"); 
}
