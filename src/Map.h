#ifndef __MAP_H__
#define __MAP_H__

#include "Utils\TextureLoader.h"
#include "TilePattern.h"
#include <string>

using namespace std;

typedef struct S_MapPos         // pozice v ramci mapy
{
    int x;
    int y;
} S_MAPPOS;

bool operator==(const S_MAPPOS a, const S_MAPPOS b);

class C_Map
{
    public :
        typedef enum E_Layer {
            LAYER_0,
            LAYER_1,
            LAYER_2,
            LAYER_3,
            LAYER_4,
            N_LAYER
        } E_LAYER;

        enum {
          MAX_TEXTURE = 21,
          MAX_LAYER = 9,

          MAX_BACKUP = 30         // pocet zaloh
        };

        typedef enum E_Direct {
            UP,
            RIGHT,
            DOWN,
            LEFT
        } E_DIRECT;

    private :
        string name;
        string src;     // cesta k souboru

        unsigned int nLayer;
        unsigned int mapWidth;
        unsigned int mapHeight;

        unsigned int ***map;
        unsigned int ***backup[MAX_BACKUP]; // zaloha mapy

        int lastBackup;                                     // index posledni zalohy
        int actualBackup;                                   // kam se ma dat nova zaloha
        int nBackup;                                        // pocet aktualnich zaloh

        void DeleteMap();       // uvolni pamet ze stare mapy
        void DeleteBackup();    // uvolni pamet ze zaloh

        C_Map(const C_Map &);
        C_Map & operator=(const C_Map &);
    public :
        C_Map();
        ~C_Map();
        void DrawMarkedTile(unsigned int mapX, unsigned int mapY, double sizeDivide);          // je-li dane policko zvyraznene, tak ho zvyrazni
        void DrawTile(int mapX, int mapY, unsigned int layer, double sizeDivide);
        void DrawTile(unsigned id, unsigned int layer, double sizeDivide);

        void GetBorders(int * left, int * right, int * top, int * bottom);
        int GetTile(int mapX, int mapY, int layer);
        void SetTile(int mapX, int mapY, int layer, int item);
        void SetName(string n_name) {name = n_name;};
        void Draw(int left, int drawWidth, int top, int drawHeight, float scale, int layerMask = 0xFFFFFFFF);
        void New(string n_name, unsigned int n_nLayer, unsigned int n_mapWidth, unsigned int n_mapHeight);
        void Resize(int n_width, int n_height, bool addLeft, bool addTop);
        void Reset();
        void Load(string n_name, string src);
        void Save(string src);
        //pro zalohovani >
        void CopyToBackup(int index);
        void CopyFromBackup(int index);

        void MakeBackup();
        void BackOutBack();
        void BackOutNext();

        void Go(E_DIRECT direct, int nStep);

        unsigned int GetNLayer() {return nLayer;}
        unsigned int GetMapWidth() {return mapWidth;}
        unsigned int GetMapHeight() {return mapHeight;}
        string GetName() {return name;};
        void SetAsTilePattern();

        void Debug();
        static S_Texture * GetTexture(int layer, int index);
};

#endif
