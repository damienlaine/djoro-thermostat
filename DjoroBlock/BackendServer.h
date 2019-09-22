#ifndef BackendServer_h
#define BackendServer_h

#include <WiFi.h>
#include <WiFiClient.h>
#include <HttpClient.h>
#include <inttypes.h>
#include <aJSON.h>

#include "DjoroTools.h"
#include "HeatingController.h"


#define MAX_BODY_LENGTH 1024

typedef struct {
  float targetTemp;
  float hysteresisThreshold;
} backend_data_t;

class BackendServer
{
  public:
    BackendServer(IPAddress serverIP, uint16_t port, char *thermostatUID, char *apiKey, HeatingController *heating);
    BackendServer(char* server, uint16_t port, char *thermostatUID, char *apiKey, HeatingController *heating);
    
    void postThermostatStatus(float measuredTemp, float targetTemp, boolean manual);
    
    /* Check if data are available from REST server.
     * If data are available, function returns 'true' and set the data parameters fields accordingly
     * If data are not available, function returns 'false' and state of data parameters is undefined might have been modified or not.
     */
    boolean checkDataAvailable(backend_data_t *data);
    
  private:
    WiFiClient _client;    

    /* Boolean used to know whether the configuration was done through IP Adress or DNS name
     * If set to false, the _serverName variable will be used to make the connection
     * If set to true, the _serverIP will be used instead
     * This boolean is configured according to the constructor used
     */     
    boolean _byIP;
    
    IPAddress _serverIP;
    char *_serverName;
    uint16_t _port;
    char *_thermostatUID;
    char *_apiKey;
    HeatingController *_heatingController;
    unsigned long _lastConnectionTime;
    boolean parseServerPayload(char *payload, backend_data_t *data);
};

#endif
