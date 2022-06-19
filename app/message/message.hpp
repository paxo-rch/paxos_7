#ifndef APP_MESSAGE_HPP
#define APP_MESSAGE_HPP

#include "../../hardware/modules/gsm.hpp"

class message : public App
{
    public:
    String getName() { return "MESSAGE"; }
    String getLogo() { return "/paxo/apps/messages/logo.bmp"; };
    void launch();
    void restore(){}
    void quit(){}

    appEvent changeTheme(App *app, Gauss* objectPrt, void* data)
    {
        Switch* object = reinterpret_cast<Switch*>(objectPrt);
        if(object->getState()==1)
            object->getParent()->setTheme(THEME_BLACK);
        else
            object->getParent()->setTheme(THEME_WHITE);
        object->getParent()->reload();
    }

    appEvent open(App *app, Gauss* objectPrt, void* data)
    {
        message* localApp = reinterpret_cast<message*>(app);
        Gauss* guiElement = reinterpret_cast<Gauss*>(objectPrt);
        int index = reinterpret_cast<int>(data);
        conversation(index);
        guiElement->getParent()->drawAll();
    }

    void home();
    void contact();
    static void conversation(int index);
    String getContact(String number); // 06.... format
    void newContact();

    String adress = "/paxo/apps/message/";
    
};

#include "message.cpp"

#endif