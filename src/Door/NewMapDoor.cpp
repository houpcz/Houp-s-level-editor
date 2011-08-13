#include <string>

#include "door.h"
#include "NewMapDoor.h"
#include "DoorSystem.h"
#include "..\Script\ScriptSystem.h"
#include "..\Map.h"
#include "..\LevelEditor.h"

C_NewMapDoor::C_NewMapDoor(FILE *fr)
{
    title = "Menu";

    Load(fr);
}

C_NewMapDoor::C_NewMapDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_NEW_MAP;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

C_NewMapDoor::~C_NewMapDoor()
{
}

void C_NewMapDoor::FirstTime()
{
    fMapName = new C_Input(100, 10, 8, 1, "map01", "Level name");
    fMapWidth = new C_InputInteger(100, 40, 2, 1, ToString(C_LevelEditor::Inst()->GetGameSetup()->GetMapWidth()).c_str(), "Map width in tiles");
    fMapHeight = new C_InputInteger(100, 70, 2, 1, ToString(C_LevelEditor::Inst()->GetGameSetup()->GetMapHeight()).c_str(), "Map height in tiles");
    label[0] = new C_Label(5, 40, 6, 1, "Width", "nohelp");
    label[1] = new C_Label(5, 70, 6, 1, "Height", "nohelp");
    label[2] = new C_Label(5, 10, 6, 1, "Name", "nohelp");
    bOk = new C_Button(60, 95, 40, 20, "OK");

    AddFormElement(fMapName);
    AddFormElement(fMapWidth);
    AddFormElement(fMapHeight);
    AddFormElement(label[0]);
    AddFormElement(label[1]);
    AddFormElement(label[2]);
    AddFormElement(bOk);

    closeable = true;
    resizeableVertical = false;
    resizeableHorizontal = false;
    minimalizeable = false;
    isTitle = true;
}

void C_NewMapDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    if(button == LEFTBUTTON)
    {
        if(bOk->GetIsPushedAndUnpush())
        {
            C_LevelEditor::Inst()->NewMap(fMapName->GetString(), 2, fMapWidth->GetInteger(), fMapHeight->GetInteger());
            C_DoorSystem::Inst()->CloseDoor(D_NEW_SCRIPT);
            C_DoorSystem::Inst()->CloseDoor(D_SET_GROUPS);
            C_ScriptSystem::Inst()->DeleteAllScript();
            C_ScriptSystem::Inst()->MakeMainScriptGroup();
            close = true;
        }
    }
}

