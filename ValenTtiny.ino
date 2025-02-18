/*
██╗░░░██╗░█████╗░██╗░░░░░███████╗███╗░░██╗████████╗████████╗██╗███╗░░██╗██╗░░░██╗
██║░░░██║██╔══██╗██║░░░░░██╔════╝████╗░██║╚══██╔══╝╚══██╔══╝██║████╗░██║╚██╗░██╔╝
╚██╗░██╔╝███████║██║░░░░░█████╗░░██╔██╗██║░░░██║░░░░░░██║░░░██║██╔██╗██║░╚████╔╝░
░╚████╔╝░██╔══██║██║░░░░░██╔══╝░░██║╚████║░░░██║░░░░░░██║░░░██║██║╚████║░░╚██╔╝░░
░░╚██╔╝░░██║░░██║███████╗███████╗██║░╚███║░░░██║░░░░░░██║░░░██║██║░╚███║░░░██║░░░
░░░╚═╝░░░╚═╝░░╚═╝╚══════╝╚══════╝╚═╝░░╚══╝░░░╚═╝░░░░░░╚═╝░░░╚═╝╚═╝░░╚══╝░░░╚═╝░░░

    ValenTtiny
    Copyright (C) 2024-2025  Behruz Erkinov

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
#include <util/delay.h>
#define NUM_LEDS 11
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_BIT 0

// Buttons
#define BUTTON_1_PIN 1
#define BUTTON_2_PIN 2
#include <EncButton.h>
Button button1(BUTTON_1_PIN);
Button button2(BUTTON_2_PIN);

// Internal
int state = 0;
double brightness = 0.2;
#define NUM_STATES 8

void setup()
{
    bitSet(LED_DDR, LED_BIT);
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
        brightness = brightness + 0.1;
        if (brightness > 1)
        {
            brightness = 1;
        }
    }

    if (button2.step())
    {
        brightness = brightness - 0.1;
        if (brightness < 0)
        {
            brightness = 0;
        }
    }

    if (state == 0)
    {
        Rainbow(10, brightness);
    }
    else if (state == 1)
    {
        showColor(255 * brightness, 0, 0);
    }
    else if (state == 2)
    {
        showColor(255 * brightness, 255 * brightness, 0);
    }
    else if (state == 3)
    {
        showColor(0, 255 * brightness, 0);
    }
    else if (state == 4)
    {
        showColor(0, 255 * brightness, 255 * brightness);
    }
    else if (state == 5)
    {
        showColor(0, 0, 255 * brightness);
    }
    else if (state == 6)
    {
        showColor(255 * brightness, 0, 255 * brightness);
    }
    else if (state == 7)
    {
        showColor(255 * brightness, 255 * brightness, 255 * brightness);
    }
}

/**
 * @brief Set the LEDs to a rainbow pattern
 *
 * @param[in] wait_ms Delay in milliseconds between each update
 * @param[in] brightness Brightness of the LEDs (0.0 - 1.0)
 */
inline void Rainbow(unsigned long wait_ms, double brightness)
{
    static uint8_t firstHue = 0;
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate >= wait_ms)
    {
        lastUpdate = millis();
        firstHue += 1;

        uint8_t colors[NUM_LEDS][3];
        for (int i = 0; i < NUM_LEDS; i++)
        {
            uint8_t hue = firstHue + (i * 255 / NUM_LEDS);
            HSVtoRGB(hue, 255, 255, colors[i][0], colors[i][1], colors[i][2]);

            colors[i][0] = colors[i][0] * brightness;
            colors[i][1] = colors[i][1] * brightness;
            colors[i][2] = colors[i][2] * brightness;
        }

        cli();
        for (int i = 0; i < NUM_LEDS; i++)
        {
            sendColor(colors[i][0], colors[i][1], colors[i][2]);
        }
        sei();
    }
}
/**
 * @brief Converts HSV color to RGB color
 *
 * @param[in] h Hue in range of 0-255
 * @param[in] s Saturation in range of 0-255
 * @param[in] v Value in range of 0-255
 *
 * @param[in,out] r Red
 * @param[in,out] g Green
 * @param[in,out] b Blue
 *
 * @return None
 */
