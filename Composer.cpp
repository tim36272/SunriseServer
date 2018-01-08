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
#include "Composer.h"
void PrintComposerHeader(ComposerFileHeader_t header)
{
    Serial.print("File header: type: ");
    Serial.print(header.type);
    Serial.print(" datastart: ");
    Serial.print(header.datastart);
    Serial.print(" fps: ");
    Serial.print(header.framerate / 1000.0);
    Serial.print(" num LEDs: ");
    Serial.println(header.num_leds);
}

uint8_t composerFileTypeToBytesPerPixel(ComposerFileType_t type)
{
    switch (type)
    {
        case COMPOSER_FILE_TYPE_RGB:
            return 3;
        default:
            return 0;
    }
}