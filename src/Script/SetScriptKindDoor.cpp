#include "ScriptSystem.h"
#include "..\Door\DoorSystem.h"
#include "SetScriptKindDoor.h"
#include "..\LevelEditor.h"

C_SetScriptKindDoor::C_SetScriptKindDoor(FILE * fr)
{
    Load(fr);
}

C_SetScriptKindDoor::C_SetScriptKindDoor()
{
    doorKind = D_SET_SCRIPT_EDITOR;

    title = "Set Script Kinds";
    x = 0;
    y = 52;
    width = 220;
    height = 200;
}

C_SetScriptKindDoor::C_SetScriptKindDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_SET_SCRIPT_EDITOR;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

C_SetScriptKindDoor::~C_SetScriptKindDoor()
{

}

void C_SetScriptKindDoor::FirstTime()
{
    resizeableVertical = true;
    resizeableHorizontal = false;
    isTitle = true;
    closeable = true;
    scrollable = true;

    char alignCenter = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;

    bNew = new C_Button(5, 5, 60, 25, "New", alignCenter, "New script kind");
    bLoad = new C_Button(75, 5, 60, 25, "Load", alignCenter, "Load script kinds");
    bSave = new C_Button(145, 5, 60, 25, "Save", alignCenter, "Save script kinds");

    AddFormElement(bNew);
    AddFormElement(bLoad);
    AddFormElement(bSave);

    MakeButtons();
}

void C_SetScriptKindDoor::MakeButtons()
{
    int loop1;
    int nButton;
    char alignLeft = FontMan::ALIGN_LEFT | FontMan::ALIGN_VCENTER;
    char alignCenter = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;

    nButton = C_ScriptSystem::Inst()->GetNScriptKind();
    bName.erase(bName.begin(), bName.end());
    bEdit.erase(bEdit.begin(), bEdit.end());
    bErase.erase(bErase.begin(), bErase.end());

    for(loop1 = 0; loop1 < nButton; loop1++)
    {
        bName.push_back(C_Button());
        bName[bName.size() - 1].Set(5, 35 + loop1 * 30, 150, 25, C_ScriptSystem::Inst()->GetScriptKindName(loop1), alignLeft, "New script");
        bEdit.push_back(C_Button());
        bEdit[bName.size() - 1].Set(160, 35 + loop1 * 30, 25, 25, "E", alignCenter, "Edit");
        bErase.push_back(C_Button());
        bErase[bName.size() - 1].Set(190, 35 + loop1 * 30, 25, 25, "D", alignCenter, "Delete");
    }
}

void C_SetScriptKindDoor::DrawContent()
{
    C_Door::DrawContent();

    unsigned int loop1;

    for(loop1 = 0; loop1 < bName.size(); loop1++)
    {
        bName[loop1].Draw();
        bEdit[loop1].Draw();
        bErase[loop1].Draw();
    }
}

void C_SetScriptKindDoor::DoorEverTime()
{
    C_Door::DoorEverTime();

    unsigned int loop1;

    for(loop1 = 0; loop1 < bName.size(); loop1++)
    {
        bName[loop1].MainLoop(interMouseX, interMouseY);
        bEdit[loop1].MainLoop(interMouseX, interMouseY);
        bErase[loop1].MainLoop(interMouseX, interMouseY);
    }
}

void C_SetScriptKindDoor::DoorActionOut()
{
    MakeButtons();
}

void C_SetScriptKindDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    if(button == LEFTBUTTON)
    {
        unsigned int loop1;

        if(bNew->GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->NewScriptKind();
            MakeButtons();
        }

        if(bLoad->GetIsPushedAndUnpush())
        {
            C_DoorSystem::Inst()->CloseDoor(D_NEW_SCRIPT_KIND);
            C_ScriptSystem::Inst()->LoadScriptKind();
            MakeButtons();
        }

        if(bSave->GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->SaveScriptKind(C_LevelEditor::Inst()->GetGameSetup()->GetName());

        }

        for(loop1 = 0; loop1 < bName.size(); loop1++)
        {
            bName[loop1].MouseClick(interMouseX, interMouseY);
            if(bName[loop1].GetIsPushedAndUnpush())
            {
                C_DoorSystem::Inst()->OpenNewDoor(D_NEW_SCRIPT, C_ScriptSystem::Inst()->NewScript(loop1));
                //close = true;
                break;
            }

            bEdit[loop1].MouseClick(interMouseX, interMouseY);
            if(bEdit[loop1].GetIsPushedAndUnpush())
            {
                C_DoorSystem::Inst()->OpenNewDoor(D_NEW_SCRIPT_KIND, loop1);
                //close = true;
                break;
            }

            bErase[loop1].MouseClick(interMouseX, interMouseY);
            if(bErase[loop1].GetIsPushedAndUnpush())
            {
                C_DoorSystem::Inst()->CloseDoor(D_NEW_SCRIPT_KIND);
                C_ScriptSystem::Inst()->DeleteScriptKind(loop1);
                MakeButtons();
                break;
            }
        }
    }
}
