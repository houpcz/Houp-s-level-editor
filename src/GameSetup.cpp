#include "GameSetup.h"
#include "LevelEditor.h"
#include <cstddef>
#include <stdio.h>
#include "Utils/FileMan.h"

GameSetup::GameSetup()
{
    tileWidth = 32;
    tileHeight = 32;
    tileInCol = 8;
    tileInRow = 8;
    mapBGColorRed = 255;
    mapBGColorGreen = 255;
    mapBGColorBlue = 255;
    name = "";
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

void GameSetup::Save()
{
    char path[256];
    if(name.size() > 0)
    {
        sprintf(path, "%s/%s.hle", name.c_str(), name.c_str());
        FILE *fw = fopen(path, "wb");
        FileMan * fm = FileMan::Inst();
        fm->SaveInteger(mapBGColorRed, fw);
        fm->SaveInteger(mapBGColorGreen, fw);
        fm->SaveInteger(mapBGColorBlue, fw);
        fm->SaveInteger(tileWidth, fw);
        fm->SaveInteger(tileHeight, fw);
        fm->SaveInteger(tileInRow, fw);
        fm->SaveInteger(tileInCol, fw);
        fclose(fw);
    }


    for(int loop1 = 0; loop1 < pattern.size(); loop1++)
    {
        sprintf(path, "%s/Patterns/pattern%d.lev", name.c_str(), loop1);
        pattern[loop1]->Save(path);
    }
}

bool GameSetup::Load(string projectName)
{
    char path[256];
    sprintf(path, "%s/%s.hle", projectName.c_str(), projectName.c_str());
    FileMan * fm = FileMan::Inst();
    FILE *fr = fopen(path, "rb");
    if(fr != NULL)
    {
        fm->LoadInteger(mapBGColorRed, fr);
        fm->LoadInteger(mapBGColorGreen, fr);
        fm->LoadInteger(mapBGColorBlue, fr);
        fm->LoadInteger(tileWidth, fr);
        fm->LoadInteger(tileHeight, fr);
        fm->LoadInteger(tileInRow, fr);
        fm->LoadInteger(tileInCol, fr);
        name = projectName;
        fclose(fr);

        pattern.clear();
        sprintf(path, "%s/Patterns/", projectName.c_str());
        string src = path;
        vector<string> patternFiles = fm->GetFileList(src + "*.lev", true);
        C_Map * tempMap;
        for(int loop1 = 0; loop1 < patternFiles.size(); loop1++)
        {
            tempMap = new C_Map();
            tempMap->Load("Pattern", path + patternFiles[loop1]);
            pattern.push_back(tempMap);
        }

        return true;
    }
    return false;
}

// TODO : Save itselves and load
