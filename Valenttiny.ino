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

// LEDs
#include <FastLED.h>
#define NUM_LEDS 13
#define LED_PIN 0
CRGB LED[NUM_LEDS];

// Buttons
#define BUTTON_1_PIN 1
#define BUTTON_2_PIN 2
#include <EncButton.h>
Button button1(BUTTON_1_PIN);
Button button2(BUTTON_2_PIN);

int state = 0;
double brightness = 0.5;
#define NUM_STATES 8

void setup()
{
    if (state >= NUM_STATES || state < 0)
    {
        state = 0;
    }
    if (brightness > 1 || brightness < 0)
    {
        brightness = 0.5;
    }

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(LED, NUM_LEDS);
    FastLED.setBrightness(brightness * 255);
    return;
}

void loop()
{

    button1.tick();
    button2.tick();

    if (button1.click())
    {
        state = (state + 1) % NUM_STATES;
    }

    if (button2.click())
    {
        state = (state + NUM_STATES - 1) % NUM_STATES;
    }

    if (button1.step())
    {
        brightness++;
        if (brightness > 1)
        {
            brightness = 1;
        }
        FastLED.setBrightness(brightness * 255);
    }

    if (button2.step())
    {
        brightness--;
        if (brightness < 0)
        {
            brightness = 0;
        }
        FastLED.setBrightness(brightness * 255);
    }

    if (state == 0)
    {
        Rainbow(10);
    }
    else if (state == 1)
    {
        // Red
        fill_solid(LED, NUM_LEDS, CRGB::Red);
    }
    else if (state == 2)
    {
        // Yellow
        fill_solid(LED, NUM_LEDS, CRGB::Yellow);
    }
    else if (state == 3)
    {
        // Green
        fill_solid(LED, NUM_LEDS, CRGB::Green);
    }
    else if (state == 4)
    {
        // Cyan
        fill_solid(LED, NUM_LEDS, CRGB::Cyan);
    }
    else if (state == 5)
    {
        // Blue
        fill_solid(LED, NUM_LEDS, CRGB::Blue);
    }
    else if (state == 6)
    {
        // Violet
        fill_solid(LED, NUM_LEDS, CRGB::Violet);
    }
    else if (state == 7)
    {
        // White
        fill_solid(LED, NUM_LEDS, CRGB::White);
    }

    FastLED.show();
    return;
}

void Rainbow(unsigned long wait)
{
    static uint8_t firstHue = 0;
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate >= wait)
    {
        lastUpdate = millis();
        fill_rainbow(LED, NUM_LEDS, firstHue, 255 / NUM_LEDS);
        firstHue += 1;
    }

    return;
}

void goodDelay(unsigned long wait)
{
    unsigned long start = millis();
    while (millis() - start < wait)
    {
        yield();
    }
    return;
}