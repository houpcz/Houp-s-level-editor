#ifndef _SCRIPTGROUPDOOR_H_
#define _SCRIPTGROUPDOOR_H_

class C_ScriptGroupDoor : public C_Door
{
    private :
        C_Button bNew;      // nova skupina
        vector<C_Input> inputGroup;        // editovatelne nazvy skupin skriptu
        vector<C_InputInteger> inputParent;
        C_ButtonGroup bDel;          // na smazani dane skupiny, ti co prijdou o skupinu, ujme se o ne rodic skupiny
        C_ButtonGroup bActiveGroup;
    public :
        C_ScriptGroupDoor(FILE * fr) {Load(fr);};
        C_ScriptGroupDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_ScriptGroupDoor() {};
        void DrawContent();
        void DoorEverTime();
        void DoorActionDown(int button);
        void FirstTime();
        void MakeContent();
};

#endif
