void Rainbow(Adafruit_NeoPixel &pixels, double &brightness, unsigned long wait)
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

void JustRED(Adafruit_NeoPixel &pixels, double &brightness)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.gamma32(pixels.Color(255 * brightness, 0, 0)));
    }
    pixels.show();

    return;
}

void JustYellow(Adafruit_NeoPixel &pixels, double &brightness)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.gamma32(pixels.Color(255 * brightness, 255 * brightness, 0)));
    }
    pixels.show();

    return;
}

void JustGreen(Adafruit_NeoPixel &pixels, double &brightness)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.gamma32(pixels.Color(0, 255 * brightness, 0)));
    }
    pixels.show();

    return;
}

void JustCyan(Adafruit_NeoPixel &pixels, double &brightness)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.gamma32(pixels.Color(0, 255 * brightness, 255 * brightness)));
    }
    pixels.show();

    return;
}

void JustBlue(Adafruit_NeoPixel &pixels, double &brightness)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.gamma32(pixels.Color(0, 0, 255 * brightness)));
    }
    pixels.show();

    return;
}

void JustViolet(Adafruit_NeoPixel &pixels, double &brightness)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.gamma32(pixels.Color(255 * brightness, 0, 255 * brightness)));
    }
    pixels.show();

    return;
}

void JustWhite(Adafruit_NeoPixel &pixels, double &brightness)
{

    for (int i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, pixels.gamma32(pixels.Color(255 * brightness, 255 * brightness, 255 * brightness)));
    }
    pixels.show();

    return;
}