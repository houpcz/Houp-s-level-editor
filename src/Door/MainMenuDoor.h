#ifndef _MAINMENUDOOR_H_
#define _MAINMENUDOOR_H_

#include "..\Utils\TextureLoader.h"

class C_MainMenuDoor : public C_Door
{
    private :
        enum {
            ROW_PROJECT = 0,
            ROW_MAP = 1,
            OPEN_MAP = 0,
            NEW_MAP = 1,
            SETUP_MAP = 2,
            SAVE_MAP = 3,
            UNDO_MAP = 4,
            REDO_MAP = 5,
            OPEN_PROJECT = 0,
            NEW_PROJECT = 1,
            SETUP_PROJECT = 2
        };
        C_MainMenuDoor(const C_MainMenuDoor &);
        C_MainMenuDoor & operator=(const C_MainMenuDoor &);
        void DoorActionIn();
        bool GetIsButtonActive(int x, int y);
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
