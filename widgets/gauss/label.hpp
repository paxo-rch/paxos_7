#ifndef Label_HPP
#define Label_HPP

class Label : public Gauss
{
public:
    byte getType(){return LABEL_TYPE;}
    Label(short x, short y, short width, short height);

    void draw();
    void remove() {}
    bool isFocuced() { return Touched(x, y, width, height); }
    void clear(color_t color) { tft.fillRect(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), color); }
    void free(){}

    void setText(String text) { this->text = text; }
    String getText() { return this->text; }
    void addText(String toAdd) { this->text += toAdd; }
    int getHeightOfText();
    void enableBackground(bool state) { this->enabledBackground = state; }

    void setTextColor(color_t textColor) { this->textColor = textColor; }
    color_t getTextColor() { return this->textColor; }
    void setFront(byte size, bool bold = false);
    
    byte frontSize = 24;

private:
    const GFXfont *front; 
    void drawText(String line, int counter, bool erase, TFT_eSprite* sprite);
    bool enabledBackground = true;
    String text = "...";
    color_t textColor = 0x0000;
};

#include "label.cpp"

#endif