/* GUI SYSTEM */

#ifndef GAUSS_HPP
#define GAUSS_HPP

#include "../extern/extern.hpp" // graphics library
#include "color.hpp"
#include <vector>
#include "touchManager.hpp"

class App;
typedef byte Alignment;
typedef void event;


#define BOX_TYPE        0
#define BUTTON_TYPE     1
#define IMAGE_TYPE      2
#define KEYBOARD_TYPE   3
#define LABEL_TYPE      4
#define WINDOW_TYPE     5
#define SWITCH_TYPE     6

#define CONTROL_BAR_SIZE 25
#define WIDTH 320
#define HEIGHT 480
#define LONG_PRESS_TIME 500

TFT_eSprite tft(&tft_root);

void init_gauss()
{
    
}

class Gauss // widget system
{
    public:
    virtual ~Gauss();
    void init(short x, short y, short width, short height);

    virtual void draw() = 0;        // draw objet
    virtual void update();      // update objet
    virtual bool isFocuced() = 0;   // return true if is touched
    virtual void clear(color_t color) = 0; // clear
    virtual void free() = 0;            // delete[] dynamics allocations before destructor
    virtual byte getType() = 0;

    virtual void drawAll();         // draw all childs and itself 
    void updateAll();       // update all children and itself 
    void clearAll(color_t color);            // clear all children and itself 
    void autoClearByMoving(color_t color, short mx); // A FINIR POUR lES COTES ARRONDIS
    void addChild(Gauss *child);    // add child
    void removechildren();        // remove all children
    void setParent(Gauss *parent);  // set parent

    virtual void reload();

    short getX();            // get relative x
    short getY();            // get relative y
    short getWidth();        // get width
    short getHeight();       // get height

    short getAbsoluteX();    // get absolute x
    short getAbsoluteY();    // get absolute y

    short getMarginX();      // get margin x
    short getMarginY();      // get margin y

    short getPaddingX();     // get padding x
    short getPaddingY();     // get padding y

    Gauss* getParent();
    Gauss* getMaster();

    color_t getColor();         // get color
    color_t getBackgroundColor();   // get background color
    color_t getBorderColor();   // get border color

    short getBorderSize();   // get border size
    Alignment getAlignment(); // get alignment
    bool isEnabled(){return this->enabled;}
    bool getRadius(){ return this->radius;}

    short getLowestY();
    short getHighestY();

    short getLowestX();
    short getHighestX();

    void setX(short x); // set relative x
    void setY(short y); // set relative y
    void setWidth(short width);   // set width
    void setHeight(short height); // set height

    void setMarginX(short marginX);  // set margin x
    void setMarginY(short marginY);  // set margin y

    void setPaddingX(short paddingX);   // set padding x
    void setPaddingY(short paddingY);   // set padding y

    void setColor(color_t color);       // colors
    void setBackgroundColor(color_t backgroundColor);

    void setBorderColor(color_t borderColor);
    void setBorderSize(short borderSize); // set border size

    void setTheme(byte theme);
    void setAlignment(Alignment alignment); // set alignment
    void setRadius(short radius); // set radius

    void setAutoSize(bool autoSize){this->autoSize=autoSize;}

    void enable(){this->enabled=true; drawAll();}
    void disable(){this->enabled=false;}

    virtual void EventOnClick();
    virtual void EventOnLongClick();
    virtual void EventOnReleased();
    virtual bool EventOnScroll();

    bool lockedSlide = false;

    void (*onclick)(App *app, Gauss* object, void* data) = nullptr;
    void (*onlongclick)(App *app, Gauss* object, void* data) = nullptr;
    void (*onreleased)(App *app, Gauss* object, void* data) = nullptr;
    void (*onscroll)(App *app, Gauss* object, void* data) = nullptr;

    App* appCallback = nullptr;
    void* dataCallback = nullptr;

    bool verticalSlide=false;
    bool horizontalSlide=false;

    protected:
    short x, y = 0;                      // position
    short width, height = 0;             // sizes
    short marginX, marginY = 0;          // margin
    short paddingX, paddingY = 0;        // padding

    color_t color = theme_color[DEFAULT_THEME][0];
    color_t backgroundColor = theme_color[DEFAULT_THEME][1];
    color_t borderColor = theme_color[DEFAULT_THEME][2];

    short borderSize = DEFAULT_BORDER_SIZE;
    Alignment alignment = DEFAULT_ALIGNEMENT;
    short radius = DEFAULT_RADIUS;

    byte statePress = 0; // 0=no 1=yes 2=wait realesed
    unsigned int timerPress = 0;

    bool autoSize = true;
    bool enabled = true;

    short inercie = 0;

    std::vector<Gauss *> children;
    Gauss *parent = nullptr;

};

Gauss *upFromDrawAll = nullptr;

#include "gauss.cpp"

#include "gauss/additional.hpp"

#include "gauss/window.hpp"
#include "gauss/box.hpp"
#include "gauss/label.hpp"
#include "gauss/button.hpp"
#include "gauss/image.hpp"
#include "gauss/keyboard.hpp"
#include "gauss/switch.hpp"

#endif /* GAUSS_HPP */