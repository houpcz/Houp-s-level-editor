#ifndef _SETEDITORDOOR_H_
#define _SETEDITORDOOR_H_

#include "..\Form\Input.h"

class C_SetEditorDoor : public C_Door            // formular k nastaveni parametru noveho levelu
{
    private :
        C_Label * label;
        C_Input * fName;
        C_InputInteger * fMapColorRed;
        C_InputInteger * fMapColorGreen;
        C_InputInteger * fMapColorBlue;
        C_InputInteger * fTileWidth;
        C_InputInteger * fTileHeight;
        C_InputInteger * fMapWidth;
        C_InputInteger * fMapHeight;
        C_Button * bOk;

        C_SetEditorDoor(const C_SetEditorDoor &);
        C_SetEditorDoor & operator=(const C_SetEditorDoor &);
    public :
        C_SetEditorDoor(FILE * fr);
        C_SetEditorDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_SetEditorDoor() {};
        void FirstTime();
        void DoorActionDown(int button);
        void DoorEverTime();
};

#endif

