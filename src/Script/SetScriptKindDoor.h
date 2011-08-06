#ifndef _SETSCRIPTKINDDOOR_H_
#define _SETSCRIPTKINDDOOR_H_

/*
// Dvere pro nastaveni ruznych druhu skriptu
*/

class C_SetScriptKindDoor : public C_Door
{
    private :
        C_Button * bNew;  // novy skriptKind
        C_Button * bSave; // ulozi nastaveni vsech skriptKind
        C_Button * bLoad; // nacte nasteveni vsech skriptKind

        vector<C_Button> bName;
        vector<C_Button> bEdit;
        vector<C_Button> bErase;
    public :
        C_SetScriptKindDoor();
        C_SetScriptKindDoor(FILE * fr);
        C_SetScriptKindDoor(int n_x, int n_y, int n_width, int n_height,string n_title);
        ~C_SetScriptKindDoor();

        void FirstTime();
        void DoorEverTime();
        void DoorActionOut();
        void DoorActionDown(int button);
        void MakeButtons(); // na zacatku pri vytvoreni dveri vytvori buttony na zaklade poctu typu dveri
        void DrawContent();
};


#endif
