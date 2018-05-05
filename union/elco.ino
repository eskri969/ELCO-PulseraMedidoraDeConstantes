#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);


#define D8 15
#define D3 0
#define DELAY 20
#define TEMP_MUESTRAS 50

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#define LOGO_FULL_GLCD_HEIGHT 32
#define LOGO_FULL_GLCD_WIDTH  128


byte hr_cont, spo2_cont;

static const unsigned char PROGMEM logo128_logo[] =
{
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000011, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00011111, B11111111, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000100, B01111111, B11111000, B00000000, B00000000, B00000000, B00000000, B00100011, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000101, B11111111, B11111000, B01111100, B00111111, B11111000, B00000000, B00101111, B11111111, B10000011, B11111111, B10000011, B11111111, B10000011, B11111111, B11000000,
  B00000101, B11111100, B01111000, B10001110, B01000111, B11111100, B00000000, B00101111, B11000000, B00000100, B01111111, B11000100, B01111111, B11000100, B01111111, B11100000,
  B00000111, B11111100, B01111000, B10111110, B01011111, B11111110, B00000000, B00111111, B11000000, B00000101, B11111111, B11100101, B11111111, B11100101, B11111111, B11000000,
  B00000111, B11111100, B01111000, B10111110, B01011111, B10001110, B00000000, B00111111, B11000000, B00000101, B11111000, B11100101, B11110001, B11100101, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111000, B11100111, B11110001, B11100111, B11110000, B00000000,
  B00000111, B11111111, B11110000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B11100111, B11111111, B11000000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B11000111, B11111111, B11100000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B10000111, B11111111, B11000000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111000, B11100111, B11111111, B11000111, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B10000111, B11111000, B11100111, B11110111, B11100111, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11100000,
  B00000111, B11111111, B11110000, B11111110, B00111111, B11111100, B00100000, B00011111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11100000,
  B00000011, B11111111, B11100000, B01111100, B00011111, B11111000, B00100000, B00001111, B11111111, B10000011, B11100000, B01000011, B11100000, B01000011, B11111111, B11000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B01110000, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B01010001, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11011001, B01000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B10001001, B01100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000111, B11111111, B11111111, B11111111, B11111111, B11111111, B10001011, B00111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11100000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00001110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
};


