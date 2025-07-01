/*
██╗░░░██╗░█████╗░██╗░░░░░███████╗███╗░░██╗████████╗████████╗██╗███╗░░██╗██╗░░░██╗
██║░░░██║██╔══██╗██║░░░░░██╔════╝████╗░██║╚══██╔══╝╚══██╔══╝██║████╗░██║╚██╗░██╔╝
╚██╗░██╔╝███████║██║░░░░░█████╗░░██╔██╗██║░░░██║░░░░░░██║░░░██║██╔██╗██║░╚████╔╝░
░╚████╔╝░██╔══██║██║░░░░░██╔══╝░░██║╚████║░░░██║░░░░░░██║░░░██║██║╚████║░░╚██╔╝░░
░░╚██╔╝░░██║░░██║███████╗███████╗██║░╚███║░░░██║░░░░░░██║░░░██║██║░╚███║░░░██║░░░
░░░╚═╝░░░╚═╝░░╚═╝╚══════╝╚══════╝╚═╝░░╚══╝░░░╚═╝░░░░░░╚═╝░░░╚═╝╚═╝░░╚══╝░░░╚═╝░░░

    ValenTtinyDucky
    Copyright (C) 2025  Behruz Erkinov

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

#define NUM_LEDs 11
uint8_t brightness = 3;

uint8_t button1 = 1;
uint8_t button2 = 2;

// HID
#include <EasyHID.h>

bool button_flag = 0;
void setup()
{
  pinMode(2, INPUT);
  bitSet(LED_DDR, LED_BIT);
}

void loop()
{
  if (digitalRead(button1) == 0 || digitalRead(button2) == 0)
  {
    FillUntil(0, 0, brightness, NUM_LEDs);
  }
  else
  {
    FillUntil(0, brightness, 0, NUM_LEDs);
  }
  if (digitalRead(button2) == 0 && digitalRead(button1) == 0 && button_flag == 0)
  {
    button_flag = 1;

    FillUntil(0, 0, 0, 11);

    HID.begin();
    FillUntil(brightness, 0, 0, 4);
    unsigned long start = millis();
    while (millis() <= start + 100)
    {
      HID.tick();
    }

    Keyboard.click(KEY_LEFT_GUI, KEY_R);
    FillUntil(brightness, 0, 0, 8);
    start = millis();
    while (millis() <= start + 350)
    {
      HID.tick();
    }

    Keyboard.print("cmd /c start https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    FillUntil(brightness, 0, 0, 11);
    Keyboard.click(KEY_ENTER);

    HID.end();
    FillUntil(0, brightness, 0, NUM_LEDs);
  }
  else if (digitalRead(button2) == 1 && digitalRead(button1) == 1 && button_flag == 1)
  {
    button_flag = 0;
    FillUntil(0, brightness, 0, NUM_LEDs);
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
 * @brief Fills the LEDs from the start of the strip up to a given number
 *        with the specified color.
 *
 * @details This function sends the same color to the first num_led LEDs in
 *          the strip using the provided red, green, and blue intensity values
 *          (0-255). It disables interrupts while updating the LEDs to ensure
 *          precise timing, then re-enables interrupts after sending the color.
 *          A small delay is added at the end to ensure the LEDs latch the
 *          color data.
 *
 * @param[in] r Red intensity (0-255).
 * @param[in] g Green intensity (0-255).
 * @param[in] b Blue intensity (0-255).
 * @param[in] num_led The number of LEDs to fill.
 *
 * @return None
 */
void FillUntil(uint8_t r, uint8_t g, uint8_t b, int num_led)
{
  cli();
  for (int p = 0; p < num_led; p++)
  {
    sendColor(r, g, b);
  }
  sei();
  _delay_us((RES / 1000UL) + 1);
}