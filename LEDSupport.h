/*
This file is part of Ionlib.  Copyright (C) 2016  Tim Sweet

Ionlib is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Ionlib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Ionlib.If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef LED_SUPPORT_H_
#define LED_SUPPORT_H_
#include <FastLED.h>
namespace ion
{
    void showLEDTestPattern(CRGB leds[], uint16_t num_leds);
    void ledProgress(uint8_t percent, CRGB leds[], uint16_t num_leds);
};
#endif //LED_SUPPORT_H_