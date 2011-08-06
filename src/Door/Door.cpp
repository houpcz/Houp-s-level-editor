#include "Door.h"
#include "DoorSystem.h"
#include "..\Engine.h"

#include <GL/GL.h>
#include "..\Utils\FileMan.h"
#include "..\Utils\FontMan.h"

long int C_Door::topLayer = 0;
bool C_Door::wasClick = false;
bool C_Door::wasOver = false;

void C_Door::Draw()
{
    if(firstTime) /// It wasnt properly initialized
        return;

    if(!minimalize)
    {
        glPushMatrix();
        glDisable(GL_TEXTURE_2D);
        glTranslatef(x, y, 0.0);

        if(isTitle)
        {
            //title//
            if(layer == topLayer)
                glColor3ub(255, 0, 0);
            else
                glColor3ub(50, 50, 255);

            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);	glVertex2i(0, 0);
                glTexCoord2f(1.0f, 0.0f);	glVertex2i(width, 0);
                glTexCoord2f(1.0f, 1.0f);	glVertex2i(width, TITLE_HEIGHT);
                glTexCoord2f(0.0f, 1.0f);	glVertex2i(0, TITLE_HEIGHT);
            glEnd();

            if(layer == topLayer)
                glColor3ub(244, 244, 244);
            else
                glColor3ub(44, 44, 44);

            glScissor(x, App::Inst()->GetScreenHeight() - (y + TITLE_HEIGHT), width - 2, TITLE_HEIGHT);// Definování oøezávací oblasti
            glEnable(GL_SCISSOR_TEST);
                FontMan::Inst()->Print("impact12", 5, TITLE_HEIGHT - 3, "%s", title.c_str());
            glDisable(GL_SCISSOR_TEST);

            // minimalize area
            if(minimalizeable)
            {
                if(layer == topLayer)
                    glColor3ub(170, 0, 0);
                else
                    glColor3ub(0, 0, 170);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f);	glVertex2i(width - (2 * TITLE_HEIGHT) - 2, 1);
                    glTexCoord2f(1.0f, 0.0f);	glVertex2i(width - (1 * TITLE_HEIGHT) - 2, 1);
                    glTexCoord2f(1.0f, 1.0f);	glVertex2i(width - (1 * TITLE_HEIGHT) - 2, TITLE_HEIGHT - 1);
                    glTexCoord2f(0.0f, 1.0f);	glVertex2i(width - (2 * TITLE_HEIGHT) - 2, TITLE_HEIGHT - 1);
                glEnd();
            }

            // close area
            if(closeable)
            {
                if(layer == topLayer)
                    glColor3ub(120, 0, 0);
                else
                    glColor3ub(0, 0, 120);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f);	glVertex2i(width - (1 * TITLE_HEIGHT) - 1, 1);
                    glTexCoord2f(1.0f, 0.0f);	glVertex2i(width - (0 * TITLE_HEIGHT) - 1, 1);
                    glTexCoord2f(1.0f, 1.0f);	glVertex2i(width - (0 * TITLE_HEIGHT) - 1, TITLE_HEIGHT - 1);
                    glTexCoord2f(0.0f, 1.0f);	glVertex2i(width - (1 * TITLE_HEIGHT) - 1, TITLE_HEIGHT - 1);
                glEnd();
            }

            glTranslatef(0.0, TITLE_HEIGHT, 0.0);

            glScissor(x, App::Inst()->GetScreenHeight()-(y + height + TITLE_HEIGHT), width, height);// Definování oøezávací oblasti
        } else  // if isTitle
            glScissor(x, App::Inst()->GetScreenHeight()-(y + height), width, height);// Definování oøezávací oblasti

        //////////
        // DOOR //
        //////////

        glColor3ub(180, 180, 180);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);	glVertex2i(0, 0);
            glTexCoord2f(1.0f, 0.0f);	glVertex2i(width, 0);
            glTexCoord2f(1.0f, 1.0f);	glVertex2i(width, height);
            glTexCoord2f(0.0f, 1.0f);	glVertex2i(0, height);
        glEnd();

        glEnable(GL_TEXTURE_2D);

        if(scrollable && maxScrollY != 0 &&
           scrollY > maxScrollY)
        {
            scrollY = maxScrollY;
        }
        glTranslated(0, -scrollY, 0);

        glEnable(GL_SCISSOR_TEST);
        glPushMatrix();
            DrawContent();
        glPopMatrix();
        glDisable(GL_SCISSOR_TEST);

        glTranslated(0, scrollY, 0);

        glDisable(GL_TEXTURE_2D);

        ///////////
        // OBRYS //
        ///////////

        if(layer == topLayer)
            glColor3ub(255, 0, 0);
        else
            glColor3ub(50, 50, 255);

        glBegin(GL_LINE_STRIP);
            glVertex2d(0,0);
            glVertex2d(width, 0);
            glVertex2d(width, height);
            glVertex2d(0, height);
            glVertex2d(0,0);
        glEnd();

        // resize area
        if(IsMouseInDoor())
            if(resizeableHorizontal || resizeableVertical)
            {
                glColor3ub(0, 180, 0);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f);	glVertex2i(width - RESIZE_AREA, height - RESIZE_AREA);
                    glTexCoord2f(1.0f, 0.0f);	glVertex2i(width, height - RESIZE_AREA);
                    glTexCoord2f(1.0f, 1.0f);	glVertex2i(width, height);
                    glTexCoord2f(0.0f, 1.0f);	glVertex2i(width - RESIZE_AREA, height);
                glEnd();
            }

        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void C_Door::DoorActionSetTop()
{
}

