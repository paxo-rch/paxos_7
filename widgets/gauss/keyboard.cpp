#ifndef Keyboard_CPP
#define Keyboard_CPP

#include "keyboard.hpp"

Keyboard::Keyboard()
{
    init(15, 310, 290, 157);
    setY(300);
    //setMarginX(15);

    this->drawAll();
}

void Keyboard::draw()
{
    tft.fillRect(0, getAbsoluteY(), 320, 200, COLOR_BACKGROUND);
    tft.setTextColor(COLOR_NOT_PRESS);
    for (byte j = 0; j < 3; j++)
    {
        for (byte i = 0; i < 10; i++)
        {
            if(alphabet[mode][j][i]!=' ')
                tft.drawCentreString(String(alphabet[mode][j][i]), getAbsoluteX() + 15 + i * 29, getAbsoluteY() + 7 + j * 37, 4);
        }
    }

    tft.drawCentreString("espace", 160, 435, 4);
    tft.drawCentreString("return", 282, 435, 4);

    Image* maj = new Image("/paxo/system/keyboard/maj_"+String(mode)+".bmp", getAbsoluteX(), getAbsoluteY()+73);   // majuscule
    Image* type = new Image("/paxo/system/keyboard/type_"+String(mode)+".bmp", getAbsoluteX(), getAbsoluteY()+110);   // majuscule
    Image* erase = new Image("/paxo/system/keyboard/delete.bmp", getAbsoluteX() + 268, getAbsoluteY() + 87);   // erase char
    Image* spa_ret = new Image("/paxo/system/keyboard/spa&ret.bmp", getAbsoluteX() + 87, getAbsoluteY() + 110);   // erase char

    this->addChild(maj);
    this->addChild(type);
    this->addChild(erase);
    this->addChild(spa_ret);

    maj->draw();
    type->draw();
    erase->draw();
    spa_ret->draw();

    this->removechildren();
}

char Keyboard::isTouch()
{
    if (touch.isTouch())
    {
        for (byte j = 0; j < 3; j++)
        {
            for (byte i = 0; i < 10; i++)
            {
                if(Touched(getAbsoluteX() + i * 29, getAbsoluteY() + j * 37, 29, 37))
                {
                    if(alphabet[mode][j][i]==' ')
                        break;
                    tft_root.setTextColor(COLOR_PRESS);
                    tft_root.drawCentreString(String(alphabet[mode][j][i]), getAbsoluteX() + 15 + i * 29, getAbsoluteY() + 7 + j * 37, 4);
                    while(touch.isTouch());
                    tft_root.setTextColor(COLOR_NOT_PRESS);
                    tft_root.drawCentreString(String(alphabet[mode][j][i]), getAbsoluteX() + 15 + i * 29, getAbsoluteY() + 7 + j * 37, 4);
                    
                    lastChar=alphabet[mode][j][i];
                    timer_last_char=millis();
                    return alphabet[mode][j][i];
                }
            }
        }
        if(Touched(252, 420, 58, 40))
        {
            while(touch.isTouch());
            lastChar='\n';
            timer_last_char=millis();
            return '\n';
        }
        if(Touched(getAbsoluteX(), getAbsoluteY()+73, 30, 36)) // maj
        {
            switch(mode)
            {
                case 0:
                    mode=1;
                break;
                case 1:
                    mode=0;
                break;
                case 2:
                    mode=3;
                break;
                case 3:
                    mode=2;
                break;
            }
            while(touch.isTouch());
            this->drawAll();
        }

        if(Touched(getAbsoluteX(), getAbsoluteY()+110, 30, 36)) // mode
        {
            switch(mode)
            {
                case 0:
                    mode=3;
                break;
                case 1:
                    mode=3;
                break;
                case 2:
                    mode=0;
                break;
                case 3:
                    mode=0;
                break;
            }
            while(touch.isTouch());
            this->drawAll();
        }

        if(Touched(getAbsoluteX() + 268, getAbsoluteY() + 87, 30, 36))
        {
            if(lastChar==KEY_ERASE)
            {
                delay(100);
            }
            lastChar=KEY_ERASE;
            timer_last_char=millis();
            return KEY_ERASE;
        }
        if(Touched(45, 440, 185, 25))
        {
            tft_root.setTextColor(darkenColor(color));
            tft_root.drawString("espace", 120, 440, 4);
            while(touch.isTouch());
            tft_root.setTextColor(color);
            tft_root.drawString("espace", 120, 440, 4);
            return ' ';
        }
    }
    return 0;
}

bool Keyboard::isEnd()
{
    return 0;
}

String Keyboard::getString()
{
    return "";
}

void Keyboard::setString(String newString)
{
}

#endif /* Keyboard_CPP */