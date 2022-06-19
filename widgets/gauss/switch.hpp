#ifndef SWITCH_BUTTON
#define SWITCH_BUTTON

class Switch : public Gauss
{
    public:
    byte getType(){return SWITCH_TYPE;}
    Switch(short x, short y, bool state);
    void draw();
    bool isFocuced() { return Touched(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight()); }
    void clear(color_t color) { tft.fillRect(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), color); }
    void free(){}

    void switchState(bool state);
    bool getState(){return state;}

    void EventOnClick();

    private:
    bool state = 0;
};

Switch::Switch(short x, short y, bool state)
{
    init(x, y, 60, 41);
    this->setRadius(20);
    this->setBackgroundColor(0x7BCF);
    this->setColor(0xFFFF);
    this->setBorderSize(0);
}

void Switch::draw()
{
    if(state==1)
    {
        tft.drawRoundRectWithBorder(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), radius, 0, 0x2E43, 0x2E43);
        tft.fillCircle(getAbsoluteX()+getWidth()-20, getAbsoluteY() + 20, 18, 0xFFFF);
    }else
    {
        tft.drawRoundRectWithBorder(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), radius, 0, 0x7BCF, 0x7BCF);
        tft.fillCircle(getAbsoluteX()+20, getAbsoluteY() + 20, 18, 0xFFFF);
    }
}

void Switch::switchState(bool state)
{
    this->state=state;
    draw();
}

void Switch::EventOnClick()
{   
    switchState(!state);
}

#endif