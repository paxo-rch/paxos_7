#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include "gpio.hpp"

#include "modules/serial.hpp"
#include "modules/sd.hpp"
#include "modules/button.hpp"
#include "modules/gsm.hpp"
#include "modules/light.hpp"
#include "modules/battery.hpp"

void init_hardware()
{
    Serial.begin(SERIAL_RATE);
    sd_manager.init();
    screen_light.init();
    SIM.begin(9600);
}

#endif /* HARDWARE_HPP */