#include "Map.h"
#include "LevelEditor.h"
#include "Script\ScriptSystem.h"
#include "Utils\TextureMan.h"
#include "Utils\FileMan.h"

bool operator==(const S_MAPPOS a, const S_MAPPOS b)
{
    if(a.x == b.x && a.y == b.y)
        return true;
    else
        return false;
}

C_Map::C_Map()
{
    lastBackup = -1;
    actualBackup = 0;
    nBackup = 0;
    map = NULL;
    for(int loop1 = 0; loop1 < MAX_BACKUP; loop1++)
    {
        backup[loop1] = NULL;
    }

    New("map01", 2, 20, 20);
}

C_Map::~C_Map()
{
    if(map != NULL)
        DeleteMap();
    if(backup[0] != NULL)
        DeleteBackup();
}

void C_Map::DeleteMap()
{
    unsigned int loop1, loop2;

    if(map != NULL)
    {
        for(loop2 = 0; loop2 < nLayer; loop2++)
        {
            for (loop1 = 0; loop1 < mapWidth; loop1++)
                delete [] map[loop2][loop1];

            delete [] map[loop2];
        }

        delete map;
    }
}

void C_Map::DeleteBackup()
{
    unsigned int loop1, loop2, loop3;

    for(loop3 = 0; loop3 < MAX_BACKUP; loop3++)
    {
        if(backup[loop3] != NULL)
        {
            for(loop2 = 0; loop2 < MAX_LAYER; loop2++)
            {
                for (loop1 = 0; loop1 < mapWidth; loop1++)
                    delete [] backup[loop3][loop2][loop1];

                delete [] backup[loop3][loop2];
            }
            delete backup[loop3];
        }
    }
}

void C_Map::New(string n_name, unsigned int n_nLayer, unsigned int n_mapWidth, unsigned int n_mapHeight)
{
    unsigned int loop1, loop2, loop3;

    DeleteMap();
    DeleteBackup();

    name = n_name;
    nLayer = n_nLayer;
    mapWidth = n_mapWidth;
    mapHeight = n_mapHeight;

    for(loop3 = 0; loop3 < MAX_BACKUP; loop3++)
    {
        backup[loop3] = new unsigned int ** [MAX_LAYER];
        for (loop2 = 0; loop2 < MAX_LAYER; loop2++)
        {
            backup[loop3][loop2] = new unsigned int * [mapWidth];
            for (loop1 = 0; loop1 < mapWidth; loop1++)
                backup[loop3][loop2][loop1] = new unsigned int[mapHeight];
        }
    }

    map = new unsigned int ** [nLayer];
	for (loop2 = 0; loop2 < nLayer; loop2++)
    {
        map[loop2] = new unsigned int * [mapWidth];
        for (loop1 = 0; loop1 < mapWidth; loop1++)
            map[loop2][loop1] = new unsigned int[mapHeight];
    }

    Reset();
}

void C_Map::Reset()
{
    unsigned int loop1, loop2, loop3;

    for(loop3 = 0; loop3 < nLayer; loop3++)
        for(loop1 = 0; loop1 < mapWidth; loop1++)
            for(loop2 = 0; loop2 < mapHeight; loop2++)
                map[loop3][loop1][loop2] = 0;
}

void C_Map::Draw(int left, int drawWidth, int top, int drawHeight, float scale, int layerMask)
{
    int loop1, loop2, loop3;
    C_LevelEditor * LE = C_LevelEditor::Inst();
    const GameSetup * GS = LE->GetGameSetup();

    unsigned int tileWidth = GS->GetTileWidth();
    unsigned int tileHeight = GS->GetTileHeight();

    int bit = 1;
    glColor3ub(255, 255, 255);
    for(loop3 = 0; loop3 < GetNLayer(); loop3++)
    {
        if(layerMask & bit)
        {
            for(loop1 = top; loop1 < top + drawHeight; loop1++)
            {
                for(loop2 = left; loop2 < left + drawWidth; loop2++)
                {
                    DrawTile(loop2, loop1, loop3, scale);

                    DrawMarkedTile(loop2, loop1, scale);
                    glTranslatef(tileWidth / scale, 0.0f, 0.0f);
                }
                glTranslatef(-((signed)drawWidth * tileWidth / scale), tileHeight / scale, 0.0f);
            }
            glTranslatef(0.0, -((signed)drawHeight * tileHeight / scale), 0.0f);
        }
        bit <<= 1;
    }
}

