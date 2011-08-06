#include <string>

#include "Door.h"
#include "..\Map.h"
#include "..\LevelEditor.h"
#include "..\Utils\FileMan.h"
#include "..\Utils\Painter.h"
#include "TileDoor.h"
#include "DoorSystem.h"

void C_TileDoor::FirstTime()
{
    closeable = true;
    resizeableVertical = true;
    resizeableHorizontal = true;
    minimalizeable = true;
    isTitle = true;
    scrollable = true;
    leftClick = false;

    maxHeight = 256;
    maxWidth = 256;
}

C_TileDoor::C_TileDoor(FILE * fr)
{
    title = "Tiles";
    ClearMarkedTiles();
    Load(fr);
}

void C_TileDoor::ClearMarkedTiles()
{
    for(int loop1 = 0; loop1 < C_LevelEditor::MAX_HEIGHT_PALETTE; loop1++)
        for(int loop2 = 0; loop2 < C_LevelEditor::MAX_WIDTH_PALETTE; loop2++)
            marked[loop2][loop1] = false;
}

C_TileDoor::C_TileDoor(int n_x, int n_y, int n_width, int n_height, string n_title, int n_index, C_Map::E_Layer n_mapLayer)
{
    doorKind = D_TILES;

    title = n_title;
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
    index = n_index;
    mapLayer = n_mapLayer;

    click1X = click2X = click1Y = click2Y = -1;

    ClearMarkedTiles();
}

void C_TileDoor::UpdateTilePattern()
{
    int minX = C_LevelEditor::MAX_WIDTH_PALETTE;
    int minY = C_LevelEditor::MAX_HEIGHT_PALETTE;
    int maxX = -1;
    int maxY = -1;
    for(int loop1 = 0; loop1 < C_LevelEditor::MAX_HEIGHT_PALETTE; loop1++)
        for(int loop2 = 0; loop2 < C_LevelEditor::MAX_WIDTH_PALETTE; loop2++)
        {
            if(marked[loop1][loop2])
            {
                if(loop1 < minX) minX = loop1;
                if(loop2 < minY) minY = loop2;
                if(loop1 > maxX) maxX = loop1;
                if(loop2 > maxY) maxY = loop2;
            }
        }

    int w = maxX - minX + 1;
    int h = maxY - minY + 1;
    int z = (int) mapLayer + 1;
    if(w > 0 && h > 0)
    {
        int *** matrix = TilePattern::CreateMatrix3D(w, h, z);
        for(int loop1 = minX; loop1 <= maxX; loop1++)
            for(int loop2 = minY; loop2 <= maxY; loop2++)
            {
                for(int loop3 = 0; loop3 < (int) mapLayer ; loop3++)
                    matrix[loop1 - minX][loop2 - minY][loop3] = -1;

                matrix[loop1 - minX][loop2 - minY][(int) mapLayer] =
                    (marked[loop1][loop2]) ? index * C_LevelEditor::Inst()->GetGameSetup()->GetTileInCol() * C_LevelEditor::Inst()->GetGameSetup()->GetTileInRow() + loop2 * C_LevelEditor::Inst()->GetGameSetup()->GetTileInCol() + loop1 : -1;
            }
        C_LevelEditor::Inst()->SetPattern(matrix, w, h, z);
    }
}

void C_TileDoor::DoorActionSetTop()
{
    UpdateTilePattern();
}

