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
            NEW_MAP,
            SETUP_MAP,
            SAVE_MAP,
            UNDO_MAP,
            REDO_MAP,
            OPEN_PROJECT = 0,
            NEW_PROJECT,
            SETUP_PROJECT
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
