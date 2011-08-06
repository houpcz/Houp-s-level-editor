#include "ScriptSystem.h"
#include "..\Door\DoorSystem.h"
#include "..\Map.h"
#include "..\Utils\FileMan.h"
#include "..\LevelEditor.h"
#include "..\Utils\XML\ManXML.h"

C_ScriptSystem * C_ScriptSystem::inst = NULL;

C_ScriptSystem * C_ScriptSystem::Inst()
{
    if (inst == NULL)
    {
        inst = new C_ScriptSystem;
    }
    return inst;
}

string C_ScriptSystem::GetScriptKindName(unsigned int index)
{
    if(index < scriptKind.size())
        return scriptKind[index].GetName();
    else
        return string("");
}

C_ScriptSystem::~C_ScriptSystem()
{
    scriptKind.clear();
}

C_ScriptSystem::C_ScriptSystem()
{
    MakeMainScriptGroup();
    activeGroup = 0;
}

void C_ScriptSystem::DeleteScriptKind(unsigned int iKind)
{
    scriptKind.erase(scriptKind.begin() + iKind, scriptKind.begin() + iKind + 1);
}

void C_ScriptSystem::DeleteScript(unsigned int iScript)
{
    script.erase(script.begin() + iScript, script.begin() + iScript + 1);
}

void C_ScriptSystem::NewScriptKind()
{
    scriptKind.push_back(C_ScriptKind("Noname kind"));
    //C_DoorSystem::Inst()->OpenNewDoor(D_NEW_SCRIPT_KIND, scriptKind.size() - 1);
}

void C_ScriptSystem::SaveScriptKind()
{
    string src = C_LevelEditor::Inst()->GetGameSetup()->GetName() + "/Script.dat";
    FILE *fw;
    int size = scriptKind.size();

    fw = fopen(src.c_str(), "wb");
    fwrite(&size, sizeof(int), 1, fw);
    for(int loop1 = 0; loop1 < size; loop1++)
        scriptKind[loop1].Save(fw);

    fclose(fw);
}

void C_ScriptSystem::LoadScriptKind()
{
    string src = C_LevelEditor::Inst()->GetGameSetup()->GetName() + "/Script.dat";
    FILE *fr;
    int size;

    if((fr = fopen(src.c_str(), "rb")) != NULL)
    {
        fread(&size, sizeof(int), 1, fr);

        scriptKind.erase(scriptKind.begin(), scriptKind.end());

        for(int loop1 = 0; loop1 < size; loop1++)
        {
            scriptKind.push_back(C_ScriptKind());
            scriptKind[loop1].Load(fr);
        }
        fclose(fr);
    }
}

void C_ScriptSystem::SaveXML()
{
    C_Map * pMap = C_LevelEditor::Inst()->GetMap();
    string src = C_LevelEditor::Inst()->GetGameSetup()->GetName() + "/Maps/" + pMap->GetName() + ".xml";

    XMLDoc doc = XMLDoc("scripts");
    XMLNode * node = doc.GetRootNode();
    for(int loop1 = 0; loop1 < script.size(); loop1++)
        node->AddChild(script[loop1].CreateXMLTree());

    doc.Save(src);
}

void C_ScriptSystem::SaveScript()
{
    FILE *fw, *fwt;
    SaveXML();

    C_Map * pMap = C_LevelEditor::Inst()->GetMap();

    int sizeScript = script.size();
    int sizeGroup = scriptGroup.size();

    string src = C_LevelEditor::Inst()->GetGameSetup()->GetName() + "/Maps/" + pMap->GetName() + ".script";
    fw = fopen(src.c_str(), "wb");
    src = "data/" + pMap->GetName() + ".tscript";
    fwt = fopen(src.c_str(), "w");

    fwrite(&sizeGroup, sizeof(int), 1, fw);
    for(int loop1 = 0; loop1 < sizeGroup; loop1++)
        scriptGroup[loop1].Save(fw);

    fwrite(&sizeScript, sizeof(int), 1, fw);
    for(int loop1 = 0; loop1 < sizeScript; loop1++)
        script[loop1].Save(fw, fwt);

    fclose(fw);
    fclose(fwt);
}

