#include <string.h>
#include <SPI.h>
#include <Wire.h>

#include "SSD1306.hpp"

/*
byte placeholder_DDRB = 0;
byte placeholder_PORTB = 0;

#define DDRB placeholder_DDRB
#define PORTB placeholder_PORTB
*/

static byte width = 128;
static byte height = 64;

const SPISettings spiconf = SPISettings(20000000, MSBFIRST, SPI_MODE0);

static byte char_to_font_index(char chr)
{
  if (chr == -2)
  {
    return 96;
  }
  else if (chr == -3)
  {
    return 97;
  }
  else if (chr == -4)
  {
    return 98;
  }
  else if (chr == -5)
  {
    return 99;
  }
  else if (chr < 32)
  {
    return 255;
  }
  else
  {
    return chr - 32;
  }
}







// set write coordinates 
// x in {0,width-1}, y in {0,height/8 - 1}
// for 128x64: {0,127},{0,7}

static void setWriteAddress_SPI(byte x, byte y)
{
  byte max_x = width - 1;
  byte max_y = height / 8 - 1;
  COMMAND_MODE

  SPI.beginTransaction(spiconf);
  CS_LOW
  SPI.transfer(0x21); // column
  SPI.transfer(x); // start address
  SPI.transfer(max_x); // and end address
  SPI.transfer(0x22); // page
  SPI.transfer(y); // start address
  SPI.transfer(max_y); // and end address
  CS_HIGH
  SPI.endTransaction();
}

static void setWriteAddress_I2C(byte x, byte y)
{
  byte max_x = width - 1;
  byte max_y = height / 8 - 1;
  Wire.beginTransmission(SSD1306_I2C_ADDR);
  /*
  // this is for page addressing mode. We use horizontal addressing mode
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(x & 0xF); // lower nibble
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(((x & 0xF0) >> 4) | 0x10); // higher nibble
  */
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0x21); // column ..
  Wire.write(x); // start address ..
  Wire.write(max_x); // and end address
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0x22); // page ..
  Wire.write(y); // start address ..
  Wire.write(max_y); // and end address
  Wire.endTransmission();
}


static void clearDisplay_SPI()
{
  setWriteAddress_SPI(0, 0);

  int n = width*height/8;;
  DATA_MODE
  SPI.beginTransaction(spiconf);
  CS_LOW
  while (n--) { SPI.transfer(0); }
  CS_HIGH
  SPI.endTransaction();
}

static void clearDisplay_I2C()
{
  setWriteAddress_I2C(0, 0);

  int byte_count = width*height/8;
  // one 'maxloop': One loop writing maximum number of bytes
  // (the oled screen only supports writing up to 31 bytes in a single I2C command)
  byte maxloops = byte_count/31;
  byte maxloop_rest = byte_count - maxloops*31;
  byte_count = 0;
  for (int i = 0; i < maxloops; i++)
  {
    Wire.beginTransmission(SSD1306_I2C_ADDR);
    Wire.write(SSD1306_I2C_DATA);
    for (int j = 0; j < 31; j++)
    {
      Wire.write(0x00);
      //byte_count++;
    }
    Wire.endTransmission();
  }
  
  Wire.beginTransmission(SSD1306_I2C_ADDR);
  Wire.write(SSD1306_I2C_DATA);
  for (int i = 0; i < maxloop_rest; i++)
  {
    Wire.write(0x00);
    //byte_count++;
  }
  Wire.endTransmission();
}


static void setupOled_SPI(int width_, int height_)
{
  SPI.begin();
 
  // setting pins D8, D9 & D10
  //             Res, DC, CS      to output
  DDRB |= 0b00000111;
  // clearing reset and D/C
  PORTB &= ~(0b00000011);

  delay(10);

  // setting reset pin and CS to high
  PORTB |= 0b00000101;

  delay(10);


  SPI.beginTransaction(spiconf);
  CS_LOW
  SPI.transfer(0xAF); // turn display on
  SPI.transfer(0xB0); // set page address
  SPI.transfer(0x81); // set contrast
  SPI.transfer(0x0F); // to 0x07F
  SPI.transfer(0x20); // set addressing mode
  SPI.transfer(0x00); // to horizontal
  SPI.transfer(0x8D); // chargepump
  SPI.transfer(0x14); // enable
  SPI.transfer(0xA1); // mirror around vertical axis
  SPI.transfer(0xC8); // mirror around horizontal axis?
  CS_HIGH
  SPI.endTransaction();

  clearDisplay_SPI();
  setWriteAddress_SPI(0, 0);
}

