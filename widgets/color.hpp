#ifndef COLOR_HPP
#define COLOR_HPP

/*###############################################*/
/*              theme configuration              */
/*###############################################*/

typedef uint16_t color_t;

#define THEME_BLACK 0
#define THEME_BLUE 1
#define THEME_GREEN 2
#define THEME_RED 3
#define THEME_YELLOW 4
#define THEME_WHITE 5
#define THEME_CONTRAST 6    // hight constrast

color_t theme_color[][3]=
{   // color, background color, border color
    {0xFFFF, 0x0000, 0xFFFF},   // black
    {0xFFFF, 0x22B5, 0x4800},   // blue
    {0xFFFF, 0x3C42, 0x4800},   // green
    {0xFFFF, 0xC861, 0x4800},   // red
    {0x0000, 0xFFC8, 0xEF37},   // yellow
    {0x0000, 0xFFFF, 0xBDF7},   // white
    {0x0000, 0xFFFF, 0x0000}    // contrast
};

/*###############################################*/
/*             alignement configuration          */
/*###############################################*/

#define LEFT_ALIGNMENT 0
#define RIGHT_ALIGNMENT 1
#define CENTER_ALIGNMENT 2

/*###############################################*/
/*             graphics configuration            */
/*###############################################*/

#define DEFAULT_THEME THEME_CONTRAST
#define DEFAULT_BORDER_SIZE 3
#define DEFAULT_ALIGNEMENT LEFT_ALIGNMENT
#define DEFAULT_RADIUS 15

#define CONTROL_BAR_SIZE 25

color_t contrastColor(color_t color)
{
    byte r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    byte g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    byte b = (((color & 0x1F) * 527) + 23) >> 6;

    int f = (r+g+b)/3;
    if(f>130)
    {
        r = 0;
        g = 0;
        b = 0;
    }else
    {
        r = 255;
        g = 255;
        b = 255;
    }
    return tft_root.color565(r, g, b);
}

color_t darkenColor(color_t color)
{
    byte r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    byte g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    byte b = (((color & 0x1F) * 527) + 23) >> 6;
    return tft_root.color565(r*2/3, g*2/3, b*2/3);
}

#endif