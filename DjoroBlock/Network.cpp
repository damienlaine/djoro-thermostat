#include "Network.h"
#include "DjoroTools.h"

static WiFiServer webserver(80);

int Network::start(){
  pinMode(RESET_BUTTON, INPUT);
  pinMode(NETWORK_LED, OUTPUT);
  this->init();
}

//-- Starts Ti's smartConfig. Blocks until _connected --//
int Network::smartConfig(){
  WiFi.clearProfiles();
  return WiFi.startSmartConfig();
}

//-- Called on start --//
int Network::init(){
  //On start, if reset button is held down, switches to smart config
  digitalWrite(NETWORK_LED, HIGH); //default state
  if (digitalRead(RESET_BUTTON) == HIGH)
  {
    djoro_println("Booting in SmartConfig mode...");
    Network::smartConfig(); //!\\ BLOCKING untill smartconfig is done!
    djoro_println("SmartConfig OK...");
  } 
  else {
    djoro_println("Connecting to Wifi network...");
    WiFi.begin();
  }
  //Waiting Connection
  //SLOW BLINK
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(NETWORK_LED, LOW);
    // print dots while we wait for an ip addresss
    djoro_print(".");
    delay(500);
    digitalWrite(NETWORK_LED, HIGH);
    delay(500);
  }

  //Waiting for DHCP
  //FAST BLINK
  while (WiFi.localIP() == INADDR_NONE) {
    digitalWrite(NETWORK_LED, LOW);
    // print dots while we wait for an ip addresss
    djoro_print(".");
    delay(250);
    digitalWrite(NETWORK_LED, HIGH);
    delay(250);
  }
  // DHCP ok, Steady Networked
  Network::printWifiStatus();
  digitalWrite(NETWORK_LED, LOW);
}


void Network::printWifiStatus(){
  // print the SSID of the network you're attached to:
  djoro_print("SSID: ");
  djoro_println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  djoro_print("IP Address: ");
  djoro_println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  djoro_print("signal strength (RSSI):");
  djoro_print(rssi);
  djoro_println(" dBm"); 
}

void Network::serverLoop(){

}




