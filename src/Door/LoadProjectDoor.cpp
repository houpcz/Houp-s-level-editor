#include <string>

#include "door.h"
#include "DoorSystem.h"
#include "LoadProjectDoor.h"
#include "..\Script\ScriptSystem.h"
#include "..\LevelEditor.h"
#include "..\Utils\FileMan.h"
C_LoadProjectDoor::C_LoadProjectDoor(FILE * fr)
{
        Load(fr);
}

C_LoadProjectDoor::C_LoadProjectDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_LOAD_PROJECT;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

C_LoadProjectDoor::~C_LoadProjectDoor()
{
}

void C_LoadProjectDoor::FirstTime()
{
    char c_str[256];
    sprintf(c_str, "*", C_LevelEditor::Inst()->GetGameSetup()->GetName().c_str());
    string allFileNames = "";
    string temp;

    fileName = FileMan::Inst()->GetDirectoryList(c_str);

    int clearID = -1;
    for(int loop1 = 0; loop1 < fileName.size(); loop1++)
    {
        temp = fileName[loop1];

        if(temp.compare("data") == 0)
        {
            clearID = loop1;
        } else {
            allFileNames += temp + ";";
        }
    }
    if(clearID >= 0)
    {
        fileName.erase(fileName.begin() + clearID, fileName.begin() + clearID + 1);
    }

    if(fileName.size() > 0)
    {
        fileNameIndex = 0;
        temp = fileName[fileNameIndex];
    }
    else
    {
        fileNameIndex = -1;
        temp = "None";
        allFileNames = "Use new project button";
    }

    fProjectName = new C_Input(100, 10, 8, 1, temp, allFileNames);
    label[0] =  new C_Label(5, 10, 6, 1, "Name", "nohelp");
    bOk = new C_Button(110, 35, 40, 20, "OK");
    bNext = new C_Button(240, 10, 20, 20, ">", FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER, "Shows next file in levels directory");
    bBack = new C_Button(215, 10, 20, 20, "<", FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER, "Shows previous file in levels directory");

    AddFormElement(fProjectName);
    AddFormElement(label[0]);
    AddFormElement(bOk);
    AddFormElement(bNext);
    AddFormElement(bBack);

    closeable = true;
    resizeableVertical = false;
    resizeableHorizontal = false;
    minimalizeable = false;
    isTitle = true;
    SetDontSave(true);
}

void C_LoadProjectDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    if(button == LEFTBUTTON)
    {
        if(bOk->GetIsPushedAndUnpush())
        {
            C_LevelEditor::Inst()->LoadProject(fProjectName->GetString());
            // TODO no spaces in project name control
            close = true;
        }
        if(bNext->GetIsPushedAndUnpush())
        {
            fProjectName->SetActive(false);
            if(fileNameIndex >= 0)
            {
                fileNameIndex = (++fileNameIndex) % fileName.size();
                fProjectName->SetTitle(fileName[fileNameIndex]);
            }
        } else
        if(bBack->GetIsPushedAndUnpush())
        {
            fProjectName->SetActive(false);
            if(fileNameIndex >= 0)
            {
                fileNameIndex = --fileNameIndex;
                if(fileNameIndex < 0) fileNameIndex = fileName.size() - 1;
                fProjectName->SetTitle(fileName[fileNameIndex]);
            }
        }
    }
}


