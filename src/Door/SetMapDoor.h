#ifndef _SETMAPDOOR_H_
#define _SETMAPDOOR_H_

#include "..\Form\Input.h"

class C_SetMapDoor : public C_Door            // formular k nastaveni parametru noveho levelu
{
    private :
        C_Label * label;

        C_Input * fMapName;
        C_InputInteger * fMapWidth;
        C_InputInteger * fMapHeight;
        C_Button * bLeft, * bTop;           // v pripade zvetseni levelu, ma se zvetsovat nahore/vlevo?
        C_Button * bOk;

        C_SetMapDoor(const C_SetMapDoor &);
        C_SetMapDoor & operator=(const C_SetMapDoor &);
    public :
        C_SetMapDoor(FILE * fr);
        C_SetMapDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_SetMapDoor() {};
        void FirstTime();
        void DoorActionDown(int button);
        void DoorEverTime();
};

#endif
