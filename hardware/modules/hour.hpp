#ifndef HOUR_HPP
#define HOUR_HPP

void getHour(int *hours, int *minutes);

int ACTUAL_HOUR = 0;
int ACTUAL_MINUTE = 0;

byte update_hour()
{
    int day,month,year,minute,second,hour;
    SIM.RTCtime(&day,&month,&year,&hour,&minute,&second);
    ACTUAL_HOUR=hour;
    ACTUAL_MINUTE=minute;

    return 0;
}

void getHour(int *hours, int *minutes)
{
    *hours = ACTUAL_HOUR;
    *minutes = ACTUAL_MINUTE;
}

#endif