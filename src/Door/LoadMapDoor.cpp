#include <string>
#include <windows.h>

#include "door.h"
#include "DoorSystem.h"
#include "LoadMapDoor.h"
#include "..\Script\ScriptSystem.h"
#include "..\LevelEditor.h"

C_LoadMapDoor::C_LoadMapDoor(FILE * fr)
{
        Load(fr);
}

C_LoadMapDoor::C_LoadMapDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_LOAD_MAP;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

C_LoadMapDoor::~C_LoadMapDoor()
{
}

void C_LoadMapDoor::FirstTime()
{
    WIN32_FIND_DATA f;
    char c_str[256];
    sprintf(c_str, "%s/Maps/*.lev", C_LevelEditor::Inst()->GetGameSetup()->GetName().c_str());
    HANDLE h = FindFirstFile(c_str, &f);
    string allFileNames = "";
    string temp;
    if(h != INVALID_HANDLE_VALUE)
    {
        do
        {
            temp = f.cFileName;
            temp = temp.substr(0, temp.length() - 4);
            allFileNames += ";" + temp;
            fileName.push_back(temp);
        } while(FindNextFile(h, &f));
    }
    else
    {
        fprintf(stderr, "Error opening directory\n");
    }

    if(fileName.size() > 0)
    {
        fileNameIndex = 0;
        temp = fileName[fileNameIndex];
    }
    else
    {
        fileNameIndex = -1;
        temp = "Map01";
    }

    fMapName = new C_Input(100, 10, 8, 1, temp, "Level name" + allFileNames);
    label[0] =  new C_Label(5, 10, 6, 1, "Name", "nohelp");
    bOk = new C_Button(110, 35, 40, 20, "OK");
    bNext = new C_Button(240, 10, 20, 20, ">", FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER, "Shows next file in levels directory");
    bBack = new C_Button(215, 10, 20, 20, "<", FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER, "Shows previous file in levels directory");

    AddFormElement(fMapName);
    AddFormElement(label[0]);
    AddFormElement(bOk);
    AddFormElement(bNext);
    AddFormElement(bBack);

    closeable = true;
    resizeableVertical = false;
    resizeableHorizontal = false;
    minimalizeable = false;
    isTitle = true;
}

void C_LoadMapDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    if(button == LEFTBUTTON)
    {
        if(bOk->GetIsPushedAndUnpush())
        {
            C_LevelEditor::Inst()->LoadMap(fMapName->GetString());
            C_DoorSystem::Inst()->CloseDoor(D_NEW_SCRIPT);
            C_DoorSystem::Inst()->CloseDoor(D_SET_GROUPS);
            C_ScriptSystem::Inst()->DeleteAllScript();
            C_ScriptSystem::Inst()->LoadScript(fMapName->GetString());
            close = true;
        }
        if(bNext->GetIsPushedAndUnpush())
        {
            fMapName->SetActive(false);
            if(fileNameIndex >= 0)
            {
                fileNameIndex = (++fileNameIndex) % fileName.size();
                fMapName->SetTitle(fileName[fileNameIndex]);
            }
        } else
        if(bBack->GetIsPushedAndUnpush())
        {
            fMapName->SetActive(false);
            if(fileNameIndex >= 0)
            {
                fileNameIndex = --fileNameIndex;
                if(fileNameIndex < 0) fileNameIndex = fileName.size() - 1;
                fMapName->SetTitle(fileName[fileNameIndex]);
            }
        }
    }
}