void C_Map::DrawTile(unsigned int id, unsigned int layer, double sizeDivide)
{
    C_LevelEditor * levelEditor = C_LevelEditor::Inst();
    const GameSetup * GS = levelEditor->GetGameSetup();

    unsigned int x, y, i;
    unsigned int tileID = id & 0x0FFFFFFF;

    i = tileID / (GS->GetTileInRow() * GS->GetTileInCol());
    tileID %= GS->GetTileInRow() * GS->GetTileInCol();
    x = tileID % GS->GetTileInRow();
    y = tileID / GS->GetTileInCol();

    int tileWidth = GS->GetTileWidth();
    int tileHeight =  GS->GetTileHeight();

    S_Texture * text = C_Map::GetTexture(layer, i);
    if(text == NULL) {
        //fprintf(stderr, "i %d x %d y %d id %d - %X tile %d", i, x, y, id, id, tileID);
        // TODO write on map that this tile doesnt exist
        return;
    }
    glBindTexture(GL_TEXTURE_2D, text->texID);

    float x1 = ((float) x  * tileWidth) / text->width;
    float x2 = ((x + 1.0f) * tileWidth) / text->width;
    float y1 = ((float) y  * tileHeight) / text->width;
    float y2 = ((y + 1.0f) * tileHeight) / text->width;

    bool mirroredX = id >> 31;
    bool mirroredY = (id >> 30) & 1;

    float vx1 = (mirroredX) ? tileWidth / sizeDivide : 0;
    float vx2 = (mirroredX) ? 0 : tileWidth / sizeDivide;
    float vy1 = (mirroredY) ? tileHeight / sizeDivide : 0;
    float vy2 = (mirroredY) ? 0 : tileHeight / sizeDivide;

    switch((id >> 28) & 0XFFFFFFF3)
    {
        case 0 : // 0°
            glBegin(GL_QUADS);
              glTexCoord2f(x1, y1);	glVertex2f( vx1,  vy1);
              glTexCoord2f(x2, y1);	glVertex2f( vx2,  vy1);
              glTexCoord2f(x2, y2);	glVertex2f( vx2, vy2);
              glTexCoord2f(x1, y2);	glVertex2f( vx1, vy2);
            glEnd();
            break;
        case 1 : // 90°
            glBegin(GL_QUADS);
              glTexCoord2f(x1, y2);	glVertex2f( vx1,  vy1);
              glTexCoord2f(x1, y1);	glVertex2f( vx2,  vy1);
              glTexCoord2f(x2, y1);	glVertex2f( vx2, vy2);
              glTexCoord2f(x2, y2);	glVertex2f( vx1, vy2);
            glEnd();
            break;
        case 2 : // 180°
            glBegin(GL_QUADS);
              glTexCoord2f(x2, y2);	glVertex2f( vx1,  vy1);
              glTexCoord2f(x1, y2);	glVertex2f( vx2,  vy1);
              glTexCoord2f(x1, y1);	glVertex2f( vx2, vy2);
              glTexCoord2f(x2, y1);	glVertex2f( vx1, vy2);
            glEnd();
            break;
        case 3 : // 270°
            glBegin(GL_QUADS);
              glTexCoord2f(x2, y1);	glVertex2f( vx1,  vy1);
              glTexCoord2f(x2, y2);	glVertex2f( vx2,  vy1);
              glTexCoord2f(x1, y2);	glVertex2f( vx2, vy2);
              glTexCoord2f(x1, y1);	glVertex2f( vx1, vy2);
            glEnd();
            break;
    }

}

