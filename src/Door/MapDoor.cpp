#include <string>

#include "Door.h"
#include "DoorSystem.h"
#include "MapDoor.h"
#include "..\Map.h"
#include "..\Leveleditor.h"
#include "..\Utils\FileMan.h"

C_MapDoor::C_MapDoor(FILE * fr)
{
    Load(fr);

    title = "Unknown map";
    map = NULL;
}

void C_MapDoor::FirstTime()
{
    closeable = true;
    resizeableVertical = true;
    resizeableHorizontal = true;
    minimalizeable = true;
    isTitle = true;
    scrollable = false;

    leftClick = false;
    rightClick = false;

    leftBorder = 0;
    topBorder = 0;

    unsigned int loop1;
    showLayer = 0xFFFFFFFF;

    sizeTable[0] = 0.25;
    sizeTable[1] = 0.33;
    sizeTable[2] = 0.5;
    sizeTable[3] = 1.0;
    sizeTable[4] = 2.0;
    sizeTable[5] = 4.0;
    sizeTable[6] = 8.0;
}

void C_MapDoor::SetMap(C_Map * n_map)
{
    map = n_map;
}

C_MapDoor::C_MapDoor(int n_x, int n_y, int n_width, int n_height, C_Map * n_map)
{
    doorKind = D_MAP;

    title = "Unknown map";
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;

    iTable = 3;
    sizeDivide = 1.0;

    SetMap(n_map);
}

void C_MapDoor::DoorActionIn()
{
    int temp1X = interMouseX / (int) (C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth() / sizeDivide);
    int temp1Y = interMouseY / (int) (C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight() / sizeDivide);

    string str = "[";
    str += ToString(temp1X + leftBorder) + "," + ToString(temp1Y + topBorder) + "]";
    C_DoorSystem::Inst()->OpenNewHelp(str);
    showBorder = true;
}

void C_MapDoor::DrawFutureTiles(int x1, int y1, int x2, int y2)
{
    C_LevelEditor * LE = C_LevelEditor::Inst();
    const GameSetup * GS = LE->GetGameSetup();
    unsigned int tileWidth = GS->GetTileWidth();
    unsigned int tileHeight = GS->GetTileHeight();
    /////////////////////////////////////////
    // Oznaceni tilu, ktere se nakresli -> //
    /////////////////////////////////////////

    if(!rightClick)
    {
        glPushMatrix();
        glColor4ub(255, 255, 255, 170);
        glTranslatef((x2 * tileWidth) / sizeDivide, (y2 * tileHeight) / sizeDivide, 0.0f);

        int tileID;
        unsigned int patternHeight = C_LevelEditor::Inst()->GetPatternHeight();
        unsigned int patternWidth = C_LevelEditor::Inst()->GetPatternWidth();
        int patternLayer = C_LevelEditor::Inst()->GetPatternLayer();

        if(patternHeight > 0 && patternWidth > 0)
        {
            int widthT = ((x1 - x2) / patternWidth) * patternWidth + patternWidth;
            int heightT = ((y1 - y2) / patternHeight) * patternHeight + patternHeight;

            for(int loop1 = 0; loop1 < heightT; loop1++)
            {
                for(int loop2 = 0; loop2 < widthT; loop2++)
                {
                    for(int loop3 = 0; loop3 < patternLayer; loop3++)
                    {
                        tileID = C_LevelEditor::Inst()->GetPatternTileID(loop2, loop1, loop3);
                        if(tileID != C_LevelEditor::DONT_CLEAR)
                            C_Map::DrawTile(tileID , loop3, sizeDivide);
                    }
                    glTranslatef(tileWidth / sizeDivide, 0.0f, 0.0f);
                }
                glTranslatef(-(widthT * tileWidth / sizeDivide), tileHeight / sizeDivide, 0.0f);
            }
        }
        glColor4ub(255, 255, 255, 255);
        glPopMatrix();

        /////////////////////////////////////////
        // <- Oznaceni tilu, ktere se nakresli //
        /////////////////////////////////////////
    }
}

