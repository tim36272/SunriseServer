
#include "FastLED.h"
#include "SimpleEsp8266.h"
#include <SPI.h>
#include <SD.h>
#include <DFMiniMp3.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "Mp3Support.h"
#include "Composer.h"
#include "Timer.h"
#include "iondef.h"
#include "memory.h"
#define LOG_ENABLE
#include "log.h"

#define TIMER_ENABLED
// How many leds are in the strip?
#define NUM_LEDS 50
//#define NUM_LEDS 100

// Data pin that led data will be written out over
#define ESP_SSID F("Shatterdome-24") // Your network name here
#define ESP_PASS F("WolfPack2014")   // Your network password here
#define CMD_PORT 5003                //Port to receive commands on
// This is an array of leds.  One item for each led in the strip.
CRGB leds[NUM_LEDS];

//Serial mapping
#ifdef __AVR_ATmega2560__
#define SERIAL_ESP Serial1
#define SERIAL_DEBUG Serial
#define SERIAL_DEBUG_REF &Serial
#define SERIAL_DFPLAYER Serial2
#define SERIAL_DFPLAYER_TYPE HardwareSerial
#define ANTICLOCKWISE_LED_DATA_PIN 6
#define CLOCKWISE_LED_DATA_PIN     5
#define SD_CS_PIN                  53
#define WIFI_RESET_PIN             4
#define LCD_RS_PIN 22
#define LCD_EN_PIN 23
#define LCD_D4_PIN 24
#define LCD_D5_PIN 25
#define LCD_D6_PIN 26
#define LCD_D7_PIN 27

#elif defined(__AVR_ATmega328P__)
//Pinout settings
#define CLOCKWISE_LED_DATA_PIN     2
#define ANTICLOCKWISE_LED_DATA_PIN 3
#define SERIAL_DFPLAYER_TX_PIN 4 //Rx/Tx is from the arduino's point of view
#define SERIAL_DFPLAYER_RX_PIN 5 //Rx/Tx is from the arduino's point of view
#define LCD_D4_PIN 6
#define LCD_D5_PIN 7
#define LCD_D6_PIN 8
#define LCD_D7_PIN 9
#define SD_CS_PIN                  10
//11-13 is SPI bus
#define WIFI_RESET_PIN             A0
#define LCD_RS_PIN A1
#define LCD_EN_PIN A2
//A3 is not conected
#define CLOCKWISE_WHITE_LED_PIN A4
#define CLOCKWISE_WHITE_LED_PIN A5
#define SERIAL_DEBUG_TX_PIN A3    //A6
#define SERIAL_DEBUG_RX_PIN A7

//Serial settings
#define SERIAL_ESP Serial
#ifdef LOG_ENABLE
SoftwareSerial software_serial_1(SERIAL_DEBUG_RX_PIN, SERIAL_DEBUG_TX_PIN); // RX, TX
#define SERIAL_DEBUG software_serial_1
#define SERIAL_DEBUG_REF &software_serial_1
//#define SERIAL_DEBUG Serial
//#define SERIAL_DEBUG_REF &Serial
#else
#define SERIAL_DEBUG NULL
#define SERIAL_DEBUG_REF NULL
#endif
SoftwareSerial software_serial_2(SERIAL_DFPLAYER_RX_PIN, SERIAL_DFPLAYER_TX_PIN); // RX, TX
#define SERIAL_DFPLAYER software_serial_2
#define SERIAL_DFPLAYER_TYPE SoftwareSerial

#endif

void ledProgress(uint8_t percent)
{
    for (uint16_t led_index = 0; led_index < NUM_LEDS; ++led_index)
    {
        if (led_index < NUM_LEDS * (percent / 100.0))
        {
            leds[led_index] = CRGB::White;
        } else
        {
            leds[led_index] = CRGB::Black;
        }
    }
    leds[NUM_LEDS - 1] = CRGB::White;
    FastLED.show();
}

void showTestPattern() {
    //Show test pattern
    FastLED.setMaxPowerInMilliWatts(20000);
    for (uint16_t led_index = 0; led_index < NUM_LEDS; ++led_index)
    {
        leds[led_index] = CRGB::Red;
    }
    FastLED.show();
    delay(200);
    for (uint16_t led_index = 0; led_index < NUM_LEDS; ++led_index)
    {
        leds[led_index] = CRGB::Green;
    }
    FastLED.show();
    delay(200);
    for (uint16_t led_index = 0; led_index < NUM_LEDS; ++led_index)
    {
        leds[led_index] = CRGB::Blue;
    }
    FastLED.show();
    delay(200);
    for (uint16_t led_index = 0; led_index < NUM_LEDS; ++led_index)
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
    for (uint16_t led_index = 0; led_index < NUM_LEDS; ++led_index)
    {
        if (led_index > 75)        {
            leds[led_index] = CRGB::White;
        } else
        {
            leds[led_index] = CRGB::Black;
        }
    }
    FastLED.show();
   
}

