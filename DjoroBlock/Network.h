#ifndef NETWORK_H
#define NETWORK_H
//Network & Config
#define RESET_BUTTON PUSH2 // pin 11
#define NETWORK_LED YELLOW_LED

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


class Network
{
  public:
    int start();
    void serverLoop();
    static void printWifiStatus();
  private:
    static int init();
    static int smartConfig();
};

#endif
