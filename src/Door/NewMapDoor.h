#ifndef _NEWMAPDOOR_H_
#define _NEWMAPDOOR_H_

#include "..\Form\Input.h"

class C_NewMapDoor : public C_Door            // formular k nastaveni parametru noveho levelu
{
    private :
        enum {
            N_LABEL = 3
        };
        C_Label * label[N_LABEL];
        C_Input * fMapName;
        C_InputInteger * fMapWidth;
        C_InputInteger * fMapHeight;
        C_Button * bOk;

        C_NewMapDoor(const C_NewMapDoor &);
        C_NewMapDoor & operator=(const C_NewMapDoor &);
    public :
        C_NewMapDoor(FILE * fr);
        C_NewMapDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_NewMapDoor();
        void FirstTime();
        void DoorActionDown(int button);
};


#endif
