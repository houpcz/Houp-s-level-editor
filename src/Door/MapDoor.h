#ifndef _MAPDOOR_H_
#define _MAPDOOR_H_

#include "..\Map.h"

class C_MapDoor : public C_Door
{
    private :
        static const int MAX_SIZE_TABLE = 7;
        int clickX, clickY;
        bool leftClick; // bylo kliknuto?
        bool rightClick;
        int showLayer;
        int leftBorder;                     // hranice kreslenych tilu
        int topBorder;                      // hranice kreslenych tilu
        unsigned int drawMapWidth;
        unsigned int drawMapHeight;

        double sizeDivide;         // kolikrat se ma zmensit mapa
        double sizeTable[MAX_SIZE_TABLE];
        int iTable;             // kde jsme v sizeTable
        bool showBorder;

        C_MapDoor(const C_MapDoor &);
        C_MapDoor & operator=(const C_MapDoor &);
        void DrawFutureTiles(int x1, int y1, int x2, int y2);
    public :
        C_MapDoor(FILE * fr);
        C_MapDoor(int n_x, int n_y, int n_width, int n_height, C_Map * n_map);
        ~C_MapDoor() {};
        void DrawContent();
        void DoorActionDown(int button);
        void DoorActionUp(int button);
        void DoorEverTime();
        void DoorActionIn();
        void FirstTime();
        void Save(FILE * fw);
        void Load(FILE * fr);
        void Go(C_Map::E_DIRECT direct, int nStep);
        int GetLeftBorder() {return leftBorder;};
        int GetTopBorder() {return topBorder;};

        void SetMap(C_Map * n_map);
};


#endif
