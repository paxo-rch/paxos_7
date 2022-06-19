#ifndef DEMO_APP_HPP
#define DEMO_APP_HPP


class demo : public App
{
    public:
    void launch();
    String getName(){return "DEMO";}
    String getLogo(){return "/file/file.bmp";}
    static void increment(void* data);
};

void demo::launch()
{
    
    Window window("demo");
    window.setBackgroundColor(0x0000);
    Keyboard* keyboard = new Keyboard();
    window.addChild(keyboard);
    Label* lab = new Label(10, 10, 300, 1000);
    Box* box = new Box(0,0,320,285);
    box->addChild(lab);
    box->verticalSlide=true;
    window.addChild(box);
    window.drawAll();
    while(true)
    {
        window.updateAll();
        char chr = keyboard->isTouch();
        if(chr!=0)
        {
            if(chr==KEY_ERASE)
                lab->setText(lab->getText().substring(0, lab->getText().length()-1));
            else
                lab->setText(lab->getText() + String(chr));
            box->drawAll();
        }
    }
}

void demo::increment(void* data)
{
    int* var = static_cast<int*>(data);
    Serial.println("var = " + String(*var));
    *var=*var+1;
}

#endif