#include <stdint.h>
/*
    Valenttiny
    Copyright (C) 2024  Behruz Erkinov

    This library is part of Valenttiny.

    Valenttiny is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Valenttiny is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Valenttiny.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/**
 * Sets all pixels on the NeoPixel strip to a rainbow pattern.
 *
 * This function shifts the starting hue of the rainbow pattern on each
 * call, and updates the color of each pixel in the strip accordingly.
 *
 * @param pixels A reference to an Adafruit_NeoPixel object representing
 *               the LED strip.
 * @param brightness The brightness to apply to the rainbow colors.
 * @param wait The time (in milliseconds) to wait between updates of the
 *             rainbow pattern.
 */
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

/**
 * Sets all pixels on the NeoPixel strip to a specified RGB color.
 *
 * This function iterates over each pixel on the NeoPixel strip and sets
 * its color to the specified red, green, and blue values. After updating
 * all pixels, the function refreshes the strip to display the new colors.
 *
 * @param pixels A reference to an Adafruit_NeoPixel object representing
 *               the LED strip.
 * @param r      The red color component (0-255).
 * @param g      The green color component (0-255).
 * @param b      The blue color component (0-255).
 */
void applyColour(Adafruit_NeoPixel &pixels, uint8_t r, uint8_t g, uint8_t b)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();

    return;
}

/**
 * Sets all pixels on the NeoPixel strip to a specified 32-bit color.
 *
 * This function iterates over each pixel on the NeoPixel strip and sets
 * its color to the specified 32-bit color value. After updating all pixels,
 * the function refreshes the strip to display the new colors.
 *
 * @param pixels A reference to an Adafruit_NeoPixel object representing
 *               the LED strip.
 * @param colour  The 32-bit color value to apply to all pixels.
 */
void applyColour(Adafruit_NeoPixel &pixels, uint32_t colour)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.gamma32(colour));
    }
    pixels.show();

    return;
}