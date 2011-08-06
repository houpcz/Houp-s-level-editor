#include "ScriptSystem.h"
#include "ScriptGroupDoor.h"

C_ScriptGroupDoor::C_ScriptGroupDoor(int n_x, int n_y, int n_width, int n_height, string n_title)
{
    doorKind = D_SET_GROUPS;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
}

void C_ScriptGroupDoor::FirstTime()
{
    isTitle = true;
    closeable = true;
    minimalizeable = true;
    resizeableVertical = true;
    resizeableHorizontal = false;
    scrollable = true;

    bNew.Set(5, 5, 50, 25, "New");

    MakeContent();
}

void C_ScriptGroupDoor::MakeContent()
{
    int top = 35;
    unsigned int loop1;
    char align = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;

    inputGroup.clear();
    bActiveGroup.Clear();
    bDel.Clear();
    inputParent.clear();
    for(loop1 = 0; loop1 < C_ScriptSystem::Inst()->GetNGroup(); loop1++)
    {
        inputGroup.push_back(C_Input(5, top, 15, 1, C_ScriptSystem::Inst()->GetScriptGroupName(loop1), "Name of script group number "+ToString(loop1)));

        inputParent.push_back(C_InputInteger(212, top, 2, 1, ToString(C_ScriptSystem::Inst()->GetScriptGroupIParent(loop1)), "Parent group - " + C_ScriptSystem::Inst()->GetScriptGroupName(C_ScriptSystem::Inst()->GetScriptGroupIParent(loop1))));
        inputParent[inputParent.size() - 1].SetMinMax(0, C_ScriptSystem::Inst()->GetNGroup() - 1);
        if(C_ScriptSystem::Inst()->GetScriptGroupIParent(loop1) == C_ScriptGroup::NO_PARENT)
            inputParent[inputParent.size() - 1].SetVisible(false);

        bActiveGroup.AddButton(250, top -1, 20, 21, "A", align, "Active group");
        bDel.AddButton(275, top -1, 20, 21, "D", align, "Delete group");
        if(loop1 == C_ScriptSystem::Inst()->GetActiveGroup())
        {
            bActiveGroup.SetPushed(true, loop1);
        }
        top += 26;
    }
    bDel.SetVisible(false, 0);
}

void C_ScriptGroupDoor::DoorEverTime()
{
    unsigned int loop1;

    bNew.MainLoop(interMouseX, interMouseY);
    if(bNew.GetIsPushedAndUnpush())
    {
        C_ScriptSystem::Inst()->AddNewScriptGroup("New group", 0, true);
        MakeContent();
    }

    for(loop1 = 1; loop1 < inputGroup.size(); loop1++)
    {
        inputGroup[loop1].MainLoop(interMouseX, interMouseY);
        if(!inputGroup[loop1].GetActive() && inputGroup[loop1].GetString() != C_ScriptSystem::Inst()->GetScriptGroupName(loop1))
        {
           C_ScriptSystem::Inst()->SetScriptGroupName(inputGroup[loop1].GetString(), loop1);
           MakeContent();
        }
    }

    for(loop1 = 1; loop1 < inputParent.size(); loop1++)
    {
        inputParent[loop1].MainLoop(interMouseX, interMouseY);
        if(!inputParent[loop1].GetActive() && (unsigned) inputParent[loop1].GetInteger() != C_ScriptSystem::Inst()->GetScriptGroupIParent(loop1))
        {
           if(C_ScriptSystem::Inst()->IsScriptGroupStillTree(inputParent[loop1].GetInteger(), loop1))
           {
                C_ScriptSystem::Inst()->SetScriptGroupIParent(inputParent[loop1].GetInteger(), loop1);
           } // WARNING DOOR
           MakeContent();
        }
    }

    bActiveGroup.MainLoop(interMouseX, interMouseY);
    bDel.MainLoop(interMouseX, interMouseY);

}

void C_ScriptGroupDoor::DrawContent()
{
    unsigned int loop1;
    for(loop1 = 0; loop1 < inputGroup.size(); loop1++)
    {
        inputGroup[loop1].Draw();
    }
    for(loop1 = 1; loop1 < inputParent.size(); loop1++)
    {
        inputParent[loop1].Draw();
    }
    bNew.Draw();
    bActiveGroup.Draw();
    bDel.Draw();
}

void C_ScriptGroupDoor::DoorActionDown(int button)
{
    unsigned int loop1;

    if(button == LEFTBUTTON)
    {
        bNew.MouseClick(interMouseX, interMouseY);
        for(loop1 = 1; loop1 < inputGroup.size(); loop1++)
        {
            inputGroup[loop1].MouseClick(interMouseX, interMouseY);
        }
        for(loop1 = 0; loop1 < inputParent.size(); loop1++)
        {
            inputParent[loop1].MouseClick(interMouseX, interMouseY);
        }

        bDel.MouseClick(interMouseX, interMouseY);
        for(loop1 = 0; loop1 < bDel.GetNButton(); loop1++)
        {
            if(bDel.GetIsNewPushed(loop1))
            {
                C_ScriptSystem::Inst()->DeleteScriptGroup(loop1);
                if(bActiveGroup.GetIsPushedAndUnpush(loop1))
                {
                    C_ScriptSystem::Inst()->SetActiveGroup(0);
                }
                MakeContent();
            }
        }

        bActiveGroup.MouseClick(interMouseX, interMouseY);
        for(loop1 = 0; loop1 < bActiveGroup.GetNButton(); loop1++)
        {
            if(bActiveGroup.GetIsNewPushed(loop1))
            {
                C_ScriptSystem::Inst()->SetActiveGroup(loop1);
            }
        }
    }
}
