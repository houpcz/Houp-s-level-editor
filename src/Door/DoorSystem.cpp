#include "DoorSystem.h"
#include "..\Script\ScriptSystem.h"
#include "..\Script\ScriptDoor.h"
#include "..\Script\ScriptGroupDoor.h"
#include "..\Script\NewScriptKindDoor.h"
#include "..\Script\SetScriptKindDoor.h"


#include "..\Engine.h"
#include "..\Utils\FontMan.h"
#include "..\Utils\FileMan.h"
#include "AllDoors.h"

C_DoorSystem * C_DoorSystem::inst = NULL;

C_DoorSystem * C_DoorSystem::Inst()
{
    if (inst == NULL)
    {
        inst = new C_DoorSystem;
    }
    return inst;
}

C_DoorSystem::C_DoorSystem()
{
    wedge = false;
    accent = false;
    worldRelX = 0;
    worldRelY = 0;
    changingWorldPos = false;
    language = CZECH_CLASSIC;
}

C_DoorSystem::~C_DoorSystem()
{
    if(inst == NULL)
        return;

    for(unsigned int loop1 = 0; loop1 < door.size(); loop1++)
        delete door[loop1];
    door.erase(door.begin(), door.end());

    delete inst;
}

void C_DoorSystem::Init()
{
    if(!Load())
    {
        door.clear();

        OpenNewDoor(D_MAIN_MENU);
        OpenNewDoor(D_MAP);
        OpenNewDoor(D_TILES);
        start.Set(App::Inst()->GetScreenWidth() - 60, App::Inst()->GetScreenHeight() - 20, 40, 20, "Menu");
    }
}

void C_DoorSystem::Save()
{
    FILE *fw;
    if((fw = fopen("setup.dat", "wb")) != NULL)
    {
        int doorSize = 0;
        for(unsigned int loop1 = 0; loop1 < door.size(); loop1++)
        {
            if(!door[loop1]->GetDontSave())
                doorSize++;
        }
        FileMan::Inst()->SaveInteger(doorSize, fw);

        for(unsigned int loop1 = 0; loop1 < door.size(); loop1++)
        {
            if(door[loop1]->GetDontSave())
                continue;

            FileMan::Inst()->SaveInteger(door[loop1]->GetDoorKind(), fw);
            door[loop1]->Save(fw);
        }
    }

    fclose(fw);
}

bool C_DoorSystem::Load()
{
    FILE *fr;
    int tempSize;
    int tempKind;
    D_DOORKIND kind;

    if((fr = fopen("setup.dat", "rb")) != NULL)
    {
        if(!FileMan::Inst()->LoadInteger(tempSize, fr))
            return false;

        for(int loop1 = 0, doorID = 0; loop1 < tempSize; loop1++)
        {
            if(!FileMan::Inst()->LoadInteger(tempKind, fr))
                return false;
            kind = (D_DOORKIND) tempKind;

            switch(kind)
            {
                case D_TILES :
                case D_MONSTER :
                case D_OBJECT :
                case D_ITEM :              door.push_back(new C_TileDoor(fr)); break;
                case D_PATTERN :           door.push_back(new PatternDoor(fr)); break;

                case D_MAIN_MENU :         door.push_back(new C_MainMenuDoor(fr)); break;
                case D_MENU_DOOR :         door.push_back(new C_MenuDoor(fr)); break;
                case D_MAP :               door.push_back(new C_MapDoor(fr)); break;
                case D_SET_SCRIPT_EDITOR : door.push_back(new C_SetScriptKindDoor(fr)); break;
                case D_SET_SCRIPTS :       door.push_back(new C_ScriptMenuDoor(fr)); break;

                // NEMUSI EXISTOVAT SKRIPT
                // case D_NEW_SCRIPT :        door.push_back(new C_ScriptDoor(fr)); break;
                // case D_NEW_SCRIPT_KIND :   door.push_back(new C_NewScriptKindDoor(fr)); break;
                case D_NEW_SCRIPT :        continue; break;
                case D_NEW_SCRIPT_KIND :   continue; break;

                case D_SET_GROUPS :        door.push_back(new C_ScriptGroupDoor(fr));break;

                case D_NEW_MAP :           door.push_back(new C_NewMapDoor(fr)); break;
                case D_SET_MAP :           door.push_back(new C_SetMapDoor(fr)); break;
                case D_LOAD_MAP :          door.push_back(new C_LoadMapDoor(fr)); break;
                case D_SAVE_MAP :          door.push_back(new C_SaveMapDoor(fr)); break;

                default : fprintf(stderr, "Door with kind number %d doesn't exist.", tempKind); return false;;
            }
            door[doorID]->SetDoorKind(tempKind);
            doorID++;
        }

        fclose(fr);

        return true;
    } else
        return false;
}

