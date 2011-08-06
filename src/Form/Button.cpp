#include "Button.h"
#include "..\Engine.h"
#include "..\Door\DoorSystem.h"
#include "..\Utils\FontMan.h"

#include <GL/GL.h>

unsigned int C_Button::iActiveButton = 0;
unsigned int C_Button::nButton = 0;

C_Button::C_Button()
{
    nButton++;
    iButton=nButton;

    visible = true;
    focus = false;
    pushed = false;
    newPushed = false;
    newUnpushed = false;
}

C_Button::~C_Button()
{

}

C_Button::C_Button(int n_x, int n_y, int n_width, int n_height, string n_title,
                   char n_textAlign, string n_help,
                   C_Color n_buttonColor, C_Color n_titleColor)
{
    C_Button();
    Set(n_x, n_y, n_width, n_height, n_title, n_textAlign, n_help, n_buttonColor, n_titleColor);
}

void C_Button::Set(int n_x, int n_y, int n_width, int n_height, string n_title, char n_textAlign, string n_help, C_Color n_buttonColor, C_Color n_titleColor)
{
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
    title = n_title;
    textAlign = n_textAlign;
    help = n_help;
    colorButton = n_buttonColor;
    colorTitle = n_titleColor;
    focus = false;
    pushed = false;
    newUnpushed = false;
    visible = true;
}

void C_Button::MainLoop(int mouseX, int mouseY)
{
    focus = IsMouseOver(mouseX, mouseY);
    if(focus && help != "nohelp")
        C_DoorSystem::Inst()->OpenNewHelp(help);
}

void C_Button::MouseClick(int mouseX, int mouseY)
{
    newUnpushed = false;
    newPushed = false;

    if(!pushed && IsPushed(mouseX, mouseY))
    {
        pushed = true;
        newPushed = true;
        iActiveButton = iButton;
    }
    else
    if(pushed && IsPushed(mouseX, mouseY))
    {
        pushed = false;
        newUnpushed = true;
    }
}

void C_Button::Draw()
{
    if(visible)
    {
        if(focus)
            glColor3ubv(colorButton.GetDifferentColor(1.2f));
        else
            glColor3ubv(colorButton);
        if(pushed && !focus)
            glColor3ubv(colorButton.GetDifferentColor(0.6f));
        else if(pushed)
            glColor3ubv(colorButton.GetDifferentColor(0.8f));

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glVertex2i(x, y);
            glVertex2i(x + width, y);
            glVertex2i(x + width, y + height);
            glVertex2i(x, y + height);
        glEnd();

        if(pushed && !focus)
            glColor3ubv(C_Color::whiteSnow);

        glBegin(GL_LINE_LOOP);
            glVertex2i(x, y);
            glVertex2i(x + width, y);
            glVertex2i(x + width, y + height);
            glVertex2i(x, y + height);
        glEnd();
        glEnable(GL_TEXTURE_2D);

        if(focus)
            glColor3ubv(colorTitle);
        else
            glColor3ubv(colorTitle);

        FontMan * fm = FontMan::Inst();

        //int titleWidth = fm->GetTextWidth("impact12", title.c_str());
        fm->SetAnchor(textAlign);

        if(textAlign & fm->ALIGN_LEFT)
            fm->Print("impact12", x + 3, y + (height >> 1), "%s", title.c_str());
        else
            fm->Print("impact12", x + (width >> 1), y + (height >> 1), "%s", title.c_str());

        FontMan::Inst()->SetAnchor(FontMan::ALIGN_LEFT | FontMan::ALIGN_BOTTOM);
    }
}

bool C_Button::IsMouseOver(int mouseX, int mouseY)
{
      if(visible &&
         mouseX >= x && mouseX <= x + width &&
         mouseY >= y && mouseY <= y + height)
      {
          return true;
      } else
      {
          return false;
      }
}

bool C_Button::IsPushed(int mouseX, int mouseY)
{
      if(visible && App::Inst()->mouseNewLeftDown && IsMouseOver(mouseX, mouseY))
      {
          return true;
      } else
      {
          return false;
      }
}

C_Button::C_Button(const C_Button & button)
{
    x = button.x;
    y = button.y;
    width = button.width;
    height = button.height;
    colorButton = button.colorButton;
    colorTitle = button.colorTitle;
    title = button.title;
    help = button.help;
    textAlign = button.textAlign;
    visible = button.visible;
    pushed = button.pushed;
    focus = button.focus;
    newUnpushed = button.newUnpushed;
    newPushed = button.newPushed;

    iButton = nButton++;
}

C_Button & C_Button::operator=(const C_Button & button)
{
    x = button.x;
    y = button.y;
    width = button.width;
    height = button.height;
    colorButton = button.colorButton;
    colorTitle = button.colorTitle;
    title = button.title;
    help = button.help;
    textAlign = button.textAlign;
    visible = button.visible;
    pushed = button.pushed;
    focus = button.focus;
    newUnpushed = button.newUnpushed;
    newPushed = button.newPushed;

    iButton = iButton;

    return *this;
}
