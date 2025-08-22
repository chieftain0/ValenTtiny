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
#define CLICKvsPRESS_TIME 500
#define PRESS_TO_ACTION_TIME 250
#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 1
const uint8_t buttonPins[] = {BUTTON_1_PIN, BUTTON_2_PIN};

// Internal
uint8_t state = 0;
uint8_t brightness = 5;
#define NUM_STATES 8

void setup()
{
    bitSet(LED_DDR, LED_BIT);
}

void loop()
{
    uint8_t click_map = 0, press_map = 0;
    PollButtons((uint8_t *)buttonPins, sizeof(buttonPins) / sizeof(buttonPins[0]), LOW, CLICKvsPRESS_TIME, &click_map, &press_map);

    if (click_map & 0b00000001)
    {
        state = (state + 1) % NUM_STATES;
    }
    else if (click_map & 0b00000010)
    {
        state = (state + NUM_STATES - 1) % NUM_STATES;
    }

    static uint32_t  press_timer1 = 0, press_timer2 = 0;
    if ((press_map & 0b00000001) && (millis() - press_timer1 > PRESS_TO_ACTION_TIME))
    {
        press_timer1 = millis();
        if (brightness <= 250)
        {
            brightness += 5;
        }
        else
        {
            brightness = 255;
        }
    }
    else if (press_map & 0b00000010 && (millis() - press_timer2 > PRESS_TO_ACTION_TIME))
    {
        press_timer2 = millis();
        if (brightness >= 5)
        {
            brightness -= 5;
        }
        else
        {
            brightness = 0;
        }
    }

    if (state == 0)
    {
        Rainbow(5, brightness, 0, NUM_LEDS);
    }
    else if (state == 1)
    {
        FillUntil(brightness, 0, 0, NUM_LEDS);
    }
    else if (state == 2)
    {
        FillUntil(brightness, brightness, 0, NUM_LEDS);
    }
    else if (state == 3)
    {
        FillUntil(0, brightness, 0, NUM_LEDS);
    }
    else if (state == 4)
    {
        FillUntil(0, brightness, brightness, NUM_LEDS);
    }
    else if (state == 5)
    {
        FillUntil(0, 0, brightness, NUM_LEDS);
    }
    else if (state == 6)
    {
        FillUntil(brightness, 0, brightness, NUM_LEDS);
    }
    else if (state == 7)
    {
        FillUntil(brightness, brightness, brightness, NUM_LEDS);
    }
}

/**
 * @brief Polls the buttons in the given array and stores the click and press states in the given pointers.
 *
 * @details This function is designed to be used in interrupt contexts. It stores the state of the buttons in the
 *          given array and stores the click and press states in the given pointers. The function uses a static
 *          array of timers to determine when a button has been pressed for longer than the given hold time.
 *
 * @param[in] button_pins Array of pins to poll.
 * @param[in] num_buttons The number of buttons in the array.
 * @param[in] press_state The state of the button when pressed (0 or 1).
 * @param[in] hold_time The time in milliseconds to hold the button before it is considered a press.
 * @param[out] click_mask A pointer to store the click states of the buttons.
 * @param[out] press_mask A pointer to store the press states of the buttons.
 *
 * @return None
 */
inline void PollButtons(uint8_t *button_pins, uint8_t num_buttons, uint8_t press_state,
                 uint32_t  hold_time, uint8_t *click_mask, uint8_t *press_mask)
{
    if (num_buttons > 8)
    {
        return;
    }

    static uint8_t button_flags = 0;
    static uint32_t  timers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t clicks = 0, presses = 0;

    for (uint8_t i = 0; i < num_buttons; i++)
    {
        uint8_t button_state = digitalRead(button_pins[i]);

        if (button_state == press_state && (button_flags & (1 << i)) == 0)
        {
            timers[i] = millis();
            button_flags |= 1 << i;
        }
        else if (button_state != press_state && (button_flags & (1 << i)) != 0)
        {
            if (millis() - timers[i] < hold_time)
            {
                clicks |= 1 << i;
            }
            button_flags &= ~(1 << i);
        }

        if ((button_flags & (1 << i)) && (millis() - timers[i] >= hold_time))
        {
            presses |= (1 << i);
        }
    }

    *click_mask = clicks;
    *press_mask = presses;
}

/**
 * @brief Set the LEDs to a rainbow pattern
 *
 * @param[in] wait_ms Delay in milliseconds between each update
 * @param[in] brightness Brightness of the LEDs (0 - 255)
 * @param[in] gammaCorrect Apply gamma correction
 * @param[in] num_leds Number of LEDs in the strip
 * 
 * @return None
 */
inline void Rainbow(unsigned long wait_ms, uint8_t brightness, bool gammaCorrect, uint8_t num_leds)
{
    static uint8_t firstHue = 0;
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate >= wait_ms)
    {
        lastUpdate = millis();
        firstHue += 1;

        uint8_t colors[num_leds][3];
        for (uint8_t i = 0; i < num_leds; i++)
        {
            uint8_t hue = firstHue + (i * 256 / num_leds);
            HSVtoRGB(hue, 255, 255, colors[i][0], colors[i][1], colors[i][2]);

            colors[i][0] = (colors[i][0] * brightness + 127) / 255;
            colors[i][1] = (colors[i][1] * brightness + 127) / 255;
            colors[i][2] = (colors[i][2] * brightness + 127) / 255;

            if (gammaCorrect)
            {
                colors[i][0] = uint8_t(pow(colors[i][0] / 255.5, 2.2) * 255.5);
                colors[i][1] = uint8_t(pow(colors[i][1] / 255.5, 2.2) * 255.5);
                colors[i][2] = uint8_t(pow(colors[i][2] / 255.5, 2.2) * 255.5);
            }
        }

        cli();
        for (uint8_t i = 0; i < num_leds; i++)
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
        sendBit(g & (0b10000000 >> bit));
    }
    for (uint8_t bit = 0; bit < 8; bit++)
    {
        sendBit(r & (0b10000000 >> bit));
    }
    for (uint8_t bit = 0; bit < 8; bit++)
    {
        sendBit(b & (0b10000000 >> bit));
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
 * @param[in] num_led The number of LEDs to fill (up to 256).
 *
 * @return None
 */
inline void FillUntil(uint8_t r, uint8_t g, uint8_t b, uint8_t num_led)
{
    cli();
    for (uint8_t p = 0; p < num_led; p++)
    {
        sendColor(r, g, b);
    }
    sei();
    _delay_us((RES / 1000UL) + 1);
}