/*
██╗░░░██╗░█████╗░██╗░░░░░██╗░░░░░███████╗███╗░░██╗████████╗██╗███╗░░██╗██╗░░░██╗
██║░░░██║██╔══██╗██║░░░░░██║░░░░░██╔════╝████╗░██║╚══██╔══╝██║████╗░██║╚██╗░██╔╝
╚██╗░██╔╝███████║██║░░░░░██║░░░░░█████╗░░██╔██╗██║░░░██║░░░██║██╔██╗██║░╚████╔╝░
░╚████╔╝░██╔══██║██║░░░░░██║░░░░░██╔══╝░░██║╚████║░░░██║░░░██║██║╚████║░░╚██╔╝░░
░░╚██╔╝░░██║░░██║███████╗███████╗███████╗██║░╚███║░░░██║░░░██║██║░╚███║░░░██║░░░
░░░╚═╝░░░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚═╝╚═╝░░╚══╝░░░╚═╝░░░
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <EncButton.h>

#define NUMPIXELS 13
#define PIXELS_PIN PB0
Adafruit_NeoPixel pixels(NUMPIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

#define INTERRUPT_PIN_1 PCINT1
#define INTERRUPT_PIN_2 PCINT2
#define BUTTON_1 PB1
#define BUTTON_2 PB2
Button button1(PB1, INPUT, LOW);
Button button2(PB1, INPUT, LOW);

void setup()
{
    cli();
    PCMSK |= (1 << INTERRUPT_PIN_1) | (1 << INTERRUPT_PIN_2);
    GIMSK |= (1 << PCIE);
    sei();
    return;
}

void loop()
{

    button1.tick();
    button2.tick();
    return;
}

ISR(PCINT0_vect)
{
    button1.tick();
    button2.tick();
}
