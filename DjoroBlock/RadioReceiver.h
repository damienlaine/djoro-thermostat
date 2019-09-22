#ifndef RADIO_RECEIVER_H
#define RADIO_RECEIVER_H

#include "DjoroMessage.h"

class RadioReceiver
{
  public:
    RadioReceiver();
    void init(uint8_t rx_pin, uint8_t tx_pin, uint8_t ptt_pin, uint16_t speed);
    /* Get a new message received by Radio
     * put the new message in the msg paramaters (this is an OUTPUT)
     * return true if a message was received
     *        false otherwise
     */
    boolean getMessage(djoro_message_t *msg);
};

#endif
