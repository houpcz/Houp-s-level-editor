#ifndef _PATTERNDOOR_H_
#define _PATTERNDOOR_H_

#include "Door.h"
#include "MapDoor.h"

class PatternDoor : public C_Door
{
    private :
        C_MapDoor * mapDoor;

        int GetPatternsInRow();
        int GetPatternID();
    public :
        ~PatternDoor();
        PatternDoor(FILE * fr);
        PatternDoor(int n_x, int n_y, int n_width, int n_height);
        void DoorActionDown(int button);
        void DrawContent();

};


#endif
