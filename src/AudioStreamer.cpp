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

#include "AudioStreamer.h"
#include <stdio.h>


AudioStreamer::AudioStreamer()
{
    log_v("Creating Audio streamer");
    m_RtpServerPort  = 0;
    m_RtcpServerPort = 0;

    m_SequenceNumber = 0;
    m_Timestamp      = 0;
    m_SendIdx        = 0;

    m_RtpSocket = NULLSOCKET;
    m_RtcpSocket = NULLSOCKET;

    m_prevMsec = 0;

    m_udpRefCount = 0;

    RtpBuf = new uint8_t[STREAMING_BUFFER_SIZE+1];

    esp_timer_create_args_t timer_args;
    timer_args.callback = AudioStreamer::doRTPStream;
    timer_args.name = "RTP_timer";
    timer_args.arg = (void*)this;
    timer_args.dispatch_method = ESP_TIMER_TASK;

    esp_timer_init();
    esp_timer_create(&timer_args, &RTP_timer);
    
}

AudioStreamer::AudioStreamer(IAudioSource * source) : AudioStreamer() {
    m_audioSource = source;
    InitAudioSource();
    log_i("Audio streamer created.  Fragment size: %i bytes", m_fragmentSize);
}

AudioStreamer::~AudioStreamer()
{
    delete[] RtpBuf;
    RtpBuf = nullptr;
}



int AudioStreamer::SendRtpPacketDirect() {
    // check buffer
    if (RtpBuf==nullptr){
        log_e("RtpBuf is nullptr");
        return -1;        
    }

    // append data to header
    if (m_audioSource == nullptr) {
        log_e("No audio source provided");
        return -1;
    }

    //unsigned char * dataBuf = &RtpBuf[m_fragmentSize];
    if (m_fragmentSize + HEADER_SIZE >= STREAMING_BUFFER_SIZE){
        log_e("STREAMIN_BUFFER_SIZE too small for the sampling rate: increase to %d",m_fragmentSize+HEADER_SIZE);
        return -1;
    }

    memset(RtpBuf,0, STREAMING_BUFFER_SIZE);

    // Prepare the 12 byte RTP header TODO this can be optimized, some is static
    RtpBuf[0]  = 0x80;                               // RTP version
    RtpBuf[1]  = 0x0b | 0x80;                        // L16 payload (11) and no marker bit
    RtpBuf[3]  = m_SequenceNumber & 0x0FF;           // each packet is counted with a sequence counter
    RtpBuf[2]  = m_SequenceNumber >> 8;
    RtpBuf[4]  = (m_Timestamp & 0xFF000000) >> 24;   // each image gets a timestamp
    RtpBuf[5]  = (m_Timestamp & 0x00FF0000) >> 16;
    RtpBuf[6] = (m_Timestamp & 0x0000FF00) >> 8;
    RtpBuf[7] = (m_Timestamp & 0x000000FF);
    RtpBuf[8] = 0x13;                               // 4 byte SSRC (sychronization source identifier)
    RtpBuf[9] = 0xf9;                               // we just an arbitrary number here to keep it simple
    RtpBuf[10] = 0x7e;
    RtpBuf[11] = 0x67;


    // determine start of data in buffer
    //unsigned char * dataBuf = ((unsigned char*)RtpBuf + HEADER_SIZE);
    unsigned char * dataBuf = &RtpBuf[HEADER_SIZE];

    int bytesRead = m_audioSource->readBytes((void*)dataBuf, m_fragmentSize);

    // convert to network format (big endian)
    m_audioSource->getFormat()->convert(dataBuf, bytesRead);

    // prepare the packet counter for the next packet
    m_SequenceNumber++;   

    udpsocketsend(m_RtpSocket, RtpBuf, HEADER_SIZE + bytesRead, m_ClientIP, m_ClientPort);


    return bytesRead;
}

u_short AudioStreamer::GetRtpServerPort()
{
    return m_RtpServerPort;
};

u_short AudioStreamer::GetRtcpServerPort()
{
    return m_RtcpServerPort;
};

bool AudioStreamer::InitUdpTransport(IPADDRESS aClientIP, IPPORT aClientPort)
{
    m_ClientIP = aClientIP;
    m_ClientPort = aClientPort;

    m_SequenceNumber = getRandom();

    if (m_udpRefCount != 0)
    {
        ++m_udpRefCount;
        return true;
    }

    for (u_short P = 6970; P < 0xFFFE; P += 2)
    {
        m_RtpSocket     = udpsocketcreate(P);
        if (m_RtpSocket)
        {   // Rtp socket was bound successfully. Lets try to bind the consecutive Rtsp socket
            m_RtcpSocket = udpsocketcreate(P + 1);
            if (m_RtcpSocket)
            {
                m_RtpServerPort  = P;
                m_RtcpServerPort = P+1;
                break;
            }
            else
            {
                udpsocketclose(m_RtpSocket);
                udpsocketclose(m_RtcpSocket);
            };
        }
    };
    ++m_udpRefCount;

    log_d("RTP Streamer set up with client IP %s and client Port %i", inet_ntoa(m_ClientIP), m_ClientPort);

    return true;
}

void AudioStreamer::ReleaseUdpTransport(void)
{
    --m_udpRefCount;
    if (m_udpRefCount == 0)
    {
        m_RtpServerPort  = 0;
        m_RtcpServerPort = 0;
        udpsocketclose(m_RtpSocket);
        udpsocketclose(m_RtcpSocket);

        m_RtpSocket = NULLSOCKET;
        m_RtcpSocket = NULLSOCKET;
    }
}

bool AudioStreamer::InitAudioSource() {
    log_i("InitAudioSource");
    m_fragmentSize = getAudioSource()->getFormat()->fragmentSize();
    m_timer_period = getAudioSource()->getFormat()->timerPeriod();
    log_i("m_fragmentSize (bytes): %d", m_fragmentSize);
    return true;
}

void AudioStreamer::Start() {
    log_i("Starting RTP Stream");

    if (m_audioSource != nullptr) {
        InitAudioSource();
        m_audioSource->start();
        esp_timer_start_periodic(RTP_timer, m_timer_period);
        log_i("Free heap size: %i KB", esp_get_free_heap_size() / 1000);

    } else {
        log_e("No streaming source");
    }
}

void AudioStreamer::Stop() {
    log_i("Stopping RTP Stream");
    if (m_audioSource != nullptr) {
        m_audioSource->stop();
    }
    esp_timer_stop(RTP_timer);
}

void AudioStreamer::doRTPStream(void * audioStreamerObj) {
    AudioStreamer * streamer = (AudioStreamer*)audioStreamerObj;
    int start, stop;

    start = esp_timer_get_time();

    int bytes = streamer->SendRtpPacketDirect();
    int samples = bytes/2;
    if (samples < 0) {
        log_w("Direct sending of RTP stream failed");
    } else if (samples > 0) {           // samples have been sent
        streamer->m_Timestamp += samples;        // no of samples sent
        log_v("%i samples sent (%ims); timestamp: %i", samples, samples / 16, streamer->m_Timestamp);
    }

    stop = esp_timer_get_time();
    if (stop - start > 20000) {
        log_w("RTP Stream can't keep up (took %i us, 20000 is max)!", stop - start);
    }
        
}