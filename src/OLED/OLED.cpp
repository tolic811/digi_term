/*
 * SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64 displays
 *
 * @created: 2014-08-12
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 *
 */

// ----------------------------------------------------------------------------

#include <Wire.h>
#include <avr/pgmspace.h>

#include "OLED.h"
#include "fontNumber.h"

// ----------------------------------------------------------------------------

// Some code based on "IIC_wtihout_ACK" by http://www.14blog.com/archives/1358

const uint8_t init_sequence [] PROGMEM = {  // Initialization Sequence
  0xAE,     // Display OFF (sleep mode)
  0x20, 0b00,   // Set Memory Addressing Mode
          // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
          // 10=Page Addressing Mode (RESET); 11=Invalid
  0xB0,     // Set Page Start Address for Page Addressing Mode, 0-7
  0xC8,     // Set COM Output Scan Direction
  0x00,     // ---set low column address
  0x10,     // ---set high column address
  0x40,     // --set start line address
  0x81, 0x3F,   // Set contrast control register
  0xA1,     // Set Segment Re-map. A0=address mapped; A1=address 127 mapped. 
  0xA6,     // Set display mode. A6=Normal; A7=Inverse
  0xA8, 0x1F,   // Set multiplex ratio(1 to 64)
  0xA4,     // Output RAM to Display
          // 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
  0xD3, 0x00,   // Set display offset. 00 = no offset
  0xD5,     // --set display clock divide ratio/oscillator frequency
  0xF0,     // --set divide ratio
  0xD9, 0x22,   // Set pre-charge period
  0xDA, 0x02,   // Set com pins hardware configuration    
  0xDB,     // --set vcomh
  0x20,     // 0x20,0.77xVcc
  0x8D, 0x14,   // Set DC-DC enable
  0xAF      // Display ON in normal mode
  
};

uint8_t oledXpos, oledYpos = 0;

// Program:    5248 bytes

SSD1306OLED::SSD1306OLED(void){}


void SSD1306OLED::begin(void)
{
  Wire.begin();
  
  for (uint8_t i = 0; i < sizeof (init_sequence); i++) {
    send_command(pgm_read_byte(&init_sequence[i]));
  }
  clear();
}


void SSD1306OLED::send_command_start(void) {
  Wire.beginTransmission(SSD1306);
  Wire.write(0x00); // write command
}

void SSD1306OLED::send_command_stop(void) {
  Wire.endTransmission();
}

void SSD1306OLED::send_data_byte(uint8_t byte)
{
  if(Wire.writeAvailable()){
    send_data_stop();
    send_data_start();
  }
  Wire.write(byte);
  
}

void SSD1306OLED::send_command(uint8_t command)
{
  send_command_start();
  Wire.write(command);
  send_command_stop();
}

void SSD1306OLED::send_data_start(void)
{
  Wire.beginTransmission(SSD1306);
  Wire.write(0x40); //write data
}

void SSD1306OLED::send_data_stop(void)
{
  Wire.endTransmission();
}

void SSD1306OLED::setCursor(uint8_t x, uint8_t y)
{
  send_command_start();
  Wire.write(0xb0 + y);
  Wire.write(((x & 0xf0) >> 4) | 0x10); // | 0x10
  Wire.write((x & 0x0f) | 0x00); // | 0x01
  send_command_stop();
  oledXpos = x;
  oledYpos = y;

}

void SSD1306OLED::clear(void)
{
  fill(0x00);
}

void SSD1306OLED::fill(uint8_t fill)
{
  uint8_t m,n;
  for (m = 0; m < 8; m++)
  {
    send_command(0xb0 + m); // page0 - page1
    send_command(0x00);   // low column start address
    send_command(0x10);   // high column start address
    send_data_start();
    for (n = 0; n < 128; n++)
    {
      send_data_byte(fill);
    }
    send_data_stop();
  }
  setCursor(0, 0);
}

size_t SSD1306OLED::write(byte c) {
  if(c == '\r')
    return 1;
  if(c == '\n'){
    return 1;
  }

  send_data_start();
  for (int j= 0; j < 4; j++) {
    for (int i= 0; i <= 16; i++) {
      if(c < 43 || c > 59)  
        send_data_byte(0);
      else        
        send_data_byte(pgm_read_byte(&bigNumbers[c-43][j*16+i]));
    }

    setCursor(oledXpos, ++oledYpos);
  }
  send_data_stop();
  setCursor(oledXpos+16, 0);

  return 1;
}

SSD1306OLED disp;

// ----------------------------------------------------------------------------