void C_Map::DrawTile(int mapX, int mapY, unsigned int layer, double sizeDivide)
{
    if(mapX >= 0 && mapX < (signed) mapWidth &&
       mapY >= 0 && mapY < (signed) mapHeight )
    {
        DrawTile(map[layer][mapX][mapY], layer, sizeDivide);
    } else
        DrawTile(0, layer, sizeDivide);
}

void C_Map::DrawMarkedTile(unsigned int mapX, unsigned int mapY, double sizeDivide)
{
    unsigned int loop1;
    bool pipetted = false;
    S_MAPPOS tempPos;
    for(unsigned int loop1 = 0; loop1 <  C_LevelEditor::Inst()->GetNPipetteTile(); loop1++)
    {
        tempPos = C_LevelEditor::Inst()->GetPipetteTile(loop1);
        if(tempPos.x == (signed) mapX && tempPos.y == (signed) mapY)
        {
            pipetted = true;
            break;
        }
    }

    int tileWidth = C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
    int tileHeight = C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();

    if(pipetted)
    {
        glDisable(GL_TEXTURE_2D);

        glColor4ub(0, 0, 230, 75);
        glBegin(GL_QUADS);
            glVertex2f( 0,  0);
            glVertex2f( tileWidth / sizeDivide,  0);
            glVertex2f( tileWidth / sizeDivide, tileHeight / sizeDivide);
            glVertex2f( 0, tileHeight / sizeDivide);
        glEnd();

        glColor4ub(150, 0, 0, 200);
        glBegin(GL_LINES);
            glVertex2f( 0,  0);
            glVertex2f( tileWidth / sizeDivide - 1,  0);

            glVertex2f( tileWidth / sizeDivide - 1,  0);
            glVertex2f( tileWidth / sizeDivide, tileHeight / sizeDivide);

            glVertex2f( tileWidth / sizeDivide, tileHeight / sizeDivide);
            glVertex2f( 0, tileHeight / sizeDivide);

            glVertex2f( 0, tileHeight / sizeDivide);
            glVertex2f( 0,  0);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255, 255, 255, 255);
    }
    for(loop1 = 0; loop1 < C_LevelEditor::Inst()->GetNMarkedTile(); loop1++)
    {
        S_MAPPOS tempPos = C_LevelEditor::Inst()->GetMarkedTile(loop1);
        if(tempPos.x == (signed) mapX && tempPos.y == (signed) mapY)
        {
            glDisable(GL_TEXTURE_2D);

            glColor4ub(199, 21, 133, 75);
            glBegin(GL_QUADS);
                glVertex2f( 0,  0);
                glVertex2f( tileWidth / sizeDivide,  0);
                glVertex2f( tileWidth / sizeDivide, tileHeight / sizeDivide);
                glVertex2f( 0, tileHeight / sizeDivide);
            glEnd();

            glColor4ub(35, 35, 96, 200);
            glBegin(GL_LINE_STRIP);
                glVertex2f( 0,  0);
                glVertex2f( tileWidth / sizeDivide - 1,  0);
                glVertex2f( tileWidth / sizeDivide - 1, tileHeight / sizeDivide);
                glVertex2f( 0, tileHeight / sizeDivide);
                glVertex2f( 0,  0);
            glEnd();
            glEnable(GL_TEXTURE_2D);
            glColor4ub(255, 255, 255, 255);
        }
    }
}

void C_Map::Save(string src)
{
	FILE *fw;
	int mapx = mapWidth;
	int mapy = mapHeight;
    int loop1, loop2, loop3;

	fw = fopen(src.c_str(), "wb");

    fwrite(&nLayer,sizeof(int),1,fw);
	fwrite(&mapx,sizeof(int),1,fw);
	fwrite(&mapy,sizeof(int),1,fw);

	for(loop1 = 0; loop1 < (signed) nLayer; loop1++)
        for(loop2 = 0; loop2 < mapy; loop2++)
            for(loop3 = 0; loop3 < mapx; loop3++)
                fwrite(&map[loop1][loop3][loop2],sizeof(unsigned int),1,fw);

	fclose(fw);
}