void C_ScriptSystem::LoadScript(string mapName)
{
    string src = C_LevelEditor::Inst()->GetGameSetup()->GetName() + "/Maps/" + mapName + ".script";
    FILE *fr;
    int size;

    if((fr = fopen(src.c_str(), "rb")) != NULL)
    {
        script.clear();
        scriptGroup.clear();
        fread(&size, sizeof(int), 1, fr);
        for(int loop1 = 0; loop1 < size; loop1++)
        {
            scriptGroup.push_back(C_ScriptGroup());
            scriptGroup[loop1].Load(fr);
        }

        fread(&size, sizeof(int), 1, fr);
        for(int loop1 = 0; loop1 < size; loop1++)
        {
            script.push_back(C_Script());
            script[loop1].Load(fr);
        }
        fclose(fr);
    }
}

void C_ScriptSystem::DeleteScriptGroup(unsigned int iGroup)
{
    unsigned int loop1;

    if(iGroup != 0 && iGroup < scriptGroup.size())
    {
        for(loop1 = 0; loop1 < script.size(); loop1++)
        {
            if(script[loop1].GetIGroup() > iGroup)
                script[loop1].DecreaseIGroup();
            else if(script[loop1].GetIGroup() == iGroup)
                script[loop1].SetIGroup(C_ScriptSystem::Inst()->GetScriptGroupIParent(iGroup));
        }
        for(loop1 = 1; loop1 < scriptGroup.size(); loop1++)
        {
            if(scriptGroup[loop1].GetIParent() == iGroup)
            {
                scriptGroup[loop1].SetIParent(GetScriptGroupIParent(iGroup));
            }

            if(scriptGroup[loop1].GetIParent() > iGroup)
                scriptGroup[loop1].SetIParent(scriptGroup[loop1].GetIParent() -1);
        }
        scriptGroup.erase(scriptGroup.begin() + iGroup, scriptGroup.begin() + iGroup + 1);
    }
}

bool C_ScriptSystem::IsScriptGroupStillTree(int parent, int child)
{
    if(parent == child)
        return false;
    else if(parent == C_ScriptGroup::MAIN_SCRIPT_GROUP)
        return true;
    else
        return IsScriptGroupStillTree(GetScriptGroupIParent(parent), child);
}

void C_ScriptSystem::LoadGroup()
{

}

void C_ScriptKind::Save(FILE *fw)
{
    FileMan::Inst()->SaveString(name, fw);
    FileMan::Inst()->SaveInteger(data.size(), fw);
    for(unsigned int loop1 = 0; loop1 < data.size(); loop1++)
    {
        fwrite(&data[loop1].kind, sizeof(E_SCRIPTDATAKIND), 1, fw);
        FileMan::Inst()->SaveString(data[loop1].name, fw);
        FileMan::Inst()->SaveString(data[loop1].help, fw);
        FileMan::Inst()->SaveString(data[loop1].defaultValue, fw);
        FileMan::Inst()->SaveInteger(data[loop1].value[0], fw);
        FileMan::Inst()->SaveInteger(data[loop1].value[1], fw);
        FileMan::Inst()->SaveInteger(data[loop1].value[2], fw);
    }
}

void C_ScriptKind::Load(FILE *fr)
{
    FileMan::Inst()->LoadString(name, fr);
    int size;
    S_ScriptData temp;
    FileMan::Inst()->LoadInteger(size, fr);
    for(int loop1 = 0; loop1 < size; loop1++)
    {
        data.push_back(temp);
        fread(&data[loop1].kind, sizeof(E_SCRIPTDATAKIND), 1, fr);
        FileMan::Inst()->LoadString(data[loop1].name, fr);
        FileMan::Inst()->LoadString(data[loop1].help, fr);
        FileMan::Inst()->LoadString(data[loop1].defaultValue, fr);
        FileMan::Inst()->LoadInteger(data[loop1].value[0], fr);
        FileMan::Inst()->LoadInteger(data[loop1].value[1], fr);
        FileMan::Inst()->LoadInteger(data[loop1].value[2], fr);
    }
}

void C_ScriptKind::AddData(E_SCRIPTDATAKIND kind)
{
        S_SCRIPTDATA temp;
        temp.kind = kind;
        temp.name = "No Name";
        temp.value[0] = 0;
        temp.value[1] = 5;
        temp.value[2] = 10;
        data.push_back(temp);
}
