#ifndef BOX_HPP
#define BOX_HPP

class Box : public Gauss
{
    public:
    byte getType(){return BOX_TYPE;}
    Box(short x, short y, short width, short height);
    void draw();
    bool isFocuced() { return Touched(x, y, width, height); }
    void clear(color_t color) { tft.fillRect(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), color); }
    void free(){}
};

Box::Box(short x, short y, short width, short height)
{
    init(x, y, width, height);
    setRadius(0);
    setTheme(THEME_CONTRAST);
}

void Box::draw()
{
    tft.drawRoundRectWithBorder(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), radius, getBorderSize(), getBackgroundColor(), getBorderColor());
}

#endif