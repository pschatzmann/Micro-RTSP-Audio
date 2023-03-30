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

#pragma once

#include "platglue.h"
#include "IAudioSource.h"
#include <esp_timer.h>
#include <WiFi.h>
#include <esp_wifi.h>


/**
 * @brief Class for streaming data from a source into an RTP stream
 */
class AudioStreamer
{
private:
    const int STREAMING_BUFFER_SIZE = 1024*2;
    unsigned char * RtpBuf = nullptr;

    IAudioSource * m_audioSource = nullptr;
    int m_fragmentSize = 0;          // changed from samples to bytes !
    int m_timer_period = 20000;
    const int HEADER_SIZE = 12;      // size of the RTP header


    UDPSOCKET m_RtpSocket;           // RTP socket for streaming RTP packets to client
    UDPSOCKET m_RtcpSocket;          // RTCP socket for sending/receiving RTCP packages

    IPPORT m_RtpServerPort;      // RTP sender port on server
    IPPORT m_RtcpServerPort;     // RTCP sender port on server

    u_short m_SequenceNumber;
    uint32_t m_Timestamp;
    int m_SendIdx;

    IPADDRESS m_ClientIP;
    IPPORT m_ClientPort;
    uint32_t m_prevMsec;

    int m_udpRefCount;

    esp_timer_handle_t RTP_timer;

public:
    /**
     * Creates a new AudioStreamer object
     */
    AudioStreamer();

    /**
     * Creates a new AudioStreamer object
     * @param source Object implementing the IAudioSource interface, used as a source for the RTP stream
     */
    AudioStreamer(IAudioSource * source);

    /**
     * Deletes allocated memory
     */
    virtual ~AudioStreamer();

    /**
     * Defines the AudioSource object (if it was not assigned in the constructor)
    */
    virtual void setAudioSource(IAudioSource * source);

    /**
     * @brief Sets up the information from the AudioSource
     * 
     * @return true 
     * @return false 
     */

    bool InitAudioSource();

    

    /**
     * Opens sockets for RTP stream
     * @param aClientIP IP address of the RTP client
     * @param aClientPort port of the RTP client
     * @return true on success
     */
    bool InitUdpTransport(IPADDRESS aClientIP, IPPORT aClientPort);

    /**
     * Close sockets used for RTP stream
     */
    void ReleaseUdpTransport(void);

    /**
     * Sends an RTP packet using the audio source given. Audio source copies data right into the RTP packet
     * @return number of samples sent in the packet
     */
    int SendRtpPacketDirect();

    /**
     * Start the RTP stream
     */
    void Start();

    /**
     * Stop the RTP stream
     */
    void Stop();

    u_short GetRtpServerPort();
    u_short GetRtcpServerPort();
    IAudioSource *getAudioSource() {return m_audioSource;}

private:
    /**
     * Task Routine for RTP stream. Carries out the stream
     * @param audioStreamObj instance of AudioStreamer 
     */
    static void doRTPStream(void * audioStreamerObj);

};

