#pragma once

#ifdef ARDUINO_ARCH_ESP32
#include "platglue-esp32.h"
#elif defined(ARDUINO_ARCH_RP2040) 
#include "platglue-rp2040.h"
#else
#include "platglue-posix.h"
#endif
