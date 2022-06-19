#ifndef WIDGET_BUTTON_HPP
#define WIDGET_BUTTON_HPP

#define POINTER_DATA 1
#define INT_DATA 2

#include "label.hpp"

class Button : public Gauss
{
    public:
    byte getType(){return BUTTON_TYPE;}
    Button(short x, short y, short width, short height, String text);
    void draw();
    bool isFocuced();
    void clear(color_t color) { tft.fillRect(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), color); }
    void free(){}

    bool isPressed();
    bool isReleased();

    void EventOnClick();
    void EventOnReleased();

    void setEvent(void (*event)(void*), void* data);
    void setEvent(void (*event)(int), int dataNb);

    void setText(String text);
    String getText();

    byte alignement = CENTER_ALIGNMENT;

    private:
    String text = "";
    byte touched = 0;
    void (*eventP)(void*) = nullptr;
    void* data = nullptr;
    void (*eventI)(int) = nullptr;
    int dataNb;
    byte dataType;
};

Button::Button(short x, short y, short width, short height, String text)
{
    init(x, y, width, height);
    this->text = text;
    setMarginX(0);
    setMarginY(0);
    setPaddingX(0);
    setPaddingY(0);
}

void Button::draw()
{
    if(!isEnabled())
        return;

    tft.setTextFont(4);
    if(statePress==0)
        tft.drawRoundRectWithBorder(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), radius, getBorderSize(), getBackgroundColor(), getBorderColor());
    else
    {
        byte r = ((((getBackgroundColor() >> 11) & 0x1F) * 527) + 23) >> 6;
        byte g = ((((getBackgroundColor() >> 5) & 0x3F) * 259) + 33) >> 6;
        byte b = (((getBackgroundColor() & 0x1F) * 527) + 23) >> 6;

        if(r==0) r=255;
        if(g==0) g=255;
        if(b==0) b=255;

        tft.drawRoundRectWithBorder(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), radius, getBorderSize(), tft.color565(r/2, g/2, b/2), getBorderColor());
    }
    Label label(getAbsoluteX(), getAbsoluteY(), width, height);
    label.enableBackground(false);
    label.setTextColor(this->color);
    label.setBackgroundColor(this->backgroundColor);
    label.setText(text);
    label.setY(getAbsoluteY()+getHeight()/2 - label.getHeightOfText()/2);
    label.setAlignment(alignement);
    label.draw();
}

void Button::EventOnClick()
{
    touched=1;
    draw();
}

void Button::EventOnReleased()
{
    draw();
    statePress=2;
}

bool Button::isFocuced()
{
    return Touched(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight());
}

bool Button::isPressed()
{
    return this->statePress==1;
}

bool Button::isReleased()
{
    if(statePress==2)
    {
        statePress=0;
        return true;
    }
}

void Button::setEvent(void (*event)(void*), void* data)
{
    this->eventP = event;
    this->data = data;
    dataType=POINTER_DATA;
}

void Button::setEvent(void (*event)(int), int dataNb)
{
    this->eventI = event;
    this->dataNb = dataNb;
    dataType=INT_DATA;
}

void Button::setText(String text)
{
    this->text = text;
}

String Button::getText()
{
    return this->text;
}

#endif