//void printDirectory(File dir, int numTabs)
//{
//    while (true)
//    {
//
//        File entry = dir.openNextFile();
//        if (!entry)
//        {
//            // no more files
//            break;
//        }
//        for (uint8_t i = 0; i < numTabs; i++)
//        {
//            SERIAL_DEBUG.print('\t');
//        }
//        SERIAL_DEBUG.print(entry.name());
//        if (entry.isDirectory())
//        {
//            SERIAL_DEBUG.println("/");
//            printDirectory(entry, numTabs + 1);
//        } else
//        {
//            // files have sizes, directories do not
//            SERIAL_DEBUG.print("\t\t");
//            SERIAL_DEBUG.println(entry.size(), DEC);
//        }
//        entry.close();
//    }
//}

// This function sets up the leds and tells the controller about them,
//  initializes the SD card, and setups the TCP Server
void setup()
{
    //Setup board
    SERIAL_ESP.begin(115200); while (!SERIAL_ESP); // Serial connection to ESP8266
#ifdef LOG_ENABLE
    ion::LogEnable();

    SERIAL_DEBUG.begin(115200); while (!SERIAL_DEBUG); // UART serial debug
    SERIAL_DEBUG.println(F("Booting up"));
#endif
    //indicate no error
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(A6, OUTPUT);
    pinMode(A7, OUTPUT);
    for (uint8_t i = 0; i < 3; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }


    //Initialize FastLED
    FastLED.addLeds<WS2812B, ANTICLOCKWISE_LED_DATA_PIN, GRB>(leds, NUM_LEDS);
    ledProgress(20);

    //Initialize SD
    SD.begin(SD_CS_PIN);

    //Test SD
    //File root;
    //root = SD.open("/");
    //
    //printDirectory(root, 0);
    LOGINFO("Setup complete");
}
//void testLoop(SimpleESP8266* wifi)
//{
//    static uint32_t run = 0;
//    run++;
//    //Debug: flash red
//    for (uint16_t led_index = 0; led_index < NUM_LEDS; ++led_index)
//    {
//        if (run % 2 == 0)
//        {
//            leds[led_index] = CRGB::Red;
//        } else
//        {
//            leds[led_index] = CRGB::Yellow;
//        }
//    }
//    FastLED.show();
//    char buffer[32];
//    wifi->tcpRecv(buffer, 32);
//    SERIAL_DEBUG.print(F("Wifi says: "));
//    SERIAL_DEBUG.println(buffer);
//    File test_file = SD.open(F("test.txt"), FILE_WRITE);
//    if (test_file)
//    {
//        test_file.println(run);
//        test_file.close();
//    }
//    test_file = SD.open(F("test.txt"));
//    if (test_file)
//    {
//        SERIAL_DEBUG.print(F("File says: "));
//        while (test_file.available())
//        {
//            SERIAL_DEBUG.write(test_file.read());
//        }
//        SERIAL_DEBUG.println(F("End file"));
//        test_file.close();
//    }
//}

