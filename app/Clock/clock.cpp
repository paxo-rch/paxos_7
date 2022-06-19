#ifndef CLOCK_APP_CPP
#define CLOCK_APP_CPP

#include "Clock.hpp"

#define SIZEX_BUTTON_CLOCK 200

void Clock::launch()
{
    menu();
}

void Clock::menu()
{
    Window window("clock");
    while(1)
    {
        Button* launchClock = new Button(160, 100, SIZEX_BUTTON_CLOCK, 40, "Clock");
        Button* launchTimer = new Button(160, 150, SIZEX_BUTTON_CLOCK, 40, "Timer");
        Button* launchAlarm = new Button(160, 200, SIZEX_BUTTON_CLOCK, 40, "Alarm");

        launchClock->onreleased=simpleClock;
        launchTimer->onreleased=simpleTimer;
        launchAlarm->onreleased=alarmClock;

        while(1)
        {
            window.update();
        }
    }
}

void Clock::simpleClock(App *app, Gauss* objectPrt, void* data)
{
    tft_root.fillScreen(0x0000);
    int hour, minute;
    while(1)
    {
        tft_root.setTextFont(7);
        tft_root.setRotation(3);
        tft_root.setTextSize(3);
        tft_root.fillScreen(0x0000);

        getHour(&hour, &minute);

        tft_root.setCursor(10, 70);
        tft_root.setTextColor(0xFFFF);
        tft_root.print(hour);
        tft_root.print(":");
        tft_root.print(minute);
        tft_root.setTextFont(1);
        tft_root.setTextSize(1);
        delay(30000);
    }
    tft.setRotation(0);
}

void Clock::simpleTimer(App *app, Gauss* objectPrt, void* data)
{
    long timer = 0;
    long timerBegin = 0;

    Window window("chronometre");

    Button* start = new Button(240, 450, 100, 40, "start");
    Button* clear = new Button(80, 450, 100, 40, "clear");

    window.addChild(start);
    window.addChild(clear);

    window.drawAll();

    long  timer_actu = millis();

    while(1)
    {
        while(1)
        {
            if(start->isReleased())
                break;
            if(clear->isReleased())
            {
                tft.fillRect(100, 90, 120, 40, 0x0000);
                tft.setTextColor(0xFFFF);
                tft.drawCentreString("0", 160, 100, 4);
            }
            timer=millis();
        }

        while(1)
        {
            if(start->isReleased())
                break;
            if(clear->isReleased())
                timer=millis();
            if(timer_actu + 10 < millis())
            {
                tft.fillRect(100, 90, 120, 40, 0x0000);
                tft.setTextColor(0xFFFF);
                double toPrint = (float) (millis() - timer)/1000;
                tft.drawCentreString(String(toPrint), 160, 100, 4);
                timer_actu=millis();
            }
        }
    }
}

void Clock::alarmClock(App *app, Gauss* objectPrt, void* data)
{
}


#endif