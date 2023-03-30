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

#include "RTSPFormat.h"

/**
 * @brief Interface for an audio source for an RTP stream
 */
class IAudioSource {
    public:
        IAudioSource() {}
        /**
         * @brief Defines the format of the audio data. If nothing has been defined we 
         * use 2 byte PCM info with 16000 samples per second on 1 channel
         */
        virtual RTSPFormat* getFormat() {
            if (p_fmt==nullptr){
                // we assume 2 byte PCM info with 16000 samples per second on 1 channel
                p_fmt = new RTSPFormatPCM();
            }
            return p_fmt;
        }

        /**
         * @brief Defines the Format 
         * 
         * @param fmt 
         */
        void setFormat(RTSPFormat *fmt){
            p_fmt = fmt;
        }

        /**
         * (Reads and) Copies up to maxSamples samples into the given buffer
         * @param dest Buffer into which the samples are to be copied
         * @param maxSamples maximum number of samples to be copied
         * @return actual number of samples that were copied
         */
        virtual int readBytes(void * dest, int maxSamples) = 0;

        /**
         * Start preparing data in order to provide it for the stream
         */
        virtual void start() {};

        /**
         * Stop preparing data as the stream has ended
         */ 
        virtual void stop() {};

    protected:
        RTSPFormat *p_fmt=nullptr;
};