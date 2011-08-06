#include <string>

#include "..\Utils\TextureMan.h"
#include "..\Utils\Painter.h"
#include "Door.h"
#include "..\Map.h"
#include "MainMenuDoor.h"
#include "DoorSystem.h"
#include "..\Script\ScriptSystem.h"
#include "..\LevelEditor.h"

C_MainMenuDoor::C_MainMenuDoor(FILE * fr)
{
    C_MainMenuDoor(0, 0, 1, 1, "Main menu");
    Load(fr);
}

void C_MainMenuDoor::FirstTime()
{
    resizeableVertical = false;
    resizeableHorizontal = false;
    closeable = false;
    minimalizeable = false;

    width = 224;
}

C_MainMenuDoor::C_MainMenuDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_MAIN_MENU;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

void C_MainMenuDoor::DrawContent()
{
    //door//
    S_Texture * text = C_TextureMan::Inst()->GetTexture("data/texture/menu.png");
    Painter::Inst()->DrawTexture(text);
}

void C_MainMenuDoor::DoorActionDown(int button)
{
    int tempX = interMouseX / 32;
    int tempY = interMouseY / 32;

    if(tempY == 0 && button == LEFTBUTTON)
    {
        if(C_LevelEditor::Inst()->GetMap() == NULL)
        {
            if(tempX > 2)
                return;
        }
        switch(tempX)
        {
            case NEW_MAP :
                C_DoorSystem::Inst()->OpenNewDoor(D_NEW_MAP);
                break;
            case SAVE_MAP :
                C_DoorSystem::Inst()->OpenNewDoor(D_SAVE_MAP);
                break;
            case OPEN_MAP :
                C_DoorSystem::Inst()->OpenNewDoor(D_LOAD_MAP);
                break;
            case UNDO_MAP :
                C_LevelEditor::Inst()->BackOutBack();
                break;
            case REDO_MAP :
                C_LevelEditor::Inst()->BackOutNext();
                break;
            case SETUP_MAP :
                C_DoorSystem::Inst()->OpenNewDoor(D_SET_MAP);
                break;
            case SETUP_EDITOR :
                C_DoorSystem::Inst()->OpenNewDoor(D_SET_EDITOR);
                break;
        }
    }
}

void C_MainMenuDoor::DoorActionIn()
{
    int tempX = interMouseX / 32;
    int tempY = interMouseY / 32;

    if(tempY == 0)
    {
        switch(tempX)
        {
            case SETUP_EDITOR : C_DoorSystem::Inst()->OpenNewHelp("Set editor"); break;
            case NEW_MAP : C_DoorSystem::Inst()->OpenNewHelp("New map"); break;
            case OPEN_MAP : C_DoorSystem::Inst()->OpenNewHelp("Open map"); break;
            case SETUP_MAP : C_DoorSystem::Inst()->OpenNewHelp("Setup map"); break;
            case SAVE_MAP : C_DoorSystem::Inst()->OpenNewHelp("Save map"); break;
            case UNDO_MAP : C_DoorSystem::Inst()->OpenNewHelp("Undo map"); break;
            case REDO_MAP : C_DoorSystem::Inst()->OpenNewHelp("Redo map"); break;

        }
    }
}

void C_MainMenuDoor::DoorEverTime()
{

}
