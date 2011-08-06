#include <stdlib.h>
#include "Painter.h"

Painter * Painter::painter = NULL;

Painter::Painter()
{
}

Painter * Painter::Inst()
{
    if(!painter)
        painter = new Painter();

    return painter;
}

void Painter::DrawTexture(S_Texture * text)
{
    glColor3ub(255, 255, 255);
    glBindTexture(GL_TEXTURE_2D,text->texID);
    glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);	glVertex2i(0, 0);
		glTexCoord2f(1.0f, 0.0f);	glVertex2i(text->width, 0);
		glTexCoord2f(1.0f, 1.0f);	glVertex2i(text->width, text->height);
		glTexCoord2f(0.0f, 1.0f);	glVertex2i(0, text->height);
	glEnd();
}

void Painter::DrawRectangle(int x1, int y1, int x2, int y2, C_Color color)
{
    glColor3ubv(color);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_STRIP);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
		glVertex2i(x1, y1);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
