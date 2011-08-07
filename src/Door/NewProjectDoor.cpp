#include <string>

#include "door.h"
#include "NewProjectDoor.h"
#include "DoorSystem.h"
#include "..\Script\ScriptSystem.h"
#include "..\Map.h"
#include "..\LevelEditor.h"

C_NewProjectDoor::C_NewProjectDoor(FILE *fr)
{
    title = "Menu";

    Load(fr);
}

C_NewProjectDoor::C_NewProjectDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_NEW_PROJECT;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

C_NewProjectDoor::~C_NewProjectDoor()
{
}

void C_NewProjectDoor::FirstTime()
{
    fProjectName = new C_Input(100, 10, 8, 1, "map01", "Level name");
    label[0] = new C_Label(5, 10, 6, 1, "Name", "nohelp");
    bOk = new C_Button(60, 35, 40, 20, "OK");

    AddFormElement(fProjectName);
    AddFormElement(label[0]);
    AddFormElement(bOk);

    closeable = true;
    resizeableVertical = false;
    resizeableHorizontal = false;
    minimalizeable = false;
    isTitle = true;
}

void C_NewProjectDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    if(button == LEFTBUTTON)
    {
        if(bOk->GetIsPushedAndUnpush())
        {
            C_LevelEditor::Inst()->NewProject(fProjectName->GetString());
            close = true;
        }
    }
}


