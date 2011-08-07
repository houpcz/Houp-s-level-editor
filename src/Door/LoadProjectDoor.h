#ifndef _LOADPROJECTDOOR_H_
#define _LOADPROJECTDOOR_H_

#include "..\Form\Input.h"

class C_LoadProjectDoor : public C_Door
{
    private :
        enum {
            N_LABEL = 1
        };
        C_Label * label[N_LABEL];
        C_Input * fProjectName;
        C_Button * bOk;
        C_Button * bNext;
        C_Button * bBack;

        int fileNameIndex;
        vector<string> fileName;

        C_LoadProjectDoor(const C_LoadProjectDoor &);
        C_LoadProjectDoor & operator=(const C_LoadProjectDoor &);
    public :
        C_LoadProjectDoor(FILE * fr);
        C_LoadProjectDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_LoadProjectDoor();
        void FirstTime();
        void DoorActionDown(int button);
};

#endif
