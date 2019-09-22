#ifndef DJORO_TOOLS
#define DJORO_TOOLS

#if defined(__CC3200R1M1RGC__)
    #include "Energia.h"
#else
    #include <Arduino.h>
#endif

/* Activate/Deactivate the DEBUG mode.
* Debug mode only have effect about Serial printing.
* If DEBUG is set to 1, Serial.print is activated.
* If DEBUG is set to 0, Serial.print is deactivated.
*/
#define DEBUG 1

#define djoro_print(x) do { if (DEBUG) Serial.print(x); } while(0);
#define djoro_println(x) do { if (DEBUG) Serial.println(x); } while(0);

/*
 * It truncates to 2 decimal numbers
 * If value is 12.345, the returned String will be "12.34"
 */
int floatToString(const float value, char* ouptut);

#endif
