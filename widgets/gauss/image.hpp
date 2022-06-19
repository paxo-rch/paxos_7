#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <SD.h>

#define BUFFPIXEL 20

uint16_t read16(fs::File &f)
{
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}

uint32_t read32(fs::File &f)
{
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}

class Image : public Gauss
{
    public:
    byte getType(){return IMAGE_TYPE;}
    Image(String filename, short x, short y);
    ~Image();
    void draw();
    bool isFocuced(){}
    void clear(color_t color) { tft.fillRect(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), getBackgroundColor()); }
    void free(){ if(image!=nullptr) close(); }
    
    void setAlpha(uint16_t alpha) { this->alpha = alpha; isAlphaEnabled=1; }
    void load();
    void close(){ delete[] image; image = nullptr; }

    private:
    void bmpDrawSD();
    String filename = "";
    uint16_t* image = nullptr;
    uint16_t alpha = 0x0000;
    bool isAlphaEnabled = 0;
    TFT_eSprite* img = nullptr;
};

Image::~Image()
{
    if(img==nullptr)
        return;
    img->deleteSprite();
    delete[] img;
}

Image::Image(String filename, short x, short y)
{
    init(x, y, 0, 0);
    this->filename=filename;
}

void Image::draw()
{
    if(img!=nullptr)
        img->pushToSprite(&tft, getAbsoluteX(),getAbsoluteY()/*,alpha*/);
    else
        bmpDrawSD();
}

void Image::load()
{
    uint16_t x = getAbsoluteX();
    uint16_t y = getAbsoluteY();
    File bmpFile;
    int bmpWidth, bmpHeight;            // W+H in pixels
    uint8_t bmpDepth;                   // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;            // Start of image data in file
    uint32_t rowSize;                   // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel buffer (R+G+B per pixel)
    uint8_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    boolean goodBmp = false;            // Set to true on valid header parse
    boolean flip = true;                // BMP is stored bottom-to-top
    int w, h, row, col;
    uint8_t r, g, b;
    uint32_t pos = 0, startTime = millis();

    if ((x >= tft.width()) || (y >= tft.height()))
        return;

    Serial.println();
    Serial.print(F("Loading image '"));
    Serial.print(filename);
    Serial.println('\'');

    // Open requested file on SD card
    if ((bmpFile = SD.open(filename)) == NULL)
    {
        Serial.print(F("File not found"));
        return;
    }

    // Parse BMP header
    if (read16(bmpFile) == 0x4D42)
    { // BMP signature
        Serial.print(F("File size: "));
        Serial.println(read32(bmpFile));
        (void)read32(bmpFile);            // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile); // Start of image data
        Serial.print(F("Image Offset: "));
        Serial.println(bmpImageoffset, DEC);
        // Read DIB header
        Serial.print(F("Header size: "));
        Serial.println(read32(bmpFile));
        bmpWidth = read32(bmpFile);
        bmpHeight = read32(bmpFile);
        setHeight(bmpHeight);
        setWidth(bmpWidth);

        img = new TFT_eSprite(&tft);
        //img->setColorDepth(4);
        img->createSprite(bmpWidth, bmpHeight);

        int SIZEBUFFER = 100; //bmpHeight;
        if (480 < SIZEBUFFER)
            SIZEBUFFER = bmpHeight;

        if (SIZEBUFFER > bmpHeight)
            SIZEBUFFER = bmpHeight;

        uint16_t *image = new uint16_t[bmpWidth * SIZEBUFFER];
        //uint16_t *image = (uint16_t *)ps_malloc(bmpWidth * SIZEBUFFER * sizeof(uint16_t));

        if (read16(bmpFile) == 1)
        {                               // # planes -- must be '1'
            bmpDepth = read16(bmpFile); // bits per pixel
            if ((bmpDepth == 24) && (read32(bmpFile) == 0))
            {

                goodBmp = true;

                rowSize = (bmpWidth * 3 + 3) & ~3;
                if (bmpHeight < 0)
                {
                    bmpHeight = -bmpHeight;
                    flip = false;
                }

                w = bmpWidth;
                h = bmpHeight;
                if ((x + w - 1) >= tft.width())
                    w = tft.width() - x;
                if ((y + h - 1) >= tft.height())
                    h = tft.height() - y;

                int counter = 0;

                for (row = 0; row < h; row++)
                {
                    if (flip)
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else
                        pos = bmpImageoffset + row * rowSize;
                    if (bmpFile.position() != pos)
                    {
                        bmpFile.seek(pos);
                        buffidx = sizeof(sdbuffer);
                    }

                    for (col = 0; col < w; col++)
                    {
                        if (buffidx >= sizeof(sdbuffer))
                        {
                            bmpFile.read(sdbuffer, sizeof(sdbuffer));
                            buffidx = 0;
                        }

                        b = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];

                        img->drawPixel(col,row,tft.color565(r, g, b));
                    }
                }
            }
        }
    }

    bmpFile.close();
}