void C_MapDoor::DrawContent()
{
    C_LevelEditor * LE = C_LevelEditor::Inst();
    const GameSetup * GS = LE->GetGameSetup();

    if(map == NULL)
    {
        map = LE->GetMap();
        return;
    }

    ///////////////////////////
    // Zacatek kresleni tilu //
    ///////////////////////////

    unsigned int loop1, loop2, loop3;
    unsigned int tileWidth = GS->GetTileWidth();
    unsigned int tileHeight = GS->GetTileHeight();

    glDisable(GL_TEXTURE_2D);
    glColor3ub(GS->GetMapBGCR(),
               GS->GetMapBGCG(),
               GS->GetMapBGCB());
    glRectd(0, 0, width, height);
    glEnable(GL_TEXTURE_2D);

    map->Draw(leftBorder, drawMapWidth,
              topBorder, drawMapHeight,
              sizeDivide, showLayer);

    if(IsMouseInDoor() && showBorder)
    {
        showBorder = false;
        unsigned int temp1X = interMouseX / (int) (tileWidth / sizeDivide);
        unsigned int temp1Y = interMouseY / (int) (tileHeight / sizeDivide);

        unsigned int temp2X;
        unsigned int temp2Y;

        if(leftClick || rightClick)
        {
            temp2X = clickX;
            temp2Y = clickY;
        }   else {
            temp2X = temp1X;
            temp2Y = temp1Y;
        }

        int temp;

        if(temp1X < temp2X)
        {
            temp = temp2X;
            temp2X = temp1X;
            temp1X = temp;
        }

        if(temp1Y < temp2Y)
        {
            temp = temp2Y;
            temp2Y = temp1Y;
            temp1Y = temp;
        }

        DrawFutureTiles(temp1X, temp1Y, temp2X, temp2Y);

        glDisable(GL_TEXTURE_2D);
        glColor4ub(255, 255, 255, 255);
        glBegin(GL_LINE_STRIP);
            glVertex2d(temp2X * tileWidth / sizeDivide                                 , temp2Y * tileHeight / sizeDivide);
            glVertex2d(temp2X * tileWidth / sizeDivide                                 , temp1Y * tileHeight / sizeDivide + tileHeight / sizeDivide);
            glVertex2d(temp1X * tileWidth / sizeDivide + tileWidth / sizeDivide, temp1Y * tileHeight / sizeDivide + tileHeight / sizeDivide);
            glVertex2d(temp1X * tileWidth / sizeDivide + tileWidth / sizeDivide, temp2Y * tileHeight / sizeDivide);
            glVertex2d(temp2X * tileWidth / sizeDivide                                 , temp2Y * tileHeight / sizeDivide);
        glEnd();

        if(rightClick)
            glColor4ub(255, 20, 20, 40);
        else
            glColor4ub(20, 20, 255, 40);

        glBegin(GL_QUADS);
            glVertex2d(temp2X * tileWidth / sizeDivide                                 , temp2Y * tileHeight / sizeDivide);
            glVertex2d(temp2X * tileWidth / sizeDivide                                 , temp1Y * tileHeight / sizeDivide + tileHeight / sizeDivide);
            glVertex2d(temp1X * tileWidth / sizeDivide + tileWidth / sizeDivide, temp1Y * tileHeight / sizeDivide + tileHeight / sizeDivide);
            glVertex2d(temp1X * tileWidth / sizeDivide + tileWidth / sizeDivide, temp2Y * tileHeight / sizeDivide);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
}

void C_MapDoor::DoorEverTime()
{
    unsigned int loop1, loop2;
    S_MapPos tempTile;
    int tileWidth = C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
    int tileHeight = C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();

    /////////////////////////
    // NASTAV DRAWMAPWIDTH //
    /////////////////////////

    if(map != NULL) {
        drawMapWidth = width / (unsigned int) (tileWidth / sizeDivide);
        if(width % (unsigned int) (tileWidth / sizeDivide) >= 3)
            drawMapWidth++;

        if(drawMapWidth > map->GetMapWidth())
            drawMapWidth = map->GetMapWidth();

        drawMapHeight = height / (int) (tileHeight / sizeDivide);
        if(height % (int) (tileHeight / sizeDivide) >= 3)
            drawMapHeight++;

        if(drawMapHeight > map->GetMapHeight())
            drawMapHeight = map->GetMapHeight();

        if(width > (signed int) (drawMapWidth *  (tileWidth / sizeDivide)))
            width = drawMapWidth * (int) (tileWidth / sizeDivide);
        if(height > (signed int) (drawMapHeight * (tileHeight / sizeDivide)))
            height = drawMapHeight * (int) (tileHeight / sizeDivide);

        ////////////////////
        //
        ///////////////

        title = map->GetName() + " - " + ToString((int) (100 / sizeDivide)) + "%";

        if(App::Inst()->GetKey(SDLK_DELETE))
        {
            map->MakeBackup();
            for(loop1 = 0; loop1 < C_LevelEditor::Inst()->GetNPipetteTile(); loop1++)
            {
                tempTile = C_LevelEditor::Inst()->GetPipetteTile(loop1);
                for(loop2 = 0; loop2 < map->GetNLayer(); loop2++)
                    map->SetTile(tempTile.x,tempTile.y,loop2,0);
            }
        }
    }

    if(GetIsTopDoor())
    {
        if(App::Inst()->GetKey(SDLK_PAGEUP))
        {
            iTable++;
            if(iTable > MAX_SIZE_TABLE - 1)
                iTable = MAX_SIZE_TABLE - 1;
            else{
                if(width > 100 && height > 100)
                {
                    width /= 2;
                    height /= 2;
                }
            }
            sizeDivide = sizeTable[iTable];
        }
        if(App::Inst()->GetKey(SDLK_PAGEDOWN))
        {
            iTable--;
            if(iTable < 0)
                iTable = 0;
            else{
                if(width < 273 && height < 273)
                {
                    width *= 2;
                    height *= 2;
                }
            }
            sizeDivide = sizeTable[iTable];
        }

        if(App::Inst()->GetKey(SDLK_UP))         Go(C_Map::UP, 1);
        if(App::Inst()->GetKey(SDLK_DOWN))       Go(C_Map::DOWN, 1);
        if(App::Inst()->GetKey(SDLK_LEFT))       Go(C_Map::LEFT, 1);
        if(App::Inst()->GetKey(SDLK_RIGHT))      Go(C_Map::RIGHT, 1);

        for(int loop1 = 0; loop1 < 10; loop1++)
        {
            if(App::Inst()->GetKey(SDLK_F1 + loop1))
                showLayer ^= 1 << loop1;
        }
    }
}

void C_MapDoor::DoorActionDown(int button)
{
    int tileWidth = C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
    int tileHeight = C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();

    clickX = interMouseX / (int) (tileWidth / sizeDivide);
    clickY = interMouseY / (int) (tileHeight / sizeDivide);

    if(button == LEFTBUTTON)
    {
        leftClick = true;
        //C_LevelEditor::Inst()->ClearPipetteTile();
    } else
    if(button == RIGHTBUTTON)
    {
        rightClick = true;
    }
}

void C_MapDoor::DoorActionUp(int button)
{
    if(map == NULL)
        return;

    int temp;
    int loop1, loop2;
    int tileWidth = C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
    int tileHeight = C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();

    int tempX = interMouseX / (int)(tileWidth / sizeDivide) + leftBorder;
    int tempY = interMouseY / (int)(tileHeight / sizeDivide) + topBorder;
    clickX += leftBorder;
    clickY += topBorder;

    if(button == LEFTBUTTON && leftClick)
    {
        //pred zmenou ulozit//
        map->MakeBackup();

        if(1)
        {
            if(tempX < clickX)
            {
                temp = clickX;
                clickX = tempX;
                tempX = temp;
            }

            if(tempY < clickY)
            {
                temp = clickY;
                clickY = tempY;
                tempY = temp;
            }

            int temp;
            int tileID;
            int patternHeight = C_LevelEditor::Inst()->GetPatternHeight();
            int patternWidth = C_LevelEditor::Inst()->GetPatternWidth();
            int patternLayer = C_LevelEditor::Inst()->GetPatternLayer();
            if(patternHeight > 0 && patternWidth > 0)
            {
                temp = (tempY - clickY + 1) / patternHeight;
                tempY = ((tempY - clickY + 1) == temp * patternHeight) ? tempY : (temp + 1) * patternHeight - 1 + clickY;
                temp = (tempX - clickX + 1) / patternWidth;
                tempX = ((tempX - clickX + 1) == temp * patternWidth) ? tempX : (temp + 1) * patternWidth - 1 + clickX;

                for(loop1 = clickY; loop1 <= tempY; loop1++)
                    for(loop2 = clickX; loop2 <= tempX; loop2++)
                        for(int loop3 = 0; loop3 < patternLayer; loop3++)
                        {
                            tileID = C_LevelEditor::Inst()->GetPatternTileID(loop2 - clickX, loop1 - clickY, loop3);
                            if(tileID != C_LevelEditor::DONT_CLEAR)
                                map->SetTile(loop2, loop1, loop3, tileID);

                            //map.SetTile(loop2, loop1, levelEditor.GetLeftButton(), levelEditor.GetILeftButtonTile(levelEditor.GetLeftButton()));
                        }
            }
        }
        leftClick = false;
    } else

    if(button == RIGHTBUTTON && rightClick)
    {
        if(1)
        {
            if(tempX < clickX)
            {
                temp = clickX;
                clickX = tempX;
                tempX = temp;
            }

            if(tempY < clickY)
            {
                temp = clickY;
                clickY = tempY;
                tempY = temp;
            }

            bool clear = false;
            if(clickY == tempY && clickX == tempX && C_LevelEditor::Inst()->GetNPipetteTile() == 1)
            {
                S_MAPPOS tempPos = C_LevelEditor::Inst()->GetPipetteTile(0);
                if(tempPos.x == clickX && tempPos.y == clickY)
                    clear = true;
            }

            if(!App::Inst()->GetKey(SDLK_LCTRL, false))
                C_LevelEditor::Inst()->ClearPipetteTile();
            if(!clear)
                for(loop1 = clickY; loop1 <= tempY; loop1++)
                    for(loop2 = clickX; loop2 <= tempX; loop2++)
                    {
                        C_LevelEditor::Inst()->AddPipetteTile(loop2, loop1);
                    }
        }
        rightClick = false;
    }
}

void C_MapDoor::Go(C_Map::E_DIRECT direct, int nStep)
{
    if(map == NULL)
        return;

    switch(direct)
    {
        case C_Map::UP :
          topBorder -= nStep;
          if(topBorder < 0)
            topBorder = 0;
          break;
        case C_Map::DOWN :
          topBorder += nStep;
          if(topBorder > (signed) (map->GetMapHeight() - drawMapHeight))
            topBorder = map->GetMapHeight() - drawMapHeight;
          break;
        case C_Map::LEFT :
          leftBorder -= nStep;
          if(leftBorder < 0)
            leftBorder = 0;
          break;
        case C_Map::RIGHT :
          leftBorder += nStep;
          if(leftBorder > (signed) (map->GetMapWidth() - drawMapWidth))
            leftBorder = map->GetMapWidth() - drawMapWidth;
          break;
    }
}

void C_MapDoor::Load(FILE * fr)
{
    C_Door::Load(fr);
    FileMan::Inst()->LoadInteger(iTable, fr);
    FileMan::Inst()->LoadDouble(sizeDivide, fr);
}

void C_MapDoor::Save(FILE * fw)
{
    C_Door::Save(fw);
    FileMan::Inst()->SaveInteger(iTable, fw);
    FileMan::Inst()->SaveDouble(sizeDivide, fw);
}

