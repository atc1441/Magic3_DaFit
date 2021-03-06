#include "pinout.h"
#include "fast_spi.h"
#include "font57.h"

#define WIDTH 240
#define HEIGHT 280

#define LCD_BUFFER_SIZE (WIDTH*HEIGHT)*2
uint8_t lcd_buffer[LCD_BUFFER_SIZE + 4];

bool last_uni_char = false;
unsigned char last_char;

void setup() {
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  init_fast_spi();
  init_display();
}

int p_x = 0;
int p_y = 0;
long time_last = 0;
int fps_c = 0;
int fps = 0;
void loop() {
  uint16_t rand_color = random(0, 0xffff);
  memset(lcd_buffer, 0x00, LCD_BUFFER_SIZE);
  displayPrintln(60, 60, "MS:" + String(millis()), rand_color, 0x0000, 3);
  displayPrintln(40, 110, "ATCnetz.de", rand_color, 0x0000, 3);
  displayPrintln(40, 140, "Update", rand_color, 0x0000, 3);
  displayPrintln(40, 180, "240x280", rand_color, 0x0000, 3);
  displayPrintln(40, 230, "FPS: " + String(fps), rand_color, 0x0000, 3);
  startWrite_display();
  write_fast_spi(lcd_buffer, LCD_BUFFER_SIZE);
  endWrite_display();
  fps_c++;
  if (millis() - time_last > 1000) {
    time_last = millis();
    fps = fps_c;
    fps_c = 0;
  }
}

void init_display() {
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_RESET, OUTPUT);
  digitalWrite(LCD_CS , HIGH);
  digitalWrite(LCD_RS , HIGH);

  digitalWrite(LCD_RESET, HIGH);
  delay(20);
  digitalWrite(LCD_RESET, LOW);
  delay(100);
  digitalWrite(LCD_RESET, HIGH);
  delay(100);
  startWrite_display();
  spiCommand(0xDF);
  spiWrite(0x98);
  spiWrite(0x51);
  spiWrite(0xE9);
  spiCommand(0xDE);
  spiWrite(0x00);
  spiCommand(0xB7);
  spiWrite(0x34);
  spiWrite(0x9a);
  spiWrite(0x3d);
  spiWrite(0x17);
  spiCommand(0xC8);
  spiWrite( 0x3F);
  spiWrite( 0x34);
  spiWrite( 0x2F);
  spiWrite( 0x2E);
  spiWrite( 0x32);
  spiWrite( 0x39);
  spiWrite( 0x36);
  spiWrite( 0x36);
  spiWrite( 0x35);
  spiWrite( 0x33);
  spiWrite( 0x2D);
  spiWrite( 0x21);
  spiWrite( 0x18);
  spiWrite( 0x16);
  spiWrite( 0x10);
  spiWrite( 0x00);
  spiWrite( 0x34);
  spiWrite( 0x2F);
  spiWrite( 0x2E);
  spiWrite( 0x32);
  spiWrite( 0x39);
  spiWrite( 0x36);
  spiWrite( 0x36);
  spiWrite( 0x35);
  spiWrite( 0x33);
  spiWrite( 0x2D);
  spiWrite( 0x21);
  spiWrite( 0x18);
  spiWrite( 0x16);
  spiWrite( 0x10);
  spiWrite( 0x00);
  spiWrite( 0x00);
  spiCommand(0xB9);
  spiWrite(0x33);
  spiWrite(0x08);
  spiWrite(0xCC);
  spiCommand(0xBB);
  spiWrite( 0x40);
  spiWrite( 0x7A);
  spiWrite( 0x40);
  spiWrite( 0xE0);
  spiWrite( 0x6C);
  spiWrite( 0x60);
  spiWrite( 0x50);
  spiWrite( 0x70);
  spiCommand(0xbc);
  spiWrite(0x38);
  spiWrite(0x3c);
  spiCommand(0xc0);
  spiWrite(0x34);
  spiWrite(0x20);
  spiCommand(0xC1);
  spiWrite(0x12);
  spiCommand(0xC3);
  spiWrite( 0x08);
  spiWrite( 0x00);
  spiWrite( 0x0F);
  spiWrite( 0x1F);
  spiWrite( 0x1C);
  spiWrite( 0x7A);
  spiWrite( 0x7C);
  spiWrite( 0x94);
  spiWrite( 0x2C);
  spiCommand(0xC4);
  spiWrite( 0x00);
  spiWrite( 0x96);
  spiWrite( 0xEF);
  spiWrite( 0x1D);
  spiWrite( 0x0A);
  spiWrite( 0x16);
  spiWrite( 0xD0);
  spiWrite( 0x0E);
  spiWrite( 0x0A);
  spiWrite( 0x16);
  spiWrite( 0xD0);
  spiWrite( 0x0E);
  spiWrite( 0x0A);
  spiWrite( 0x16);
  spiWrite( 0x82);
  spiWrite( 0x00);
  spiWrite( 0x03);
  spiCommand(0xD0);
  spiWrite(0x04);
  spiWrite(0x3f);
  spiWrite(0x90);
  spiWrite(0x0e);
  spiWrite(0x00);
  spiWrite(0x03);
  spiCommand(0xDE);
  spiWrite(0x02);
  spiCommand(0xB8);
  spiWrite(0x1d);
  spiWrite(0xae);
  spiWrite(0x2f);
  spiWrite(0x08);
  spiWrite(0x34);
  spiCommand(0xC1);
  spiWrite(0x10);
  spiWrite(0x66);
  spiWrite(0x66);
  spiWrite(0x01);
  spiCommand(0xc4);
  spiWrite(0x7e);
  spiWrite(0x0f);
  spiCommand(0xDE);
  spiWrite(0x00);
  spiCommand(0x3a);
  spiWrite(0x05);// 12bpp = 0x03, 16bpp = 0x05
  spiCommand(0x11);
  delay(120);
  spiCommand(0x2a);
  spiWrite(0x00);
  spiWrite(0x00);
  spiWrite(0x00);
  spiWrite(0xef);
  spiCommand(0x2b);
  spiWrite(0x00);
  spiWrite(0x14);
  spiWrite(0x01);
  spiWrite(0x2b);
  spiCommand(0x29);
  spiCommand(0x35);
  spiWrite(0x00);
  setAddrWindowDisplay(0, 20, WIDTH, HEIGHT);
  endWrite_display();
}