void C_Map::Load(string n_name, string src)
{
	FILE *fr;
	int mapx;
	int mapy;
    int loop1, loop2, loop3;

    int tempLayer;
	if((fr = fopen(src.c_str(), "rb"))!=NULL)
	{
        fread(&tempLayer,sizeof(int),1,fr);
        fread(&mapx,sizeof(int),1,fr);
        fread(&mapy,sizeof(int),1,fr);

        New(n_name, tempLayer, mapx, mapy);

        for(loop1 = 0; loop1 < (signed) nLayer; loop1++)
            for(loop2 = 0; loop2 < mapy; loop2++)
                for(loop3 = 0; loop3 < mapx; loop3++)
                    fread(&map[loop1][loop3][loop2],sizeof(unsigned int),1,fr);

        fclose(fr);
	} else {
	    fprintf(stderr, "Mapu %s se nepodarilo nacist.", src.c_str());
        // Zde WARNING File src doesnt exist
	}
}

unsigned int *** addMatrix(unsigned int *** matrix3D, unsigned int ** matrix2D, unsigned int & depth)
{
    unsigned int loop1;
    unsigned int *** a = new unsigned int **[depth + 1];
    for(loop1 = 0; loop1 < depth; loop1++)
    {
        a[loop1] = matrix3D[loop1];
    }
    a[depth] = matrix2D;
    delete [] matrix3D;
    depth++;
    //cout << depth << endl;
    //cout << a[2][0][0] << endl;
    return a;
}

void C_Map::SetTile(int mapX, int mapY, int layer, int item)
{
    if(mapX >= 0 && mapX < (signed) mapWidth &&
       mapY >= 0 && mapY < (signed) mapHeight)
    {
        if(layer >= (signed) nLayer) {
            unsigned int ** newLayer = new unsigned int *[mapWidth];
            for(unsigned int loop1 = 0; loop1 < mapWidth; loop1++)
                newLayer[loop1] = new unsigned int[mapHeight];
            for(unsigned int loop1 = 0; loop1 < mapHeight; loop1++)
                for(unsigned int loop2 = 0; loop2 < mapWidth; loop2++)
                {
                    newLayer[loop2][loop1] = 0;
                }
            map = addMatrix(map, newLayer, nLayer);
        }
        //cout << layer << " " << nLayer << endl;
        map[layer][mapX][mapY] = item;
    }
}

int C_Map::GetTile(int mapX, int mapY, int layer)
{
    if(mapX >= 0 && mapX < (signed) mapWidth &&
       mapY >= 0 && mapY < (signed) mapHeight)
        return map[layer][mapX][mapY];

    return -1;
}

void C_Map::CopyToBackup(int index)
{
    unsigned int loop1, loop2, loop3;
    for(loop3 = 0; loop3 < nLayer; loop3++)
        for(loop1 = 0; loop1 < mapWidth; loop1++)
            for(loop2 = 0; loop2 < mapHeight; loop2++)
                backup[index][loop3][loop1][loop2] = map[loop3][loop1][loop2];
}

void C_Map::CopyFromBackup(int index)
{
    unsigned int loop1, loop2, loop3;
    for(loop3 = 0; loop3 < nLayer; loop3++)
        for(loop1 = 0; loop1 < mapWidth; loop1++)
            for(loop2 = 0; loop2 < mapHeight; loop2++)
                map[loop3][loop1][loop2] = backup[index][loop3][loop1][loop2];
}

void C_Map::MakeBackup()
{
    CopyToBackup(actualBackup);
    lastBackup = actualBackup;
    actualBackup++;
    if(actualBackup >= MAX_BACKUP)
      actualBackup = 0;
    nBackup++;
    if(nBackup >= MAX_BACKUP)
        nBackup = MAX_BACKUP;
}

void C_Map::BackOutBack()
{
    if(nBackup > 0)
    {
        if(actualBackup - 1 == lastBackup) // jeli-to prvni zpet, ulozit novou zalohu
        {
            MakeBackup();
            actualBackup--;
            nBackup--;
        }
        actualBackup--;
        if(actualBackup < 0)
            actualBackup = MAX_BACKUP - 1;
        CopyFromBackup(actualBackup);
        nBackup--;
    }
}

