#include "RadioReceiver.h"
#include "DjoroMessage.h"
#include "DjoroTools.h"
#include <VirtualWire.h>

RadioReceiver::RadioReceiver(){
}

void RadioReceiver::init(uint8_t rx_pin, uint8_t tx_pin, uint8_t ptt_pin, uint16_t speed){
  vw_set_rx_pin(rx_pin);
  vw_set_tx_pin(tx_pin);
  vw_set_ptt_pin(ptt_pin);
  vw_setup(speed);
  vw_rx_start();
}

boolean RadioReceiver::getMessage(djoro_message_t *msg) {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen))
  {
      float *p = (float*)&buf;
      msg->temperature = *p; p++;
      msg->humidity = *p; p++;
      uint8_t *c = (uint8_t*)p;
      msg->command = *c;
      djoro_print("temperature : ");
      djoro_println(msg->temperature);
      djoro_print("humidity : ");
      djoro_println(msg->humidity);
      djoro_print("command : ");
      djoro_println(msg->command);

      djoro_print("Number of good messages received: ");
      djoro_println(vw_get_rx_good());

      djoro_print("Number of bad messages received: ");
      djoro_println(vw_get_rx_bad());
      return true;
  } else {
    return false;
  }
}
