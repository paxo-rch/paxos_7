
#ifndef APPS_HPP
#define APPS_HPP

#include "../widgets/gauss.hpp"
#include "../system/files/files.hpp"

class App
{
    public:
    virtual void launch() = 0;
    virtual String getName() = 0;
    virtual String getLogo() = 0;
    Window* window = nullptr;
};

bool watchDogCall()
{
    return true;
}

#define while(condition) while(watchDogCall() && condition)
#define appEvent static void

#include "demo/demo.hpp"
#include "Clock/Clock.hpp"
#include "message/message.hpp"

#define NB_APPS 3

App *application[NB_APPS] = {
    new demo,
    new Clock,
    new message
};

String apps_name[NB_APPS] = 
{
    "DEMO",
    "CLOCK",
    "MESSAGE"
};

String apps_logo[NB_APPS] = 
{
    "/paxo/apps/time/logo.bmp",
    "/paxo/apps/time/logo.bmp",
    "/paxo/apps/messages/logo.bmp"
};

void runApp(int index)
{
    application[index]->launch();
}

#endif /* APPS_HPP */
