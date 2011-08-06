#include "ScriptSystem.h"
#include "NewScriptKindDoor.h"

void C_NewScriptKindDoor::Save(FILE * fw)
{
}

void C_NewScriptKindDoor::Load(FILE * fr)
{
}

C_NewScriptKindDoor::C_NewScriptKindDoor(int n_x, int n_y, int n_width, int n_height, string n_title, int n_iKind)
{
    doorKind = D_NEW_SCRIPT_KIND;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
    iKind = n_iKind;
}

void C_NewScriptKindDoor::DrawContent()
{
    C_Door::DrawContent();

    unsigned int loop1;

    for(loop1 = 0; loop1 < dataName.size(); loop1++)
        dataName[loop1].Draw();
    for(loop1 = 0; loop1 < dataHelp.size(); loop1++)
        dataHelp[loop1].Draw();
    for(loop1 = 0; loop1 < dataDefault.size(); loop1++)
        dataDefault[loop1].Draw();
    for(loop1 = 0; loop1 < dataSeparator.size(); loop1++)
        dataSeparator[loop1].Draw();
}

void C_NewScriptKindDoor::DoorEverTime()
{
    unsigned int loop1;
    C_Door::DoorEverTime();

    title = scriptKindName->GetString();
    C_ScriptSystem::Inst()->SetScriptKindName(scriptKindName->GetString(), iKind);

    for(loop1 = 0; loop1 < dataName.size(); loop1++)
    {
        dataName[loop1].MainLoop(interMouseX, interMouseY);
        C_ScriptSystem::Inst()->SetScriptKindDataName(dataName[loop1].GetString(), iKind, loop1);
    }
    for(loop1 = 0; loop1 < dataHelp.size(); loop1++)
    {
        dataHelp[loop1].MainLoop(interMouseX, interMouseY);
        C_ScriptSystem::Inst()->SetScriptKindDataHelp(dataHelp[loop1].GetString(), iKind, loop1);
        if(dataHelp[loop1].GetUpdateNLines())
            MakeContent();
    }
    for(loop1 = 0; loop1 < dataDefault.size(); loop1++)
    {
        dataDefault[loop1].MainLoop(interMouseX, interMouseY);
        C_ScriptSystem::Inst()->SetScriptKindDataDefault(dataDefault[loop1].GetString(), iKind, loop1);
        if(dataDefault[loop1].GetUpdateNLines())
            MakeContent();
    }
}

void C_NewScriptKindDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    unsigned int loop1;
    if(button == LEFTBUTTON)
    {
        for(loop1 = 0; loop1 < dataName.size(); loop1++)
            dataName[loop1].MouseClick(interMouseX, interMouseY);
        for(loop1 = 0; loop1 < dataHelp.size(); loop1++)
            dataHelp[loop1].MouseClick(interMouseX, interMouseY);
        for(loop1 = 0; loop1 < dataDefault.size(); loop1++)
            dataDefault[loop1].MouseClick(interMouseX, interMouseY);

        if(bDouble->GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->AddNewScriptDataKind(DOUBLE, iKind);
            C_ScriptSystem::Inst()->SetScriptKindDataHelp("nohelp", iKind, dataName.size());
            MakeContent();
        }

        if(bInteger->GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->AddNewScriptDataKind(INTEGER, iKind);
            C_ScriptSystem::Inst()->SetScriptKindDataHelp("nohelp", iKind, dataName.size());
            MakeContent();
        }

        if(bString->GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->AddNewScriptDataKind(STRING, iKind);
            C_ScriptSystem::Inst()->SetScriptKindDataHelp("nohelp", iKind, dataName.size());
            MakeContent();
        }
        if(bColor->GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->AddNewScriptDataKind(TILE_LINK, iKind);
            C_ScriptSystem::Inst()->SetScriptKindDataValue(CLASSIC, iKind, C_ScriptSystem::Inst()->GetNScriptKindData(iKind) - 1, 0);
            C_ScriptSystem::Inst()->SetScriptKindDataHelp("nohelp", iKind, dataName.size());
            MakeContent();
        }
        if(bPath->GetIsPushedAndUnpush())
        {
            C_ScriptSystem::Inst()->AddNewScriptDataKind(TILE_LINK, iKind);
            C_ScriptSystem::Inst()->SetScriptKindDataValue(PATH, iKind, C_ScriptSystem::Inst()->GetNScriptKindData(iKind) - 1, 0);
            C_ScriptSystem::Inst()->SetScriptKindDataHelp("nohelp", iKind, dataName.size());
            MakeContent();
        }
    }
}

