#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include "../app/apps.hpp"

void launchApp(App *app, Gauss* objectPrt, void* data)
{
    Serial.println("run app");
    runApp(*reinterpret_cast<int*>(data));
}

#define LOGO_SIZE 70

class Menu
{
    public:
    void load();
    Window* window = nullptr;
};

void Menu::load()
{
    window = new Window("menu");
    
    int counter = 0;

    for (int cx = 0; counter < NB_APPS; cx++)
    {
        /*Image* newLogo = new Image(apps_logo[cx], 80-LOGO_SIZE/2+(counter%2)*160, 10+(counter/2)*(LOGO_SIZE+50));
        newLogo->load();*/

        Label* newName = new Label(35, 200+counter*38, 210, 30);
        newName->setBorderSize(0);
        newName->setText(apps_name[cx]);
        newName->frontSize = 24;

        window->addChild(newName);

        counter++;
    }

    window->drawAll();
    window->verticalSlide=true;

    while(true)
        window->updateAll();
}

#endif