inline void HSVtoRGB(uint8_t h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b)
{
    uint8_t region = h / 43;
    uint8_t remainder = (h - (region * 43)) * 6;

    uint8_t p = (v * (255 - s)) >> 8;
    uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    default:
        r = v;
        g = p;
        b = q;
        break;
    }
}

// credits to https://github.com/bigjosh/SimpleNeoPixelDemo
#define T1H 900
#define T1L 600
#define T0H 400
#define T0L 900

#define RES 250000
#define NS_PER_SEC (1000000000L)
#define CYCLES_PER_SEC (F_CPU)
#define NS_PER_CYCLE (NS_PER_SEC / CYCLES_PER_SEC)
#define NS_TO_CYCLES(n) ((n) / NS_PER_CYCLE)

/**
 * @brief Send a single bit over the WS2812B protocol
 *
 * @details This function sends a single bit over the WS2812B protocol using
 *          bit-banging. It takes a boolean value as argument and sends a
 *          logical 1 or 0 accordingly. The function uses the timing
 *          definitions in the header file to generate the correct timing
 *          for the protocol.
 *
 * @param[in] bitVal The value of the bit to be sent (true for 1, false for 0)
 *
 * @return None
 */
inline void sendBit(bool bitVal)
{
    if (bitVal)
    {
        asm volatile(
            "sbi %[port], %[bit] \n\t"
            ".rept %[onCycles] \n\t"
            "nop \n\t"
            ".endr \n\t"
            "cbi %[port], %[bit] \n\t"
            ".rept %[offCycles] \n\t"
            "nop \n\t"
            ".endr \n\t" ::
                [port] "I"(_SFR_IO_ADDR(LED_PORT)),
            [bit] "I"(LED_BIT),
            [onCycles] "I"(NS_TO_CYCLES(T1H) - 2),
            [offCycles] "I"(NS_TO_CYCLES(T1L) - 2)

        );
    }
    else
    {
        asm volatile(
            "sbi %[port], %[bit] \n\t"
            ".rept %[onCycles] \n\t"
            "nop \n\t"
            ".endr \n\t"
            "cbi %[port], %[bit] \n\t"
            ".rept %[offCycles] \n\t"
            "nop \n\t"
            ".endr \n\t" ::
                [port] "I"(_SFR_IO_ADDR(LED_PORT)),
            [bit] "I"(LED_BIT),
            [onCycles] "I"(NS_TO_CYCLES(T0H) - 2),
            [offCycles] "I"(NS_TO_CYCLES(T0L) - 2)

        );
    }
}

/**
 * @brief Sends the specified RGB color to a single LED.
 *
 * This function transmits the RGB values to an LED by sending each color
 * component bit by bit. It is crucial that the function executes with precise
 * timing to ensure the LED correctly interprets the data.
 *
 * @param[in] r The red component of the color (0-255).
 * @param[in] g The green component of the color (0-255).
 * @param[in] b The blue component of the color (0-255).
 *
 * @return None
 *
 * @warning This function is time-sensitive down to the nanosecond. Ensure that
 * no additional operations are performed when passing the parameters to avoid
 * timing issues.
 */
inline void sendColor(uint8_t r, uint8_t g, uint8_t b)
{
    for (uint8_t bit = 0; bit < 8; bit++)
    {
        sendBit(bitRead(g, 7));
        g <<= 1;
    }
    for (uint8_t bit = 0; bit < 8; bit++)
    {
        sendBit(bitRead(r, 7));
        r <<= 1;
    }
    for (uint8_t bit = 0; bit < 8; bit++)
    {
        sendBit(bitRead(b, 7));
        b <<= 1;
    }
}

/**
 * @brief Sets all LEDs to the specified RGB color.
 *
 * @details This function sends the same color to all LEDs in the strip using the
 * provided red, green, and blue intensity values (0-255). It disables
 * interrupts while updating the LEDs to ensure precise timing, then re-enables
 * interrupts after sending the color. A small delay is added at the end to
 * ensure the LEDs latch the color data.
 *
 * @param[in] r Red intensity (0-255).
 * @param[in] g Green intensity (0-255).
 * @param[in] b Blue intensity (0-255).
 *
 * @return None
 */
inline void showColor(uint8_t r, uint8_t g, uint8_t b)
{
    cli();
    for (int p = 0; p < NUM_LEDS; p++)
    {
        sendColor(r, g, b);
    }
    sei();
    _delay_us((RES / 1000UL) + 1);
}