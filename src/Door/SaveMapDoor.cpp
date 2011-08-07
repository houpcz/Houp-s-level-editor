#include <string>

#include "Door.h"
#include "SaveMapDoor.h"
#include "..\Map.h"
#include "..\LevelEditor.h"

C_SaveMapDoor::C_SaveMapDoor(FILE *fr)
{
    title = "Menu";

    Load(fr);
}

C_SaveMapDoor::C_SaveMapDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_SAVE_MAP;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

void C_SaveMapDoor::FirstTime()
{
    string mapName = C_LevelEditor::Inst()->GetMap()->GetName();
    fMapName = new C_Input(100, 10, 8, 1, mapName, "Map name");
    label[0] = new C_Label(5, 10, 6, 1, "Name", "nohelp", 0, 0, 0, 155, 155, 155, 155, 155, 155, 155, 155, 155);
    bOk = new C_Button((width - 40) >> 1, 35, 40, 20, "OK");

    AddFormElement(fMapName);
    AddFormElement(label[0]);
    AddFormElement(bOk);

    closeable = true;
    resizeableVertical = false;
    resizeableHorizontal = false;
    minimalizeable = false;
    isTitle = true;
}

void C_SaveMapDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    C_Map * pMap = C_LevelEditor::Inst()->GetMap();
    if(button == LEFTBUTTON)
    {
        if(bOk->GetIsPushedAndUnpush())
        {
            pMap->SetName(fMapName->GetString());
            C_LevelEditor::Inst()->SaveMap();
            close = true;
        }
    }
}
