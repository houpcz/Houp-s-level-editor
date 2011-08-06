#ifndef _COLOR_H_
#define _COLOR_H_

#include <GL/GL.h>

/** \brief
C_Color stores red, green, blue part of color in one object
which can be directly used in glColor3ubv()
 */
class C_Color
{
    private :
        GLubyte color[3];/**< red, green, blue */

    public :
        C_Color(const C_Color &);
        C_Color & operator=(const C_Color &);
        C_Color(char n_red, char n_green, char n_blue);
        ~C_Color();
        C_Color() { C_Color(255, 255, 255);};


        /** \brief
         * Use in glColor3ubv
         * \return operator const
         *
         */
        operator const GLubyte*() { return color;};


        /** \brief
         * Adds i to each part of color
         *
         * \param i int
         * \return C_Color
         *
         */
        C_Color operator+(int i) const;


        /** \brief
         * Multiply red, green, blue by m
         * \param m float multiplayer
         * \return C_Color
         *
         */
        C_Color GetDifferentColor(float m);

        static C_Color blueDark;
        static C_Color blueDarkLighter;
        static C_Color blueSteel;
        static C_Color blueLight;
        static C_Color redDark;
        static C_Color whiteSnow;
        static C_Color greyDarkSlate;
        static C_Color black;
};

#endif
