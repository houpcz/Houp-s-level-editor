#include <string>

#include "Door.h"
#include "SetMapDoor.h"
#include "..\Script\ScriptSystem.h"
#include "..\LevelEditor.h"

C_SetMapDoor::C_SetMapDoor(FILE * fr)
{
    title = "Map";

    Load(fr);
}

C_SetMapDoor::C_SetMapDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_SET_MAP;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

void C_SetMapDoor::FirstTime()
{
    C_LevelEditor * levelEditor = C_LevelEditor::Inst();
    C_Map * pMap = C_LevelEditor::Inst()->GetMap();
    char alignCenter = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;

    if(C_LevelEditor::Inst()->GetMap() == NULL)
    {
        close = true;
        return;
    }

    fMapName = new C_Input(100, 10, 8, 1, pMap->GetName(), "Level name");
    fMapWidth = new C_InputInteger(100, 40, 3, 1, ToString(pMap->GetMapWidth()).c_str(), "Map width in tiles");
    fMapHeight = new C_InputInteger(100, 70, 3, 1, ToString(pMap->GetMapHeight()).c_str(), "Map height int tiles");
    label = new C_Label(5, 10, 6, 1, "Name", "nohelp", 0, 0, 0, 155, 155, 155, 155, 155, 155, 155, 155, 155);
    AddFormElement(label);
    label = new C_Label(5, 40, 6, 1, "Width", "nohelp", 0, 0, 0, 155, 155, 155, 155, 155, 155, 155, 155, 155);
    AddFormElement(label);
    label = new C_Label(5, 70, 6, 1, "Height", "nohelp", 0, 0, 0, 155, 155, 155, 155, 155, 155, 155, 155, 155);
    AddFormElement(label);
    bLeft = new C_Button(185, 40, 25, 21, "L", alignCenter, "Add tiles on the left of map");
    bTop = new C_Button(185, 70, 25, 21, "T", alignCenter, "Add tiles on the top of map");
    AddFormElement(fMapName);
    AddFormElement(fMapHeight);
    AddFormElement(fMapWidth);
    AddFormElement(bLeft);
    AddFormElement(bTop);

    bOk = new C_Button(90, 100, 40, 20, "OK", alignCenter);
    width = 230;
    height = 140;

    AddFormElement(bOk);

    closeable = true;
    resizeableVertical = false;
    resizeableHorizontal = false;
    minimalizeable = false;
    isTitle = true;
}

void C_SetMapDoor::DoorEverTime()
{
    C_Door::DoorEverTime();

    if(C_LevelEditor::Inst()->GetMap() == NULL)
        close = true;
}

void C_SetMapDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);
    C_LevelEditor * levelEditor = C_LevelEditor::Inst();

    if(button == LEFTBUTTON)
    {
        C_Map * pMap = C_LevelEditor::Inst()->GetMap();

        if(bOk->GetIsPushedAndUnpush())
        {
            pMap->SetName(fMapName->GetString());
            pMap->Resize(fMapWidth->GetInteger(), fMapHeight->GetInteger(), bLeft->GetIsPushed(), bTop->GetIsPushed());

            close = true;
        }
    }
}


