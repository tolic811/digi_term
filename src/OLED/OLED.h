/*
 * SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64 displays
 *
 * @created: 2014-08-12
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 *
 */
#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <util/delay.h>

#ifndef DIGISPARKOLED_H
#define DIGISPARKOLED_H

// ----------------------------------------------------------------------------

#ifndef SSD1306
#define SSD1306   0x3C  // Slave address
#endif

// ----------------------------------------------------------------------------

class SSD1306OLED: public Print {

    public:
    SSD1306OLED(void);
    void begin(void);
    void send_command(uint8_t command);
    void send_data_byte(uint8_t byte);
    void send_data_start(void);
    void send_data_stop(void);
    void setCursor(uint8_t x, uint8_t y);
    void fill(uint8_t fill);
    void clear(void);
    void send_command_start(void);
    void send_command_stop(void);
    void char_f8x16(uint8_t x, uint8_t y, const char ch[]);
    virtual size_t write(byte c);
    
    using Print::write;
};


extern SSD1306OLED disp;

// ----------------------------------------------------------------------------

#endif

