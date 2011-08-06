//----------------------------------------------------------------------------------------
/**
 * \file       LevelEditor.h
 * \author     Lukas Beran
 * \date       2010/07/26
 * \brief      Consists C_LevelEditor class which works with tiles in map
 *
*/
//----------------------------------------------------------------------------------------

#ifndef __LEVELEDITOR_H__
#define __LEVELEDITOR_H__

#include "Map.h"
#include "TilePattern.h"
#include "GameSetup.h"
#include <map>
#include <vector>

using namespace std;

/// Singleton class which works with tiles in map
/**

*/

class C_LevelEditor
{
    public :
        enum {
            DONT_CLEAR = -1,            ///< Don't repaint tiles in map by DONT_CLEAR tiles in palette
            NO_TILE = 0,                ///< In each layer NO_TILE is value for blank tile
            MAX_WIDTH_PALETTE = 16,     ///< Game palettes must be narrower then this value
            MAX_HEIGHT_PALETTE = 16     ///< Game palettes must be shorter then this value
        };
        enum {
            ROT_0,
            ROT_90,
            ROT_180,
            ROT_270
        };
    private :
        vector<S_MAPPOS> pipette;           ///< Pipetted tiles in map (eg. you can copy them, delete them)
        vector<S_MAPPOS> markedTile;        ///< Marked tiles in map from scripts which holds tile positions (eg. path, checkpoints)
        vector<vector<unsigned int> > copyTileWhat;   ///< vector of tile ids in each layer
        vector<S_MAPPOS> copyTileWhere;               ///< vector of tile coords from where are tiles copies

        C_Map * map;              ///< Only one map can be loaded at time
        GameSetup * gameSetup;      ///< level editor setup specific for one game

        TilePattern pattern;

        static C_LevelEditor * inst;    ///< Singleton instance
        C_LevelEditor();
        ~C_LevelEditor();
        void CreatePatternFromPipette();
        void CreatePattern(int width, int height, int layer);
        void DisposePattern();
    public :
        static C_LevelEditor * Inst();
        void MainLoop();
        void Save();
        void Load();
        void CopyToClipboard();         // kopiruje oznacenou cast mapy do vnitrni schranky
        void CopyFromClipboard();       // kopiruje ze schranky na oznacene miso
        void ClearPipetteTile() {pipette.clear();};
        void PipetteAll();
        void AddPipetteTile(int x, int y);
        void AddPipetteTile(S_MAPPOS n_pipette) {AddPipetteTile(n_pipette.x, n_pipette.y);};
        unsigned int GetNPipetteTile() {return pipette.size();};
        S_MAPPOS GetPipetteTile(int iPipette) {return pipette[iPipette];};

        unsigned int GetNMarkedTile() {return markedTile.size();};
        S_MAPPOS GetMarkedTile(int iMarkedTile) {return markedTile[iMarkedTile];};

        const GameSetup * GetGameSetup() { return gameSetup; };
        void SetTileWidth(int n_tileWidth) { gameSetup->SetTileWidth(n_tileWidth);};
        void SetTileHeight(int n_tileHeight) { gameSetup->SetTileHeight(n_tileHeight);};
        void SetMapBGCR(int red) {gameSetup->SetMapBGCR(red);};
        void SetMapBGCG(int green) {gameSetup->SetMapBGCG(green);};
        void SetMapBGCB(int blue) {gameSetup->SetMapBGCB(blue);};
        void SetGameName(string name) {gameSetup->SetName(name);};
        C_Map * GetPattern(int id) {return gameSetup->GetPattern(id);};
        int GetPatternSize() { return gameSetup->GetPatternCount();};

        int GetPatternTileID(int startX, int startY, int layer) {return pattern.GetTileID(startX, startY, layer); };
        int GetPatternWidth() {return pattern.GetWidth();};
        int GetPatternHeight() {return pattern.GetHeight();};
        int GetPatternLayer() { return pattern.GetLayer();};
        void SetPattern(int *** matrix, int width, int height, int layer) { pattern.Set(matrix, width, height, layer); };
        void MarkedTile(vector<S_MAPPOS> n_markedTile);
        void UnmarkedTile(vector<S_MAPPOS> n_markedTile);

        bool IsCorrectTile(S_MAPPOS tempTile);

        C_Map * GetMap() {return map;};
        string GetMapSrc(string name);
        void LoadMap(string name);
        void NewMap(string name, int nLayers, int width, int height);

        void BackOutBack() {map->BackOutBack();};
        void BackOutNext() {map->BackOutNext();};
};

#endif
