#include <Arduino.h>

#include "RTSPServer.h"
#include "AudioStreamer.h"
#include "AudioTestSource.h"
#include <WiFi.h>


const char* ssid = "ssid";
const char* password = "password";

int port = 554;
AudioTestSource testSource = AudioTestSource();
AudioStreamer streamer = AudioStreamer(&testSource);
RTSPServer rtsp(&streamer, port);


bool connectToWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }
  Serial.println(WiFi.localIP());
  return true;
}


void setup() {
    Serial.begin(114200);
    
    connectToWiFi(ssid, password);
    rtsp.runAsync();
    
    log_i("Set up done");
}

void loop() {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    // log_i("Free heap size: %i KB", esp_get_free_heap_size() / 1000);
    // log_i("%s: Stack high watermark: %i KB",     
    //     pcTaskGetTaskName(NULL),     
    //     uxTaskGetStackHighWaterMark(NULL) / 1000     
    // );
    // log_i("%s: Stack high watermark: %i KB",     
    //     pcTaskGetTaskName(NULL),     
    //     uxTaskGetStackHighWaterMark(NULL) / 1000     
    // );
}