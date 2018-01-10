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
#include "LEDSupport.h"
namespace ion
{
    void showLEDTestPattern(CRGB leds[], uint16_t num_leds)
    {
        //Show test pattern
        FastLED.setMaxPowerInMilliWatts(20000);
        for (uint16_t led_index = 0; led_index < num_leds; ++led_index)
        {
            leds[led_index] = CRGB::Red;
        }
        FastLED.show();
        delay(200);
        for (uint16_t led_index = 0; led_index < num_leds; ++led_index)
        {
            leds[led_index] = CRGB::Green;
        }
        FastLED.show();
        delay(200);
        for (uint16_t led_index = 0; led_index < num_leds; ++led_index)
        {
            leds[led_index] = CRGB::Blue;
        }
        FastLED.show();
        delay(200);
        for (uint16_t led_index = 0; led_index < num_leds; ++led_index)
        {
            leds[led_index] = CRGB::White;
            uint16_t pos_in_second_string = led_index - 149;
            float pct_through_second_string = (float)pos_in_second_string / 150;
            float scale = (1.0 - pct_through_second_string) * 255;
            leds[led_index].red -= scale;
            leds[led_index].blue -= scale;
            leds[led_index].green -= scale;

        }
        FastLED.show();
        delay(200);
        for (uint16_t led_index = 0; led_index < num_leds; ++led_index)
        {
            if (led_index > 75)
            {
                leds[led_index] = CRGB::White;
            } else
            {
                leds[led_index] = CRGB::Black;
            }
        }
        FastLED.show();

    }

    void ledProgress(uint8_t percent, CRGB leds[], uint16_t num_leds)
    {
        for (uint16_t led_index = 0; led_index < num_leds; ++led_index)
        {
            if (led_index < num_leds * (percent / 100.0))
            {
                leds[led_index] = CRGB::White;
            } else
            {
                leds[led_index] = CRGB::Black;
            }
        }
        leds[num_leds - 1] = CRGB::White;
        FastLED.show();
    }
};