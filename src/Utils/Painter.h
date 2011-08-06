#ifndef _PAINTER_H_
#define _PAINTER_H_

#include "TextureLoader.h"
#include "Color.h"

class Painter
{
  private :
    static Painter * painter;
    Painter();
  public :
    static Painter * Inst();
    void DrawTexture(S_Texture * text);
    void DrawRectangle(int x1, int y1, int x2, int y2, C_Color color);
};

#endif
