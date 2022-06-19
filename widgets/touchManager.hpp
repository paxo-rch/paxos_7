#ifndef TOUCH_MANAGER
#define TOUCH_MANAGER

#define SLIDE_UP 1
#define SLIDE_DOWN 2
#define SLIDE_RIGHT 3
#define SLIDE_LEFT 4

#define FLUIDITY 2

#define timeToWait 10

class TouchManager
{
    public:
    uint16_t getX();
    uint16_t getY();
    bool isTouch();

    uint16_t isSlidingHorizontally(); // sliding right
    uint16_t isSlidingVertically();   // sliding up

    void resetScrollVertical();
    void resetScrollHorizontal();
   
    void update();
    
    byte stateSlider=0;

    private:
    bool isTouchRead();
    bool validTouch();
    
    
    
    uint16_t tx, ty;
    bool stateTouch;
    double lastClick = millis();
    bool isPreviousTouch = 0;
    unsigned long timerSlider;
    
    uint16_t fx, fy = 0;
    unsigned long timerUpdate = millis();
};

void TouchManager::update()
{
    if(timerUpdate+5<millis())
    {
        tft_root.getTouch(&tx, &ty);
        timerUpdate=millis();
    }
}

uint16_t TouchManager::getX()
{
    update();
    return tx;
}

uint16_t TouchManager::getY()
{
    update();
    return ty;
}

bool TouchManager::isTouch()
{
    update();
    stateTouch=validTouch();
    if(stateTouch)
    {
        if(stateSlider==0 && tx!=0)
        {
            stateSlider=1;
            fx=tx;
            fy=ty;
            
        }
    }
    else
    {
        stateSlider=0;
        fx=tx;
        fy=ty;
    }
    

    return stateTouch;
}

uint16_t TouchManager::isSlidingHorizontally()
{
    if(isTouch() && stateSlider==1 && (fx-FLUIDITY>tx || fx+FLUIDITY<tx))
    {
        short pixels = fx-tx;
        stateTouch=0;
        return pixels;
    }
        
    return false;
}

uint16_t TouchManager::isSlidingVertically()
{
    if(isTouch() && stateSlider==1 && (fy+FLUIDITY<ty || fy-FLUIDITY>ty))
    {
        short pixels = fy-ty;
        stateTouch=0;
        return pixels;
    }

    return false;
}

void TouchManager::resetScrollVertical()
{
    fy=ty;
}

void TouchManager::resetScrollHorizontal()
{
    fx=tx;
}

bool TouchManager::isTouchRead() // simple detector
{
    update();
    uint16_t z = tft_root.getTouchRawZ();
    if (z > 300 && z < 4000 && lastClick + timeToWait > millis())
    {
        lastClick = millis();
        return 1;
    }
    if (z > 300 && z < 4000)
    {
        lastClick = millis();
        return 1;
    }
    if ((z < 4000 || z > 300) && lastClick + timeToWait < millis())
    {
        tx = 0;
        ty = 0;
        return 0;
    }
    else
    {
        return 1;
    }
}

bool TouchManager::validTouch() // auto standby
{
    bool state = isTouchRead();

    return state;
}

TouchManager touch;

class touchZone
{
    public:
    touchZone(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        this->x=x;
        this->y=y;
        this->width=width;
        this->height=height;
    }
    bool isTouch()
    {
        uint16_t xx = touch.getX();
        uint16_t yy = touch.getY();
        return x<xx && xx<x+width && y<yy && yy<y+height;
    }
    private:
    uint16_t x, y, width, height;
};

bool Touched(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t xx = touch.getX();
    uint16_t yy = touch.getY();
    return x<xx && xx<x+width && y<yy && yy<y+height;
}

#endif // TOUCH_MANAGER