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
#ifndef MP3SUPPORT_H_
#define MP3SUPPORT_H_
class Mp3Notify
{
public:
    static void OnError(uint16_t errorCode)
    {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }

    static void OnPlayFinished(uint16_t globalTrack)
    {
        Serial.println();
        Serial.print("Play finished for #");
        Serial.println(globalTrack);
    }

    static void OnCardOnline(uint16_t code)
    {
        Serial.println();
        Serial.print("Card online ");
        Serial.println(code);
    }

    static void OnCardInserted(uint16_t code)
    {
        Serial.println();
        Serial.print("Card inserted ");
        Serial.println(code);
    }

    static void OnCardRemoved(uint16_t code)
    {
        Serial.println();
        Serial.print("Card removed ");
        Serial.println(code);
    }
};
#endif //MP3SUPPORT_H_