#define SD_CS           4
#define SD_POWER        0
#define SCREEN_LED      25
#define SCREEN_POWER    22
#define HOME_BUTTON     32
#define SIM_RX 26
#define SIM_TX 27


void gpio_set(int gpio, bool value)
{
    digitalWrite(gpio, value);
}