bool openCprFile(const char* filename, ComposerFileHeader_t* header, uint8_t* bytes_per_pixel, File* led_file, float* fps) {
  //Open the file and read the header
    *led_file = SD.open(filename);
    LOGINFO("Called SD.open");
    if (*led_file)
    {
        size_t bytes_read = led_file->readBytes((char*)header, sizeof(ComposerFileHeader_t));
        //fill it in manually for now
        header->datastart = 0;
        header->type = COMPOSER_FILE_TYPE_RGB;
        header->framerate = 32.019 * 1000;
        header->num_leds = 150;
        PrintComposerHeader(*header);
        if (bytes_read != sizeof(ComposerFileHeader_t))
        {
            LOGINFO("Read %u bytes from %s expected %u", bytes_read, filename, sizeof(ComposerFileHeader_t));
            return false;
        }
        if (header->type != COMPOSER_FILE_TYPE_RGB)
        {
            LOGINFO("Only data type RGB (%d) is supported, not (%d)", COMPOSER_FILE_TYPE_RGB, header->type);
            return false;
        }
        if (header->num_leds > NUM_LEDS)
        {
            LOGINFO("File defines too many LEDS, they will be truncated: expected <= %u, file needs %u", NUM_LEDS, header->num_leds);
            header->num_leds = NUM_LEDS;
        }
        *bytes_per_pixel = composerFileTypeToBytesPerPixel(header->type);
        led_file->seek(header->datastart);
        *fps = header->framerate / 1000.0;
    } else
    {
        LOGINFO("Failed to open %s", filename);
        return false;
    }
    return true;
}
void playSyncdFile(const char* filename, SimpleESP8266* wifi)
{
    SyncCmd_t cmd;
    cmd.cmdType = SYNC_CMD_TYPE_INVALID;
    int32_t bytes_recv = 0;
    ComposerFileHeader_t header;
    uint8_t bytes_per_pixel;
    uint32_t frame_number = 0;
    uint32_t t0;
    float fps; //the framerate in Hz (instead of microhertz)
    uint32_t file_time;
    //The following average times found experimentally with 150 pixels
    const uint32_t mean_sd_read_usec = 17890;
    const uint32_t mean_sd_seek_usec = 730;
    const uint32_t mean_led_show_usec = 1780;
    const uint32_t mean_tcp_usec = 1430;
#ifdef TIMER_ENABLED
    PerfMon_t perf_led;
    PerfMon_t perf_tcp_rx;
    PerfMon_t perf_tcp_pass;
    PerfMon_t perf_sd_read;
    PerfMon_t perf_sd_seek;
    InitPerf(&perf_led);
    InitPerf(&perf_tcp_rx);
    InitPerf(&perf_tcp_pass);
    InitPerf(&perf_sd_read);
    InitPerf(&perf_sd_seek);
#endif
    //Announce file
    LOGINFO("Playing sync'd file: %s", filename);
    //Open the animation file
    File led_file;
    bool file_open = openCprFile(filename, &header, &bytes_per_pixel, &led_file, &fps);

    if(!file_open) {
      return;
    }
    
    LOGINFO("Awaiting start command");
    while (bytes_recv != sizeof(SyncCmd_t) || cmd.cmdType != SYNC_CMD_TYPE_START)
    {
        //Wait for start command
        bytes_recv = wifi->tcpRecv((char*)&cmd, sizeof(SyncCmd_t));
    }

    t0 = micros();

    LOGINFO("Starting music");

    //Set the socket timeout as short as possible so that we can get non-blocking-like behavior
    wifi->setTimeouts(10, 0, 0, 0, 1);
    //Turn off wifi debugging
    wifi->setDebug(NULL);

    //Play the file
    if (led_file)
    {
        while (led_file.available())
        {
            //Read the first num_leds*bytes_per_pixel bytes
#ifdef TIMER_ENABLED
            StartTimer(&perf_sd_read);
#endif
            led_file.readBytes((uint8_t*)leds, header.num_leds * bytes_per_pixel);
#ifdef TIMER_ENABLED
            StopTimer(&perf_sd_read);
#endif

            //Compute what time this is suppose to be sent
            uint32_t ideal_time = (frame_number / fps) * SECONDS_TO_MICROSECONDS;
            //Get the current time
            file_time = micros() - t0;
            if (ideal_time > file_time)
            {
                delayMicroseconds(ideal_time - file_time - mean_led_show_usec);
            }
#ifdef TIMER_ENABLED
            StartTimer(&perf_led);
#endif
            FastLED.show();
#ifdef TIMER_ENABLED
            StopTimer(&perf_led);
#endif

            uint32_t tcp_start = micros();
            bytes_recv = wifi->tcpRecv((char*)&cmd, sizeof(SyncCmd_t));

            if (bytes_recv == sizeof(SyncCmd_t))
            {
#ifdef TIMER_ENABLED
                AddPerfSample(micros() - tcp_start, &perf_tcp_rx);
#endif
                file_time = micros() - t0;
                //Adjust the commanded microseconds based on how much time has probably elapsed since the message was sent and how much time will pass before the next LED can be shown
                cmd.microseconds += mean_tcp_usec + mean_sd_seek_usec + mean_sd_read_usec;
                //update the estimate of t0
                t0 = micros() - cmd.microseconds;
                //seek to the correct place in the file
                frame_number = (cmd.microseconds * MICROSECONDS_TO_SECONDS) * fps;
                uint32_t file_pos = frame_number * header.num_leds * bytes_per_pixel;
#ifdef TIMER_ENABLED
                StartTimer(&perf_sd_seek);
#endif
                led_file.seek(file_pos);
#ifdef TIMER_ENABLED
                StopTimer(&perf_sd_seek);
                //print stats
                //SERIAL_DEBUG.print(F("SD Read: "));
                //PrintPerf(perf_sd_read);
                //SERIAL_DEBUG.print(F("SD Seek: "));
                //PrintPerf(perf_sd_seek);
                //SERIAL_DEBUG.print(F("LED: "));
                //PrintPerf(perf_led);
                //SERIAL_DEBUG.print(F("TCP RX: "));
                //PrintPerf(perf_tcp_rx);
                //SERIAL_DEBUG.print(F("TCP Pass: "));
                //PrintPerf(perf_tcp_pass);
#endif
            } else
            {
#ifdef TIMER_ENABLED
                AddPerfSample(micros() - tcp_start, &perf_tcp_pass);
#endif
            }
            frame_number++;
        }
    }
    //Set the timeout back to the default values
    wifi->setDefaultTimeouts();
    wifi->setDebug(SERIAL_DEBUG_REF);

}

