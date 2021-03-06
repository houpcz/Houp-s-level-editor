#include "LevelEditor.h"
#include "engine.h"
#include "map.h"
#include "Utils/FileMan.h"
#include "Script/ScriptSystem.h"

using namespace std;


C_LevelEditor * C_LevelEditor::inst = NULL;

C_LevelEditor * C_LevelEditor::Inst()
{
    if (inst == NULL)
    {
        inst = new C_LevelEditor;
    }

    return inst;
}

C_LevelEditor::~C_LevelEditor()
{
    if(inst != NULL)
        delete inst;
    inst = NULL;
    delete map;
    delete gameSetup;
    map = NULL;
}

C_LevelEditor::C_LevelEditor()
{
    pipette.clear();
    markedTile.clear();
    copyTileWhat.clear();
    copyTileWhere.clear();

    gameSetup = new GameSetup();

    map = NULL;
}

void C_LevelEditor::PipetteAll()
{
    unsigned int loop1, loop2;
    S_MAPPOS tempPos;
    pipette.clear();

    for(loop2 = 0; loop2 < map->GetMapHeight(); loop2++)
        for(loop1 = 0; loop1 < map->GetMapWidth(); loop1++)
        {
            tempPos.x = loop1;
            tempPos.y = loop2;
            pipette.push_back(tempPos);
        }
}

void C_LevelEditor::MainLoop()
{
    vector<unsigned int> tempTile;

    if(App::Inst()->GetKey(SDLK_LCTRL, false) && App::Inst()->GetKey(SDLK_a))
    {
        PipetteAll();
    }

    if(App::Inst()->GetKey(SDLK_LCTRL, false) && App::Inst()->GetKey(SDLK_c))
    {
        CopyToClipboard();
    }

    if(App::Inst()->GetKey(SDLK_LCTRL, false) && App::Inst()->GetKey(SDLK_v))
    {
        CopyFromClipboard();
    }

    if(App::Inst()->GetKey(SDLK_z) && (App::Inst()->GetKey(SDLK_LCTRL, false) || App::Inst()->GetKey(SDLK_RCTRL, false)))
    {
        map->BackOutBack();
    }
    if(App::Inst()->GetKey(SDLK_x) && (App::Inst()->GetKey(SDLK_LCTRL, false) || App::Inst()->GetKey(SDLK_RCTRL, false)))
    {
        map->BackOutNext();
    }

    if(App::Inst()->GetKey(SDLK_r, true))
    {
        pattern.RotateRight();
        /*
        patternRotation = (++patternRotation) % 4;
        patternTransform = (patternTransform & 0XCFFFFFFF) | (patternRotation << 28);
        int temp = patternHeight;
        patternHeight = patternWidth;
        patternWidth = temp;*/
    }

    if(App::Inst()->GetKey(SDLK_w, true))
    {
        pattern.MirrorHorizontal();
        /*
        patternMirrorX = !patternMirrorX;
        patternTransform = (patternTransform & 0X7FFFFFFF) | (patternMirrorX << 31);
        */
    }

    if(App::Inst()->GetKey(SDLK_e, true))
    {
        pattern.MirrorVertical();
        /*
        patternMirrorY = !patternMirrorY;
        patternTransform = (patternTransform & 0XBFFFFFFF) | (patternMirrorY << 30);
        */
    }
}

void C_LevelEditor::CopyToClipboard()
{
    unsigned int loop1, loop2;
    vector<unsigned int> tempTile;

    for(loop1 = 0; loop1 < copyTileWhat.size(); loop1++)
    {
        copyTileWhat[loop1].clear();
    }
    copyTileWhat.clear();

    for(loop1 = 0; loop1 < pipette.size(); loop1++)
    {
        tempTile.clear();
        for(loop2 = 0; loop2 < map->GetNLayer(); loop2++)
            tempTile.push_back(map->GetTile(pipette[loop1].x, pipette[loop1].y, loop2));
        copyTileWhat.push_back(tempTile);
    }
    copyTileWhere = pipette;

    CreatePatternFromPipette();
    ClearPipetteTile();
}

