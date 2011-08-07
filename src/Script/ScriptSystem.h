/*
    Hlavicky vsech dveri potrebnych pro scriptovani
*/

#ifndef _SCRIPTSYSTEM_H_
#define _SCRIPTSYSTEM_H_

#include "Script.h"
#include "..\Door\Door.h"
#include "..\Form\ButtonGroup.h"
#include "..\Form\Form.h"
#include "ScriptKind.h"
#include "ScriptGroup.h"
#include <vector>

class C_ScriptSystem
{
    private :
        vector<C_ScriptKind> scriptKind;
        vector<C_Script> script;
        vector<C_ScriptGroup> scriptGroup;
        unsigned int activeGroup;               // ktera skupina je aktivni, ve ktere se tvori nove skripty

        static C_ScriptSystem * inst;

        C_ScriptSystem();
        ~C_ScriptSystem();
    public :
        static C_ScriptSystem * Inst();

        void TranslateTiles(unsigned int iScript, int dX, int dY) {script[iScript].TranslateTiles(dX, dY);};       // posune vsechny tily ve scriptu, kdyz to nejde, dany tile vyhodi

        void DeleteScriptKind(unsigned int iKind);
        void NewScriptKind();
        void SaveScriptKind(string name);
        void LoadScriptKind();
        void CreateNewProject(string name);

        void DeleteScript(unsigned int iScript);
        void DeleteAllScript() {script.clear(); DeleteAllGroup();};
        void DeleteAllGroup() {scriptGroup.clear();};
        int NewScript(int iKind) {script.push_back(C_Script(scriptKind[iKind].GetData(), scriptKind[iKind].GetName())); return (script.size() - 1);};
        void CopyScript(int iScript) {script.push_back(script[iScript]);};
        void SaveScript();
        void SaveXML();
        void LoadScript(string mapName);
        void LoadGroup();

        void            MakeMainScriptGroup() {AddNewScriptGroup("Main group", C_ScriptGroup::NO_PARENT, true);};
        void            AddNewScriptGroup(string n_name, int iParent, bool n_visible) {scriptGroup.push_back(C_ScriptGroup(n_name, iParent, n_visible));};
        bool            IsScriptGroupStillTree(int parent, int child);
        void            DeleteScriptGroup(unsigned int iGroup);

        unsigned int    GetScriptGroupIParent(unsigned int iGroup) {return scriptGroup[iGroup].GetIParent();};
        string          GetScriptGroupName(unsigned int iGroup) {if(iGroup < scriptGroup.size()) return scriptGroup[iGroup].GetName(); else return "";};
        bool            GetScriptGroupVisible(unsigned int iGroup) {return scriptGroup[iGroup].GetVisible();};
        unsigned int    GetActiveGroup() {return activeGroup;};
        unsigned int    GetNGroup() {return scriptGroup.size();};

        void            SetScriptGroupIParent(unsigned int iParent, unsigned int iGroup) {scriptGroup[iGroup].SetIParent(iParent);};
        void            SetScriptGroupVisible(bool n_visible, unsigned int iGroup) {scriptGroup[iGroup].SetVisible(n_visible);};
        void            SetScriptGroupName(string n_name, unsigned int iGroup) {scriptGroup[iGroup].SetName(n_name);};
        void            SetActiveGroup(int n_activeGroup) {activeGroup = n_activeGroup;};

        string           GetScriptKindName(unsigned int index);
        int              GetNScriptKind() {return scriptKind.size();};
        int              GetNScriptKindData(int iKind) {return scriptKind[iKind].GetNData();};
        E_SCRIPTDATAKIND GetScriptKindDataKind(int iKind, int iData) {return scriptKind[iKind].GetDataKind(iData);};
        string           GetScriptKindDataName(int iKind, int iData) {return scriptKind[iKind].GetDataName(iData);};
        string           GetScriptKindDataHelp(int iKind, int iData) {return scriptKind[iKind].GetDataHelp(iData);};
        string           GetScriptKindDataDefault(int iKind, int iData) {return scriptKind[iKind].GetDataDefault(iData);};
        int              GetScriptKindDataValue(int iKind, int iData, int iValue) {return scriptKind[iKind].GetDataValue(iData, iValue);};

