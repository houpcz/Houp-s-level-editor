#ifndef _NEWSCRIPTKINDDOOR_H_
#define _NEWSCRIPTKINDDOOR_H_

/*
// Dvere pro novy druh skriptu, ale i pro upraveni jiz existujiciho
*/

class C_NewScriptKindDoor : public C_Door
{
    private :
        C_Input * scriptKindName;
        C_Button * bDouble;
        C_Button * bInteger;
        C_Button * bString;
        C_Button * bColor;
        C_Button * bPath;

        vector<C_Input> dataName;      // nazvy jednotlivych stringu, integeru, double v druhu skriptu
        vector<C_Input> dataHelp;
        vector<C_Input> dataDefault;    // defaultni hodnoty pri vytvoreni noveho skriptu
        vector<C_Button> dataSeparator; // cara oddelujici jednotlive data, je to C_Button, protoze neni nic jineho na "cary" :)
        int iKind;              // index druhu skriptu v scriptSystem
    public :
        C_NewScriptKindDoor(FILE * fr) {}; // neni jistota existence daneho druhu, nepouzivat
        C_NewScriptKindDoor(int n_x, int n_y, int n_width, int n_height, string n_title, int n_iKind);
        ~C_NewScriptKindDoor() {};
        void Save(FILE * fw);
        void Load(FILE * fr);
        void DrawContent();
        void DoorEverTime();
        void DoorActionDown(int button);
        void FirstTime();
        void MakeContent();
};

#endif
