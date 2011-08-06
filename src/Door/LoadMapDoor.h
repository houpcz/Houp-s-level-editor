#ifndef _LOADMAPDOOR_H_
#define _LOADMAPDOOR_H_

#include "..\Form\Input.h"

class C_LoadMapDoor : public C_Door
{
    private :
        enum {
            N_LABEL = 1
        };
        C_Label * label[N_LABEL];
        C_Input * fMapName;
        C_Button * bOk;
        C_Button * bNext;
        C_Button * bBack;

        int fileNameIndex;
        vector<string> fileName;

        C_LoadMapDoor(const C_LoadMapDoor &);
        C_LoadMapDoor & operator=(const C_LoadMapDoor &);
    public :
        C_LoadMapDoor(FILE * fr);
        C_LoadMapDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_LoadMapDoor();
        void FirstTime();
        void DoorActionDown(int button);
};

#endif