void C_Door::MainLoop()
{
    if(firstTime)
    {
        FirstTime();
        firstTime = false;
    }

    if(!minimalize)
    {
        x += C_DoorSystem::Inst()->GetWorldRelX();
        y += C_DoorSystem::Inst()->GetWorldRelY();

        DoorEverTime();

        if(IsMouseInTitle() || IsMouseInDoor())
            wasOver = true;

        if(IsMouseInTitle() && !wasClick)
        {
            wasClick = true;

            if(App::Inst()->mouseNewLeftDown)
            {
                SetTopDoor();
                drag = true;
            }

            if(IsMouseInMinimalize() && App::Inst()->mouseNewLeftDown && minimalizeable)
            {
                minimalize = true;
            }

            if(IsMouseInClose() && App::Inst()->mouseNewLeftDown && closeable)
            {
                close = true;
            }
            interMouseX = -1;
            interMouseY = -1;
        } else

        if(IsMouseInDoor() && !wasClick)
        {
            wasClick = true;

            interMouseX = App::Inst()->mouseX - x;
            interMouseY = App::Inst()->mouseY - y - TITLE_HEIGHT + (int) scrollY;

            if(App::Inst()->mouseNewLeftDown)
            {
                topLayer++;
                layer = topLayer;

                if(IsMouseInResize() && (resizeableVertical || resizeableHorizontal))
                {
                    resize = true;
                } else
                    DoorActionDown(LEFTBUTTON);
            }

            DoorActionIn();

            if(App::Inst()->mouseNewRightDown && !resize)
            {
                topLayer++;
                layer = topLayer;
                DoorActionDown(RIGHTBUTTON);
            }

            if(App::Inst()->mouseNewLeftUp && !resize)
            {
                DoorActionUp(LEFTBUTTON);
            }

            if(App::Inst()->mouseNewRightUp && !resize)
            {
                DoorActionUp(RIGHTBUTTON);
            }
        } else
        {
            interMouseX = -1;
            interMouseY = -1;
            DoorActionOut();
        }


        if(scrollable)
        {
            if((IsMouseInDoor() && App::Inst()->mouseWheelDown) ||
               (GetIsTopDoor() && App::Inst()->GetKey(SDLK_PAGEDOWN, false)))
               {
                scrollY += App::Inst()->GetDiffTime() * 600;
               }
            if((IsMouseInDoor() && App::Inst()->mouseWheelUp)   ||
               (GetIsTopDoor() && App::Inst()->GetKey(SDLK_PAGEUP, false)))
                scrollY -= App::Inst()->GetDiffTime() * 600;

            if(scrollY < 0)
                scrollY = 0;
        }

        if(drag && layer == topLayer)
        {
           x += App::Inst()->mouseRelX;
           y += App::Inst()->mouseRelY;
        }

        if(resize && layer == topLayer)
        {
            if(resizeableHorizontal)
            {
                width += App::Inst()->mouseRelX;
                if(width < MIN_WIDTH)
                    width = MIN_WIDTH;
                if(maxWidth != UNLIMITED && width > maxWidth)
                    width = maxWidth;
            }

            if(resizeableVertical)
            {
                height += App::Inst()->mouseRelY;
                if(height < MIN_HEIGHT)
                    height =  MIN_HEIGHT;
                if(maxHeight != UNLIMITED && height > maxHeight)
                    height = maxHeight;
            }
        }

        if(!App::Inst()->mouseLeft)
        {
            drag = false;
            resize = false;
        }
    }
}

bool C_Door::IsMouseInTitle()
{
    if(!isTitle)        // neni-li titulek, nemuze byt mys v nem
        return false;
    else
        if(App::Inst()->mouseX >= x && App::Inst()->mouseX <= x + width &&
           App::Inst()->mouseY >= y && App::Inst()->mouseY <= y + TITLE_HEIGHT)
           return true;
        else
           return false;
}

