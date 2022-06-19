#ifndef WINDOW_HPP
#define WINDOW_HPP

class Window : public Gauss
{
    public:
    byte getType(){return WINDOW_TYPE;}
    Window(String name = "");
    void draw();        // draw objet
    bool isFocuced();   // return true if is touched
    void clear(color_t color) {}
    void free(){}
    void setTitle(String title) { this->name = title; draw(); }

    void drawAll();
    void reload();

    void EnableScroolVertical(bool enable);
    void ActiveScroolHorizontal(bool enable);

    private:
    static Window* actualWindow;
    bool scroolHorizontal = false;
    bool scroolVertical = false;
    String name;
    int fps = 0;
};

void Window::drawAll()
{
    if(upFromDrawAll==nullptr)
    {
        upFromDrawAll=this;
        tft.deleteSprite();
        tft.createSprite(this->getWidth(), this->getHeight());
    }
    tft.fillRect(0,0,320,480,getBackgroundColor());
    if(!isEnabled())
        return;
    
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr && (children[i]->getAbsoluteY() + children[i]->getHeight() > 0  || children[i]->getAbsoluteY() > 0) && children[i]->getAbsoluteY() < 480)
            children[i]->drawAll();
    }
    
    draw();
    if(upFromDrawAll==this)
    {
        upFromDrawAll=nullptr;
        tft.pushSprite(0, 0);
    }
}

void Window::reload()
{
    tft.fillRect(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), getBackgroundColor());
    drawAll();
}

Window::Window(String name)
{
    init(0, 0, 320, 480);
    setMarginX(0);
    setMarginY(25);
    setPaddingX(0);
    setPaddingY(0);
    this->name = name;
    tft.fillRect(0, CONTROL_BAR_SIZE, 320, 480-CONTROL_BAR_SIZE, getBackgroundColor());
}

void Window::draw()
{
    tft.fillRect(0,0,320,CONTROL_BAR_SIZE,getBackgroundColor());
    tft.drawFastHLine(0,CONTROL_BAR_SIZE,320,0x9CF3);
    tft.drawFastHLine(0,CONTROL_BAR_SIZE-1,320,0x9CF3);
    tft.setTextColor(contrastColor(getBackgroundColor()));
    tft.drawCentreString(name, 160, 2, 4);
}

bool Window::isFocuced()
{
    return touch.isTouch();
}

void Window::EnableScroolVertical(bool enable)
{
    this->verticalSlide=enable;
}

void Window::ActiveScroolHorizontal(bool enable)
{
    this->horizontalSlide=enable;
}

#endif