        void SetScriptKindName(string n_name, int iKind) {scriptKind[iKind].SetName(n_name);};
        void SetScriptKindDataName(string n_name, int iKind, int iData) {scriptKind[iKind].SetDataName(iData, n_name);};
        void SetScriptKindDataHelp(string n_help, int iKind, int iData) {scriptKind[iKind].SetDataHelp(iData, n_help);};
        void SetScriptKindDataDefault(string n_default, int iKind, int iData) {scriptKind[iKind].SetDataDefault(iData, n_default);};
        void SetScriptKindDataValue(int n_value, int iKind, int iData, int iValue) {scriptKind[iKind].SetDataValue(iData, iValue, n_value);};
        void AddNewScriptDataKind(E_SCRIPTDATAKIND kind, int iKind) {scriptKind[iKind].AddData(kind);};

        void             ShowScript(int iScript) {script[iScript].Show();};
        unsigned int     GetNScript() {return script.size();};
        string           GetScriptName(int iScript) {return script[iScript].GetName();};
        unsigned int     GetNScriptData(int iScript) {return script[iScript].GetNData();};
        unsigned int     GetScriptIGroup(int iScript) {return script[iScript].GetIGroup();};
        E_SCRIPTDATAKIND GetScriptDataKind(int iScript, int iData) {return script[iScript].GetDataKind(iData);};
        string           GetScriptDataName(int iScript, int iData) {return script[iScript].GetDataName(iData);};
        string           GetScriptDataHelp(int iScript, int iData) {return script[iScript].GetDataHelp(iData);};
        int              GetScriptDataValue(int iScript, int iData, int iValue) {return script[iScript].GetDataValue(iData, iValue);};

        string           GetScriptDataString(int iScript, int iString) {return script[iScript].GetString(iString);};
        double           GetScriptDataDouble(int iScript, int iDouble) {return script[iScript].GetDouble(iDouble);};
        int              GetScriptDataInt(int iScript, int iInt) {return script[iScript].GetInt(iInt);};
        unsigned int     GetScriptDataNTileInTileLink(int iScript, int iTileLink) {return script[iScript].GetNTileInTileLink(iTileLink);};
        S_MAPPOS         GetScriptDataTileInTileLink(int iScript, int iTileLink, int iTile) {return script[iScript].GetTileInTileLink(iTileLink, iTile);};
        vector<S_MAPPOS> GetScriptDataTileLink(int iScript, int iTileLink) {return script[iScript].GetTileLink(iTileLink);};

        void SetScriptName(string n_name, int iScript) {script[iScript].SetName(n_name);};
        void SetScriptIGroup(unsigned int n_iGroup, int iScript) {script[iScript].SetIGroup(n_iGroup);};
        void SetScriptDataString(string n_string, int iScript, int iString) {script[iScript].SetString(n_string, iString);};
        void SetScriptDataDouble(double n_double, int iScript, int iDouble) {script[iScript].SetDouble(n_double, iDouble);};
        void SetScriptDataInt(int n_int, int iScript, int iInt) {script[iScript].SetInt(n_int, iInt);};

        void AddScriptDataTileInTileLink(S_MAPPOS n_tile, int iScript, int iTileLink) {script[iScript].AddTileInTileLink(n_tile, iTileLink);};
        void DeleteScriptDataTileInTileLink(int iScript, int iTileLink, int iTile) {script[iScript].DeleteTileInTileLink(iTileLink, iTile);};
        void DeleteScriptDataAllTileInTileLink(int iScript, int iTileLink) {script[iScript].DeleteAllTileInTileLink(iTileLink);};
};

#endif
