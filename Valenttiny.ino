/*
██╗░░░██╗░█████╗░██╗░░░░░███████╗███╗░░██╗████████╗████████╗██╗███╗░░██╗██╗░░░██╗
██║░░░██║██╔══██╗██║░░░░░██╔════╝████╗░██║╚══██╔══╝╚══██╔══╝██║████╗░██║╚██╗░██╔╝
╚██╗░██╔╝███████║██║░░░░░█████╗░░██╔██╗██║░░░██║░░░░░░██║░░░██║██╔██╗██║░╚████╔╝░
░╚████╔╝░██╔══██║██║░░░░░██╔══╝░░██║╚████║░░░██║░░░░░░██║░░░██║██║╚████║░░╚██╔╝░░
░░╚██╔╝░░██║░░██║███████╗███████╗██║░╚███║░░░██║░░░░░░██║░░░██║██║░╚███║░░░██║░░░
░░░╚═╝░░░╚═╝░░╚═╝╚══════╝╚══════╝╚═╝░░╚══╝░░░╚═╝░░░░░░╚═╝░░░╚═╝╚═╝░░╚══╝░░░╚═╝░░░

    Valenttiny
    Copyright (C) 2024  Behruz Erkinov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


*/

#include <Arduino.h>

// EEPROM
#include <EEPROM.h>
#define STATE_ADDRESS 0
#define BRIGHTNESS_ADDRESS 2

// LEDs
#define NUMPIXELS 13
#define PIXELS_PIN PB0
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels(NUMPIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

// Buttons
#define BUTTON_1_PIN PB1
#define BUTTON_2_PIN PB2
#include <EncButton.h>
Button button1(BUTTON_1_PIN);
Button button2(BUTTON_2_PIN);

int state = 0;
double brightness = 0.5;
#define NUM_STATES 8

void setup()
{
    pixels.begin();
    pixels.clear();

    state = EEPROM.read(STATE_ADDRESS);
    brightness = EEPROM.read(BRIGHTNESS_ADDRESS);
    if (state >= NUM_STATES || state < 0)
    {
        state = 0;
    }
    if (brightness > 1 || brightness < 0)
    {
        brightness = 0.5;
    }

    return;
}

void loop()
{

    button1.tick();
    button2.tick();

    if (button1.click())
    {
        state = (state + 1) % NUM_STATES;

        EEPROM.write(STATE_ADDRESS, state);
    }

    if (button2.click())
    {
        state = (state + NUM_STATES - 1) % NUM_STATES;

        EEPROM.write(STATE_ADDRESS, state);
    }

    if (button1.step())
    {
        brightness++;
        if (brightness > 1)
        {
            brightness = 1;
        }
        EEPROM.write(BRIGHTNESS_ADDRESS, brightness);
    }

    if (button2.step())
    {
        brightness--;
        if (brightness < 0)
        {
            brightness = 0;
        }
        EEPROM.write(BRIGHTNESS_ADDRESS, brightness);
    }

    if (state == 0)
    {
        Rainbow(pixels, brightness, 10);
    }
    else if (state == 1)
    {
        // Red
        pixels.fill(pixels.Color(255 * brightness, 0, 0));
    }
    else if (state == 2)
    {
        // Yellow
        pixels.fill(pixels.Color(255 * brightness, 255 * brightness, 0));
    }
    else if (state == 3)
    {
        // Green
        pixels.fill(pixels.Color(0, 255 * brightness, 0));
    }
    else if (state == 4)
    {
        // Cyan
        pixels.fill(pixels.Color(0, 255 * brightness, 255 * brightness));
    }
    else if (state == 5)
    {
        // Blue
        pixels.fill(pixels.Color(0, 0, 255 * brightness));
    }
    else if (state == 6)
    {
        // Violet
        pixels.fill(pixels.Color(255 * brightness, 0, 255 * brightness));
    }
    else if (state == 7)
    {
        // White
        pixels.fill(pixels.Color(255 * brightness, 255 * brightness, 255 * brightness));
    }
    pixels.show();
    return;
}

void Rainbow(Adafruit_NeoPixel &pixels, double brightness, unsigned long wait)
{
    static long firstPixelHue = 0;
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate >= wait)
    {
        lastUpdate = millis();
        for (int i = 0; i < pixels.numPixels(); i++)
        {
            int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
            uint32_t rawColor = pixels.ColorHSV(pixelHue);

            uint8_t r = (rawColor >> 16) & 0xFF;
            uint8_t g = (rawColor >> 8) & 0xFF;
            uint8_t b = rawColor & 0xFF;

            r = static_cast<uint8_t>(r * brightness);
            g = static_cast<uint8_t>(g * brightness);
            b = static_cast<uint8_t>(b * brightness);

            pixels.setPixelColor(i, pixels.Color(r, g, b));
        }
        pixels.show();

        firstPixelHue += 256;
        if (firstPixelHue >= 5 * 65536)
        {
            firstPixelHue = 0;
        }
    }

    return;
}

void goodDelay(unsigned long ms)
{
    unsigned long start = millis();
    while (millis() - start < ms)
    {
        yield();
    }
    return;
}