static void setupOled_I2C(int width_, int height_)
{
  Wire.begin();
  delay(50);

  Wire.beginTransmission(SSD1306_I2C_ADDR);
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0xAF); // turn display on
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0x81); // set contrast
  Wire.write(0x0F); // to 0x07F
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0x20); // set addressing mode
  Wire.write(0x00); // to horizontal
  //Wire.write(0x01); // to vertical
  
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0xDA); // COM pin configuration
  Wire.write(0x12); // 

  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0x8D); // chargepump
  Wire.write(0x14); // enable
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0x40); // Set display ram start line reg to 0
  
  Wire.write(SSD1306_I2C_COMMAND);
  //Wire.write(0xA5); // display entire display ON
  Wire.write(0xA4); // display RAM contents
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0xA1); // mirror around vertical axis, write A0 to disable
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0xC8); // mirror around horizontal axis, write C0 to disable
  Wire.endTransmission();

  delay(150);

  clearDisplay_I2C();
  setWriteAddress_I2C(0, 0);
}


static void setTestPattern_SPI()
{
  setWriteAddress_SPI(0, 0);

  int n = width*height/8;
  DATA_MODE

  SPI.beginTransaction(spiconf);
  CS_LOW
  while (n--) { SPI.transfer(n); }
  CS_HIGH
  SPI.endTransaction();
}

static void setTestPattern_I2C()
{
  setWriteAddress_I2C(0, 0);

  int byte_count = width*height/8;
  byte maxloops = byte_count/31;
  byte maxloop_rest = byte_count - maxloops*31;
  byte_count = 0;
  for (int i = 0; i < maxloops; i++)
  {
    Wire.beginTransmission(SSD1306_I2C_ADDR);
    Wire.write(SSD1306_I2C_DATA);
    for (int i = 0; i < 31; i++)
    {
      Wire.write(byte_count++);
    }
    Wire.endTransmission();
  }

  Wire.beginTransmission(SSD1306_I2C_ADDR);
  Wire.write(SSD1306_I2C_DATA);
  for (int i = 0; i < maxloop_rest; i++)
  {
    Wire.write(byte_count);
  }
  Wire.endTransmission();
}


static void writeFont_SPI()
{
  DATA_MODE
  
  byte char_count = 99;
  SPI.beginTransaction(spiconf);
  CS_LOW
  do
  {
    uint8_t* font_char = (uint8_t*)pgm_read_word(&(font[99-char_count]));
    SPI.transfer(pgm_read_byte_near(font_char+0));
    SPI.transfer(pgm_read_byte_near(font_char+1));
    SPI.transfer(pgm_read_byte_near(font_char+2));
    SPI.transfer(pgm_read_byte_near(font_char+3));
    SPI.transfer(pgm_read_byte_near(font_char+4));

    /*
    SPI.transfer(font[90-char_count][0]);
    SPI.transfer(font[90-char_count][1]);
    SPI.transfer(font[90-char_count][2]);
    SPI.transfer(font[90-char_count][3]);
    SPI.transfer(font[90-char_count][4]);*/
    SPI.transfer(0);
  }
  while (char_count--);
  CS_HIGH
  SPI.endTransaction();
}

static void writeFont_I2C()
{
  byte char_count = 99;
  do
  {
    uint8_t* font_char = (uint8_t*)pgm_read_word(&(font[99-char_count]));
    Wire.beginTransmission(SSD1306_I2C_ADDR);
    Wire.write(SSD1306_I2C_DATA);
    Wire.write(pgm_read_byte_near(font_char+0));
    Wire.write(pgm_read_byte_near(font_char+1));
    Wire.write(pgm_read_byte_near(font_char+2));
    Wire.write(pgm_read_byte_near(font_char+3));
    Wire.write(pgm_read_byte_near(font_char+4));

    /*
    Wire.write(font[90 - char_count][0]);
    Wire.write(font[90 - char_count][1]);
    Wire.write(font[90 - char_count][2]);
    Wire.write(font[90 - char_count][3]);
    Wire.write(font[90 - char_count][4]);
    */
    Wire.write(0);
    Wire.endTransmission();
  }
  while (char_count--);
}


