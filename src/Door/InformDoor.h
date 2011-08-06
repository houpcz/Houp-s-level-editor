#ifndef _INFORMDOOR_H_
#define _INFORMDOOR_H_

#include "Door.h"

class C_InformDoor : public C_Door
{
        private :
            string informString;

            C_InformDoor(const C_InformDoor &);
            C_InformDoor & operator=(const C_InformDoor &);
        public :
            C_InformDoor();
            C_InformDoor(int n_x, int n_y, string n_informString);
            void Set(int n_x, int n_y, string n_informString);
            ~C_InformDoor() {};

            void FirstTime();
            void DrawContent();
};

#endif
