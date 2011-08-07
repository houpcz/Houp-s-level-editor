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

    width = 196;
    height = 64;
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

    if(button == LEFTBUTTON &&
       GetIsButtonActive(tempX, tempY))
    {
        switch(tempY)
        {
        case ROW_MAP :
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
                    break;            }
            break;
        case ROW_PROJECT :
            switch(tempX)
            {
                case SETUP_PROJECT :
                    C_DoorSystem::Inst()->OpenNewDoor(D_SET_EDITOR);
                    break;
                case OPEN_PROJECT :
                    C_DoorSystem::Inst()->OpenNewDoor(D_LOAD_PROJECT);
                    break;
                case NEW_PROJECT :
                    C_DoorSystem::Inst()->OpenNewDoor(D_NEW_PROJECT);
                    break;
            }
            break;
        }
    }
}

void C_MainMenuDoor::DoorActionIn()
{
    int tempX = interMouseX / 32;
    int tempY = interMouseY / 32;

    string activeString = "";
    if(!GetIsButtonActive(tempX, tempY))
    {
        activeString = " - not active";
    }

    switch(tempY)
    {
        case ROW_MAP :
            switch(tempX)
            {
                case NEW_MAP : C_DoorSystem::Inst()->OpenNewHelp("New map" + activeString); break;
                case OPEN_MAP : C_DoorSystem::Inst()->OpenNewHelp("Open map" + activeString); break;
                case SETUP_MAP : C_DoorSystem::Inst()->OpenNewHelp("Setup map" + activeString); break;
                case SAVE_MAP : C_DoorSystem::Inst()->OpenNewHelp("Save map" + activeString); break;
                case UNDO_MAP : C_DoorSystem::Inst()->OpenNewHelp("Undo map" + activeString); break;
                case REDO_MAP : C_DoorSystem::Inst()->OpenNewHelp("Redo map" + activeString); break;
            }
            break;
        case ROW_PROJECT :
            switch(tempX)
            {
                case SETUP_PROJECT : C_DoorSystem::Inst()->OpenNewHelp("Setup project" + activeString); break;
                case OPEN_PROJECT : C_DoorSystem::Inst()->OpenNewHelp("Open project" + activeString); break;
                case NEW_PROJECT : C_DoorSystem::Inst()->OpenNewHelp("New project" + activeString); break;
            }
            break;
    }
}

bool C_MainMenuDoor::GetIsButtonActive(int x, int y)
{
     switch(y)
     {
         case ROW_PROJECT :
            if(C_LevelEditor::Inst()->GetGameSetup()->GetName().size() != 0 ||
               x == NEW_PROJECT || x == OPEN_PROJECT)
            {
                return true;
            } else
                return false;
            break;

         case ROW_MAP :
            if(C_LevelEditor::Inst()->GetGameSetup()->GetName().size() != 0)
            {
                if(C_LevelEditor::Inst()->GetMap() != NULL)
                {
                    return true;
                } else {
                    if(x == OPEN_MAP || x == NEW_MAP) {
                        return true;
                    } else
                        return false;
                }
            } else
                return false;

            break;
     }
}

void C_MainMenuDoor::DoorEverTime()
{

}
