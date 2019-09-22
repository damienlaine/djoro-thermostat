#ifndef HEATING_CONTROLLER_H
#define HEATING_CONTROLLER_H

#include <inttypes.h>
#include "Energia.h"

class HeatingController
{
public:
  HeatingController(uint8_t relayPin);
  void init(void);
  void boilerControl(float measuredTemp, float targetTemp, float hysteresisThreshold);
  boolean isBoilerOn(void);

private:
  uint8_t _relayPin;
  uint8_t _boilerOn;
};

#endif

