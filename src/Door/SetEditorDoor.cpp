#include <string>

#include "Door.h"
#include "SetEditorDoor.h"
#include "..\Script\ScriptSystem.h"
#include "..\LevelEditor.h"

C_SetEditorDoor::C_SetEditorDoor(FILE * fr)
{
    title = "Map";

    Load(fr);
}

C_SetEditorDoor::C_SetEditorDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_SET_EDITOR;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

void C_SetEditorDoor::FirstTime()
{
    C_LevelEditor * levelEditor = C_LevelEditor::Inst();
    char alignCenter = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;

    label = new C_Label(5, 10, 6, 1, "Name", "Game name", 0, 0, 0, 155, 155, 155, 155, 155, 155, 155, 155, 155);
    AddFormElement(label);
    label = new C_Label(5, 40, 6, 1, "Tile W", "Tile Width", 0, 0, 0, 155, 155, 155, 155, 155, 155, 155, 155, 155);
    AddFormElement(label);
    label = new C_Label(5, 70, 6, 1, "Tile H", "Tile Height", 0, 0, 0, 155, 155, 155, 155, 155, 155, 155, 155, 155);
    AddFormElement(label);

    fName = new C_Input(100, 10, 10, 1, levelEditor->GetGameSetup()->GetName().c_str(), "Game name");
    fTileWidth = new C_InputInteger(100, 40, 3, 1, ToString(levelEditor->GetGameSetup()->GetTileWidth()).c_str(), "Tile width");
    fTileHeight = new C_InputInteger(100, 70, 3, 1, ToString(levelEditor->GetGameSetup()->GetTileHeight()).c_str(), "Tile height");
    fTileWidth->SetMinMax(16, 256);
    fTileHeight->SetMinMax(16, 256);
    AddFormElement(fTileWidth);
    AddFormElement(fTileHeight);
    AddFormElement(fName);

    label = new C_Label(5, 100, 6, 1, "Map BG", "Map background color", 0, 0, 0, 155, 155, 155, 155, 155, 155, 155, 155, 155);
    AddFormElement(label);
    fMapColorRed = new C_InputInteger(100, 100, 3, 1, ToString(levelEditor->GetGameSetup()->GetMapBGCR()).c_str(), "Red 0 - 255");
    fMapColorRed->SetMinMax(0, 255);
    fMapColorGreen = new C_InputInteger(150, 100, 3, 1, ToString(levelEditor->GetGameSetup()->GetMapBGCG()).c_str(), "Green 0 - 255");
    fMapColorGreen->SetMinMax(0, 255);
    fMapColorBlue = new C_InputInteger(200, 100, 3, 1, ToString(levelEditor->GetGameSetup()->GetMapBGCB()).c_str(), "Blue 0 - 255");
    fMapColorBlue->SetMinMax(0, 255);
    AddFormElement(fMapColorBlue);
    AddFormElement(fMapColorGreen);
    AddFormElement(fMapColorRed);

    bOk = new C_Button(105, 130, 40, 20, "OK", alignCenter);
    title = "Set editor";
    width = 250;
    height = 160;

    AddFormElement(bOk);

    closeable = true;
    resizeableVertical = false;
    resizeableHorizontal = false;
    minimalizeable = false;
    isTitle = true;
    SetDontSave(true);
}

void C_SetEditorDoor::DoorEverTime()
{
    C_Door::DoorEverTime();
}

void C_SetEditorDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);
    C_LevelEditor * levelEditor = C_LevelEditor::Inst();

    if(button == LEFTBUTTON)
    {
        C_Map * pMap = C_LevelEditor::Inst()->GetMap();

        if(bOk->GetIsPushedAndUnpush())
        {
            levelEditor->SetTileWidth(fTileWidth->GetInteger());
            levelEditor->SetTileHeight(fTileHeight->GetInteger());
            levelEditor->SetMapBGCR(fMapColorRed->GetInteger());
            levelEditor->SetMapBGCG(fMapColorGreen->GetInteger());
            levelEditor->SetMapBGCB(fMapColorBlue->GetInteger());
            levelEditor->SetGameName(fName->GetString());
            close = true;
        }
    }
}

