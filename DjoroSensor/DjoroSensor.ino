#include <VirtualWire.h>
#include <OneWire.h>
#include "DjoroMessage.h"

#define TEMPPIN 7   
#define TXPIN 9
#define POTPIN 0
#define DELAY 100


int getTemperature(float *celsius);
djoro_message_t msg;   
OneWire  ds(TEMPPIN);  // on pin 10 (a 4.7K resistor is necessary)


void setup()
{
    Serial.begin(9600);
    pinMode(POTPIN, INPUT);
    vw_setup(2000);         
    vw_set_tx_pin(TXPIN);
}

float t;
float h = 0.0;
void loop()
{ 
   //The potval is mapped between 0 to 10.
   // Eleven commands from Auto to Max: 
   // Auto, 16, 17, 18, 19, 20, 21, 22, 23, 24, Max
   int val = analogRead(POTPIN);
   Serial.print("pot read: ");
   Serial.println(val);
   uint8_t potVal = map(val, 1024, 0, 0, 10);
   Serial.print("mapped pot value");
   Serial.println(potVal);
   msg.command = potVal;
   // Read temperature as Celsius
   int err = getTemperature(&t);
   if (err == 0){
     Serial.print("Temperature: "); 
     Serial.print(t);
     Serial.println(" *C ");

     h += 1.0;    
     Serial.print("Humidity: "); 
     Serial.print(h);

    
     msg.temperature = t;
     msg.humidity = h;
     
     vw_send((uint8_t*)&msg, sizeof(msg));
     vw_wait_tx();   // On attend que le message complet soit envoyé.
     Serial.println("Message Sent");
     Serial.print("Message size : ");
     Serial.println(sizeof(msg));
     
   }
   delay(DELAY);
}

int checkAndResetDigitalSensor() {

}

/**
 * return 0 on success. In that case, the celsius parameter is assigned the measured temp in °C
 * return -1 on failure.
 */
int getTemperature(float *celsius){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12]; 
  byte addr[8];
  float fahrenheit;
  
  //Check if digital 
  if (!ds.search(addr)) {
      Serial.println("No more addresses.");
      Serial.println();
      ds.reset_search();
      delay(250);
      return -1;
   }  

  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return -1;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  *celsius = (float)raw / 16.0;
  fahrenheit = *celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(*celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  return 0;
}

