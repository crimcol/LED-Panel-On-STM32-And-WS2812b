#ifndef __WS2812B_H
#define __WS2812B_H

#include <stdint.h>

#define WS2812B_PULSE_HIGH      20
#define WS2812B_PULSE_LOW       10

typedef struct LedColor
{
    uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
} LedColor;

typedef struct PwmColor
//typedef struct __attribute__((packed)) PwmColor
{
    uint8_t Green[8];
    uint8_t Red[8];
    uint8_t Blue[8];
} PwmColor;

void ConvertColorLedToPwm(LedColor *source, PwmColor *destination);
void ClearLedColor(LedColor *color);
void ClearPwmColor(PwmColor *color);

#endif //__WS2812B_H
