# Micro-RTSP-Audio

Library to asynchronously run an RTSP-Server on an ESP32, in order to stream raw audio.

I modified the library Micro-RTSP for video streaming by Kevin Hester: https://github.com/geeksville/Micro-RTSP<br>
Copyright 2018 S. Kevin Hester-Chow, kevinh@geeksville.com (MIT License)

# Usage 

```C++
// set up an Audio source
IAudioSource audioSource = AudioDevice();
// create the Audio Streamer using the audio source
AudioStreamer streamer = AudioStreamer(&audioSource);
    
// create the RTSPServer using the streamer
RTSPServer rtsp = RTSPServer(&streamer);
// start the server asynchronously 
rtsp.runAsync();
```

The Audio Source must implement the IAudioInterface, mainly the function `readBytes()` which provides the audio data to the streamer.

# Connecting to Server

Open VLC, ->File -> Open Network and enter rtsp://ip-address

# Limitations

For the time beeing only a sampling rate of 16000 with a mono signal is supported. If you try other settings you will need to adjust the fragment_size and timer_period. But I noticed that the UDP of the ESP32 is just not fast enough to handle more and the only way around this limitation is to use CODECs.

# Using this Library with the Arduino Audio Tools

The AudioTools provide some extended functionality, so I recommend to use this library together with the extended classes.
