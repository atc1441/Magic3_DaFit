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
  spiCommand(0x11);

  delay(120);

  spiCommand(0x36);
  spiWrite(0);

  spiCommand(0x3a);
  spiWrite(5);

  spiCommand(0xb2);
  spiWrite(0xB);
  spiWrite(0xB);
  spiWrite(0x33);
  spiWrite(0);
  spiWrite(0x33);

  spiCommand(0xB7);
  spiWrite(0x11);

  spiCommand(0xBB);
  spiWrite(0x35);


  spiCommand(0xC0);
  spiWrite(0x2c);


  spiCommand(0xC2);
  spiWrite(1);
  spiCommand(0xC3);
  spiWrite(8);
  spiCommand(0xC4);
  spiWrite(0x20);
  spiCommand(0xC6);
  spiWrite(0x1f);

  spiCommand(0xd0);
  spiWrite(0xa4);
  spiWrite(0xa1);

  spiCommand(0xe0);

  spiWrite(0xF0);
  spiWrite(4);
  spiWrite(0xA);
  spiWrite(0xA);
  spiWrite(8);
  spiWrite(0x25);
  spiWrite(0x33);
  spiWrite(0x27);
  spiWrite(0x3D);
  spiWrite( 0x38);
  spiWrite( 0x14);
  spiWrite( 0x14);
  spiWrite( 0x25);
  spiWrite( 0x2A);

  spiCommand(0xe1);

  spiWrite(0xF0);
  spiWrite(5);
  spiWrite(8);
  spiWrite(7);
  spiWrite(6);
  spiWrite(2);
  spiWrite(0x26);
  spiWrite(0x32);
  spiWrite(0x3D);
  spiWrite( 0x3A);
  spiWrite( 0x16);
  spiWrite( 0x16);
  spiWrite( 0x26);
  spiWrite( 0x2C);

  spiCommand(0x21);
  spiCommand(0x35);
  spiWrite(0);

  spiCommand(0x44);
  spiWrite(0x25);
  spiWrite(0);
  spiWrite(0);
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
          case 0x84://Ä
            c = 0x8E;
            break;
          case 0xA4://ä
            c = 0x84;
            break;
          case 0x96://Ö
            c = 0x99;
            break;
          case 0xB6://ö
            c = 0x94;
            break;
          case 0x9C://Ü
            c = 0x9A;
            break;
          case 0xBC://ü
            c = 0x81;
            break;
          case 0x9F://ß
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
