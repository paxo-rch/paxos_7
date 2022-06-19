#ifndef BUTTON_HPP
#define BUTTON_HPP

class HomeButton
{
    public:
    bool pressed();
    void resetButtonTimer();

    private:
    unsigned int button_chrono = 0;
    byte state = 0;
};

bool HomeButton::pressed()
{
    bool input = !digitalRead(HOME_BUTTON);

    if(input == 0)
        button_chrono = millis();

    if(millis() - button_chrono > 400)
    {
        button_chrono = millis();
    }

    if(state==0 && input == 1)
    {
        state = 1;
        return 0;
    }

    if(state==1 && input == 0)
    {
        state = 0;
        return 1;
    }

    return 0;
}

void HomeButton::resetButtonTimer()
{
    button_chrono = millis();
}

HomeButton home_button;

#endif