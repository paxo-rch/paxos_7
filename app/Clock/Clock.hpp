#ifndef CLOCK_APP_HPP
#define CLOCK_APP_HPP

class Clock : public App
{
    public:
    String getName() { return "CLOCK"; }
    String getLogo() { return "/paxo/apps/time/logo.bmp"; }

    void launch();

    void menu();

    appEvent simpleClock(App *app, Gauss* objectPrt, void* data);
    appEvent simpleTimer(App *app, Gauss* objectPrt, void* data);
    appEvent alarmClock(App *app, Gauss* objectPrt, void* data);

    private:
};

#include "clock.cpp"

#endif