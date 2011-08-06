#ifndef _SCRIPTKIND_H_
#define _SCRIPTKIND_H_

using namespace std;

class C_ScriptKind
{
    private :
        string name;                // nazev druhu skriptu
        vector<S_ScriptData> data;
    public :
        C_ScriptKind() {};
        C_ScriptKind(string n_name) {name = n_name;};
        ~C_ScriptKind() {};

        void Save(FILE *fw);
        void Load(FILE *fr);

        string              GetName() {return name;};
        int                 GetNData() {return data.size();};               // vrati pocet prvku v data
        string              GetDataName(int iData) {return data[iData].name;};   // vrati jmeno danych "dat"
        string              GetDataHelp(int iData) {return data[iData].help;};
        string              GetDataDefault(int iData) {return data[iData].defaultValue;};
        E_SCRIPTDATAKIND    GetDataKind(int iData) {return data[iData].kind;};
        vector<S_ScriptData> GetData() {return data;};
        int                 GetDataValue(int iData, int iValue) {return data[iData].value[iValue];};

        void SetDataName(int iData, string n_name) {data[iData].name = n_name;};
        void SetDataHelp(int iData, string n_help) {data[iData].help = n_help;};
        void SetDataDefault(int iData, string n_default) {data[iData].defaultValue = n_default;};
        void SetName(string n_name) {name = n_name;};
        void SetDataValue(int iData, int iValue, int n_value) {data[iData].value[iValue] = n_value;};
        void AddData(E_SCRIPTDATAKIND kind);
};

#endif
