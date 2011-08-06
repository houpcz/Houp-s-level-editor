#ifndef _MENUDOOR_H_
#define _MENUDOOR_H_

class C_MenuDoor : public C_Door            // vyber oken na otevreni
{
    private :
        enum {
            N_BUTTON = 9
        };
        C_Button * mButton[N_BUTTON];

        C_MenuDoor(const C_MenuDoor &);
        C_MenuDoor & operator=(const C_MenuDoor &);
    public :
        C_MenuDoor(FILE * fr);
        C_MenuDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_MenuDoor();
        void FirstTime();
        void DoorActionDown(int button);
};

#endif
