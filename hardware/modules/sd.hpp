#ifndef SD_HPP
#define SD_HPP

#include <SD.h>

class SD_Manager
{
    public:
    void init();    // control
    void exit();

    bool isConnected();
};

void SD_Manager::init()
{
    //gpio_set(SD_POWER, ON);
    SD.begin(SD_CS);
}

void SD_Manager::exit()
{
    SD.end();
    //gpio_set(SD_POWER, OFF);
}

bool SD_Manager::isConnected()
{
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
        return false;
    else
        return true;
}

SD_Manager sd_manager;

#endif