void C_NewScriptKindDoor::FirstTime()
{
    isTitle = true;
    closeable = true;
    minimalizeable = true;
    resizeableVertical = true;
    resizeableHorizontal = false;
    scrollable = true;

    char align = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;
    scriptKindName = new C_Input(5, 5, 20, 1, C_ScriptSystem::Inst()->GetScriptKindName(iKind), "Script kind name");
    bDouble = new C_Button(5, 30, 49, 25, "D", align, "New double in script");
    bInteger = new C_Button(59, 30, 49, 25, "I", align, "New integer in script");
    bString = new C_Button(113, 30, 49, 25, "S", align, "New integer in script");
    bColor = new C_Button(167, 30, 49, 25, "T", align, "New link tiles in script - Classic");
    bPath = new C_Button(221, 30, 49, 25, "P", align, "New link tiles in script - Path");

    AddFormElement(scriptKindName);
    AddFormElement(bDouble);
    AddFormElement(bInteger);
    AddFormElement(bString);
    AddFormElement(bColor);
    AddFormElement(bPath);

    MakeContent();
}

void C_NewScriptKindDoor::MakeContent()
{
    unsigned int loop1;
    unsigned int nData = C_ScriptSystem::Inst()->GetNScriptKindData(iKind);
    int top = 60;

    dataName.clear();
    dataSeparator.clear();

    vector<bool> wasHelpActive;
    vector<int> helpPos;
    for(loop1 = 0; loop1 < dataHelp.size(); loop1++)
    {
        wasHelpActive.push_back(dataHelp[loop1].GetActive());
        helpPos.push_back(dataHelp[loop1].GetPos());
    }
    unsigned int NHelp = dataHelp.size();
    dataHelp.clear();

    vector<bool> wasDefaultActive;
    vector<int> defaultPos;
    for(loop1 = 0; loop1 < dataDefault.size(); loop1++)
    {
        wasDefaultActive.push_back(dataDefault[loop1].GetActive());
        defaultPos.push_back(dataDefault[loop1].GetPos());
    }
    unsigned int NDefault = dataDefault.size();
    dataDefault.clear();

    for(loop1 = 0; loop1 < nData; loop1++)
    {
        E_SCRIPTDATAKIND temp;
        string help;
        temp = C_ScriptSystem::Inst()->GetScriptKindDataKind(iKind, loop1);
        switch(temp)
        {
            case DOUBLE :  help = "Double type"; break;
            case INTEGER : help = "Integer type"; break;
            case STRING : help = "String type"; break;
            case TILE_LINK : help = "Tile link type"; break;
        }
        dataName.push_back(C_Input());
        dataName[dataName.size() - 1].Set(5, top, 20, 1, C_ScriptSystem::Inst()->GetScriptKindDataName(iKind, loop1), help);
        top += 23;

        dataHelp.push_back(C_Input());
        dataHelp[dataHelp.size() - 1].Set(5, top, 20, C_Input::AUTORESIZE, C_ScriptSystem::Inst()->GetScriptKindDataHelp(iKind, loop1), "Help");
        top += 6 + dataHelp[dataHelp.size() - 1].GetNLines() * 20;

        //if(temp != TILE_LINK) (vytváøet vždy, ale nìkdy nezobrazovat;
        {
            dataDefault.push_back(C_Input());
            dataDefault[dataDefault.size() - 1].Set(5, top, 20, C_Input::AUTORESIZE, C_ScriptSystem::Inst()->GetScriptKindDataDefault(iKind, loop1), "Default value - " + help);
            if(temp != TILE_LINK) //(kdyz se nezobrazeje, tak se ani neoddali zbytek
                top += 6 + dataDefault[dataDefault.size() - 1].GetNLines() * 20;
            else
                dataDefault[dataDefault.size() - 1].SetVisible(false);
        }

        dataSeparator.push_back(C_Button());
        dataSeparator[dataSeparator.size() - 1].Set(5, top, 267, 2, "", 0, "", C_Color(222, 0, 0), C_Color::black);
        top += 7;
    }

    if(NHelp == dataHelp.size())
    {
        for(loop1 = 0; loop1 < dataHelp.size(); loop1++)
        {
            dataHelp[loop1].SetActive(wasHelpActive[loop1]);
            dataHelp[loop1].SetPos(helpPos[loop1]);
        }
        wasHelpActive.clear();
        helpPos.clear();
    }

    if(NDefault == dataDefault.size())
    {
        for(loop1 = 0; loop1 < dataDefault.size(); loop1++)
        {
            dataDefault[loop1].SetActive(wasDefaultActive[loop1]);
            dataDefault[loop1].SetPos(defaultPos[loop1]);
        }
        wasDefaultActive.clear();
        defaultPos.clear();
    }
}
