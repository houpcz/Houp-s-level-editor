#ifndef _TILEDOOR_H_
#define _TILEDOOR_H_

#include "..\Map.h"
#include "..\LevelEditor.h"

class C_TileDoor : public C_Door
{
    private :
        C_Map::E_LAYER mapLayer;
        int index;                              // ktere dvere jsme..

        bool marked[C_LevelEditor::MAX_WIDTH_PALETTE][C_LevelEditor::MAX_HEIGHT_PALETTE];
        bool leftClick;
        int click1X, click1Y;
        int click2X, click2Y;

        void UpdateTilePattern();
        C_TileDoor(const C_TileDoor &);
        C_TileDoor & operator=(const C_TileDoor &);
    public :
        C_TileDoor(FILE * fr);
        C_TileDoor(int n_x, int n_y, int n_width, int n_height, string n_title, int n_index, C_Map::E_LAYER n_mapLayer);
        ~C_TileDoor() {};
        void DoorActionSetTop();
        void DoorActionOut();
        void DoorActionIn();
        void DoorActionDown(int button);      // co se udela v okne po kliknuti do nej
        void DoorActionUp(int button);
        void DrawContent();
        void DoorEverTime();
        void FirstTime();
        void ClearMarkedTiles();
        void Save(FILE * fw);
        void Load(FILE * fr);
};

#endif