static void writeString_SPI(const char* string)
{
  int len = strlen(string);
  int i = 0;
  byte index; 

  DATA_MODE

  SPI.beginTransaction(spiconf);
  CS_LOW
  while (len--)
  {
    index = char_to_font_index(string[i++]);
    if (index == 255)
    {
      continue;
    }

    uint8_t* font_char = (uint8_t*)pgm_read_word(&(font[index]));
    SPI.transfer(pgm_read_byte_near(font_char+0));
    SPI.transfer(pgm_read_byte_near(font_char+1));
    SPI.transfer(pgm_read_byte_near(font_char+2));
    SPI.transfer(pgm_read_byte_near(font_char+3));
    SPI.transfer(pgm_read_byte_near(font_char+4));
    SPI.transfer(0);
  }
  CS_HIGH
  SPI.endTransaction();
}

static void writeString_I2C(const char* string)
{
  int len = strlen(string);
  int i = 0;
  byte index;

  while (len--)
  {
    index = char_to_font_index(string[i++]);
    if (index == 255)
    {
      continue;
    }

    uint8_t* font_char = (uint8_t*)pgm_read_word(&(font[index]));
    Wire.beginTransmission(SSD1306_I2C_ADDR);
    Wire.write(SSD1306_I2C_DATA);
    
    Wire.write(pgm_read_byte_near(font_char+0));
    Wire.write(pgm_read_byte_near(font_char+1));
    Wire.write(pgm_read_byte_near(font_char+2));
    Wire.write(pgm_read_byte_near(font_char+3));
    Wire.write(pgm_read_byte_near(font_char+4));
    Wire.write(0);
    Wire.endTransmission();
  }
}


static void writeText_SPI(const char* string, byte x, byte y)
{
  setWriteAddress_SPI(x,y);
  writeString_SPI(string);
}
static void writeText_I2C(const char* string, byte x, byte y)
{
  setWriteAddress_I2C(x, y);
  writeString_I2C(string);
}





void oled_setup(int width_, int height_)
{
  width = width_;
  height = height_;
  if (SPI_OLED)
  {
    setupOled_SPI(width_, height_);
  }
  else
  {
    setupOled_I2C(width_, height_);
  }
}

void oled_clear()
{
  if (SPI_OLED)
  {
    clearDisplay_SPI();
  }
  else
  {
    clearDisplay_I2C();
  }
}
void oled_setTestPattern()
{
  if (SPI_OLED)
  {
    setTestPattern_SPI();
  }
  else
  {
    setTestPattern_I2C();
  }
}
void oled_writeFont()
{
  if (SPI_OLED)
  {
    writeFont_SPI();
  }
  else
  {
    writeFont_I2C();
  }
}

void oled_setWriteAddress(byte x, byte y)
{
  if (SPI_OLED)
  {
    setWriteAddress_SPI(x, y);
  }
  else
  {
    setWriteAddress_I2C(x, y);
  }
}
void oled_writeString(const char* string)
{
  if (SPI_OLED)
  {
    writeString_SPI(string);
  }
  else
  {
    writeString_I2C(string);
  }
}

void oled_writeText(const char* string, byte x, byte y)
{
  if (SPI_OLED)
  {
    writeText_SPI(string, x, y);
  }
  else
  {
    writeText_I2C(string, x, y);
  }
}









// Unused, work in progress functions

static void initScroll_SPI()
{
  COMMAND_MODE
  
  SPI.beginTransaction(spiconf);
  CS_LOW
  SPI.transfer(0x26); // right horizontal scroll
  SPI.transfer(0x00); // dummy dum dum
  SPI.transfer(0x00); // starting page address?
  SPI.transfer(0x00); // time between scroll steps?
  SPI.transfer(0x07); // end page address?
  SPI.transfer(0x00); // dummy dum
  SPI.transfer(0xFF); // dum dummy
  CS_HIGH
  SPI.endTransaction();
}

static void startScroll_SPI()
{
  COMMAND_MODE
  
  SPI.beginTransaction(spiconf);
  CS_LOW
  SPI.transfer(0x2F);
  CS_HIGH
  SPI.endTransaction();
}

static void stopScroll_SPI()
{
  COMMAND_MODE
  
  SPI.beginTransaction(spiconf);
  CS_LOW
  SPI.transfer(0x2E);
  CS_HIGH
  SPI.endTransaction();
}

static void setWriteRect_I2C(byte x, byte y, byte lx, byte ly)
{
  if (y > 3)
  {
    y = 3;
  }
  Wire.beginTransmission(SSD1306_I2C_ADDR);
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0x21); // column ..
  Wire.write(x); // start address ..
  Wire.write(x+lx); // and end address
  Wire.write(SSD1306_I2C_COMMAND);
  Wire.write(0x22); // page ..
  Wire.write(y); // start address ..
  Wire.write(y+ly); // and end address
  Wire.endTransmission();
}
