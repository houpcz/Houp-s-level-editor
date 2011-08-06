#include "GameSetup.h"
#include "LevelEditor.h"
#include <cstddef>

GameSetup::GameSetup()
{
}

GameSetup::~GameSetup()
{
    //pattern[0]->Save();
    for(int loop1 = 0; loop1 < pattern.size(); loop1++)
    {
        delete pattern[loop1];
    }
}

void GameSetup::SetTileWidth(int n_tileWidth) {
    tileWidth = n_tileWidth;
    tileInRow = 256 / tileWidth;
}

void GameSetup::SetTileHeight(int n_tileHeight) {
    tileHeight = n_tileHeight;
    tileInCol = 256 / tileHeight;
}

C_Map * GameSetup::GetPattern(int id)
{
    if(id >= 0 && id < pattern.size())
        return pattern[id];
    else
    {
        C_Map * one = new C_Map();
        one->New("Pattern", 3, 8, 8);
        pattern.push_back(one);
        return one;
    }
}

// TODO : Save itselves and load
