#ifndef _SAVEMAPDOOR_H_
#define _SAVEMAPDOOR_H_

#include "..\Form\Input.h"

class C_SaveMapDoor : public C_Door
{
    private :
        enum {
            N_LABEL = 1
        };
        C_Label * label[N_LABEL];
        C_Input * fMapName;
        C_Button * bOk;

        C_SaveMapDoor(const C_SaveMapDoor &);
        C_SaveMapDoor & operator=(const C_SaveMapDoor &);
    public :
        C_SaveMapDoor(FILE * fr);
        C_SaveMapDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_SaveMapDoor() {};
        void FirstTime();
        void DoorActionDown(int button);
};

#endif