const unsigned char PROGMEM Logo_TFB [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xF3, 0x80, 0x01, 0x87, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0x00, 0x00, 0xDF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x7F, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFC, 0x00, 0x01, 0xFF, 0x8F, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x7C, 0x07, 0xFF, 0xFF, 0x83, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x3C, 0x0F, 0xFF, 0xFF, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x7C, 0x0F, 0xFF, 0xFF, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x78, 0x0F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x78, 0x00, 0x1F, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x78, 0x00, 0x0F, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x78, 0x00, 0x0F, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x70, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x70, 0x00, 0x1F, 0xFE, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x30, 0x1F, 0xFF, 0xFE, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x30, 0x1F, 0xFF, 0xFC, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x30, 0x3F, 0xFF, 0xFC, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM Logo_PhotonPedia [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xDF, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x67, 0x80, 0x6C, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x73, 0x80, 0xC8, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x61, 0xC1, 0xD8, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xE1, 0xC1, 0x90, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xC3, 0x30, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xC3, 0x60, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xC0, 0xC7, 0x40, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xC0, 0xC6, 0xC0, 0x18, 0x0E, 0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xC1, 0xC7, 0x80, 0x3E, 0x0E, 0x07, 0xC1, 0xC0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xC3, 0x8F, 0x00, 0x7F, 0x1E, 0x0F, 0xE3, 0xC0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xCF, 0x0E, 0x60, 0xF7, 0x3C, 0x1E, 0xE7, 0x8E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x0C, 0xE1, 0xF3, 0x6C, 0x3C, 0x6F, 0x9E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xE0, 0x1C, 0xE3, 0xC3, 0xCC, 0x78, 0xDB, 0xBE, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0x80, 0x1D, 0xFE, 0xC7, 0x8C, 0xF8, 0xF3, 0x67, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x80, 0x0D, 0x7D, 0xCF, 0x1F, 0xB9, 0xE3, 0xC7, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x80, 0x0F, 0x31, 0xFA, 0x1E, 0x3F, 0xC3, 0x83, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x80, 0x0E, 0x00, 0xF0, 0x1C, 0x1E, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x80, 0x0C, 0x00, 0x00, 0x18, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x70, 0x00, 0x7B, 0xEE, 0x66, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x48, 0x70, 0x70, 0x91, 0x00, 0x7B, 0xFF, 0x66, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x88, 0x48, 0x90, 0x89, 0xC0, 0xCF, 0x19, 0x6E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0x04, 0x88, 0x89, 0x07, 0xF8, 0xFB, 0xD9, 0x5A, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x84, 0x89, 0x01, 0xC0, 0xE6, 0x1B, 0xDF, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x07, 0x01, 0x00, 0xC7, 0xFF, 0xF3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x87, 0xFC, 0xF3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM Logo_ETSIT [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xF9, 0xFF, 0x8F, 0xE1, 0xCF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xF9, 0xFF, 0x9F, 0xE1, 0xCF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x38, 0x21, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x30, 0x01, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x30, 0x01, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x30, 0x01, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x38, 0x01, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x3C, 0x01, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x1F, 0x01, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xF8, 0x18, 0x07, 0xC1, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xF8, 0x18, 0x03, 0xE1, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x00, 0xF1, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x00, 0x77, 0xC3, 0xFF, 0x8E, 0x00, 0xE0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x00, 0x37, 0xC3, 0xFF, 0xCE, 0x00, 0xE0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x00, 0x37, 0xC3, 0xF0, 0x6F, 0x01, 0xE0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x00, 0x37, 0xC3, 0xF0, 0x6F, 0x01, 0xE0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x00, 0x37, 0xC3, 0xF0, 0x7F, 0x01, 0xE0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x20, 0x77, 0xC3, 0xF0, 0x7D, 0x83, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xF8, 0x18, 0x3F, 0xE7, 0xC3, 0xF0, 0x6D, 0x83, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xF8, 0x18, 0x3F, 0x87, 0xC3, 0xF0, 0x6D, 0x83, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x3F, 0xCC, 0x82, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x3F, 0x8C, 0xC6, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x30, 0x0C, 0xC6, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x30, 0x0C, 0x44, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x30, 0x0C, 0x6C, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x30, 0x0C, 0x6C, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x30, 0x0C, 0x28, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x30, 0x0C, 0x38, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x8E, 0x30, 0x0C, 0x38, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x30, 0x0C, 0x10, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM Logo_Politec [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x78, 0x01, 0xE0, 0x30, 0x67, 0xF9, 0xFE, 0x07, 0x84, 0x00, 0xCC, 0x03, 0xC0, 0x10, 0x00,
  0x00, 0x7E, 0x07, 0xF8, 0x30, 0x67, 0xF9, 0xFE, 0x1F, 0xE6, 0x00, 0xCC, 0x0F, 0xF0, 0x10, 0x00,
  0x00, 0x67, 0x0F, 0x3C, 0x30, 0x60, 0xC1, 0x80, 0x3C, 0xE7, 0x00, 0xCC, 0x1E, 0x70, 0x38, 0x00,
  0x00, 0x63, 0x1C, 0x0E, 0x30, 0x60, 0xC1, 0x80, 0x70, 0x27, 0x80, 0xCC, 0x38, 0x10, 0x38, 0x00,
  0x00, 0x63, 0x38, 0x07, 0x30, 0x60, 0xC1, 0x80, 0xE0, 0x07, 0xC0, 0xCC, 0x70, 0x00, 0x7C, 0x00,
  0x00, 0x63, 0x30, 0x03, 0x30, 0x60, 0xC1, 0x80, 0xC0, 0x06, 0xE0, 0xCC, 0xE0, 0x00, 0x6C, 0x00,
  0x00, 0x67, 0x30, 0x03, 0x30, 0x60, 0xC1, 0xFE, 0xC0, 0x06, 0x70, 0xCC, 0xE0, 0x00, 0xEE, 0x00,
  0x00, 0x7E, 0x30, 0x03, 0x30, 0x60, 0xC1, 0xFE, 0xC0, 0x06, 0x38, 0xCC, 0xE0, 0x00, 0xC6, 0x00,
  0x00, 0x78, 0x30, 0x03, 0x30, 0x60, 0xC1, 0x80, 0xC0, 0x06, 0x1C, 0xCC, 0xE0, 0x01, 0xC7, 0x00,
  0x00, 0x60, 0x30, 0x03, 0x30, 0x60, 0xC1, 0x80, 0xC0, 0x06, 0x0E, 0xCC, 0xE0, 0x01, 0xFF, 0x00,
  0x00, 0x60, 0x38, 0x07, 0x30, 0x60, 0xC1, 0x80, 0xE0, 0x06, 0x07, 0xCC, 0xF0, 0x03, 0xFF, 0x80,
  0x00, 0x60, 0x1C, 0x0E, 0x30, 0x60, 0xC1, 0x80, 0x70, 0x26, 0x03, 0xCC, 0x78, 0x13, 0x01, 0x80,
  0x00, 0x60, 0x0F, 0x3C, 0x30, 0x60, 0xC1, 0x80, 0x3C, 0xE6, 0x01, 0xCC, 0x3E, 0x77, 0x01, 0xC0,
  0x00, 0x60, 0x07, 0xF8, 0x3E, 0x60, 0xC1, 0xFE, 0x1F, 0xE6, 0x00, 0xCC, 0x1F, 0xF6, 0x00, 0xC0,
  0x00, 0x60, 0x01, 0xE0, 0x3E, 0x60, 0xC1, 0xFE, 0x07, 0x86, 0x00, 0x4C, 0x07, 0xC6, 0x00, 0xC0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



static const unsigned char PROGMEM logo16_pulse_0[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00001110, B01110000,
  B00001111, B11110000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_pulse_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00011100, B00111000,
  B00111110, B01111100,
  B00111111, B11111100,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_pulse_2[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B01111100, B00111110,
  B11111110, B01111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B01111111, B11111110,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_temp_1[] =
{
  B00000000, B00000000,
  B00000011, B11000000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_temp_2[] =
{
  B00000000, B00000000,
  B00000011, B11000000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_o2_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000001, B10000000,
  B00000011, B11000000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00111111, B11111100,
  B01111111, B11111110,
  B01111111, B11111110,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_o2_2[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00001110, B00000000,
  B00110001, B10000000,
  B01100000, B11000000,
  B01100000, B11001100,
  B01100000, B11010010,
  B01100000, B11000100,
  B00110001, B10001000,
  B00001110, B00011110,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_conect_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00001111,
  B00000000, B00001111,
  B00000000, B00001111,
  B00000011, B11001111,
  B00000011, B11001111,
  B00000011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_wifi_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00011111, B11111000,
  B01100000, B00000110,
  B10000000, B00000001,
  B00000000, B00000000,
  B00001111, B11110000,
  B00010000, B00001000,
  B00000000, B00000000,
  B00000001, B10000000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_server_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00111111, B11111100,
  B01100011, B11111110,
  B01111111, B11111110,
  B00000000, B00000000,
  B01111111, B11111110,
  B01100011, B11111110,
  B01111111, B11111110,
  B00000000, B00000000,
  B01111111, B11111110,
  B01100011, B11111110,
  B01111111, B11111110,
  B00000000, B00000000,
};

static const unsigned char PROGMEM logo16_error_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000001, B00000100,
  B00000011, B10001110,
  B00000111, B11011111,
  B00000011, B11111110,
  B00000001, B11111100,
  B00000000, B11111000,
  B00000001, B11111100,
  B00000011, B11111110,
  B00000111, B11011111,
  B00000011, B10001110,
  B00000001, B00000100,
  B00000000, B00000000,
};


/* POX GLOBAL VARIABLES */


MAX30105 pox;
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.

long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;

uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
int32_t bufferLength = 100; //data length

int pox_samples;

int HRAvg, spo2_avg, HR_final, spo2_final;
int hr2;


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temp[TEMP_MUESTRAS];
int pulseResult = 0;
float tempResult = 0;
int spo2Result = 0;

bool animacion = true;
bool supermenu = true;

int menu_number = 0;
int current_state = 0;
int inside_menu = 0;

int count_sleep_display = 0;
bool sleep_display = false;
bool error = false;


// WIFI VARIABLES

WiFiClient client;
char* ssid;      // SSID
char* password;      // Password
char* host;  // IP serveur - Server IP
int port;

void setup_wifi() {
  ssid = "JAZZTEL_H6Ng";      // SSID
  password = "dmjx43kcb4js";      // Password
  host = "192.168.1.128";  // IP serveur - Server IP
  port = 3000;
}



void setup_pox() {
  // Initialize sensor
  if (!pox.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  pox_samples = 0;

  byte ledBrightness = 0x2F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 163840

  pox.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings


  for (byte i = 0; i < 100; i++)
  {
    redBuffer[i] = bufferLength;
    irBuffer[i] = bufferLength;
  }

  HRAvg = 1;
  spo2_avg = 1;
  HR_final = spo2_final = 0;
  hr_cont = spo2_cont = 0;
}


void setup_display() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Clear the buffer.

  display.clearDisplay();
  display.drawBitmap(0, 0,  logo128_logo, 128, 32, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.drawBitmap(0, 0,  Logo_TFB, 128, 32, WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.drawBitmap(0, 0,  Logo_PhotonPedia, 128, 32, WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.drawBitmap(0, 0,  Logo_Politec, 128, 32, WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.drawBitmap(0, 0,  Logo_ETSIT, 128, 32, WHITE);
  display.display();
  delay(1000);
}



bool connect_wifi() {
  WiFi.begin(ssid, password);
  setDisplayMeasure();
  display.println("Conectando con\nWiFi...");
  display.drawBitmap(110, 15,  logo16_wifi_1, 16, 16, WHITE);
  display.display();
  delay(5000);
  return (WiFi.status() == WL_CONNECTED);
}


bool connect_server() {
  client.connect(host, port);
  setDisplayMeasure();
  display.println("Conectando con\nServidor...");
  display.drawBitmap(110, 15,  logo16_server_1, 16, 16, WHITE);
  display.display();
  delay(5000);
  return (client.available());
}


void setup() {
  // BUTTON INTERRUPTIONS
  pinMode(D8, INPUT_PULLUP);
  //digitalWrite(D8, HIGH);
  attachInterrupt(digitalPinToInterrupt(D3), menu_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(D8), select_button, RISING);
  Serial.begin(9600);
  //Wire.setClock(100000);
  //Wire.useInternalPullups(false);
  setup_wifi();
  setup_display();
  setup_pox();
  mlx.begin();//set up temp sensor
}


void menu_button() {
  if (sleep_display) {
    display.ssd1306_command(SSD1306_DISPLAYON);
    sleep_display = false;
    return;
  }

  if (inside_menu == 0) {
    //Serial.print("\nBoton de menu presionado");
    if (supermenu) {
      if (menu_number == 0) {
        //Serial.print("\nEntra de 0 a 1");
        menu_number = 1;
        current_state = 0;
      }
      else {
        //Serial.print("\nEntra de 1 a 0");
        menu_number = 0;
        current_state = 0;
      }
    }
    else {
      menu_number = menu_number + 1;
      current_state = 0;
    }
  }
}

void select_button() {
  if (sleep_display) {
    display.ssd1306_command(SSD1306_DISPLAYON);
    sleep_display = false;
    return;
  }

  Serial.print("\nBoton de seleccion presionado");
  if ((menu_number == 0) && (current_state == 1) && (inside_menu == 1)) {
    current_state = 0;
    inside_menu = 0;
  }
  else {
    inside_menu = 1;
  }
}

void setDisplayMenu() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
}

void setDisplayMeasure() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void setDisplayResult() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
}

void sleepDisplay() {
  if (!sleep_display) {
    delay(1);
    count_sleep_display++;
  }
  if (count_sleep_display >= 20000) {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    count_sleep_display = 0;
    sleep_display = true;
  }
}


void midePulso() {
  bool end_measure = false;
  int c_anim = 0;
  while (!end_measure) {
    c_anim++;
    //Serial.println(c_anim);
    if (c_anim >= 3000) {
      error = true;
      display.clearDisplay();
      setDisplayMeasure();
      display.println("Error midiendo\npulso");
      display.drawBitmap(110, 15,  logo16_error_1, 16, 16, WHITE);
      display.display();
      delay(3000);
      return;
    }
    pox.check();
    if (pox.available()) {
      shift_pox_array(irBuffer, pox.getIR());
      shift_pox_array(redBuffer,  pox.getRed());
      pox.nextSample();
      pox_samples++;


      if (checkForBeat(irBuffer[100]) == true) {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);
        Serial.print(", fuera: ");
        Serial.println(beatsPerMinute);
        if (beatsPerMinute < 200 && beatsPerMinute > 30)
        {
          hr_cont++;
          HRAvg = HRAvg + beatsPerMinute;

          if (hr_cont >= RATE_SIZE) {
            pulseResult = HRAvg / RATE_SIZE;
            Serial.print(", HR: ");
            Serial.println(pulseResult);
            HRAvg = 0;
            hr_cont = 0;
            end_measure = true;
            reset_pox_buffer(irBuffer);
            reset_pox_buffer(redBuffer);
          }
        }
      }

    }
    if (((c_anim % 50) == 0) and (animacion == true)) {
      setDisplayMeasure();
      display.println("Midiendo\npulso . .");
      display.drawBitmap(110, 15,  logo16_pulse_1, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      //delay(500);
      animacion = !animacion;
    }
    else if (((c_anim % 50) == 0) and (animacion == false)) {
      setDisplayMeasure();
      display.println("Midiendo\npulso . . .");
      display.drawBitmap(110, 15,  logo16_pulse_2, 16, 16, WHITE);
      display.display();
      //delay(500);
      display.clearDisplay();
      animacion = !animacion;
    }
    delay(1);
  }
  //animacion = true;
  setDisplayResult();
  //spo2Result = 92;
  display.print(pulseResult);
  //display.setTextSize(1);
  display.println(" lpm");
  display.drawBitmap(110, 15,  logo16_pulse_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}







void mideTemp() {
  tempResult = 0;
  for (int i = 0; i <= (TEMP_MUESTRAS - 1); i++) {

    if (((i % 5) == 0) and (animacion == true)) {
      setDisplayMeasure();
      display.println("Midiendo\ntemperatura . .");
      display.drawBitmap(110, 15,  logo16_temp_1, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      animacion = !animacion;
    }
    else if ((((i % 5) == 0) or (i == (TEMP_MUESTRAS - 1)) ) and (animacion == false)) {
      setDisplayMeasure();
      display.println("Midiendo\ntemperatura . . .");
      display.drawBitmap(110, 15,  logo16_temp_2, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      animacion = !animacion;
    }

    temp[i] = mlx.readObjectTempC();
    //Serial.print("Temp Array = "); Serial.println(temp[i]);

    tempResult = tempResult + temp[i];
    //
    //    if (i == (TEMP_MUESTRAS - 1)) {
    //      tempResult = (tempResult / TEMP_MUESTRAS);
    //      //Serial.print("Temp Result = "); Serial.println(tempResult);
    //      //Serial.println();
    //    }
    delay(100);
  }
  tempResult = (tempResult / TEMP_MUESTRAS);
  Serial.print("Temp Result = "); Serial.println(tempResult);
  if ((tempResult < 27) or (tempResult > 43)) {
    error = true;
    display.clearDisplay();
    setDisplayMeasure();
    display.println("Error midiendo\ntemperatura");
    display.drawBitmap(110, 15,  logo16_error_1, 16, 16, WHITE);
    display.display();
    delay(3000);
    return;
  }
  animacion = true;

  setDisplayResult();
  display.print(tempResult);
  display.print((char)247); //degree symbol
  display.println("C");
  display.drawBitmap(110, 15,  logo16_temp_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

}



void mideSPO2() {
  int c_anim = 0;
  bool end_measure = false;
  while (!end_measure) {
    c_anim++;
    Serial.println(c_anim);
    if (c_anim >= 3000) {
      error = true;
      display.clearDisplay();
      setDisplayMeasure();
      display.println("Error midiendo\nSpO2");
      display.drawBitmap(110, 15,  logo16_error_1, 16, 16, WHITE);
      display.display();
      delay(3000);
      return;
    }
    pox.check();
    if (pox.available()) {
      shift_pox_array(irBuffer, pox.getIR());
      shift_pox_array(redBuffer,  pox.getRed());
      pox.nextSample();
      pox_samples++;
      maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
      Serial.print(", fuera: ");
      Serial.print(spo2);
      Serial.print(", : ");
      Serial.println(spo2_cont);
      if (spo2 > 90 && spo2 < 100) {
        Serial.print(", dentro: ");
        Serial.print(spo2);
        Serial.print(", : ");
        Serial.println(spo2_cont);
        spo2_cont = spo2_cont + 1;
        spo2_avg = spo2_avg + spo2;
        if (spo2_cont >= RATE_SIZE) {
          spo2Result = spo2_avg / RATE_SIZE;
          Serial.print(", HR: ");
          Serial.println(HR_final);
          Serial.print("SPO2: ");
          Serial.println(spo2Result);
          spo2_avg = 0;
          spo2_cont = 0;
          pox_samples = 0;
          end_measure = true;
          reset_pox_buffer(irBuffer);
          reset_pox_buffer(redBuffer);
        }
      }
    }
    if (((c_anim % 50) == 0) and (animacion == true)) {
      setDisplayMeasure();
      display.println("Midiendo SpO2 . .");
      display.drawBitmap(110, 15,  logo16_o2_1, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      //delay(500);
      animacion = !animacion;
    }
    else if (((c_anim % 50) == 0) and (animacion == false)) {
      setDisplayMeasure();
      display.println("Midiendo SpO2 . . .");
      display.drawBitmap(110, 15,  logo16_o2_2, 16, 16, WHITE);
      display.display();
      //delay(500);
      display.clearDisplay();
      animacion = !animacion;
    }
    delay(1);
  }
  animacion = true;
  setDisplayResult();
  //spo2Result = 92;
  display.print(spo2Result);
  //display.setTextSize(1);
  display.println("%");
  display.drawBitmap(110, 15,  logo16_o2_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}


void mideMedidas() {
  midePulso();
  if (error) {
    return;
  }
  mideTemp();
  if (error) {
    return;
  }
  mideSPO2();
}



void showResults() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Pulso: ");
  display.print(pulseResult);
  display.println(" lpm");
  display.setCursor(0, 10);
  display.print("Temperatura: ");
  display.print(tempResult);
  display.print((char)247); //degree symbol
  display.println("C");
  display.setCursor(0, 20);
  display.print("SpO2: ");
  display.print(spo2Result);
  display.println("%");
  display.display();
  delay(5000);
  display.clearDisplay();
}


#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
void shift_pox_array(uint16_t *pox_arr, uint16_t new_sample)
#else
void shift_pox_array(uint32_t *pox_arr, uint32_t new_sample)
#endif
{
  byte i;
  for (i = 0; i < 100; i++) {
    *(pox_arr + i ) = *(pox_arr + i + 1);
  }
  *(pox_arr + i) = new_sample;

}


void reset_pox_buffer(uint32_t *pox_arr)
{
  byte i;
  for (i = 0; i <= 100; i++) {
    *(pox_arr + i ) = 1;
  }
}



void loop() {

  if (menu_number == 0) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        supermenu = false;


        //while(inside_menu){
        if (WiFi.status() != WL_CONNECTED ) {
          Serial.println("Entra en WIFI");
          connect_wifi();
          Serial.println("WIFI connection");
        }
        if (!client.available()) {

          Serial.println("Entra en Server");
          connect_server();
          Serial.println("Server connection");
        }
        if (WiFi.status() == WL_CONNECTED &&  client.available()) {
          setDisplayMeasure();
          display.println("Comenzando nuevas\nmedidas");
          display.display();
          delay(1500);

          Serial.println("connected");

          //          midePulso();
          //          mideTemp();
          //          mideSPO2();
          mideMedidas();
          if ((!error) and (WiFi.status() == WL_CONNECTED &&  client.available())) {
            client.print(String(pulseResult) + ":" + String(tempResult) + ":" + String(spo2Result) + "\r\n");
            showResults();
          }
          else{
            error = false;
            display.clearDisplay();
            setDisplayMeasure();
            display.println("Medidas no\nregistradas");
            //display.drawBitmap(110, 15,  logo16_error_1, 16, 16, WHITE);
            display.display();
            delay(3000);
          }

          //current_state = 0;
          supermenu = true;
          //inside_menu = 0;
        }
      }

      //do nothing -> sleep Display
      sleepDisplay();

    } else {
      error = false;
      current_state = 1;
      count_sleep_display = 0;
      WiFi.disconnect(true);
      setDisplayMenu();
      display.println("Auto");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 1) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        menu_number = menu_number + 1;
        current_state = 0;
        supermenu = false;
        inside_menu = 0;
      }
      //do nothing -> sleep Display
      sleepDisplay();
    } else {
      error = false;
      current_state = 1;
      count_sleep_display = 0;
      setDisplayMenu();
      display.println("Manual");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 2) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        midePulso();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing -> sleep Display
      sleepDisplay();
    } else {
      error = false;
      current_state = 1;
      count_sleep_display = 0;
      setDisplayMenu();
      display.println("Pulso");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 3) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        mideTemp();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing -> sleep Display
      sleepDisplay();
    } else {
      error = false;
      current_state = 1;
      count_sleep_display = 0;
      setDisplayMenu();
      display.println("Temperat.");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 4) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        mideSPO2();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing -> sleep Display
      sleepDisplay();
    } else {
      error = false;
      current_state = 1;
      count_sleep_display = 0;
      setDisplayMenu();
      display.println("SpO2");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 5) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        menu_number = 1;
        current_state = 0;
        supermenu = true;
        inside_menu = 0;
      }
      //do nothing -> sleep Display
      sleepDisplay();
    } else {
      error = false;
      current_state = 1;
      count_sleep_display = 0;
      setDisplayMenu();
      display.println("Exit");
      //display.drawBitmap(110, 0,  logo16_conect_2, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
    }
  }


  else if (menu_number > 5) {
    menu_number = 2;
  }
  else {
    setDisplayMenu();
    display.println("Raro raro");
    display.display();
    delay(200);
    display.clearDisplay();
  }
}



