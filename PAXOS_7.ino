#include "soc/rtc_wdt.h"
#define ESP32 1
#include "hardware/hardware.hpp"
#include "extern/extern.hpp"
#include "task/task.hpp"
#include "widgets/gauss.hpp"
#include "system/encoder.hpp"
#include "launcher/launcher.hpp"
#include "system/sim800l_reader.hpp"
#include "system/hour/hour.hpp"

void setupTasks()
{
    Task *autoHourUpdate = new Task(5000, update_hour, TASKS_LOOP); // auto hour update
    SystemTasks.addTask(autoHourUpdate);
}

void setup()
{
    psramInit();

    pinMode(SCREEN_LED, OUTPUT);    // declare LED
    pinMode(SCREEN_POWER, OUTPUT);  // declare power
    pinMode(HOME_BUTTON, INPUT_PULLUP);
    pinMode(23, OUTPUT);

    digitalWrite(SCREEN_POWER, HIGH);   // power on
    
    ledcAttachPin(SCREEN_LED, PWM1_Ch);
    ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
    
    init_hardware();    // init the hardware
    tft_root.init();         // init the screen

    tft_root.fillScreen(0x0000);

    tft.setColorDepth(16);
    tft.createSprite(320, 480);
    tft.fillScreen(0xFFFF);

    tft_root.fillScreen(0xFFFF);
    
    Image logo("/logo.bmp", 3, 0);
    logo.draw();

    tft.pushSprite(0,0);

    screen_light.ON();

    delay(500);
}

void backgroundLoop(void* arg)
{
    while(true)
    {
        vTaskDelay(10);
        taskmanager.executeAll();
    }
}

void loop()
{
    Menu menu;
    menu.load();
    while(true);
    
    while(1)
    {
        Serial.println("coucou");
        delay(100);
    }
}