void C_DoorSystem::ResizeWindow()
{
    start.Set(App::Inst()->GetScreenWidth() - 60, App::Inst()->GetScreenHeight() - 20, 40, 20, "Menu");
}

void C_DoorSystem::Draw()
{
    unsigned int loop1, loop2;
 	int min, startMin, minIndex;
 	FontMan * fm = FontMan::Inst();

	minIndex = 0;
	startMin = 0;

    glLoadIdentity();
	for(loop1 = 0; loop1 < door.size(); loop1++)
	{
       min = door[0]->GetTopLayer() + 1;
       for(loop2 = 0; loop2 < door.size(); loop2++)
	   {
            if(door[loop2]->GetLayer() < min && door[loop2]->GetLayer() > startMin)
            {
                min = door[loop2]->GetLayer();
                minIndex = loop2;
            }
       }
       door[minIndex]->Draw();
       startMin = min;
    }

    for(loop1 = 0; loop1 < door.size(); loop1++)
	{
        if(door[loop1]->GetIsClose())
        {
            delete door[loop1];
            door.erase(door.begin() + loop1, door.begin() + loop1 + 1);
        }
	}

    // lišta //
    glColor3ub(180, 180, 250);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glVertex2i(0, App::Inst()->GetScreenHeight());
        glVertex2i(App::Inst()->GetScreenWidth(), App::Inst()->GetScreenHeight());
        glVertex2i(App::Inst()->GetScreenWidth(), App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT);
        glVertex2i(0, App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT);
    glEnd();
    glColor3ub(0, 0, 33);
    glBegin(GL_QUADS);
        glVertex2i(0, App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT - 1);
        glVertex2i(App::Inst()->GetScreenWidth(), App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT - 1);
        glVertex2i(App::Inst()->GetScreenWidth(), App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT);
        glVertex2i(0, App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    for(loop1 = 0; loop1 < door.size() && loop1 < BUTTON_BAR_MAX; loop1++)
	{
	    glScissor(loop1 * BUTTON_BAR_BUTTON_WIDTH, 0, BUTTON_BAR_BUTTON_WIDTH, BUTTON_BAR_HEIGHT);// Definování oøezávací oblasti
        glEnable(GL_SCISSOR_TEST);
        if(door[loop1]->GetIsTopDoor())
        {
            glColor3ub(150, 40, 40);
            glDisable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
                glVertex2i(loop1 * BUTTON_BAR_BUTTON_WIDTH, App::Inst()->GetScreenHeight());
                glVertex2i((loop1 + 1) * BUTTON_BAR_BUTTON_WIDTH, App::Inst()->GetScreenHeight());
                glVertex2i((loop1 + 1) * BUTTON_BAR_BUTTON_WIDTH, App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT);
                glVertex2i(loop1 * BUTTON_BAR_BUTTON_WIDTH, App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT);
            glEnd();
            glEnable(GL_TEXTURE_2D);

            glColor3ub(250, 233, 233);
            fm->Print("impact12", loop1 * BUTTON_BAR_BUTTON_WIDTH + 5, App::Inst()->GetScreenHeight() - 3, "%s", (door[loop1]->GetTitle()).c_str());
        } else {
            if(loop1 % 2 == 0)
                glColor3ub(120, 120, 240);
            else
                glColor3ub(80, 80, 240);

            glDisable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
                glVertex2i(loop1 * BUTTON_BAR_BUTTON_WIDTH, App::Inst()->GetScreenHeight());
                glVertex2i((loop1 + 1) * BUTTON_BAR_BUTTON_WIDTH, App::Inst()->GetScreenHeight());
                glVertex2i((loop1 + 1) * BUTTON_BAR_BUTTON_WIDTH, App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT);
                glVertex2i(loop1 * BUTTON_BAR_BUTTON_WIDTH, App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT);
            glEnd();
            glEnable(GL_TEXTURE_2D);

            glColor3ub(22, 22, 44);
            fm->Print("impact12", loop1 * BUTTON_BAR_BUTTON_WIDTH + 5, App::Inst()->GetScreenHeight() - 3, "%s", door[loop1]->GetTitle().c_str());
        }
        glDisable(GL_SCISSOR_TEST);
	}
	start.Draw();

	// jazyk prostredi
	string lan;
	glColor3ub(22, 22, 44);
	switch(language)
	{
        case CZECH_CLASSIC : lan = "CZ"; break;
        case ENGLISH_CLASSIC : lan = "EN"; break;
	}
	fm->Print("impact12", start.GetX() + start.GetWidth() + 2, start.GetY() + start.GetHeight() - 3 , "%s", lan.c_str());

	///////////////
    // konec listy
    ///////////////

    helpDoor.Draw();
}

void C_DoorSystem::MainLoop()
{
    unsigned int loop1, loop2;

    ButtonBar();
    start.MainLoop(App::Inst()->mouseX, App::Inst()->mouseY);
    helpDoor.MainLoop();
    helpDoor.SetMinimalize(true);   // pokud neotevre nekdo behem framu znovu okno, nebude videt

    start.MouseClick(App::Inst()->mouseX, App::Inst()->mouseY);
    if(start.GetIsPushedAndUnpush())
    {
        OpenNewDoor(D_MENU_DOOR);
        //door.push_back(new C_MenuDoor(300, 50, 120, 240, "Menu"));
    }
    if(App::Inst()->GetKey(SDLK_F10, true))
        door.erase(door.begin() + 1, door.end());

 	long int max, startMax, maxIndex;
	maxIndex = 0;
	startMax = door[0]->GetTopLayer() + 1;

	C_Door::ResetWasClick();

    // Zmena polohy vsech oken //

    if(changingWorldPos)
    {
        worldRelX = App::Inst()->mouseRelX;
        worldRelY = App::Inst()->mouseRelY;
    }

    unsigned int nDoor = door.size();
	for(loop1 = 0; loop1 < nDoor; loop1++)
	{
       max = 0;
       for(loop2 = 0; loop2 < nDoor; loop2++)
	   {
            if(door[loop2]->GetLayer() > max && door[loop2]->GetLayer() < startMax)
            {
                max = door[loop2]->GetLayer();
                maxIndex = loop2;
            }
       }
       door[maxIndex]->MainLoop();
       startMax = max;
    }

    if(!IsMouseInButtonBar() && !C_Door::GetWasClick() &&
       App::Inst()->mouseNewLeftDown)
    {
        fprintf(stderr, "mouse x %d mouse y %d\n", App::Inst()->mouseX, App::Inst()->mouseY);
        changingWorldPos = true;
    }
    else if(!App::Inst()->mouseLeft)
        changingWorldPos = false;

    worldRelX = 0;
    worldRelY = 0;

    /////////////////////
    // Jazyk prostredi //
    /////////////////////

    if(App::Inst()->GetKey(SDLK_LALT, false) && App::Inst()->GetKey(SDLK_LSHIFT, true))
    {
        language++;
        if(language >= N_LANGUAGE)
            language = 0;
    }
    if(language == CZECH_CLASSIC)
    {
        if((App::Inst()->GetKey(SDLK_LSHIFT, false) || App::Inst()->GetKey(SDLK_RSHIFT, false)) && App::Inst()->GetKey(SDLK_EQUALS, true))
        {
            wedge = true;
        } else if(App::Inst()->GetKey(SDLK_EQUALS, true))
        {
            if(!wedge)
                accent = true;
        }
    }
}

void C_DoorSystem::ButtonBar()
{
    unsigned int index = 0;
    unsigned int loop1;

    if(IsMouseInButtonBar() && App::Inst()->mouseNewLeftDown)
    {
        //App::Inst()->mouseNewLeftDown = false;       // uz nebude vyuzito kliknuti mysi
        index = App::Inst()->mouseX / BUTTON_BAR_BUTTON_WIDTH;
        if(index < door.size())
        {
            door[index]->SetTopDoor();
            door[index]->SetMinimalize(false);
        }
    }

    ////////////////////////
    //
    // PREPINANI MEZI OKNY
    //
    //////////////////////

    for(loop1 = 0; loop1 < door.size(); loop1++)
    {
        if(door[loop1]->GetIsTopDoor())
        {
            index = loop1;
            break;
        }
    }
    if(App::Inst()->GetKey(SDLK_TAB))
    {
        if(App::Inst()->GetKey(SDLK_LSHIFT, false) || App::Inst()->GetKey(SDLK_RSHIFT, false))
        {
            index--;
            if(index <= 0)
                index = door.size() - 1;

            door[index]->SetTopDoor();
            door[index]->SetMinimalize(false);
        } else {
            index++;
            if(index > door.size() - 1)
                index = 0;

            door[index]->SetTopDoor();
            door[index]->SetMinimalize(false);
        }
    }

    /////////////////
    //
    // KONEC PREPINANI MEZI OKNY
    //
    ///////////////////
}

bool C_DoorSystem::IsMouseInButtonBar()
{
    if(App::Inst()->mouseY <= App::Inst()->GetScreenHeight() &&
       App::Inst()->mouseY >= App::Inst()->GetScreenHeight() - BUTTON_BAR_HEIGHT)
        return true;
    else
        return false;
}

void C_DoorSystem::CloseDoor(D_DOORKIND doorKind)
{
    for(unsigned int loop1 = 0; loop1 < door.size(); loop1++)
    {
        if(door[loop1]->GetDoorKind() == doorKind)
        {
            door.erase(door.begin() + loop1, door.begin() + loop1 + 1);
        }
    }
}

void C_DoorSystem::OneCopyOfDoor(int kind) // pouze jedna kopie danych dveri pro nektere druhy
{
    if(kind == D_NEW_SCRIPT_KIND ||
       kind == D_NEW_SCRIPT      ||
       kind == D_NEW_MAP         ||
       kind == D_SET_MAP         ||
       kind == D_LOAD_MAP        ||
       kind == D_SAVE_MAP        ||
       kind == D_MENU_DOOR)
    {
        for(unsigned int loop1 = 0; loop1 < door.size(); loop1++)
        {
            if(door[loop1]->GetDoorKind() == kind)
            {
                door.erase(door.begin() + loop1, door.begin() + loop1 + 1);
                break;
            }
        }
    }
}

void C_DoorSystem::OpenNewDoor(C_Door * newDoor)
{
    door.push_back(newDoor);
}

void C_DoorSystem::OpenNewDoor(int kind, int temp)
{
    OneCopyOfDoor(kind);

    switch(kind)
    {
        case D_MAIN_MENU : door.push_back(new C_MainMenuDoor(0,0, 224, 32, "Main menu"));
        case D_MENU_DOOR : door.push_back(new C_MenuDoor(App::Inst()->GetScreenWidth() - 120, App::Inst()->GetScreenHeight() - 240 - BUTTON_BAR_HEIGHT - C_Door::TITLE_HEIGHT - 30, 120, 240, "Menu")); break;

        case D_MAP : door.push_back(new C_MapDoor(256, 0, 544, 544, C_LevelEditor::Inst()->GetMap())); break;
        case D_TILES : door.push_back(new C_TileDoor(0,52, 256, 256, "Tiles", 0, C_Map::LAYER_0)); break;
        case D_PATTERN : door.push_back(new PatternDoor(0, 52, 256, 256)); break;
        case D_MONSTER : door.push_back(new C_TileDoor(800,0, 256, 64, "Monster", 10, C_Map::LAYER_1)); break;
        case D_OBJECT : door.push_back(new C_TileDoor(800,360, 256, 96, "Object", 15, C_Map::LAYER_1)); break;
        case D_ITEM : door.push_back(new C_TileDoor(800,84, 256, 256, "Item", 20, C_Map::LAYER_1)); break;

        case D_SET_SCRIPT_EDITOR : door.push_back(new C_SetScriptKindDoor()); break;
        case D_SET_SCRIPTS : door.push_back(new C_ScriptMenuDoor()); break;
        case D_NEW_SCRIPT_KIND : door.push_back(new C_NewScriptKindDoor(800, 0, 277, 550, "Script Kind", temp)); break;
        case D_NEW_SCRIPT : door.push_back(new C_ScriptDoor(800, 0, 300, 550, "Script", temp)); break;
        case D_SET_GROUPS : door.push_back(new C_ScriptGroupDoor(200, 200, 300, 400, "Script groups"));break;

        case D_SET_EDITOR : door.push_back(new C_SetEditorDoor(130,111, 220, 120, "Set editor")); break;
        case D_LOAD_PROJECT : door.push_back(new C_LoadProjectDoor(50,54, 260, 60, "Load project")); break;

        case D_NEW_MAP : door.push_back(new C_NewMapDoor(110,111, 220, 120, "New map")); break;
        case D_SET_MAP : door.push_back(new C_SetMapDoor(130,111, 220, 120, "Set map")); break;
        case D_LOAD_MAP : door.push_back(new C_LoadMapDoor(50,54, 260, 60, "Load map")); break;
        case D_SAVE_MAP : door.push_back(new C_SaveMapDoor(110,111, 220, 60, "Save map")); break;
    }
}

void C_DoorSystem::OpenNewHelp(string help)
{
    helpDoor.Set(App::Inst()->mouseX + 8, App::Inst()->mouseY, help);
    helpDoor.SetMinimalize(false);
}
