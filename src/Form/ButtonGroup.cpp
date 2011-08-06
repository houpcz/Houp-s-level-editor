#include "ButtonGroup.h"

C_ButtonGroup::C_ButtonGroup()
{
    kind = ONLY_ONE_PUSHED;
}

C_ButtonGroup::~C_ButtonGroup()
{
    button.clear();
}

void C_ButtonGroup::Draw()
{
    unsigned int loop1;
    for(loop1 = 0; loop1 < button.size(); loop1++)
        button[loop1].Draw();
}

void C_ButtonGroup::MainLoop(int mouseX, int mouseY)
{
    unsigned int loop1;
    for(loop1 = 0; loop1 < button.size(); loop1++)
        button[loop1].MainLoop(mouseX, mouseY);
}

void C_ButtonGroup::MouseClick(int mouseX, int mouseY)
{
    unsigned int loop1, loop2;
    for(loop1 = 0; loop1 < button.size(); loop1++)
        button[loop1].MouseClick(mouseX, mouseY);

    for(loop1 = 0; loop1 < button.size(); loop1++)
    {
        if(button[loop1].GetNewPushed() && kind == ONLY_ONE_PUSHED)
        {
            for(loop2 = 0; loop2 < button.size(); loop2++)
            {
                if(loop2 != loop1)
                    button[loop2].SetIsPushed(false);
            }
            break;
        }
    }
}

void C_ButtonGroup::SetPushed(bool n_pushed, int iButton)
{
    if(iButton == 0)
        iButton = iButton;

    if(n_pushed && kind == ONLY_ONE_PUSHED)
    {
        for(unsigned int loop1 = 0; loop1 < button.size(); loop1++)
            button[loop1].SetIsPushed(false);
    }

    button[iButton].SetIsPushed(n_pushed);
}

void C_ButtonGroup::AddButton(int n_x, int n_y, int n_width, int n_height, string n_title, char n_textAlign, string n_help, C_Color n_buttonColor, C_Color n_titleColor)
{
    button.push_back(C_Button(n_x, n_y, n_width, n_height, n_title, n_textAlign, n_help, n_buttonColor, n_titleColor));
}
