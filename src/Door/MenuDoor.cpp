#include <string>

#include "Door.h"
#include "MenuDoor.h"
#include "DoorSystem.h"
#include "..\Map.h"

C_MenuDoor::C_MenuDoor(FILE * fr)
{
    title = "Menu";
    Load(fr);
}

C_MenuDoor::C_MenuDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_MENU_DOOR;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

C_MenuDoor::~C_MenuDoor()
{
}

void C_MenuDoor::FirstTime()
{
    int buttonWidth = 120;
    int buttonHeight = 30;

    char align = FontMan::ALIGN_VCENTER | FontMan::ALIGN_HCENTER;

    mButton[0] = new C_Button(0, 0 * buttonHeight, buttonWidth, buttonHeight, "Map", align);
    mButton[1] = new C_Button(0, 1 * buttonHeight, buttonWidth, buttonHeight, "Tiles", align, "nohelp", C_Color::blueDarkLighter, C_Color::whiteSnow);
    mButton[2] = new C_Button(0, 2 * buttonHeight, buttonWidth, buttonHeight, "Patterns", align);
    mButton[3] = new C_Button(0, 3 * buttonHeight, buttonWidth, buttonHeight, "Monster", align, "nohelp", C_Color::blueDarkLighter, C_Color::whiteSnow);
    mButton[4] = new C_Button(0, 4 * buttonHeight, buttonWidth, buttonHeight, "Object", align);
    mButton[5] = new C_Button(0, 5 * buttonHeight, buttonWidth, buttonHeight, "Item", align, "nohelp", C_Color::blueDarkLighter, C_Color::whiteSnow);
    mButton[6] = new C_Button(0, 6 * buttonHeight, buttonWidth, buttonHeight, "Script kind", align, "Setup script kinds");
    mButton[7] = new C_Button(0, 7 * buttonHeight, buttonWidth, buttonHeight, "Script", align, "Setup scripts", C_Color::blueDarkLighter, C_Color::whiteSnow);
    mButton[8] = new C_Button(0, 8 * buttonHeight, buttonWidth, buttonHeight, "Script groups", align, "Setup script groups");

    height = 30 * 9;

    for(int loop1 = 0; loop1 < N_BUTTON; loop1++)
    {
        AddFormElement(mButton[loop1]);
    }

    closeable = true;
    resizeableVertical = false;
    resizeableHorizontal = false;
    minimalizeable = false;
    isTitle = true;
    scrollable = false;

}

void C_MenuDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    int loop1;
    if(button == LEFTBUTTON)
    {
        for(loop1 = 0; loop1 < N_BUTTON; loop1++)
        {
            if(mButton[loop1]->GetIsPushedAndUnpush())
                C_DoorSystem::Inst()->OpenNewDoor(loop1);
        }
        layer = ++topLayer;
    }
}

