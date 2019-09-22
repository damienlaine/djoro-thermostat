#include "HeatingController.h"
#include "Energia.h"
#include "DjoroTools.h"

HeatingController::HeatingController(uint8_t relayPin){
  _relayPin = relayPin;
  _boilerOn = false;
}

void HeatingController::init(void){
  pinMode(_relayPin, OUTPUT);  
}

void HeatingController::boilerControl(float measuredTemp, float targetTemp, float hysteresisThreshold){
  if (measuredTemp < targetTemp - hysteresisThreshold) {
    djoro_print("Boiler on: tmes = "); djoro_print(measuredTemp); djoro_print(", tcons = "); djoro_println(targetTemp);
    digitalWrite(_relayPin, HIGH);
    _boilerOn = 1;
  } else if (measuredTemp > targetTemp + hysteresisThreshold) {
     djoro_print("Boiler off: tmes = "); djoro_print(measuredTemp); djoro_print(", tcons = "); djoro_println(targetTemp);
     digitalWrite(_relayPin, LOW);
     _boilerOn = 0;
  } 
}

boolean HeatingController::isBoilerOn(void) {
  if (_boilerOn == 0) {
    return false;
  } else {
    return true;
  }
}
