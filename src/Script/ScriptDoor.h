#ifndef _SCRIPTDOOR_H_
#define _SCRIPTDOOR_H_

#include "../Door/Door.h"

#include "..\Form\Form.h"
#include "..\Form\Input.h"
#include "..\Form\ButtonGroup.h"

class C_ScriptDoor : public C_Door
{
    private :
        C_Input * scriptName;
        C_InputInteger * group;           // cislo skupiny
        C_Button * groupName;             // jeji nazev

        vector<C_Input*> input;
        vector<C_Label> label;
        C_ButtonGroup newTileLink;       // tlacitko pro pridani noveho tilu
        C_ButtonGroup showTileLink;      // ukaze vsechny tlacitka v danem tileLink
        vector<C_Button> delAllTile;        // smaze vsechny tily v tileLink
        vector<vector<C_Button> > delTile;  // tlacitko pro smazani konkretniho tilu z tile link
        vector<vector<C_Button> > showTile; // tlacitko pro ukazani na mape konkretniho tilu
        int iScript;          // Druh skriptu, platny index pouze na zacatku
        unsigned int nData;          // kolik formularovych informaci je
    public :
        C_ScriptDoor(FILE * fr);
        C_ScriptDoor(int n_x, int n_y, int n_width, int n_height, string n_title, int n_iScript);
        ~C_ScriptDoor();
        void Save(FILE * fw);
        void Load(FILE * fr);
        void FirstTime();
        void DrawContent();
        void DoorEverTime();
        void DoorActionDown(int button);
        void MakeContent();
};

#endif
