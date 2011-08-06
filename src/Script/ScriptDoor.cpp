#include "ScriptDoor.h"
#include "ScriptSystem.h"
#include "..\LevelEditor.h"
#include "..\Map.h"

///////////////////////////////////////////
// NEPOUZIVAT !!! ->                     //
// neni jistota existence daneho skriptu //
///////////////////////////////////////////

C_ScriptDoor::C_ScriptDoor(FILE * fr)
{
    Load(fr);
}

void C_ScriptDoor::Load(FILE * fr)
{
}

void C_ScriptDoor::Save(FILE * fw)
{
}

///////////////////////
// <- NEPOUZIVAT !!! //
///////////////////////

C_ScriptDoor::C_ScriptDoor(int n_x, int n_y, int n_width, int n_height, string n_title, int n_iScript)
{
    doorKind = D_NEW_SCRIPT;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
    iScript = n_iScript;
}

C_ScriptDoor::~C_ScriptDoor()
{
    label.clear();
    input.clear();
}

void C_ScriptDoor::FirstTime()
{
    resize = false;
    minimalizeable = true;
    closeable = true;
    resizeableVertical = true;
    resizeableHorizontal = false;
    isTitle = true;
    scrollable = true;

    MakeContent();
}

void C_ScriptDoor::MakeContent()
{
    unsigned int loop1, loop2;

    int iString = 0;
    int iDouble = 0;
    int iInteger = 0;
    int iTileLink = 0;

    int iLabel = 0;
    int iInput = 0;

    int top = 35;
    int left = 4;

    bool isPushed;

    char align = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER;

    vector<bool> wasInputActive;
    vector<int> inputPos;
    for(loop1 = 0; loop1 < input.size(); loop1++)
    {
        wasInputActive.push_back(input[loop1]->GetActive());
        inputPos.push_back(input[loop1]->GetPos());
    }
    unsigned int NInput = input.size();

    label.clear();
    input.clear();

    for(loop1 = 0; loop1 < delAllTile.size(); loop1++)
    {
        delTile[loop1].clear();
        showTile[loop1].clear();
    }

    vector<bool> wasBNewTileLinkPushed;
    for(loop1 = 0; loop1 < newTileLink.GetNButton(); loop1++) {
        isPushed = newTileLink.GetIsPushedAndUnpush(loop1);
        wasBNewTileLinkPushed.push_back(isPushed);
    }
    unsigned int NNewTileLink = newTileLink.GetNButton();
    newTileLink.Clear();

    vector<bool> wasBShowTileLinkPushed;
    for(loop1 = 0; loop1 < showTileLink.GetNButton(); loop1++)
    {
        isPushed = showTileLink.GetIsPushedAndUnpush(loop1);
        wasBShowTileLinkPushed.push_back(isPushed);
    }
    unsigned int NShowTileLink = showTileLink.GetNButton();
    showTileLink.Clear();

    delAllTile.clear();

    delTile.clear();
    showTile.clear();

    S_MAPPOS tempTile;
    vector<C_Button> tempVecButton;

    title = C_ScriptSystem::Inst()->GetScriptName(iScript);
    scriptName = new C_Input(left, 5, 22, 1, title);
    nData = C_ScriptSystem::Inst()->GetNScriptData(iScript);

    unsigned int groupMax = C_ScriptSystem::Inst()->GetNGroup();
    string help = C_ScriptSystem::Inst()->GetScriptGroupName(0);
    for(unsigned int loop1 = 1; loop1 < groupMax; loop1++)
    {
        help += ";" + C_ScriptSystem::Inst()->GetScriptGroupName(loop1);
    }

    char tempString[20];
    sprintf(tempString, "0 .. %d", groupMax - 1);
    group = new C_InputInteger(left, top, 2, 1, ToString(C_ScriptSystem::Inst()->GetScriptIGroup(iScript)), tempString);
    groupName = new C_Button(left + 40, top, 200, 20, C_ScriptSystem::Inst()->GetScriptGroupName(C_ScriptSystem::Inst()->GetScriptIGroup(iScript)), FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER, help);
    group->SetMinMax(0, groupMax  - 1);
    top += 24;

    AddFormElement(group);
    AddFormElement(groupName);
    AddFormElement(scriptName);

    for(loop1 = 0; loop1 < nData; loop1++)
    {
        switch(C_ScriptSystem::Inst()->GetScriptDataKind(iScript, loop1))
        {
            case DOUBLE :
                label.push_back(C_Label());
                label[iLabel++].Set(left, top, 22, 1, C_ScriptSystem::Inst()->GetScriptDataName(iScript, loop1), C_ScriptSystem::Inst()->GetScriptDataHelp(iScript, loop1), 255, 255, 233, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                input.push_back(new C_InputDouble);
                input[iInput++]->Set(left, top + 30, 22, 1, ToString(C_ScriptSystem::Inst()->GetScriptDataDouble(iScript, iDouble++)), "Double type");
                top += 60;
                break;
            case INTEGER :
                label.push_back(C_Label());
                label[iLabel++].Set(left, top, 22, 1, C_ScriptSystem::Inst()->GetScriptDataName(iScript, loop1), C_ScriptSystem::Inst()->GetScriptDataHelp(iScript, loop1), 255, 255, 233, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                input.push_back(new C_InputInteger);
                //fprintf(stderr, "%s \n", ToString(C_ScriptSystem::Inst()->GetScriptDataInt(iScript, iInteger)).c_str());
                input[iInput++]->Set(left, top + 30, 22, 1, ToString(C_ScriptSystem::Inst()->GetScriptDataInt(iScript, iInteger++)), "Integer type");
                top += 60;
                break;
            case STRING :
                label.push_back(C_Label());
                label[iLabel++].Set(left, top, 22, 1, C_ScriptSystem::Inst()->GetScriptDataName(iScript, loop1), C_ScriptSystem::Inst()->GetScriptDataHelp(iScript, loop1), 255, 255, 233, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                input.push_back(new C_Input);
                input[iInput++]->Set(left, top + 30, 22, C_Input::AUTORESIZE, C_ScriptSystem::Inst()->GetScriptDataString(iScript, iString++), "String type");
                top += input[iInput-1]->GetNLines() * 20 + 40;
                break;
            case TILE_LINK :
                label.push_back(C_Label());
                label[iLabel++].Set(left, top, 22, 1, C_ScriptSystem::Inst()->GetScriptDataName(iScript, loop1), C_ScriptSystem::Inst()->GetScriptDataHelp(iScript, loop1), 255, 255, 233, 0, 0, 0, 0, 0, 0, 0, 0, 0);

                newTileLink.AddButton(left, top + 25, 60, 25, "New", align, "Add tile from pipette");
                showTileLink.AddButton(left + 65, top + 25, 60, 25, "Show", align, "Show all tiles");
                delAllTile.push_back(C_Button());
                delAllTile[iTileLink].Set(left + 130, top + 25, 60, 25, "Del", align, "Del all tiles");

                delTile.push_back(tempVecButton);
                showTile.push_back(tempVecButton);

                top += 55;
                for(loop2 = 0; loop2 < C_ScriptSystem::Inst()->GetScriptDataNTileInTileLink(iScript, iTileLink); loop2++)
                {
                    if(loop2 % 2 == 0)
                        left = 4;
                    else
                        left = 155;
                    tempTile = C_ScriptSystem::Inst()->GetScriptDataTileInTileLink(iScript, iTileLink, loop2);

                    delTile[iTileLink].push_back(C_Button());
                    delTile[iTileLink][loop2].Set(left + 100, top, 20, 20, "D", align, "Delete", C_Color::greyDarkSlate, C_Color::whiteSnow);

                    showTile[iTileLink].push_back(C_Button());
                    showTile[iTileLink][loop2].Set(left + 125, top, 20, 20, "S", align, "Show in map", C_Color::greyDarkSlate, C_Color::whiteSnow);
                    //fprintf(stderr, "D %d S %d %d\n", delTile[iTileLink].size(), showTile[iTileLink].size(), loop2);
                    label.push_back(C_Label());
                    label[iLabel++].Set(left, top, 3, 1, ToString(tempTile.x), "x coords", 22, 22, 44, 255, 233, 233, 255, 233, 233, 255, 233, 233);
                    label.push_back(C_Label());
                    label[iLabel++].Set(left + 50, top, 3, 1, ToString(tempTile.y), "y coords", 22, 22, 44, 255, 233, 233, 255, 233, 233, 255, 233, 233);
                    if(loop2 % 2 == 1)
                        top += 27;
                }
                if(loop2 % 2 == 1)
                        top += 27;

                iTileLink++;

                left = 4;
                top += 5;
                break;
        }
    }

    if(newTileLink.GetNButton() == NNewTileLink && showTileLink.GetNButton() == NShowTileLink)
    {
        for(loop1 = 0; loop1 < newTileLink.GetNButton(); loop1++)
        {
            isPushed = wasBNewTileLinkPushed[loop1];
            newTileLink.SetPushed(isPushed, loop1);
        }
        for(loop1 = 0; loop1 < showTileLink.GetNButton(); loop1++)
        {
            isPushed = wasBShowTileLinkPushed[loop1];
            showTileLink.SetPushed(isPushed, loop1);
        }
    }
    if(NInput == input.size())
    {
        for(loop1 = 0; loop1 < input.size(); loop1++)
        {
            input[loop1]->SetActive(wasInputActive[loop1]);
            input[loop1]->SetPos(inputPos[loop1]);
        }
    }
    wasInputActive.clear();
    inputPos.clear();
    wasBNewTileLinkPushed.clear();
    wasBShowTileLinkPushed.clear();
}

void C_ScriptDoor::DrawContent()
{
    C_Door::DrawContent();

    unsigned int loop1, loop2;

    for(loop1 = 0; loop1 < label.size(); loop1++)
        label[loop1].Draw();
    for(loop1 = 0; loop1 < input.size(); loop1++)
        input[loop1]->Draw();

    newTileLink.Draw();
    showTileLink.Draw();
    for(loop1 = 0; loop1 < delAllTile.size(); loop1++)
    {
        delAllTile[loop1].Draw();
        for(loop2 = 0; loop2 < delTile[loop1].size(); loop2++)
            delTile[loop1][loop2].Draw();
        for(loop2 = 0; loop2 < showTile[loop1].size(); loop2++)
            showTile[loop1][loop2].Draw();
    }
}

void C_ScriptDoor::DoorEverTime()
{
    C_Door::DoorEverTime();

    unsigned int loop1, loop2;

    int iString = 0;
    int iDouble = 0;
    int iInteger = 0;
    int iTileLink = 0;
    C_Button * tempButton;

    if(!group->GetActive() && (unsigned) group->GetInteger() != C_ScriptSystem::Inst()->GetScriptIGroup(iScript))
    {
       C_ScriptSystem::Inst()->SetScriptIGroup(group->GetInteger(), iScript);
       MakeContent();
    }

    title = scriptName->GetString();
    C_ScriptSystem::Inst()->SetScriptName(scriptName->GetString(), iScript);

    for(loop1 = 0; loop1 < label.size(); loop1++)
        label[loop1].MainLoop(interMouseX, interMouseY);

    for(loop1 = 0; loop1 < input.size(); loop1++)
        input[loop1]->MainLoop(interMouseX, interMouseY);

    int iInput = 0;
    S_MAPPOS tempTile;

    newTileLink.MainLoop(interMouseX, interMouseY);
    showTileLink.MainLoop(interMouseX, interMouseY);
    for(loop1 = 0; loop1 < C_ScriptSystem::Inst()->GetNScriptData(iScript); loop1++)
    {
        switch(C_ScriptSystem::Inst()->GetScriptDataKind(iScript, loop1))
        {
            case STRING :
                C_ScriptSystem::Inst()->SetScriptDataString(input[iInput++]->GetString(), iScript, iString++);
                break;
            case DOUBLE :
                C_ScriptSystem::Inst()->SetScriptDataDouble(input[iInput++]->GetDouble(), iScript, iDouble++);
                break;
            case INTEGER :
                C_ScriptSystem::Inst()->SetScriptDataInt(input[iInput++]->GetInteger(), iScript, iInteger++);
                break;
            case TILE_LINK :    // resi se zcela jinak
                delAllTile[iTileLink].MainLoop(interMouseX, interMouseY);
                if(delAllTile[iTileLink].GetIsPushedAndUnpush())
                {
                    C_ScriptSystem::Inst()->DeleteScriptDataAllTileInTileLink(iScript, iTileLink);
                    MakeContent();
                }

                tempButton = newTileLink.GetButton(iTileLink);
                if(tempButton->GetIsPushed())
                {
                    for(unsigned int loop3 = 0; loop3 < C_LevelEditor::Inst()->GetNPipetteTile(); loop3++)
                    {
                        tempTile = C_LevelEditor::Inst()->GetPipetteTile(loop3);
                        if(C_LevelEditor::Inst()->IsCorrectTile(tempTile))
                        {
                            bool newTile = true;        // ma se pridat novy tile? Zalezi na typu tilu
                            if(C_ScriptSystem::Inst()->GetScriptDataValue(iScript, loop1, 0) == CLASSIC)
                            {
                                for(loop2 = 0; loop2 < C_ScriptSystem::Inst()->GetScriptDataNTileInTileLink(iScript, iTileLink); loop2++)
                                {
                                    if(tempTile == C_ScriptSystem::Inst()->GetScriptDataTileInTileLink(iScript, iTileLink, loop2))
                                    {
                                        C_ScriptSystem::Inst()->DeleteScriptDataTileInTileLink(iScript, iTileLink, loop2);
                                        newTile = false;
                                        break;
                                    }
                                }
                            } else if(C_ScriptSystem::Inst()->GetScriptDataValue(iScript, loop1, 0) == PATH)
                            {
                                if(C_ScriptSystem::Inst()->GetScriptDataNTileInTileLink(iScript, iTileLink) >= 1)
                                {
                                    S_MAPPOS lastTile = C_ScriptSystem::Inst()->GetScriptDataTileInTileLink(iScript, iTileLink, C_ScriptSystem::Inst()->GetScriptDataNTileInTileLink(iScript, iTileLink) - 1);
                                    // je-li novy tile sousedni, muze se pridat, jinak ne
                                    if((lastTile.x == tempTile.x && lastTile.y == tempTile.y + 1) ||
                                       (lastTile.x == tempTile.x && lastTile.y == tempTile.y - 1) ||
                                       (lastTile.x == tempTile.x + 1 && lastTile.y == tempTile.y) ||
                                       (lastTile.x == tempTile.x - 1 && lastTile.y == tempTile.y) ||
                                       (lastTile.x == tempTile.x && lastTile.y == tempTile.y + 2) ||
                                       (lastTile.x == tempTile.x && lastTile.y == tempTile.y - 2) ||
                                       (lastTile.x == tempTile.x + 2 && lastTile.y == tempTile.y) ||
                                       (lastTile.x == tempTile.x - 2 && lastTile.y == tempTile.y) ||
                                       (lastTile.x == tempTile.x && lastTile.y == tempTile.y + 3) ||
                                       (lastTile.x == tempTile.x && lastTile.y == tempTile.y - 3) ||
                                       (lastTile.x == tempTile.x + 3 && lastTile.y == tempTile.y) ||
                                       (lastTile.x == tempTile.x - 3 && lastTile.y == tempTile.y) )
                                        newTile = true;
                                    else
                                        newTile = false;  // Zde WARNING tile cant be in the path
                                } else
                                    newTile = true;
                            }
                            if(newTile)
                                C_ScriptSystem::Inst()->AddScriptDataTileInTileLink(tempTile, iScript, iTileLink);
                        }
                    }
                    if(C_LevelEditor::Inst()->GetNPipetteTile() > 0)
                    {
                        C_LevelEditor::Inst()->ClearPipetteTile();
                        MakeContent();
                    }
                }

                tempButton = showTileLink.GetButton(iTileLink);
                if(tempButton->GetIsPushed())
                {
                    C_LevelEditor::Inst()->MarkedTile(C_ScriptSystem::Inst()->GetScriptDataTileLink(iScript, iTileLink));
                } else if(tempButton->GetNewUnpushed())
                    C_LevelEditor::Inst()->UnmarkedTile(C_ScriptSystem::Inst()->GetScriptDataTileLink(iScript, iTileLink));

                for(loop2 = 0; loop2 < showTile[iTileLink].size(); loop2++)
                    showTile[iTileLink][loop2].MainLoop(interMouseX, interMouseY);
                for(loop2 = 0; loop2 < delTile[iTileLink].size(); loop2++)
                    delTile[iTileLink][loop2].MainLoop(interMouseX, interMouseY);
                iTileLink++;
                break;
        }
    }

    for(loop1 = 0; loop1 < input.size(); loop1++)
        if(input[loop1]->GetUpdateNLines())
        {
            MakeContent();
            break;
        }

    if(App::Inst()->GetKey(SDLK_LEFT))
    {
        C_ScriptSystem::Inst()->TranslateTiles(iScript, -1,0);
        MakeContent();
    }
    if(App::Inst()->GetKey(SDLK_RIGHT))
    {
        C_ScriptSystem::Inst()->TranslateTiles(iScript, 1,0);
        MakeContent();
    }
    if(App::Inst()->GetKey(SDLK_UP))
    {
        C_ScriptSystem::Inst()->TranslateTiles(iScript, 0,-1);
        MakeContent();
    }
    if(App::Inst()->GetKey(SDLK_DOWN))
    {
        C_ScriptSystem::Inst()->TranslateTiles(iScript, 0,1);
        MakeContent();
    }
}

void C_ScriptDoor::DoorActionDown(int button)
{
    C_Door::DoorActionDown(button);

    unsigned int loop1, loop2;

    if(button == LEFTBUTTON)
    {
        if(groupName->GetIsPushedAndUnpush())
        {
            unsigned int groupID = group->GetInteger() + 1;
            if(groupID == C_ScriptSystem::Inst()->GetNGroup())
                groupID = 0;
            C_ScriptSystem::Inst()->SetScriptIGroup(groupID, iScript);
            MakeContent();
        }

        for(loop1 = 0; loop1 < input.size(); loop1++)
        {
            input[loop1]->MouseClick(interMouseX, interMouseY);
        }

        showTileLink.MouseClick(interMouseX, interMouseY);
        newTileLink.MouseClick(interMouseX, interMouseY);
        for(loop1 = 0; loop1 < delAllTile.size(); loop1++)
        {
            delAllTile[loop1].MouseClick(interMouseX, interMouseY);
        }

        for(loop1 = 0; loop1 < delAllTile.size(); loop1++)
        {
            for(loop2 = 0; loop2 < delTile[loop1].size(); loop2++)
            {
                delTile[loop1][loop2].MouseClick(interMouseX, interMouseY);
                if(delTile[loop1][loop2].GetIsPushedAndUnpush())
                {
                    C_ScriptSystem::Inst()->DeleteScriptDataTileInTileLink(iScript, loop1, loop2);
                    MakeContent();
                }
            }
            for(loop2 = 0; loop2 < showTile[loop1].size(); loop2++)
            {
                showTile[loop1][loop2].MouseClick(interMouseX, interMouseY);
                if(showTile[loop1][loop2].GetIsPushedAndUnpush())
                {
                    vector<S_MAPPOS> tempMapPos;
                    tempMapPos.push_back(C_ScriptSystem::Inst()->GetScriptDataTileInTileLink(iScript, loop1, loop2));
                    C_LevelEditor::Inst()->MarkedTile(tempMapPos);
                }
            }
        }
    }
}