void C_LevelEditor::CreatePatternFromPipette()
{
    int minX = 32000, minY = 32000, maxX = 0, maxY = 0;
    for(int loop1 = 0; loop1 < pipette.size(); loop1++)
    {
        if(pipette[loop1].x < minX)
            minX = pipette[loop1].x;
        if(pipette[loop1].x > maxX)
            maxX = pipette[loop1].x;

        if(pipette[loop1].y < minY)
            minY = pipette[loop1].y;
        if(pipette[loop1].y > maxY)
            maxY = pipette[loop1].y;
    }

    int *** matrix = TilePattern::CreateMatrix3D(maxX - minX + 1, maxY - minY + 1, map->GetNLayer());

    int tempID;
    for(int loop1 = 0; loop1 < pipette.size(); loop1++)
    {
        for(int loop2 = 0; loop2 < map->GetNLayer(); loop2++)
        {
            tempID = map->GetTile(pipette[loop1].x, pipette[loop1].y, loop2);
            matrix[pipette[loop1].x - minX][pipette[loop1].y - minY][loop2] = (tempID == 0) ? DONT_CLEAR : tempID;
        }
    }

    pattern.Set(matrix, maxX-minX + 1, maxY - minY + 1, map->GetNLayer());
}

void C_LevelEditor::CopyFromClipboard()
{
    unsigned int loop1, loop2;
    vector<unsigned int> tempTile;

    if(pipette.size() > 0)
    {
        map->MakeBackup();
        int x, y;
        for(loop1 = 0; loop1 < copyTileWhere.size(); loop1++)
        {
            x = pipette[0].x - copyTileWhere[0].x + copyTileWhere[loop1].x;
            y = pipette[0].y - copyTileWhere[0].y + copyTileWhere[loop1].y;
            for(loop2 = 0; loop2 < map->GetNLayer(); loop2++)
                if(copyTileWhat[loop1][loop2] != NO_TILE)       // nemazu
                    map->SetTile(x, y, loop2, copyTileWhat[loop1][loop2]);
        }
    }
}

void C_LevelEditor::AddPipetteTile(int x, int y)
{
    S_MAPPOS tempPos;
    tempPos.x = x;
    tempPos.y = y;

    pipette.push_back(tempPos);
}

/*
void C_LevelEditor::SetPalette(bool (*n_palette)[MAX_WIDTH_PALETTE][MAX_HEIGHT_PALETTE], int mapLayer, int id)
{
    palette = n_palette;
    paletteLayer = mapLayer;
    paletteIndex = id;

    patternX = MAX_WIDTH_PALETTE;
    patternY = MAX_HEIGHT_PALETTE;
    int maxX = -1;
    int maxY = -1;
    for(int loop1 = 0; loop1 < MAX_HEIGHT_PALETTE; loop1++)
        for(int loop2 = 0; loop2 < MAX_WIDTH_PALETTE; loop2++)
        {
            if((*palette)[loop2][loop1])
            {
                if(loop2 < patternX)
                    patternX = loop2;
                if(loop1 < patternY)
                    patternY = loop1;
                if(loop2 > maxX)
                    maxX = loop2;
                if(loop1 > maxY)
                    maxY = loop1;
            }
        }
    patternWidth = (maxX - patternX < 0) ? 0 : maxX - patternX + 1;
    patternHeight = (maxY - patternY < 0) ? 0 : maxY - patternY + 1;

    //fprintf(stderr, "%d %d %d %d \n", patternX, patternY, patternHeight, patternWidth);
}*/

/*
int C_LevelEditor::GetPaletteTileID(int startX, int startY)
{
    if(palette == NULL)
        return NO_TILE;

    bool finalMirrorX = patternMirrorX;
    bool finalMirrorY = patternMirrorY;

    int temp;
    switch(patternRotation)
    {
        case 1 :
            finalMirrorY = !finalMirrorY;
            temp = startX;
            startX = startY;
            startY = temp;// + (patternWidth - startX % patternWidth);
            break;

        case 2 :
            finalMirrorX = !finalMirrorX;
            finalMirrorY = !finalMirrorY;
            break;

        case 3 :
            finalMirrorX = !finalMirrorX;
            temp = startX;
            startX = startY;
            startY = temp;
            break;
        default : break;
    }

    int idX = patternX + ((finalMirrorX) ? (patternWidth - 1) - (startX % patternWidth) : (startX % patternWidth));
    int idY = patternY + ((finalMirrorY) ? (patternHeight - 1) - (startY % patternHeight) : (startY % patternHeight));

    if((*palette)[idX][idY])
        return (idX + idY * tileInRow + paletteIndex * tileInRow * tileInCol) | patternTransform;
    else
        return DONT_CLEAR;
}*/

