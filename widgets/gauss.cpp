#ifndef GAUSS_CPP
#define GAUSS_CPP

#include "gauss.hpp"

Gauss::~Gauss()
{
    for(int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
        {
            delete children[i];
        }
    }
}

void Gauss::init(short x, short y, short width, short height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    setMarginX(0);
    setMarginY(0);
    setPaddingX(0);
    setPaddingY(0);
    onclick = nullptr;
    onlongclick = nullptr;
    onreleased = nullptr;
    onscroll = nullptr;
}

void Gauss::EventOnClick()
{
    return; // do nothing
}

void Gauss::EventOnLongClick()
{
    return; // do nothing
}

void Gauss::EventOnReleased()
{
    Serial.println("released");
    return; // do nothing
}

bool Gauss::EventOnScroll()
{
    short mvtX=touch.isSlidingHorizontally();
    short mvtY=touch.isSlidingVertically();


    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        clearAll(getBackgroundColor());
    
    if(mvtX && horizontalSlide)
    {
        this->paddingX-=mvtX;
        touch.resetScrollHorizontal();
    }
    if(mvtY && verticalSlide)
    {
        this->paddingY-=mvtY;
        touch.resetScrollVertical();
    }

    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        drawAll();
    return mvtX || mvtY;
}

void Gauss::update()
{
    bool s = isFocuced();
    if(s)
    {
        if(statePress==0 && lockedSlide==0)
        {
            statePress=1;
            timerPress=millis();
            EventOnClick();
            if(onclick!=nullptr)
                onclick(appCallback, this, dataCallback);
        }
        if(statePress==1)
        {
            if(timerPress+LONG_PRESS_TIME<millis())
            {
                EventOnLongClick();
                if(onlongclick!=nullptr)
                {
                    onlongclick(appCallback, this, dataCallback);
                    statePress=2;
                }
            }
        }
        if(statePress && touch.stateSlider && isFocuced())
        {
            if(EventOnScroll())
            {
                statePress==0;
                lockedSlide=1;
            }
            if(onscroll!=nullptr)
            {
                onscroll(appCallback, this, dataCallback);
            }
                
        }
    }else
    if(statePress!=0)
    {
        if(lockedSlide==0)
        {
            EventOnReleased();
            if(onreleased!=nullptr)
            {
                onreleased(appCallback, this, dataCallback);
            }
        }
        
        statePress=0;
    }
    if(!s)
    {
        lockedSlide=0;
    }
}

void Gauss::drawAll()
{
    if(upFromDrawAll==nullptr)
    {
        upFromDrawAll=this;
        if(tft.width()!=getWidth() || tft.height()!=getHeight())
        {
            tft.deleteSprite();
            tft.createSprite(this->getWidth(), this->getHeight());
            tft.fillSprite(getBackgroundColor());
        }
    }
        
    if(!isEnabled())
        return;
    draw();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr && (children[i]->getAbsoluteY() < this->getAbsoluteY() + this->getHeight())  && (children[i]->getAbsoluteX() < this->getAbsoluteX() + this->getWidth()))
            children[i]->drawAll();
    }
    if(upFromDrawAll==this)
    {
        upFromDrawAll=nullptr;
        tft.pushSprite(getAbsoluteX(), getAbsoluteY());
    }
}

void Gauss::updateAll()
{
    if(!isEnabled())
        return;
    update();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            children[i]->updateAll();
    }
}

void Gauss::clearAll(color_t color)
{
    clear(color);
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
           children[i]->clearAll(color);
    }
}

void Gauss::autoClearByMoving(color_t color, short my)
{
    if(my>height || my<-height)
        my=height;
    if(my<0)
        tft.fillRect(getAbsoluteX(), getAbsoluteY() + getHeight() + my - radius, getWidth(), -my + radius, color);
    else
        tft.fillRect(getAbsoluteX(), getAbsoluteY(), getWidth(), my + radius, color);

    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            children[i]->autoClearByMoving(color, my);
    }
}

void Gauss::addChild(Gauss *child)
{
    if(child == nullptr)
        return;
    children.push_back(child);
    child->setBackgroundColor(getBackgroundColor());
    child->setParent(this);
}

