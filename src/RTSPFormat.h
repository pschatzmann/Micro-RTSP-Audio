#pragma once
#include "stdint.h"
#include "platglue.h"

/**
 * @brief Class which provides the format information that will be retured for
 * the DESCRIBE event
 *
 */

class RTSPFormat {
 public:
  // Provides the format information: see see https://en.wikipedia.org/wiki/RTP_payload_formats
  virtual const char* format(char* buffer, int len) = 0;
  // Potential data convertsion e.g. to network format. len is in samples, the result in bytes
  virtual void convert(void* data, int len) { }
  // Provides the fragment size in bytes
  virtual int fragmentSize() = 0;
  virtual int timerPeriod() {return 20000;}

 protected:
  const char* STD_URL_PRE_SUFFIX = "trackID";
};

/**
 * @brief PCM format
 * Basic Audio information for PCM format
 */

class PCMInfo {
 public:
  PCMInfo(int sampleRate = 16000, int channels = 1, int sampleSize = 2) {
    sample_rate = sampleRate;
    this->channels = channels;
    this->sample_size = sampleSize;
  }
  /**
   * Returns sample rate at which data is provided
   * @return the sample rate
   */
  virtual int getSampleRate() { return sample_rate; }
  /**
   * @brief Defines the number of channels
   *
   * @return int
   */
  virtual int getChannels() { return channels; }
  /**
   * Returns size of a single sample in bytes
   * @return sample size in bytes
   */
  virtual int getSampleSizeBytes() { return sample_size; };

 protected:
  int sample_rate, channels, sample_size;
};

/**
 * @brief RTSPFormatPCM PCM information for RTSP
 * see https://en.wikipedia.org/wiki/RTP_payload_formats
 */
class RTSPFormatPCM : public RTSPFormat {
 public:
  RTSPFormatPCM(PCMInfo& info) { p_info = &info; }

  RTSPFormatPCM() { p_info = new PCMInfo(); }

  /**
   * @brief Provide format 10 or 11
   * 
   * @param buffer 
   * @param len 
   * @return const char* 
   */
  const char* format(char* buffer, int len) override {
    snprintf(
        buffer, len,
        "s=Microphone\r\n"      // Stream Name
        "c=IN IP4 0.0.0.0\r\n"  // Connection Information
        "t=0 0\r\n"  // start / stop - 0 -> unbounded and permanent session
        "m=audio 0 RTP/AVP %d\r\n"  // UDP sessions with format 10 or 11
        "a=rtpmap:%s\r\n"
        "a=rate:%i\r\n",  // provide sample rate
        format(channels()),payloadFormat(sampleRate(), channels()), sampleRate());
    return (const char*) buffer;
  }

  /**
   * @brief Convert to network format
   *
   * @param data
   * @param byteSize
   */
  void convert(void* data, int samples) {
    // convert to network format (big endian)
    int16_t* pt_16 = (int16_t*)data;
    for (int j = 0; j < samples/2; j++) {
      pt_16[j] = htons(pt_16[j]);
    }
  }


  int sampleRate() { return p_info->getSampleRate(); }
  int channels() { return p_info->getChannels(); }
  int bytesPerSample() { return p_info->getSampleSizeBytes(); }
  // for sample rate 16000
  int fragmentSize() { return 320; }
  int timerPeriod() {return 20000;}

  PCMInfo& info() { return *p_info; }


 protected:
  PCMInfo* p_info = nullptr;
  char payload_fromat[30];

  const char* payloadFormat(int sampleRate, int channels) {
    // see https://en.wikipedia.org/wiki/RTP_payload_formats
    // 11 L16/%i/%i

    switch (channels) {
      case 1:
        snprintf(payload_fromat, 30, "%d L16/%i/%i", format(channels), sampleRate,
                 channels);
        break;
      case 2:
        snprintf(payload_fromat, 30, "%d L16/%i/%i", format(channels), sampleRate,
                 channels);
        break;
      default:
        log_e("unsupported audio type");
        break;
    }
    return payload_fromat;
  }

  const int format(int channels) {
    int result = 0;
    switch (channels) {
      case 1:
        result = 11;
        break;
      case 2:
        result = 10;
        break;
      default:
        log_e("unsupported audio type");
        break;
    }
    return result;
  }
};