void C_Map::BackOutNext()
{
    if(actualBackup != lastBackup + 1 && actualBackup != lastBackup)
    {
        if(++actualBackup >= MAX_BACKUP)
            actualBackup = 0;

        CopyFromBackup(actualBackup);
        nBackup++;
    }
}

void C_Map::Resize(int n_width, int n_height, bool addLeft, bool addTop)
{
    if(n_width != (signed) mapWidth || n_height != (signed) mapHeight)
    {
        S_MAPPOS tempTile1;
        tempTile1.x = n_width - mapWidth;
        tempTile1.y = n_height - mapHeight;
        if(!addLeft)
            tempTile1.x = 0;
        if(!addTop)
            tempTile1.y = 0;

        C_LevelEditor::Inst()->PipetteAll();
        C_LevelEditor::Inst()->CopyToClipboard();
        New(name, nLayer, n_width, n_height);
        C_LevelEditor::Inst()->ClearPipetteTile();
        C_LevelEditor::Inst()->AddPipetteTile(tempTile1);
        C_LevelEditor::Inst()->CopyFromClipboard();

        mapWidth = n_width;
        mapHeight = n_height;
        for(unsigned int loop1 = 0; loop1 < C_ScriptSystem::Inst()->GetNScript(); loop1++)
        {
            C_ScriptSystem::Inst()->TranslateTiles(loop1, tempTile1.x, tempTile1.y);
        }
    }
}

S_Texture * C_Map::GetTexture(int layer, int index)
{
    //C_TextureMan::Inst()->GetTexture();
    char src_c[50];
    string src = C_LevelEditor::Inst()->GetGameSetup()->GetName();

    sprintf(src_c, "%s/Tiles/tileL%dI%d.tga", src.c_str(), layer, index);

    S_Texture * texture = C_TextureMan::Inst()->GetTexture(src_c);

    return texture;
}

void C_Map::GetBorders(int * left, int * right, int * top, int * bottom)
{
    int minX = mapWidth + 1, maxX = -1;
    int minY = mapHeight + 1, maxY = -1;

    for(int loop1 = 0; loop1 < mapWidth; loop1++)
        for(int loop2 = 0; loop2 < mapHeight; loop2++)
            for(int loop3 = 0; loop3 < nLayer; loop3++)
            {
                if(map[loop3][loop1][loop2] != 0)
                {
                    if(loop1 < minX) minX = loop1;
                    if(loop1 > maxX) maxX = loop1;
                    if(loop2 < minY) minY = loop2;
                    if(loop2 > maxY) maxY = loop2;
                }
            }
    *left = minX;
    *right = maxX;
    *top = maxY;
    *bottom = minY;
}

void C_Map::SetAsTilePattern()
{
    if(map == NULL)
        return;

    int minX, maxX;
    int minY, maxY;
    GetBorders(&minX, &maxX, &maxY, &minY);

    if(minX > maxX ||
       minY > maxY)
       return;

    int w = maxX - minX + 1;
    int h = maxY - minY + 1;

    int *** copy = TilePattern::CreateMatrix3D(w, h, nLayer);
    for(int loop1 = minX; loop1 <= maxX; loop1++)
    {
        for(int loop2 = minY; loop2 <= maxY; loop2++)
            for(int loop3 = 0; loop3 < nLayer; loop3++)
                copy[loop1 - minX][loop2 - minY][loop3] = (map[loop3][loop1][loop2] == 0) ? TilePattern::DONT_CLEAR : map[loop3][loop1][loop2];
    }

    C_LevelEditor::Inst()->SetPattern(copy, w, h, nLayer);
}

void C_Map::Debug()
{
    //fm->Print("impact12", 20, 20, "nBackup : %2d > actualBackup : %2d > lastBackup : %2d\n", nBackup, actualBackup, lastBackup);
    //fm->Print("impact12", 20, 40, "%d", backup[1][0][0].item);
}