void C_TileDoor::DrawContent()
{
    C_LevelEditor * LE = C_LevelEditor::Inst();

    glDisable(GL_TEXTURE_2D);
    glColor3ub(LE->GetGameSetup()->GetMapBGCR(),
               LE->GetGameSetup()->GetMapBGCG(),
               LE->GetGameSetup()->GetMapBGCB());
    glRectd(0, 0, width, height);
    glEnable(GL_TEXTURE_2D);

    maxScrollY = 256 - height + 1;

    S_Texture * text = C_Map::GetTexture((int) mapLayer, index);
    if(text == NULL)
    {
        glColor3ubv(C_Color::redDark);
        char anchor = FontMan::Inst()->GetAnchor();
        FontMan::Inst()->SetAnchor(FontMan::ALIGN_TOP);
        FontMan::Inst()->Print("impact12", 6, 6, "Texture tileL%dI%d doesnt exist.", (int) mapLayer, index);
        FontMan::Inst()->SetAnchor(anchor);
        index = 0;
        return;
    }

    Painter::Inst()->DrawTexture(text);

    C_LevelEditor * levelEditor = C_LevelEditor::Inst();
    int tileW = levelEditor->GetGameSetup()->GetTileWidth();
    int tileH = levelEditor->GetGameSetup()->GetTileHeight();

    glDisable(GL_TEXTURE_2D);
    glColor4ub(200, 200, 255, 90);
    for(int loop1 = 0; loop1 < levelEditor->GetGameSetup()->GetTileInCol(); loop1++)
        for(int loop2 = 0; loop2 < levelEditor->GetGameSetup()->GetTileInRow(); loop2++)
        {
            if(marked[loop2][loop1])
            {
                glBegin(GL_QUADS);
                    glVertex2i(loop2 * tileW, loop1 * tileH);
                    glVertex2i(loop2 * tileW + tileW, loop1 * tileH);
                    glVertex2i(loop2 * tileW + tileW, loop1 * tileH + tileH);
                    glVertex2i(loop2 * tileW, loop1 * tileH + tileH);
                glEnd();
            }
        }

    ////////////////
    // Ohraniceni oznacenych tilu //
    ////////////////////////////////

    glColor4ub(255,  80, 207, 255);
    glLineWidth(1.2);
    glBegin(GL_LINES);
    for(int loop1 = 0; loop1 < levelEditor->GetGameSetup()->GetTileInCol(); loop1++)
        for(int loop2 = 0; loop2 < levelEditor->GetGameSetup()->GetTileInRow(); loop2++)
        {
            // svisle cary
            if((loop2 == 0 && marked[loop2][loop1]) ||
               (loop2 > 0 && (
               (marked[loop2][loop1] && !marked[loop2 - 1][loop1]) ||
               (!marked[loop2][loop1] &&  marked[loop2 - 1][loop1]))))
            {
                glVertex2i(loop2 * tileW, loop1 * tileH);
                glVertex2i(loop2 * tileW, loop1 * tileH + tileH);
            }
            // vodorovne cary
            if((loop1 == 0 && marked[loop2][loop1]) || (loop1 > 0 && (
              (marked[loop2][loop1] && !marked[loop2][loop1 - 1]) ||
              (!marked[loop2][loop1] &&  marked[loop2][loop1 - 1]))))
            {
                glVertex2i(loop2 * tileW, loop1 * tileH);
                glVertex2i(loop2 * tileW + tileW, loop1 * tileH);
            }
        }
    glEnd();

    glEnable(GL_TEXTURE_2D);

    if(leftClick)
    {
        int temp1X, temp2X, temp1Y, temp2Y;
        int temp;
        temp1X = (interMouseX / C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth()) * C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
        temp1Y = (interMouseY / C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight()) * C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();
        temp2X = click1X * C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
        temp2Y = click1Y * C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();
        if(temp1X > temp2X)
        {
            temp = temp1X;
            temp1X = temp2X;
            temp2X = temp;
        }
        if(temp1Y > temp2Y)
        {
            temp = temp1Y;
            temp1Y = temp2Y;
            temp2Y = temp;
        }
        temp2X += C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
        temp2Y += C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();

        glDisable(GL_TEXTURE_2D);
        glColor4ub(255, 255, 255, 255);
        glBegin(GL_LINE_STRIP);
            glVertex2d(temp1X, temp1Y);
            glVertex2d(temp2X, temp1Y);
            glVertex2d(temp2X, temp2Y);
            glVertex2d(temp1X, temp2Y);
            glVertex2d(temp1X, temp1Y);
        glEnd();

        glColor4ub(255, 255, 255, 50);
        glBegin(GL_QUADS);
            glVertex2d(temp1X, temp1Y);
            glVertex2d(temp2X, temp1Y);
            glVertex2d(temp2X, temp2Y);
            glVertex2d(temp1X, temp2Y);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
}

void C_TileDoor::DoorActionOut()
{
    leftClick = false;
}

void C_TileDoor::DoorActionIn()
{
    C_LevelEditor * levelEditor = C_LevelEditor::Inst();
    const GameSetup * GS = levelEditor->GetGameSetup();
    int tempx = interMouseX / GS->GetTileWidth();
    int tempy = interMouseY / GS->GetTileHeight();

    int id = tempx + tempy * GS->GetTileInRow() + index * GS->GetTileInRow() * GS->GetTileInCol();
    string str = "[";
    str += ToString(id) + "]";
    C_DoorSystem::Inst()->OpenNewHelp(str);
}

void C_TileDoor::DoorActionDown(int button)
{
    int tempx = interMouseX / C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
    int tempy = interMouseY / C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();

    if(button == LEFTBUTTON)
    {
        click1X = tempx;
        click1Y = tempy;
        leftClick = true;
    }
}

void C_TileDoor::DoorActionUp(int button)
{
    int tempx = interMouseX / C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth();
    int tempy = interMouseY / C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight();
    int temp;

    if(button == LEFTBUTTON && leftClick)
    {
        leftClick = false;

        click2X = tempx;
        click2Y = tempy;

        if(click1X > click2X)
        {
            temp = click1X;
            click1X = click2X;
            click2X = temp;
        }
        if(click1Y > click2Y)
        {
            temp = click1Y;
            click1Y = click2Y;
            click2Y = temp;
        }

        if(!(App::Inst()->GetKey(SDLK_LCTRL, false) || App::Inst()->GetKey(SDLK_RCTRL, false)))
        {
            ClearMarkedTiles();
        }
        for(int loop1 = click1Y; loop1 <= click2Y; loop1++)
            for(int loop2 = click1X; loop2 <= click2X; loop2++)
            {
                if(click2X - click1X == 0 &&
                   click2Y - click1Y == 0)
                   marked[loop2][loop1] = !marked[loop2][loop1];
                else
                   marked[loop2][loop1] = true;
            }

        UpdateTilePattern();
    }
}

void C_TileDoor::DoorEverTime()
{
    if(GetIsTopDoor())
    {
        if(App::Inst()->GetKey(SDLK_F1))
        {
            mapLayer = C_Map::LAYER_0;
            index = 0;
        }
        if(App::Inst()->GetKey(SDLK_F2))
        {
            mapLayer = C_Map::LAYER_1;
            index = 0;
        }
        if(App::Inst()->GetKey(SDLK_F3))
        {
            mapLayer = C_Map::LAYER_2;
            index = 0;
        }
        if(App::Inst()->GetKey(SDLK_1) || App::Inst()->GetKey(SDLK_KP1))
            index = 0;
        if(App::Inst()->GetKey(SDLK_2) || App::Inst()->GetKey(SDLK_KP2))
            index = 1;
        if(App::Inst()->GetKey(SDLK_3) || App::Inst()->GetKey(SDLK_KP3))
            index = 2;
        if(App::Inst()->GetKey(SDLK_4) || App::Inst()->GetKey(SDLK_KP4))
            index = 3;
        if(App::Inst()->GetKey(SDLK_5) || App::Inst()->GetKey(SDLK_KP5))
            index = 4;
        if(App::Inst()->GetKey(SDLK_6) || App::Inst()->GetKey(SDLK_KP6))
            index = 5;
        if(App::Inst()->GetKey(SDLK_7) || App::Inst()->GetKey(SDLK_KP7))
            index = 6;
        if(App::Inst()->GetKey(SDLK_8) || App::Inst()->GetKey(SDLK_KP8))
            index = 7;
        if(App::Inst()->GetKey(SDLK_9) || App::Inst()->GetKey(SDLK_KP9))
            index = 8;
        if(App::Inst()->GetKey(SDLK_0) || App::Inst()->GetKey(SDLK_KP0))
            index = 9;

        char tempChar[20];
        sprintf(tempChar, "L - %d ID - %d", mapLayer + 1, index + 1);
        title = tempChar;
    }
}

void C_TileDoor::Load(FILE * fr)
{
    C_Door::Load(fr);
    FileMan::Inst()->LoadInteger(index, fr);
    int temp;
    FileMan::Inst()->LoadInteger(temp, fr);
    mapLayer = (C_Map::E_Layer) temp;

    FileMan::Inst()->LoadInteger(click1X, fr);
    FileMan::Inst()->LoadInteger(click1Y, fr);
    FileMan::Inst()->LoadInteger(click2X, fr);
    FileMan::Inst()->LoadInteger(click2Y, fr);
}

void C_TileDoor::Save(FILE * fw)
{
    C_Door::Save(fw);
    FileMan::Inst()->SaveInteger(index, fw);
    FileMan::Inst()->SaveInteger(mapLayer, fw);
    FileMan::Inst()->SaveInteger(click1X, fw);
    FileMan::Inst()->SaveInteger(click1Y, fw);
    FileMan::Inst()->SaveInteger(click2X, fw);
    FileMan::Inst()->SaveInteger(click2Y, fw);
}
