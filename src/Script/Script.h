#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include "..\Door\Door.h"

#include "..\Form\Form.h"
#include "..\Form\Input.h"
#include "..\Form\ButtonGroup.h"

#include "..\Utils\XML\XMLNode.h"

#include "..\Map.h"

#include <vector>

namespace script
{

typedef enum E_ScriptDataKind {
    DOUBLE,
    INTEGER,
    STRING,
    TILE_LINK
} E_SCRIPTDATAKIND;

}

typedef enum E_TileLink {
    CLASSIC =0,                // Kazdy tile maximalne jednou
    PATH                    // Tily by mely tvorit "cestu", mohou se opakovat
} E_TILELINK;

using namespace script;
typedef struct S_ScriptData {
            string name;
            string help;
            string defaultValue;
            E_SCRIPTDATAKIND kind;
            int value[3];               // tri hodnoty urcujici dany druh (napr. min, max a treti bez fce)
} S_SCRIPTDATA;

using namespace std;

class C_Script
{
    private :
        unsigned int iGroup;        // do jake skupiny patri
        string kindName;            // jmeno kindScript, ktery tento skript vytvoril
        string name;                // nazev skriptu
        vector<S_ScriptData> data;  // prevezme z scriptKind data

        vector<int> intData;
        vector<string> stringData;
        vector<double> doubleData;
        vector<vector<S_MAPPOS> > tileData;
    public :
        C_Script() {iGroup = 0;};
        C_Script(vector<S_ScriptData> n_data, string n_name);
        ~C_Script();

        void Save(FILE *fw, FILE *fwt);
        void Load(FILE *fr);
        XMLNode * CreateXMLTree();

        void             TranslateTiles(int dX, int dY);

        unsigned int     GetIGroup() {return iGroup;};
        string           GetName() {return name;};
        int              GetNData() {return data.size();};               // vrati pocet prvku v data
        string           GetDataName(int i) {return data[i].name;};   // vrati jmeno danych "dat"
        string           GetDataHelp(int i) {return data[i].help;};
        E_SCRIPTDATAKIND GetDataKind(int i) {return data[i].kind;};
        int              GetDataValue(int iData, int iValue) {return data[iData].value[iValue];};
        string           GetString(int iString) {return stringData[iString];};
        double           GetDouble(int iDouble) {return doubleData[iDouble];};
        int              GetInt(int iInt) {return intData[iInt];};
        int              GetNTileInTileLink(int iTileLink) {return tileData[iTileLink].size();};
        S_MAPPOS         GetTileInTileLink(int iTileLink, int iTile) {return tileData[iTileLink][iTile];};
        vector<S_MAPPOS> GetTileLink(int iTileLink) { return tileData[iTileLink];};


        void SetName(string n_name) {name = n_name;};
        void SetIGroup(int n_iGroup) {iGroup = n_iGroup;};
        void DecreaseIGroup() {iGroup--;};
        void SetString(string n_string, int iString) {stringData[iString] = n_string;};
        void SetDouble(double n_double, int iDouble) {doubleData[iDouble] = n_double;};
        void SetInt(int n_int, int iInt) {intData[iInt] = n_int;};

        void AddTileInTileLink(S_MAPPOS n_tile, int iTileLink) {tileData[iTileLink].push_back(n_tile);};
        void DeleteTileInTileLink(int iTileLink, int iTile) {tileData[iTileLink].erase(tileData[iTileLink].begin() + iTile, tileData[iTileLink].begin() + iTile + 1);};
        void DeleteAllTileInTileLink(int iTileLink) {tileData[iTileLink].clear();};

        void Show();        // ukaze vsechny tily ze vsech tilelinku v sobe
};

// dvere obsahujici vsechny skripty v danem levelu
class C_ScriptMenuDoor : public C_Door
{
    private :
        vector<unsigned int> iBScript;  // iBScript[0] obsahuje index do skriptu v button delTile[0]
        vector<unsigned int> iBGroup;   // tlacitko bGroup patri jakemu script group?
        vector<C_Button> bGroup;    // pro jednotlive skupiny tlacitka
        vector<C_Button> bName;
        vector<C_Button> bEdit;
        vector<C_Button> bShow;
        vector<C_Button> bErase;
    public :
        C_ScriptMenuDoor();
        C_ScriptMenuDoor(FILE * fr);
        C_ScriptMenuDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_ScriptMenuDoor();

        void FirstTime();
        void DoorActionIn();
        void DoorActionOut();
        void DoorActionDown(int button);
        void MakeButtons(); // na zacatku pri vytvoreni dveri vytvori buttony na zaklade poctu skriptu
        void MkButWithParent(unsigned int parent, int layer, int & top);
        void DrawContent();
};

#endif
