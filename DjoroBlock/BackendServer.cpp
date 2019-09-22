#include <HttpClient.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include "BackendServer.h"
#include "DjoroTools.h"
#include "Energia.h"

//#define RBIN "1242j831"

BackendServer::BackendServer(IPAddress serverIP, uint16_t port, char *thermostatUID, char *apiKey, HeatingController *heating) {
  _serverIP = serverIP;
  _port = port;
  _thermostatUID = thermostatUID;
  _apiKey = apiKey;
  _heatingController = heating;
  _byIP = true;
  
}

BackendServer::BackendServer(char* serverName, uint16_t port, char *thermostatUID, char *apiKey, HeatingController *heating) {  
  _serverName = serverName;
  _port = port;
  _thermostatUID = thermostatUID;
  _apiKey = apiKey;
  _heatingController = heating;
  _byIP = false;
}

boolean BackendServer::checkDataAvailable(backend_data_t *data) {  
  HttpClient http(_client);
  char body[MAX_BODY_LENGTH] = {0};
    
  if (http.available()) {
    int err = http.skipResponseHeaders();
    if (err >= 0) {
      djoro_println("Body returned follows:");
      uint16_t index = 0;
      while (http.available()) {        
        char c = http.read();
        
        if (index >= MAX_BODY_LENGTH) {
          //TODO should implement error code to indicate to the caller what happened here
          return false;
        }
        body[index++] = c;
      }
      //To end the string, we add 0
      body[index] = '\0';
      djoro_println("----- begin body -----");
      djoro_println(body);
      djoro_println("----- end body -----");
      boolean result = parseServerPayload(body, data);
      if (result) {
        djoro_print("TARGET_TEMP: ");
        djoro_println(data->targetTemp);
        djoro_print("HYSTERESIS: ");
        djoro_println(data->hysteresisThreshold);
      } else {
         djoro_println("ERROR"); 
      }
      return result;
    }
  }

  return false;
}

void BackendServer::postThermostatStatus(float measuredTemp, float targetTemp, boolean manual) {
  char body[MAX_BODY_LENGTH] = {0};
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  _client.stop();

  // if there's a successful connection:
  int connection = 0;
  if (_byIP) {
    connection = _client.connect(_serverIP, _port);
  } else {
    connection = _client.connect(_serverName, _port);
  }
  
  if (connection) {
    djoro_println("connected !");
    djoro_println(measuredTemp);
    
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["uid"] = _thermostatUID;
    root["apikey"] = _apiKey;
    root["temperature"] = measuredTemp;
    root["boilerOn"] = _heatingController->isBoilerOn();
    root["tcons"] = targetTemp;
    root["manual"] = manual;
    
    root.printTo(body, sizeof(body));
    
    int size = strlen(body);
    if (size > 0 ) {
      djoro_print("size of body to send = ");
      djoro_println(size);
      djoro_print("body :");
      djoro_println(body);
#ifndef RBIN
      // send the HTTP POST request:
      _client.println("POST /device/status/ HTTP/1.1");
      _client.println("Host: djoro.thermlabs.com");
      _client.println("Content-Type: application/json");
      _client.println("Accept: */*");
      _client.print("Content-Length: ");
      _client.println(size);
      _client.println("Connection: close");       
      _client.println("");
      _client.println(body);
      _client.println("");
#else
      _client.println("GET /1242j831 HTTP/1.1");
      _client.println("Host: requestb.in:80");
      //_client.println("Content-Type: application/json");
      _client.print("Content-Length: ");
      _client.println(size);
      _client.println("Connection: close");    
      //_client.println("");
      //_client.println(body);
      _client.println("");
#endif    
      // note the time that the connection was made:
      _lastConnectionTime = millis(); 
    }
  }
  else {
    // if you couldn't make a connection:
    djoro_println("connection failed");
  }
}

//{"target_temp":17.0,"coef":0.3}
boolean BackendServer::parseServerPayload(char *payload, backend_data_t *data) {

  //
  // Step 1: Reserve memory space
  //
  StaticJsonBuffer<100> jsonBuffer;
  
  //
  // Step 2: Deserialize the JSON string
  //
  JsonObject& root = jsonBuffer.parseObject(payload);
  
  if (!root.success())
  {
    djoro_println("parseObject() failed");
    return false;
  }
  
  //
  // Step 3: Retrieve the values
  //
  data->targetTemp = root["target_temp"];
  data->hysteresisThreshold = root["coef"];

  return true;
}
