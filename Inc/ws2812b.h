#ifndef __WS2812B_H
#define __WS2812B_H

#include <stdint.h>

#define WS2812B_PULSE_HIGH      21
#define WS2812B_PULSE_LOW       9

typedef struct LedColor
{
    uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
} LedColor;

typedef __packed struct PwmColor
{
    uint8_t Green[8];
    uint8_t Red[8];
    uint8_t Blue[8];
} PwmColor;

void ConvertColorLedToPwm(LedColor *source, PwmColor *destination);

#endif //__WS2812B_H