void playLocalFile(uint8_t track_id, const char* filename)
{
    ComposerFileHeader_t header;
    uint8_t bytes_per_pixel;
    float fps; //the framerate in Hz (instead of microhertz)
    uint32_t frame_number = 0;
    uint32_t file_time;
    uint32_t t0;
    const uint32_t mean_led_show_usec = 1780;
    uint8_t led_on = 1;
    uint8_t delays = 0;
    const float fps_fudge = ((25.87+3.8)/25.87) / (237.0/236.0);
    const uint32_t start_fudge = 50;

    LOGINFO("Playing local music file #%u with animation %s", track_id, filename);
    LOGSRAM();
    //Setup the DFPlayer (i.e. the SD card reader that supports audio)
    DFMiniMp3<SERIAL_DFPLAYER_TYPE, Mp3Notify> mp3(SERIAL_DFPLAYER);
    mp3.begin();
    mp3.setVolume(15);

    LOGINFO("Created DFPlayer");
    LOGSRAM();


    //Open the LED file
    File led_file;
    bool file_open = openCprFile(filename, &header, &bytes_per_pixel, &led_file, &fps);
    //fps = 10;
    if(!file_open) {
        LOGINFO("Failed to open video file");
        return;
    }
    LOGINFO("Video open");

    //Show heartbeat
    //memset(leds, 0, header.num_leds * bytes_per_pixel);
    //for(uint32_t i=0; i< 30; i++) {
    //  //toggle the LED
    //  leds[0] = 20;
    //  FastLED.show();
    //  delay(1000);
    //  leds[0] = 0;
    //  FastLED.show();
    //  delay(1000);
    //}
    mp3.playMp3FolderTrack(track_id);
    delay(start_fudge);
    t0 = micros();
    fps = fps * fps_fudge;

    LOGINFO("Starting animation");
    LOGSRAM();

    while (led_file.available())
        {
            //Read the first num_leds*bytes_per_pixel bytes
            led_file.readBytes((uint8_t*)leds, header.num_leds * bytes_per_pixel);

            //Compute what time this is suppose to be sent
            uint32_t ideal_time = (frame_number / fps) * SECONDS_TO_MICROSECONDS;
            //Get the current time
            file_time = micros() - t0;
            if (ideal_time > file_time)
            {
                delayMicroseconds(ideal_time - file_time - mean_led_show_usec);
            }
            FastLED.show();

            frame_number++;
        }
    LOGINFO("Animation complete");
}
// This function runs over and over, and is where you do the magic to light
// your leds.
void loop()
{
    ledProgress(40);

    //Initialize ESP8266
    SimpleESP8266 wifi(&SERIAL_ESP, SERIAL_DEBUG_REF, WIFI_RESET_PIN);
    LOGINFO("Setting up WiFi");
    boolean setup = wifi.setupTcpServer(ESP_SSID, ESP_PASS, CMD_PORT);
    if (setup)
    {
        LOGINFO("Setup complete");
    } else
    {
        LOGINFO("Wifi setup failed");
        delay(1000);
        return;
    }
    ledProgress(60);

    LOGSRAM();
    delay(1000);
    SunriseCommand_t cmd;
    ledProgress(100);

    int32_t bytes_recieved = wifi.tcpRecv((char*)&cmd, sizeof(SunriseCommand_t));

    if (bytes_recieved >= sizeof(SunriseAction_t))
    {
        switch (cmd.action)
        {
            case SUNRISE_ACTION_PLAY_SYNCED_MUSIC_FILE:
                playSyncdFile(cmd.syncedMusic.filename, &wifi); //"col.cpr"
                break;
            case SUNRISE_ACTION_PLAY_LOCAL_MUSIC_FILE:
                playLocalFile(cmd.localMusic.trackId, cmd.localMusic.filename);
                break;
            default:
                LOGINFO("Received invalid command with action: %d", cmd.action);
                break;
        }
    }
}

