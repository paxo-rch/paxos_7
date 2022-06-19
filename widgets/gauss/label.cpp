#ifndef Label_CPP
#define Label_CPP

#include "label.hpp"

#define CALIBRATION_X 5
#define CALIBRATION_Y 0

Label::Label(short x, short y, short width, short height)
{
    setX(x);
    setY(y);
    setHeight(height);
    setWidth(width);
    setMarginX(0);
    setMarginY(0);
    setPaddingX(0);
    setPaddingY(0);
    setAlignment(LEFT_ALIGNMENT);
    setTextColor(0x0000);
    front = &FreeMono24pt7b;
}

void Label::drawText(String line, int counter, bool erase, TFT_eSprite* sprite)
{
    sprite->setFreeFont(front);

    if((tft.fontHeight() + 3) * (counter + 1) > 480)
        return;
    if((tft.fontHeight() + 3) * (counter + 1) < 0)
        return;

    sprite->setTextColor((erase)?(getBackgroundColor()):(textColor));


    switch (alignment)
    {
    case LEFT_ALIGNMENT:
        sprite->setCursor(CALIBRATION_X, (tft.fontHeight() + 3) * counter + CALIBRATION_Y + tft.fontHeight());
        break;
    case RIGHT_ALIGNMENT:
        sprite->setCursor(getWidth() - tft.textWidth(line) - CALIBRATION_X, (tft.fontHeight() + 3) * counter + CALIBRATION_Y + tft.fontHeight());
        break;
    case CENTER_ALIGNMENT:
        sprite->setCursor(getWidth()/2 - tft.textWidth(line)/2, (tft.fontHeight() + 3) * counter + CALIBRATION_Y + tft.fontHeight());
        break;
    }


    //sprite->fillRect(CALIBRATION_X, (tft.fontHeight() + 3) * counter + CALIBRATION_Y, tft.textWidth(line), tft.fontHeight(), 0x0000);
    sprite->print(line);
}

void Label::draw()
{
    if(!isEnabled())
        return;

    // select the best front for this size to use less ram
    const GFXfont *allFronts[] = {&FreeSansBold9pt7b, &FreeSansBold12pt7b, &FreeSansBold18pt7b, &FreeSansBold24pt7b};
    for(int i = 0; i < 4; i++)
    {
        tft.setFreeFont(allFronts[i]);
        if(tft.fontHeight()>frontSize)
        {
            front = allFronts[i];
            break;
        }
    }

    if(tft.fontHeight()<frontSize)
    {
        Serial.println("Warning: the front used is too huge !");
        return;
    }

    setBorderColor(0xE73C);

    if(enabledBackground)
        tft.drawRoundRectWithBorder(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), radius, getBorderSize(), getBackgroundColor(), getBorderColor());

    tft.setFreeFont(front);
    int lineCounter = 0;
    int actualIndex = 0;
    int parserIndex = 0;
    String line = "";
    String txt = text+"  ";
    String world = "";

    float factor = (float) tft.fontHeight()/frontSize;
    Serial.println("factor: " + String(factor));
    Serial.println("used front size: " + String(tft.fontHeight()) + " ;  base front size: " + String(frontSize) + " ;  generated front size: " + String(frontSize*factor));
    Serial.println("allocating: " + String(getWidth()*factor*getHeight()*factor*2) + " octets");

    TFT_eSprite sprite(&tft_root);
    sprite.createSprite(getWidth()*factor, getHeight()*factor);
    sprite.fillRect(0,0,getWidth()*factor,getHeight()*factor,getBackgroundColor());

    uint16_t width = getWidth()*factor-5;

    while(actualIndex < txt.length())
    {
        while(actualIndex < txt.length() && tft.textWidth(line+world) <= width)
        {
            if(world.indexOf("\n")!=-1)
            {
                world=world.substring(0, world.length()-1);
                line+=world;
                parserIndex+=world.length()+1;
                world="";
                actualIndex=parserIndex;
                break;
            }
            if(world[world.length()-1] == ' ')
            {
                line+=world;
                parserIndex+=world.length();
                world="";
                actualIndex=parserIndex;
            }
            world+=txt[actualIndex];
            actualIndex++;
        }

        if(tft.textWidth(world) >= width)
        {
            line=world.substring(0, world.length()-2);
            parserIndex+=world.length()-2;
            world="";
            actualIndex=parserIndex;
        }
        
        lineCounter++;
        /*if(getAbsoluteY() + (tft.fontHeight() + 3) * (lineCounter) > 480)
            return;*/
        drawText(line, lineCounter-1, false, &sprite);
        line="";
    }

    //sprite.pushToSprite(&tft, getAbsoluteX(), getAbsoluteY());

    uint16_t* img = resize(reinterpret_cast<uint16_t*>(sprite.getPointer()), sprite.width(), sprite.height(), factor);    

    int newW = (float) sprite.width()/(factor);
    int newH = (float) sprite.height()/(factor);

    tft.pushImage(getAbsoluteX(), getAbsoluteY(), newW, newH, img);

    sprite.deleteSprite();
    delete[] img;
}

int Label::getHeightOfText()
{
    tft.setFreeFont(front);
    int lineCounter = 0;
    int actualIndex = 0;
    int parserIndex = 0;
    String line = "";
    String txt = text+"  ";
    String world = "";

    while(actualIndex < txt.length())
    {
        while(actualIndex < txt.length() && tft.textWidth(line+world) <= width)
        {
            if(world.indexOf("\n")!=-1)
            {
                world=world.substring(0, world.length()-1);
                line+=world;
                parserIndex+=world.length()+1;
                world="";
                actualIndex=parserIndex;
                break;
            }
            if(world[world.length()-1] == ' ')
            {
                line+=world;
                parserIndex+=world.length();
                world="";
                actualIndex=parserIndex;
            }
            world+=txt[actualIndex];
            actualIndex++;
        }

        if(tft.textWidth(world) > width)
        {
            line=world.substring(0, world.length()-2);
            parserIndex+=world.length()-2;
            world="";
            actualIndex=parserIndex;
        }
        
        lineCounter++;
        line="";
    }

    return (tft.fontHeight() + 3) * lineCounter;
}

void Label::setFront(byte size, bool bold)
{
    if(bold)
        switch (size)
        {
            case 9:
            front = &FreeMono9pt7b;
            break;
            case 12:
            front = &FreeMono12pt7b;
            break;
            case 18:
            front = &FreeMono18pt7b;
            break;
            case 24:
            front = &FreeMono24pt7b;
            break;
        }
    else
        switch (size)
        {
            case 9:
            front = &FreeMonoBold9pt7b;
            break;
            case 12:
            front = &FreeMonoBold12pt7b;
            break;
            case 18:
            front = &FreeMonoBold18pt7b;
            break;
            case 24:
            front = &FreeMonoBold24pt7b;
            break;
        }
}

#endif