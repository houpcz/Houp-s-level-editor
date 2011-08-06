#include "Color.h"

C_Color C_Color::black           = C_Color(0, 0, 0);
C_Color C_Color::blueLight       = C_Color(173, 216, 230);
C_Color C_Color::blueSteel       = C_Color(70, 130, 180);
C_Color C_Color::blueDarkLighter = C_Color(33, 33, 88);
C_Color C_Color::blueDark        = C_Color(33, 33, 66);
C_Color C_Color::redDark         = C_Color(66, 33, 33);
C_Color C_Color::whiteSnow       = C_Color(255, 250, 250);
C_Color C_Color::greyDarkSlate   = C_Color(47, 79, 79);


C_Color::C_Color(const C_Color & c2)
{
    color[0] = c2.color[0];
    color[1] = c2.color[1];
    color[2] = c2.color[2];
}
C_Color & C_Color::operator=(const C_Color & c2)
{
    color[0] = c2.color[0];
    color[1] = c2.color[1];
    color[2] = c2.color[2];

    return *this;
}

C_Color::C_Color(char n_red, char n_green, char n_blue)
{
    color[0] = n_red;
    color[1] = n_green;
    color[2] = n_blue;
}

C_Color::~C_Color()
{
}

C_Color C_Color::GetDifferentColor(float m)
{
    return C_Color((GLubyte) (color[0] * m),
            (GLubyte) (color[1] * m),
            (GLubyte) (color[2] * m));
}

C_Color C_Color::operator+(int i) const
{
    GLubyte c[3];
    for(int loop1 = 0; loop1 < 3; loop1++)
    {
        if(color[loop1] + i > 255)
            c[loop1] = 255;
        else if(color[loop1] + i < 0)
            c[loop1] = 0;
        else
            c[loop1] = color[0] + i;
    }

    return C_Color(c[0], c[1], c[2]);
}