bool C_Door::IsMouseInMinimalize()
{
    if(App::Inst()->mouseX >= x + width - (2 * TITLE_HEIGHT) - 2 && App::Inst()->mouseX <= x + width - TITLE_HEIGHT - 2 &&
       App::Inst()->mouseY >= y + 1 && App::Inst()->mouseY <= y + TITLE_HEIGHT - 1)
       return true;
    else
       return false;
}

bool C_Door::IsMouseInClose()
{
    if(App::Inst()->mouseX >= x + width - (1 * TITLE_HEIGHT) - 1 && App::Inst()->mouseX <= x + width - 1 &&
       App::Inst()->mouseY >= y + 1 && App::Inst()->mouseY <= y + TITLE_HEIGHT - 1)
       return true;
    else
       return false;
}

bool C_Door::IsMouseInResize()
{
    if(isTitle)
        if(App::Inst()->mouseX >= x + width - RESIZE_AREA && App::Inst()->mouseX <= x + width &&
           App::Inst()->mouseY >= y + TITLE_HEIGHT + height - RESIZE_AREA && App::Inst()->mouseY <= y + TITLE_HEIGHT + height)
           return true;
        else
           return false;
    else
        if(App::Inst()->mouseX >= x + width - RESIZE_AREA && App::Inst()->mouseX <= x + width &&
           App::Inst()->mouseY >= y + height - RESIZE_AREA && App::Inst()->mouseY <= y + height)
           return true;
        else
           return false;
}

bool C_Door::IsMouseInDoor()
{
    if(isTitle)
        if(App::Inst()->mouseX >= x && App::Inst()->mouseX <= x + width &&
           App::Inst()->mouseY >= y + TITLE_HEIGHT && App::Inst()->mouseY <= y + TITLE_HEIGHT + height &&
           !C_DoorSystem::Inst()->IsMouseInButtonBar())
           return true;
        else
           return false;
    else
        if(App::Inst()->mouseX >= x && App::Inst()->mouseX <= x + width &&
           App::Inst()->mouseY >= y && App::Inst()->mouseY <= y + height &&
           !C_DoorSystem::Inst()->IsMouseInButtonBar())
           return true;
        else
           return false;
}

C_Door::C_Door()
{
    x = 0;
    y = 0;
    width = 200;
    height = 50;
    maxScrollY = 0;
    topLayer++;
    layer = topLayer;
    drag = false;
    minimalize = false;
    resize = false;
    close = false;
    firstTime = true;
    dontSave = false;

    resizeableVertical = true;
    resizeableHorizontal = true;
    maxHeight = UNLIMITED;
    maxWidth = UNLIMITED;

    closeable = true;
    minimalizeable = true;
    isTitle = true;
    scrollable = false;

    doorKind = D_NONAME;

    scrollY = 0;
}

void C_Door::FirstTime()
{
    resizeableVertical = true;
    resizeableHorizontal = true;
    closeable = true;
    minimalizeable = true;
    isTitle = true;
}

void C_Door::SetTopDoor()
{
    topLayer++;
    layer = topLayer;

    if(x > App::Inst()->GetScreenWidth() ||
       x < -width)
        x = (App::Inst()->GetScreenWidth() - width) / 2;
    if(y > App::Inst()->GetScreenHeight() ||
       y < -height)
        y = (App::Inst()->GetScreenHeight() - height) / 2;

    DoorActionSetTop();
}

void C_Door::DrawContent()
{
    for(vector<C_FormElement *>::iterator it = formElement.begin(); it != formElement.end(); it++) {
        (*it)->Draw();
    }
}

void C_Door::DoorActionDown(int button)
{
    if(button == LEFTBUTTON)
        for(vector<C_FormElement *>::iterator it = formElement.begin(); it != formElement.end(); it++) {
            (*it)->MouseClick(interMouseX, interMouseY);
        }
}

void C_Door::DoorEverTime()
{
    for(vector<C_FormElement *>::iterator it = formElement.begin(); it != formElement.end(); it++) {
        (*it)->MainLoop(interMouseX, interMouseY);
    }
}

void C_Door::Save(FILE * fw)
{
    FileMan::Inst()->SaveString(title, fw);
    FileMan::Inst()->SaveInteger(x, fw);
    FileMan::Inst()->SaveInteger(y, fw);
    FileMan::Inst()->SaveInteger(width, fw);
    FileMan::Inst()->SaveInteger(height, fw);
}

void C_Door::Load(FILE * fr)
{
    FileMan::Inst()->LoadString(title, fr);
    FileMan::Inst()->LoadInteger(x, fr);
    FileMan::Inst()->LoadInteger(y, fr);
    FileMan::Inst()->LoadInteger(width, fr);
    FileMan::Inst()->LoadInteger(height, fr);
}

C_Door::~C_Door()
{
    for(vector<C_FormElement *>::iterator it = formElement.begin(); it != formElement.end(); it++) {
        delete *it;
    }

    formElement.clear();
}
