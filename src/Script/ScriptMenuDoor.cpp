#include "ScriptSystem.h"
#include "..\Door\DoorSystem.h"

C_ScriptMenuDoor::C_ScriptMenuDoor(FILE * fr)
{
    Load(fr);
}

C_ScriptMenuDoor::C_ScriptMenuDoor()
{
    doorKind = D_SET_SCRIPTS;

    title = "Set Scripts";
    x = 0;
    y = 277;
    width = 250;
    height = 500;
}

C_ScriptMenuDoor::C_ScriptMenuDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_SET_SCRIPTS;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

C_ScriptMenuDoor::~C_ScriptMenuDoor()
{

}

void C_ScriptMenuDoor::FirstTime()
{
    resizeableVertical = true;
    resizeableHorizontal = false;
    isTitle = true;
    closeable = true;
    minimalizeable = true;
    scrollable = true;

    MakeButtons();
}

void C_ScriptMenuDoor::MkButWithParent(unsigned int parent, int layer, int & top)
{
    unsigned int loop1, loop2;
    char alignLeft = FontMan::ALIGN_LEFT | FontMan::ALIGN_VCENTER;
    char alignCenter = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;

    for(loop2 = 0; loop2 < C_ScriptSystem::Inst()->GetNGroup(); loop2++)
    {
        if(C_ScriptSystem::Inst()->GetScriptGroupIParent(loop2) == parent &&
           (C_ScriptSystem::Inst()->GetScriptGroupVisible(parent) || parent == 0))
        {
            bGroup.push_back(C_Button(5 + layer * 5, top, 240, 25, C_ScriptSystem::Inst()->GetScriptGroupName(loop2), alignLeft,  "nohelp", C_Color::redDark));
            iBGroup.push_back(loop2);

            top += 30;
            if(C_ScriptSystem::Inst()->GetScriptGroupVisible(loop2))
            {
                bGroup[bGroup.size() - 1].SetIsPushed(true);
                for(loop1 = 0; loop1 < C_ScriptSystem::Inst()->GetNScript(); loop1++)
                {
                    if(C_ScriptSystem::Inst()->GetScriptIGroup(loop1) == loop2)
                    {
                        iBScript.push_back(loop1);
                        bName.push_back(C_Button());
                        bName[bName.size() - 1].Set(5 + layer * 5, top, 150 - layer * 5, 25, C_ScriptSystem::Inst()->GetScriptName(loop1), alignLeft, "Copy");
                        bEdit.push_back(C_Button());
                        bEdit[bName.size() - 1].Set(160, top, 25, 25, "E", alignCenter,  "Edit");
                        bShow.push_back(C_Button());
                        bShow[bName.size() - 1].Set(190, top, 25, 25, "S", alignCenter, "Show all tiles in script");
                        bErase.push_back(C_Button());
                        bErase[bName.size() - 1].Set(220, top, 25, 25, "D", alignCenter, "Delete");

                        top += 30;
                    }
                }
            }
            MkButWithParent(loop2, layer + 1, top);
        }
    }
}

void C_ScriptMenuDoor::MakeButtons()
{
    unsigned int loop1;
    int top = 5;
    char alignLeft = FontMan::ALIGN_LEFT | FontMan::ALIGN_VCENTER;
    char alignCenter = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;

    bName.clear();
    bEdit.clear();
    bErase.clear();
    bGroup.clear();
    iBGroup.clear();
    iBScript.clear();

    for(loop1 = 0; loop1 < C_ScriptSystem::Inst()->GetNScript(); loop1++)
    {
        if(C_ScriptSystem::Inst()->GetScriptIGroup(loop1) == 0) // main group
        {
            iBScript.push_back(loop1);
            bName.push_back(C_Button());
            bName[bName.size() - 1].Set(5, top, 150, 25, C_ScriptSystem::Inst()->GetScriptName(loop1), alignLeft, "Copy");
            bEdit.push_back(C_Button());
            bEdit[bName.size() - 1].Set(160, top, 25, 25, "E", alignCenter, "Edit");
            bShow.push_back(C_Button());
            bShow[bName.size() - 1].Set(190, top, 25, 25, "S", alignCenter, "Show all tiles in script");
            bErase.push_back(C_Button());
            bErase[bName.size() - 1].Set(220, top, 25, 25, "D", alignCenter, "Delete");

            top += 30;
        }
    }
    MkButWithParent(0, 0, top);
}

void C_ScriptMenuDoor::DrawContent()
{
    unsigned int loop1;

    for(loop1 = 0; loop1 < bGroup.size(); loop1++)
    {
        bGroup[loop1].Draw();
    }

    for(loop1 = 0; loop1 < bName.size(); loop1++)
    {
        bName[loop1].Draw();
        bEdit[loop1].Draw();
        bErase[loop1].Draw();
        bShow[loop1].Draw();
    }
}

void C_ScriptMenuDoor::DoorActionIn()
{
    unsigned int loop1;

    for(loop1 = 0; loop1 < bGroup.size(); loop1++)
    {
        bGroup[loop1].MainLoop(interMouseX, interMouseY);
    }

    for(loop1 = 0; loop1 < bName.size(); loop1++)
    {
        bName[loop1].MainLoop(interMouseX, interMouseY);
        bEdit[loop1].MainLoop(interMouseX, interMouseY);
        bErase[loop1].MainLoop(interMouseX, interMouseY);
        bShow[loop1].MainLoop(interMouseX, interMouseY);

        if(bEdit[loop1].GetIsPushedAndUnpush())
        {
            C_DoorSystem::Inst()->OpenNewDoor(D_NEW_SCRIPT, iBScript[loop1]);
            //close = true;
            break;
        }

        if(bName[loop1].GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->CopyScript(iBScript[loop1]);
            MakeButtons();
            break;
        }

        if(bErase[loop1].GetIsPushedAndUnpush())
        {
            C_DoorSystem::Inst()->CloseDoor(D_NEW_SCRIPT);
            C_ScriptSystem::Inst()->DeleteScript(iBScript[loop1]);
            MakeButtons();
            break;
        }

        if(bShow[loop1].GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->ShowScript(iBScript[loop1]);
            break;
        }
    }
}

void C_ScriptMenuDoor::DoorActionOut()
{
    MakeButtons();
}

void C_ScriptMenuDoor::DoorActionDown(int button)
{
    unsigned int loop1;

    if(button == LEFTBUTTON)
    {
        for(loop1 = 0; loop1 < bGroup.size(); loop1++)
        {
            bGroup[loop1].MouseClick(interMouseX, interMouseY);
            if(bGroup[loop1].GetNewPushed()) {
                C_ScriptSystem::Inst()->SetScriptGroupVisible(bGroup[loop1].GetIsPushed(),
                                                   iBGroup[loop1]);
                MakeButtons();
            } else if(bGroup[loop1].GetNewUnpushed()) {
                C_ScriptSystem::Inst()->SetScriptGroupVisible(bGroup[loop1].GetIsPushed(),
                                                   iBGroup[loop1]);
                MakeButtons();
            }
        }

        for(loop1 = 0; loop1 < bName.size(); loop1++)
        {
            bName[loop1].MouseClick(interMouseX, interMouseY);
            bEdit[loop1].MouseClick(interMouseX, interMouseY);
            bErase[loop1].MouseClick(interMouseX, interMouseY);
            bShow[loop1].MouseClick(interMouseX, interMouseY);
        }
    }
}

