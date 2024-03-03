# Micro-RTSP-Audio

Library to asynchronously run an RTSP-Server on an ESP32, in order to stream raw audio.

I modified the library Micro-RTSP for video streaming by Kevin Hester: https://github.com/geeksville/Micro-RTSP<br>
Copyright 2018 S. Kevin Hester-Chow, kevinh@geeksville.com (MIT License)

# Usage 

```C++
// set up an Audio source (with your implementation for IAudioSource)
AudioTestSource audioSource; 
// create the Audio Streamer using the audio source
AudioStreamer streamer(&audioSource);    
// create the RTSPServer using the streamer
RTSPServer rtsp = RTSPServer(&streamer);
// start the server asynchronously 
rtsp.runAsync();
```

The Audio Source must implement the IAudioInterface, mainly the function `readBytes()` which provides the audio data to the streamer.

# Connecting to Server

Open VLC, ->File -> Open Network and enter rtsp://ip-address

# Comments

For the time being only a sampling rate of 16000 with a mono signal is supported. If you try other settings you will need to adjust the fragment_size and timer_period. But I noticed that the UDP of the ESP32 is just not fast enough to handle more and the only way around this limitation is to use CODECs.

The [AudioTools project](https://github.com/pschatzmann/arduino-audio-tools) provides this extended functionality, so I recommend to use this library together with the extended classes, codecs and resample functionality. Examples can be found [in the communications examples folder](https://github.com/pschatzmann/arduino-audio-tools/tree/main/examples/examples-communication/rtsp).

# Class Documentation

Here is the [documentation for the classes](https://pschatzmann.github.io/Micro-RTSP-Audio/docs/html/annotated.html) that are provided by this project. 

# Installation

For Arduino you can download the library as zip and call include Library -> zip library. Or you can git clone this project into the Arduino libraries folder e.g. with

```
cd  ~/Documents/Arduino/libraries
git clone https://github.com/pschatzmann/Micro-RTSP-Audio.git
```