void setAddrWindowDisplay(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  spiCommand(0x2A);
  spiWrite((x) >> 8);
  spiWrite(x);
  spiWrite((x + w - 1) >> 8);
  spiWrite(x + w - 1);
  spiCommand(0x2B);
  spiWrite((y) >> 8);
  spiWrite(y);
  spiWrite(((y + h - 1) ) >> 8);
  spiWrite((y + h - 1) );
  spiCommand(0x2C);
}

void spiCommand(uint8_t d) {
  digitalWrite(LCD_RS , LOW);
  write_fast_spi(&d, 1);
  digitalWrite(LCD_RS , HIGH);
}

void spiWrite(uint8_t d) {
  write_fast_spi(&d, 1);
}

void startWrite_display(void) {
  enable_spi(true);
  digitalWrite(LCD_CS , LOW);
}

void endWrite_display(void) {
  digitalWrite(LCD_CS , HIGH);
  enable_spi(false);
}

void set_pixel(int16_t x, int16_t y, uint16_t color) {
  lcd_buffer[2 * (x + (y * WIDTH))] = color >> 8;
  lcd_buffer[2 * (x + (y * WIDTH)) + 1] = color & 0xff;
}

bool drawChar(uint32_t x, uint32_t y, unsigned char c, uint16_t color, uint16_t bg, uint32_t size) {
  if (c < 32)return false;
  if (c >= 127) {
    if (!last_uni_char) {
      last_char = c;
      last_uni_char = true;
      return false;
    } else {
      last_uni_char = false;
      if (last_char == 0xC3) {
        switch (c) {
          case 0x84://??
            c = 0x8E;
            break;
          case 0xA4://??
            c = 0x84;
            break;
          case 0x96://??
            c = 0x99;
            break;
          case 0xB6://??
            c = 0x94;
            break;
          case 0x9C://??
            c = 0x9A;
            break;
          case 0xBC://??
            c = 0x81;
            break;
          case 0x9F://??
            c = 0x98;
            break;
          default:
            return false;
            break;
        }
      } else if (last_char == 0xF0 && c == 0x9F)
        c = 0x02;
      else
        return false;
    }
  }
  for (int8_t i = 0; i < 5; i++) {
    uint8_t line = font57[c * 5 + i];
    for (int8_t j = 0; j < 8; j++, line >>= 1) {
      if (line & 1) {
        displayRect(x + i * size, y + j * size, size, size, color);
      } else if (bg != color) {
        displayRect(x + i * size, y + j * size, size, size, bg);
      }
    }
  }
  if (bg != color) {
    displayRect(x + 5 * size, y, size, 8 * size, bg);
  }
  return true;
}

void displayPrintln(uint32_t x, uint32_t y, String text, uint16_t color, uint16_t bg, uint32_t size) {
  int tempPosition = 0;
  for (int f = 0; f < text.length(); f++)
  {
    if (x + (tempPosition * 6 * size) >= 234) {
      x = -(tempPosition * 6 * size);
      y += (8 * size);
    }
    if (drawChar(x + (tempPosition * 6 * size), y, text[f], color, bg, size)) {
      tempPosition++;
    }
  }
}

void displayRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color) {
  for (int _x = x; _x < (x + w); _x++) {
    for (int _y = y; _y < (y + h); _y++) {
      set_pixel(_x, _y,  color);
    }
  }
}
