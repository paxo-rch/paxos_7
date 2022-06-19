#ifndef BATTERY_HPP
#define BATTERY_HPP

#include <Wire.h>

#define ADC_BAT 35

int bat_mv = 0;

class Battery
{
    int getVolatge(){return (analogRead(35)*2) * 1000 / 1135;}
    int getLevel();
};

int Battery::getLevel()
{
  int v = getVolatge();
  if(v>4200)
    return 100;
  if(v>4100)
    return 90;
  if(v>3970)
    return 80;
  if(v>3920)
    return 70;
  if(v>3870)
    return 60;
  if(v>3830)
    return 50;
  if(v>3790)
    return 40;
  if(v>3750)
    return 30;
  if(v>3700)
    return 20;
  if(v>3600)
    return 10;
  if(v>3300)
    return 5;
  return 0;
}

Battery battery;


#endif /* BATTERY_HPP */