void Image::bmpDrawSD()
{
    uint16_t x = getAbsoluteX();
    uint16_t y = getAbsoluteY();
    File bmpFile;
    int bmpWidth, bmpHeight;            // W+H in pixels
    uint8_t bmpDepth;                   // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;            // Start of image data in file
    uint32_t rowSize;                   // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel buffer (R+G+B per pixel)
    uint8_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    boolean goodBmp = false;            // Set to true on valid header parse
    boolean flip = true;                // BMP is stored bottom-to-top
    int w, h, row, col;
    uint8_t r, g, b;
    uint32_t pos = 0, startTime = millis();

    if ((x >= tft.width()) || (y >= tft.height()))
        return;

    Serial.println();
    Serial.print(F("Loading image '"));
    Serial.print(filename);
    Serial.println('\'');

    // Open requested file on SD card
    if ((bmpFile = SD.open(filename)) == NULL)
    {
        Serial.print(F("File not found"));
        return;
    }

    // Parse BMP header
    if (read16(bmpFile) == 0x4D42)
    { // BMP signature
        Serial.print(F("File size: "));
        Serial.println(read32(bmpFile));
        (void)read32(bmpFile);            // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile); // Start of image data
        Serial.print(F("Image Offset: "));
        Serial.println(bmpImageoffset, DEC);
        // Read DIB header
        Serial.print(F("Header size: "));
        Serial.println(read32(bmpFile));
        bmpWidth = read32(bmpFile);
        bmpHeight = read32(bmpFile);
        setHeight(bmpHeight);
        setWidth(bmpWidth);

        int SIZEBUFFER = 100; //bmpHeight;
        if (480 < SIZEBUFFER)
            SIZEBUFFER = bmpHeight;

        if (SIZEBUFFER > bmpHeight)
            SIZEBUFFER = bmpHeight;

        uint16_t *image = new uint16_t[bmpWidth * SIZEBUFFER];
        //uint16_t *image = (uint16_t *)ps_malloc(bmpWidth * SIZEBUFFER * sizeof(uint16_t));

        if (read16(bmpFile) == 1)
        {                               // # planes -- must be '1'
            bmpDepth = read16(bmpFile); // bits per pixel
            if ((bmpDepth == 24) && (read32(bmpFile) == 0))
            {

                goodBmp = true;

                rowSize = (bmpWidth * 3 + 3) & ~3;
                if (bmpHeight < 0)
                {
                    bmpHeight = -bmpHeight;
                    flip = false;
                }

                w = bmpWidth;
                h = bmpHeight;
                if ((x + w - 1) >= tft.width())
                    w = tft.width() - x;
                if ((y + h - 1) >= tft.height())
                    h = tft.height() - y;

                int counter = 0;

                for (row = 0; row < h; row++)
                {
                    if (flip)
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else
                        pos = bmpImageoffset + row * rowSize;
                    if (bmpFile.position() != pos)
                    {
                        bmpFile.seek(pos);
                        buffidx = sizeof(sdbuffer);
                    }

                    for (col = 0; col < w; col++)
                    {
                        if (buffidx >= sizeof(sdbuffer))
                        {
                            bmpFile.read(sdbuffer, sizeof(sdbuffer));
                            buffidx = 0;
                        }

                        b = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];

                        image[(row - counter) * bmpWidth + col] = tft.color565(r, g, b);
                    }

                    if (row - SIZEBUFFER  + 1 == counter)
                    {
                        tft.setSwapBytes(true);
                        if(isAlphaEnabled)
                            tft.pushImage(x, y + counter, bmpWidth, SIZEBUFFER, image, alpha);
                        else
                            tft.pushImage(x, y + counter, bmpWidth, SIZEBUFFER, image);
                        counter = row + 1;
                    }
                    if (bmpHeight - row < SIZEBUFFER && row + 1 == bmpHeight)
                        if(isAlphaEnabled)
                            tft.pushImage(x, y + counter, bmpWidth, bmpHeight - counter, image, alpha);
                        else
                            tft.pushImage(x, y + counter, bmpWidth, bmpHeight - counter, image);
                }
            }
        }

        delete[] image;
    }

    bmpFile.close();

    if (!goodBmp)
        Serial.println("invalid image");
}


#endif