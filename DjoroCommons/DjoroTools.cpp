#include "DjoroTools.h"

int floatToString(const float value, char* output) {
    char intPart[10];
    char decPart[10];

    itoa(int(value), intPart, 10);
    int dec = (value - int(value)) * 100;
    itoa(abs(dec), decPart, 10);

    char *p = intPart;
    int output_index = 0;
    while (*p != 0) {
        output[output_index++] = *p;
        p++;
    }
    output[output_index++] = '.';

    p = decPart;
    while (*p != 0) {
        output[output_index++] = *p;
        p++;
    }
    output[output_index++] = '\0';

    return output_index;
}
