#include "PatternDoor.h"
#include "DoorSystem.h"
#include "MapDoor.h"
#include "../LevelEditor.h"
#include "../utils/painter.h"

PatternDoor::PatternDoor(FILE * fr)
{
    title = "Pattern door";
    Load(fr);
}

PatternDoor::PatternDoor(int n_x, int n_y, int n_width, int n_height)
{
    doorKind = D_PATTERN;

    title = "Pattern door";
    x = n_x;
    y = n_y;
    width = n_width;
    height = n_height;
    mapDoor = NULL;
}

PatternDoor::~PatternDoor()
{
}

int PatternDoor::GetPatternsInRow()
{
    return width / (2 * C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth());
}

void PatternDoor::DrawContent()
{
    int minX, maxX;
    int minY, maxY;
    C_LevelEditor * LE = C_LevelEditor::Inst();
    const GameSetup * GS = LE->GetGameSetup();

    glDisable(GL_TEXTURE_2D);
    glColor3ub(GS->GetMapBGCR(),
               GS->GetMapBGCG(),
               GS->GetMapBGCB());
    glRectd(0, 0, width, height);
    glEnable(GL_TEXTURE_2D);

    C_Map * pattern;
    int patternSize = C_LevelEditor::Inst()->GetPatternSize();
    int patternHor = (patternSize > GetPatternsInRow()) ? GetPatternsInRow() : patternSize;
    int patternVer = 1 + patternSize / GetPatternsInRow();
    int id;

    for(int loop2 = 0; loop2 < patternVer; loop2++)
    {
        for(int loop1 = 0; loop1 < patternHor; loop1++)
        {
            id = loop1 + loop2 * patternHor;
            if(id >= patternSize)
                return;

            pattern = C_LevelEditor::Inst()->GetPattern(id);
            pattern->GetBorders(&minX, &maxX, &maxY, &minY);
            if(minX > maxX || minY > maxY)
                {
                    minX = maxX = minY = maxY = 0;
                }
            int w = maxX - minX + 1;
            int h = maxY - minY + 1;
            int scale = (w > h) ? w : h;

            pattern->Draw(minX - ((h > w) ? (h - w) / 2 : 0), scale, minY - ((w > h) ? (w - h) / 2 : 0), scale, (float) scale / 2.0f);
            Painter::Inst()->DrawRectangle(0, 0, -1 + 2 * LE->GetGameSetup()->GetTileWidth(), -1 + 2 * LE->GetGameSetup()->GetTileHeight(),
                                           (GetPatternID() == id) ? C_Color::whiteSnow : C_Color::blueDark);
            glTranslatef(2 * LE->GetGameSetup()->GetTileWidth(), 0, 0);
        }
        glTranslatef(-(patternHor) * 2 * LE->GetGameSetup()->GetTileWidth(), 2 * LE->GetGameSetup()->GetTileHeight(), 0);
    }
}

int PatternDoor::GetPatternID()
{
    if(interMouseX <= 0 && interMouseY <=0)
        return -1;

    int tempx = interMouseX / (2 * C_LevelEditor::Inst()->GetGameSetup()->GetTileWidth());
    int tempy = interMouseY / (2 * C_LevelEditor::Inst()->GetGameSetup()->GetTileHeight());

    return tempx + tempy * GetPatternsInRow();
}

void PatternDoor::DoorActionDown(int button)
{
    int id = GetPatternID();
    C_Map * pattern = C_LevelEditor::Inst()->GetPattern(id);
    if(button == C_Door::RIGHTBUTTON)
    {
        if(mapDoor != NULL)
        {
            mapDoor->Close();
        }
        mapDoor = new C_MapDoor((x > 256) ? x - 256 : x, y, 255, 255, pattern);
        mapDoor->SetDontSave(true);
        C_DoorSystem::Inst()->OpenNewDoor(mapDoor);
    } else {
        pattern->SetAsTilePattern();
    }

}
