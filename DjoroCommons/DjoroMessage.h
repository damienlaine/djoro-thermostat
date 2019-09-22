#ifndef DJORO_MESSAGE_H
#define DJORO_MESSAGE_H

#include <Arduino.h>

typedef struct {
    float temperature;
    float humidity;        
    uint16_t command;
    uint16_t battery;
} djoro_message_t;

#endif
