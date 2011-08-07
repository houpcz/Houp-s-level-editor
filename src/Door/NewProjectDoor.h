#ifndef _NEWPROJECTDOOR_H_
#define _NEWPROJECTDOOR_H_

#include "..\Form\Input.h"

class C_NewProjectDoor : public C_Door            // formular k nastaveni parametru noveho levelu
{
    private :
        enum {
            N_LABEL = 1
        };
        C_Label * label[N_LABEL];
        C_Input * fProjectName;
        C_Button * bOk;

        C_NewProjectDoor(const C_NewProjectDoor &);
        C_NewProjectDoor & operator=(const C_NewProjectDoor &);
    public :
        C_NewProjectDoor(FILE * fr);
        C_NewProjectDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_NewProjectDoor();
        void FirstTime();
        void DoorActionDown(int button);
};


#endif

