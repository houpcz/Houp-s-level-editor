#ifndef _GAMESETUP_H_
#define _GAMESETUP_H_

#include "map.h"
#include <vector>
using namespace std;

class GameSetup
{
    private :
        int mapBGColorRed;      ///< Red source of color in MapDoor
        int mapBGColorGreen;    ///< Green piece of color in background color in MapDoor
        int mapBGColorBlue;     ///< Blue piece of color in background color in MapDoor
        int tileWidth;         ///< Width of tiles (eg. 32)
        int tileHeight;        ///< Height of tiles (eg. 32)
        int tileInRow;          ///< Number of tiles in row in palette
        int tileInCol;          ///< Number of tiles in column in palette
        vector<C_Map *> pattern;
    public :
        GameSetup();
        ~GameSetup();

        int GetTileWidth() const {return tileWidth;};
        int GetTileHeight() const {return tileHeight;};
        int GetTileInRow() const {return tileInRow;};
        int GetTileInCol() const {return tileInCol;};
        int GetMapBGCR() const {return mapBGColorRed;};
        int GetMapBGCG() const {return mapBGColorGreen;};
        int GetMapBGCB() const {return mapBGColorBlue;};
        void SetTileWidth(int n_tileWidth);
        void SetTileHeight(int n_tileHeight);
        void SetMapBGCR(int red) {mapBGColorRed = red;};
        void SetMapBGCG(int green) {mapBGColorGreen = green;};
        void SetMapBGCB(int blue) {mapBGColorBlue = blue;};

        int GetPatternCount() const { return pattern.size();};
        C_Map * GetPattern(int id);
};

#endif