void C_LevelEditor::MarkedTile(vector<S_MAPPOS> n_markedTile)
{
    markedTile = n_markedTile;
}

void C_LevelEditor::UnmarkedTile(vector<S_MAPPOS> n_markedTile)
{
    if(markedTile == n_markedTile)  // odznaci pouze pri uplne shode
        markedTile.clear();
}

bool C_LevelEditor::IsCorrectTile(S_MAPPOS tempTile)
{
    if(tempTile.x >= 0 && tempTile.y >= 0)
        return true;
    else
        return false;
}

void C_LevelEditor::Save()
{
    FILE *fw;

    if((fw = fopen("init.txt", "w")) != NULL)
    {
      fprintf(fw, "Screen resolution   > %d x %d\n", App::Inst()->GetScreenWidth(), App::Inst()->GetScreenHeight());
    } else {
      fprintf(stderr, "Init soubor neexistuje.");
    }
    fclose(fw);

    SaveActiveProject(); // TODO should be optional
}

void C_LevelEditor::Load()
{
    int screenWidth, screenHeight;
    FILE *fr;
    if((fr = fopen("init.txt", "r")) != NULL)
    {
        if(fscanf(fr, "Screen resolution   > %d x %d\n", &screenWidth, &screenHeight) == 2)
        {
            App::Inst()->SetScreenWidth(screenWidth);
            App::Inst()->SetScreenHeight(screenHeight);
        }
    } else {
        fprintf(stderr, "Init soubor neexistuje.");
    }

    fclose(fr);
}

void C_LevelEditor::LoadProject(string name)
{
    SaveActiveProject();
    // TODO save opened project
    if(gameSetup->Load(name))
    {
        if(map != NULL)
            map->New("no name", 2, 20, 20);

        C_ScriptSystem::Inst()->LoadScriptKind();
    }
}

void C_LevelEditor::SaveActiveProject()
{
    gameSetup->Save();
    C_ScriptSystem::Inst()->SaveScriptKind(gameSetup->GetName());
    SaveMap(); // TODO should be optional
}

void C_LevelEditor::NewProject(string name)
{
    SaveActiveProject();
    // TODO save opened project
    if(FileMan::Inst()->MakeDirectory(name))
    {
        if(map != NULL)
            map->New("no name", 2, 20, 20);

        FileMan::Inst()->MakeDirectory(name + "/Maps");
        FileMan::Inst()->MakeDirectory(name + "/Tiles");
        FileMan::Inst()->MakeDirectory(name + "/Patterns");

        delete gameSetup;
        gameSetup = new GameSetup();
        SetGameName(name);
        C_ScriptSystem::Inst()->CreateNewProject(name);
        SaveActiveProject();
    }
}

string C_LevelEditor::GetMapSrc(string name)
{
    string src;

    src = gameSetup->GetName() + "/Maps/";
    src += name;
    src += ".lev";

    return src;
}

void C_LevelEditor::SetGameName(string name)
{
    rename(gameSetup->GetName().c_str(), name.c_str());
    gameSetup->SetName(name);
}

void C_LevelEditor::SaveMap()
{
    if(map != NULL)
    {
        map->Save(GetMapSrc(map->GetName()));
        C_ScriptSystem::Inst()->SaveScript();
    }
}

void C_LevelEditor::LoadMap(string name)
{
    // TODO Save opened map
    if(map == NULL)
        map = new C_Map();

    map->Load(name, GetMapSrc(name));
}

void C_LevelEditor::NewMap(string name, int nLayers, int width, int height)
{
    // TODO Save opened map
    if(map == NULL)
        map = new C_Map();
    map->New(name, nLayers, width, height);
}
