/*
 * Author: Thomas Pfitzinger
 * github: https://github.com/Tomp0801/Micro-RTSP-Audio
 *
 * Based on Micro-RTSP library for video streaming by Kevin Hester:
 * 
 * https://github.com/geeksville/Micro-RTSP
 * 
 * Copyright 2018 S. Kevin Hester-Chow, kevinh@geeksville.com (MIT License)
 */

#include "RTSPServer.h"
#include "RTSPSession.h"
#ifdef ESP32
#include <esp_wifi.h>
#endif

RTSPServer::RTSPServer(AudioStreamer * streamer, int port, int core) {
    this->streamer = streamer;
    this->port = port;
    this->core = core;
}

int RTSPServer::begin(const char* ssid, const char* password){
  // Start Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

#ifdef ESP32
  esp_wifi_set_ps(WIFI_PS_NONE);
#endif

  Serial.println();
  Serial.print("connect to rtsp://");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(port);
  return runAsync();
}


int RTSPServer::runAsync() {
    int error;

    log_i("Running RTSP server on port %d",port);

    streamer->InitAudioSource();

    ServerAddr.sin_family      = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port        = htons(port);                 // listen on RTSP port 8554 as default
    int s = socket(AF_INET,SOCK_STREAM,0);
    log_d("Master socket fd: %i", s);
    MasterSocket               = new WiFiClient(s);
    if (MasterSocket == NULL) {
        log_e("MasterSocket object couldnt be created");
        return -1;
    }

    log_d("Master Socket created; fd: %i", MasterSocket->fd());

    int enable = 1;
    error = setsockopt(MasterSocket->fd(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (error < 0) {
        log_e("setsockopt(SO_REUSEADDR) failed");
        return error;
    }

    log_v("Socket options set");

    // bind our master socket to the RTSP port and listen for a client connection
    error = bind(MasterSocket->fd(),(sockaddr*)&ServerAddr,sizeof(ServerAddr));
    if (error != 0) {
        log_e("error can't bind port errno=%d", errno);
        return error;
    }
    log_v("Socket bound. Starting to listen");
    error = listen(MasterSocket->fd(),5);
    if (error != 0) {
        log_e("Error while listening");
        return error;
    }

    if (xTaskCreatePinnedToCore(RTSPServer::serverThread, "RTSPServerThread", 10000, (void*)this, 5, &serverTaskHandle, core) != pdPASS) {
        log_e("Couldn't create server thread");
        return -1;
    } 

    return 0;
}

void RTSPServer::serverThread(void* server_obj) {
    socklen_t ClientAddrLen = sizeof(ClientAddr);
    RTSPServer * server = (RTSPServer*) server_obj;
    TickType_t prevWakeTime = xTaskGetTickCount();
   
    log_i("Server thread listening...");

    while (true)
    {   // loop forever to accept client connections
        // only allow one client at a time
        
        if (server->numClients == 0) {
            server->ClientSocket = new WiFiClient(accept(server->MasterSocket->fd(),(struct sockaddr*)&server->ClientAddr,&ClientAddrLen));
            log_i("Client connected. Client address: %s",inet_ntoa(server->ClientAddr.sin_addr));
            if (xTaskCreatePinnedToCore(RTSPServer::sessionThread, "RTSPSessionTask", 8000, (void*)server, 8, &server->sessionTaskHandle, server->core) != pdPASS) {
                log_e("Couldn't create sessionThread");
            } else {
                log_d("Created sessionThread");
                server->numClients++;
            }
        }

        vTaskDelayUntil(&prevWakeTime, 200/portTICK_PERIOD_MS);
    }

    // should never be reached
    closesocket(server->MasterSocket);

    log_e("Error: %s is returning", pcTaskGetTaskName(NULL));
}


void RTSPServer::sessionThread(void * server_obj) {
    RTSPServer * server = (RTSPServer*)server_obj;
    AudioStreamer * streamer = server->streamer;
    SOCKET s = server->ClientSocket;
    TickType_t prevWakeTime = xTaskGetTickCount();

    // stop this task - wait for a client to connect
    //vTaskSuspend(NULL);
    // TODO check if everything is ok to go
    log_v("RTSP Task running");

    RtspSession * rtsp = new RtspSession(*s, streamer);     // our threads RTSP session and state

    log_i("Session ready");

    while (rtsp->m_sessionOpen)
    {
        uint32_t timeout = 30;
        if(!rtsp->handleRequests(timeout)) {
            log_v("Request handling timed out");

        } else {
            log_v("Request handling successful");
        }

        vTaskDelayUntil(&prevWakeTime, timeout/portTICK_PERIOD_MS);
    }

    
    // should never be reached
    log_i("sessionThread stopped, deleting task");
    delete rtsp;
    server->numClients--;

    vTaskDelete(NULL);
}