
#include "InformDoor.h"
#include "..\Utils\FileMan.h"
#include "..\Utils\FontMan.h"
#include <string.h>

C_InformDoor::C_InformDoor()
{
    doorKind = D_INFORM_DOOR;
    title = "Help";
    FirstTime();
}

void C_InformDoor::FirstTime()
{
    resizeableVertical = false;
    resizeableHorizontal = false;
    closeable = false;
    minimalizeable = false;
    isTitle = false;
}

C_InformDoor::C_InformDoor(int n_x, int n_y, string n_informString)
{
    Set(n_x, n_y, n_informString);
}

void C_InformDoor::Set(int n_x, int n_y, string n_informString)
{
    x = n_x;
    y = n_y;
    title = "Help";
    informString = n_informString;

    int nLines = 1;
    unsigned int lastPos = 0;
    unsigned int pos = 0;
    unsigned int maxWidth = 0;           // nejdelsi radek v informDoor
    string tempString;

    while((pos = informString.find(';', lastPos)) != string::npos)
    {
        nLines++;
        if(pos - lastPos > maxWidth)
        {
            maxWidth = pos - lastPos;
        }
        lastPos = pos + 1;
    }
    if((informString.size()) - lastPos > maxWidth)
    {
            maxWidth = (informString.size()) - lastPos;
    }

    width = maxWidth * LUCIDA_CHAR_WIDTH_PX8 + 10;
    height = 9 * nLines + 8; //(int) lucida[PX16].h + 5;

    if(x + width > App::Inst()->GetScreenWidth())
        x = App::Inst()->GetScreenWidth() - width;
    if(y + height > App::Inst()->GetScreenHeight())
        y = App::Inst()->GetScreenHeight() - height;
}

void C_InformDoor::DrawContent()
{
    FontMan * fm = FontMan::Inst();

    //door//
    glDisable(GL_TEXTURE_2D);
    glColor3ub(44, 244, 244);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);	glVertex2i(0, 0);
        glTexCoord2f(1.0f, 0.0f);	glVertex2i(width, 0);
        glTexCoord2f(1.0f, 1.0f);	glVertex2i(width, height);
        glTexCoord2f(0.0f, 1.0f);	glVertex2i(0, height);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    string temp1;
    unsigned int line = 0;
    unsigned int lastPos = 0;
    unsigned int pos = 0;

    while((pos = informString.find(';', lastPos)) != string::npos)
    {
        line++;
        temp1 = informString.substr(lastPos, pos - lastPos);
        if(line % 2 == 0)
            glColor3ub(0, 0, 0);
        else
            glColor3ub(0, 0, 20);

        fm->Print("lucida8", 5, (9 * line) + 3, "%s", temp1.c_str());
        lastPos = pos + 1;
    }
    line++;
    temp1 = informString.substr(lastPos, informString.size() - lastPos);
    if(line % 2 == 0)
        glColor3ub(0, 0, 0);
    else
        glColor3ub(0, 0, 20);

    pos = 8 * (line + 1) + 3;
    fm->Print("lucida8", 5, (9 * line) + 3, "%s", temp1.c_str());

    //fm->Print("lucida8", 5, 11, "%s", informString.c_str());
}