void Gauss::removechildren()
{
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            delete children[i];
    }
    for (; children.size();)
    {
        children.erase(children.begin());
    }
}

void Gauss::setParent(Gauss *parent)
{
    this->parent = parent;
}

Gauss* Gauss::getMaster()
{
    if(parent == nullptr)
        return this;
    else
        return parent->getMaster();
}

short Gauss::getX()
{
    return this->x;
}

short Gauss::getY()
{
    return this->y;
}

short Gauss::getWidth()
{
    return this->width;
}

short Gauss::getHeight()
{
    return this->height;
}

short Gauss::getAbsoluteX()
{
    if(parent==nullptr)
        return this->getMarginX() + x;
    else
        return (upFromDrawAll==this) ? (0) : (parent->getAbsoluteX() + parent->getPaddingX()) + this->getMarginX() + x;
}

short Gauss::getAbsoluteY()
{
    if(parent==nullptr)
        return this->getMarginY() + y;
    else
        return (upFromDrawAll==this) ? (0) : (parent->getAbsoluteY() + parent->getPaddingY()) + this->getMarginY() + y;
}

short Gauss::getMarginX()
{
    return this->marginX;
}

short Gauss::getMarginY()
{
    return this->marginY;
}

short Gauss::getPaddingX()
{
    return this->paddingX;
}

short Gauss::getPaddingY()
{
    return this->paddingY;
}

color_t Gauss::getColor()
{
    return this->color;
}

color_t Gauss::getBackgroundColor()
{
    return this->backgroundColor;
}

color_t Gauss::getBorderColor()
{
    return this->borderColor;
}

short Gauss::getBorderSize()
{
    return this->borderSize;
}

Alignment Gauss::getAlignment()
{
    return this->alignment;
}

void Gauss::setX(short x)
{
    this->x=x;
}

void Gauss::setY(short y)
{
    this->y=y;
}

void Gauss::setWidth(short width)
{
    this->width=width;
}

void Gauss::setHeight(short height)
{
    this->height=height;
}

void Gauss::setMarginX(short marginX)
{
    this->marginX=marginX;
}

void Gauss::setMarginY(short marginY)
{
    this->marginY=marginY;
}

void Gauss::setPaddingX(short paddingX)
{
    this->paddingX=paddingX;
}

void Gauss::setPaddingY(short setPaddingY)
{
    this->paddingY=paddingY;
}

void Gauss::setColor(color_t color)
{
    this->color=color;
}

void Gauss::setBackgroundColor(color_t backgroundColor)
{
    this->backgroundColor=backgroundColor;
}

void Gauss::setBorderColor(color_t borderColor)
{
    this->borderColor=borderColor;
}

void Gauss::setBorderSize(short borderSize)
{
    this->borderSize=borderSize;
}

void Gauss::setTheme(byte theme)
{
    color = theme_color[theme][0];
    backgroundColor = theme_color[theme][1];
    borderColor = theme_color[theme][2];
}

void Gauss::setAlignment(Alignment alignment)
{
    this->alignment=alignment;
}

void Gauss::setRadius(short radius)
{
    this->radius=radius;
}

short Gauss::getLowestY()
{
    short y = this->getAbsoluteY()+getHeight();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestY()+children[i]->getHeight()>y)
                y=children[i]->getLowestY()+children[i]->getHeight();
    }
    return y;
}

short Gauss::getLowestX()
{
    short x = this->getAbsoluteX()+getWidth();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestX()+children[i]->getWidth()>x)
                x=children[i]->getLowestX()+children[i]->getWidth();
    }
    return x;
}

short Gauss::getHighestY()
{
    short y = this->getAbsoluteY();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestY()<y)
                y=children[i]->getLowestY();
    }
    return y;
}

short Gauss::getHighestX()
{
    short x = this->getAbsoluteX();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestX()<x)
                x=children[i]->getLowestX();
    }
    return x;
}

Gauss* Gauss::getParent()
{
    return this->parent; // warning, can return nullptr
}

void Gauss::reload()
{
    drawAll();
}

#endif