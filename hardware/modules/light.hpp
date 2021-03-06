#ifndef LIGHT_HPP
#define LIGHT_HPP

#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000

class ScreenLight
{
    private:
    int STATE_LIGHT = 0;
    
    public:
    void analog(int state)
    {
        if (state < STATE_LIGHT)
        {
            while (STATE_LIGHT > state)
            {
                ledcWrite(PWM1_Ch, STATE_LIGHT--);
                delay(1);
            }
            return;
        }
        if (state > STATE_LIGHT)
        {
            while (STATE_LIGHT < state)
            {
                ledcWrite(PWM1_Ch, STATE_LIGHT++);
                delay(1);
            }
            return;
        }
    }

    void ON() { analog(255); }
    void OFF() { analog(-1); }
    void init()
    {
    }
};

ScreenLight screen_light;

#endif