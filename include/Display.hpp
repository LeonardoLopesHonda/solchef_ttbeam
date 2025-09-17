#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C  // Endereço I2C padrão para SSD1306

class Display 
{
    public:
        Display(int sdaPin, int sclPin, int screenWidth, int screenHeight, int OLED_RESET);
        void Start();
        void PrintLine(uint8_t line, const char* texto);
        void Clear();

    private:
        Adafruit_SSD1306 display;
        int sdaPin;
        int sclPin;
        int screenWidth;
        int screenHeight;
};

#endif