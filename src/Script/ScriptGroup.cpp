#include "ScriptSystem.h"
#include "..\Utils\FileMan.h"

unsigned int C_ScriptGroup::amount=0;

C_ScriptGroup::C_ScriptGroup(string n_name, int n_iParent, bool n_visible)
{
    name = n_name;
    iParent = n_iParent;
    visible = n_visible;
    i = amount++;
}

void C_ScriptGroup::Load(FILE *fr)
{
    int tempInt;

    FileMan::Inst()->LoadString(name, fr);
    FileMan::Inst()->LoadInteger(tempInt, fr);
    i = tempInt;
    FileMan::Inst()->LoadInteger(tempInt, fr);
    iParent = tempInt;
    amount++;
    if(i == 0)
        visible = true;
    else
        visible = false;
}

void C_ScriptGroup::Save(FILE *fw)
{
    FileMan::Inst()->SaveString(name, fw);
    FileMan::Inst()->SaveInteger(i, fw);
    FileMan::Inst()->SaveInteger(iParent, fw);
}
