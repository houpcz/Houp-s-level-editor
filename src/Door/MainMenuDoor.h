#ifndef _MAINMENUDOOR_H_
#define _MAINMENUDOOR_H_

#include "..\Utils\TextureLoader.h"

class C_MainMenuDoor : public C_Door
{
    private :
        enum {
            SETUP_EDITOR,
            NEW_MAP,
            OPEN_MAP,
            SETUP_MAP,
            SAVE_MAP,
            UNDO_MAP,
            REDO_MAP
        };
        C_MainMenuDoor(const C_MainMenuDoor &);
        C_MainMenuDoor & operator=(const C_MainMenuDoor &);
        void DoorActionIn();
    public :
        C_MainMenuDoor(FILE * fr);
        C_MainMenuDoor(int n_x, int n_y, int n_width, int n_height, string n_title);
        ~C_MainMenuDoor() {};
        void DoorEverTime();
        void FirstTime();
        void DrawContent();
        void DoorActionDown(int button);
};

#endif
