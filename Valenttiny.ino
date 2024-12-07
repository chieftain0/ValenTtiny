/*
██╗░░░██╗░█████╗░██╗░░░░░██╗░░░░░███████╗███╗░░██╗████████╗██╗███╗░░██╗██╗░░░██╗
██║░░░██║██╔══██╗██║░░░░░██║░░░░░██╔════╝████╗░██║╚══██╔══╝██║████╗░██║╚██╗░██╔╝
╚██╗░██╔╝███████║██║░░░░░██║░░░░░█████╗░░██╔██╗██║░░░██║░░░██║██╔██╗██║░╚████╔╝░
░╚████╔╝░██╔══██║██║░░░░░██║░░░░░██╔══╝░░██║╚████║░░░██║░░░██║██║╚████║░░╚██╔╝░░
░░╚██╔╝░░██║░░██║███████╗███████╗███████╗██║░╚███║░░░██║░░░██║██║░╚███║░░░██║░░░
░░░╚═╝░░░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚═╝╚═╝░░╚══╝░░░╚═╝░░░
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

#ifdef EEPROM_h
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
#endif

    return;
}

void loop()
{

    button1.tick();
    button2.tick();

    if (button1.click())
    {
        state++;
        if (state >= NUM_STATES)
        {
            state = 0;
        }
#ifdef EEPROM_h
        EEPROM.write(STATE_ADDRESS, state);
#endif
    }
    if (button2.click())
    {
        state--;
        if (state < 0)
        {
            state = NUM_STATES - 1;
        }
#ifdef EEPROM_h
        EEPROM.write(STATE_ADDRESS, state);
#endif
    }

    if (button1.step())
    {
        brightness += 0.1;
        if (brightness > 1.0)
        {
            brightness = 1.0;
        }
#ifdef EEPROM_h
        EEPROM.write(BRIGHTNESS_ADDRESS, brightness);
#endif
    }
    if (button2.step())
    {
        brightness -= 0.1;
        if (brightness < 0.0)
        {
            brightness = 0.0;
        }
#ifdef EEPROM_h
        EEPROM.write(BRIGHTNESS_ADDRESS, brightness);
#endif
    }

    if (state == 0)
    {
        Rainbow(pixels, brightness, 10);
    }
    else if (state == 1)
    {
        JustRED(pixels, brightness);
    }
    else if (state == 2)
    {
        JustYellow(pixels, brightness);
    }
    else if (state == 3)
    {
        JustGreen(pixels, brightness);
    }
    else if (state == 4)
    {
        JustCyan(pixels, brightness);
    }
    else if (state == 5)
    {
        JustBlue(pixels, brightness);
    }
    else if (state == 6)
    {
        JustViolet(pixels, brightness);
    }
    else if (state == 7)
    {
        JustWhite(pixels, brightness);
    }

    return;
}