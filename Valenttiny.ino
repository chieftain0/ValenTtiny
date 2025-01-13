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
#include <util/delay.h>
#define NUM_LEDS 13
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_BIT 0

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

    bitSet(LED_DDR, LED_BIT);
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
        Rainbow(10);
    }
    else if (state == 1)
    {
        showColor(255 * brightness * brightness, 0, 0);
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
    return;
}

void Rainbow(unsigned long wait)
{
    static uint8_t firstHue = 0;
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate >= wait)
    {
        lastUpdate = millis();
        firstHue += 1;

        uint8_t colors[NUM_LEDS][3];

        for (int i = 0; i < NUM_LEDS; i++)
        {
            uint8_t hue = firstHue + (i * 255 / NUM_LEDS);
            HSVtoRGB(hue, 255, 255 * brightness, colors[i][0], colors[i][1], colors[i][2]);
        }

        cli();
        for (int i = 0; i < NUM_LEDS; i++)
        {
            sendColor(colors[i][0], colors[i][1], colors[i][2]);
        }
        sei();
    }
}
void HSVtoRGB(uint8_t h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b)
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
void sendColor(uint8_t r, uint8_t g, uint8_t b)
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

void showColor(uint8_t r, uint8_t g, uint8_t b)
{

    cli();
    for (int p = 0; p < NUM_LEDS; p++)
    {
        sendColor(r, g, b);
    }
    sei();
    _delay_us((RES / 1000UL) + 1);
}