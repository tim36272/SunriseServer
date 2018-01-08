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
#ifndef ION_COMPOSER_H_
#define ION_COMPOSER_H_
#include <Arduino.h>

typedef enum SunriseAction_e
{
    SUNRISE_ACTION_PLAY_SYNCED_MUSIC_FILE = 0,
    SUNRISE_ACTION_PLAY_LOCAL_MUSIC_FILE = 1
} SunriseAction_t;

#define WIFI_MAX_MSG_LEN (32)
#define SUNRISE_COMMAND_MAX_ARG_LEN (WIFI_MAX_MSG_LEN-sizeof(SunriseAction_t))
typedef struct SunriseCommand_s
{
    SunriseAction_t action;
    union
    {
        struct {
          char filename[SUNRISE_COMMAND_MAX_ARG_LEN]; //SUNRISE_ACTION_PLAY_SYNCED_MUSIC_FILE
        } syncedMusic;
        struct {
          uint8_t trackId; //the track ID, such as 1 for 0001.mp3
          char filename[(SUNRISE_COMMAND_MAX_ARG_LEN - sizeof(uint8_t))];
        }localMusic;
    };
} SunriseCommand_t;
typedef enum SyncCmdType_s : uint8_t
{
    SYNC_CMD_TYPE_INVALID,
    SYNC_CMD_TYPE_START,
    SYNC_CMD_TYPE_PROGRESS
} SyncCmdType_t;

typedef struct SyncCmd_s
{
    SyncCmdType_t cmdType;
    uint32_t  microseconds; //number of microseconds since the start of the file
} SyncCmd_t;
typedef enum ComposerFileType_s : uint8_t
{
    COMPOSER_FILE_TYPE_RGB
} ComposerFileType_t;

typedef struct ComposerFileHeader_s
{
    ComposerFileType_t type;
    uint8_t datastart; //offset of the start of the data (i.e. the end of the header) in bytes
    uint16_t framerate; //the framerate in microhertz (so that it can be stored in an integer), i.e. take Hz and multiply by 1000
    uint16_t num_leds; //the number of LEDs in the stream
} ComposerFileHeader_t;

void PrintComposerHeader(ComposerFileHeader_t header);
uint8_t composerFileTypeToBytesPerPixel(ComposerFileType_t type);
#endif //ION_COMPOSER_H_
