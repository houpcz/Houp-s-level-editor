
#include <cstddef>
#include <stdio.h>wre
#include "TilePattern.h"
#include "LevelEditor.h"

TilePattern::TilePattern()
{
    pattern = NULL;
    width = 0;
    height = 0;
    layer = 0;
}

void TilePattern::Set(int *** matrix, int n_width, int n_height, int n_layer)
{
    Clear();
    pattern = matrix;
    width = n_width;
    height = n_height;
    layer = n_layer;
}

void TilePattern::Clear()
{
    if(pattern != NULL)
    {
        for(int loop1 = 0; loop1 < width; loop1++)
        {
            for(int loop2 = 0; loop2 < height; loop2++)
            {
                delete[] pattern[loop1][loop2];
            }
            delete[] pattern[loop1];
        }
        delete[] pattern;
        width = 0;
        height = 0;
        layer = 0;
        pattern = NULL;
    }
}

TilePattern::~TilePattern()
{
    Clear();
}

int TilePattern::GetTileID(int startX, int startY, int layer)
{
    return pattern[startX % width][startY % height][layer];
}

int *** TilePattern::CreateMatrix3D(int x, int y, int z)
{
    int *** matrix = new int**[x];
    for(int loop1 = 0; loop1 < x; loop1++)
    {
        matrix[loop1] = new int*[y];

        for(int loop2 = 0; loop2 < y; loop2++)
        {
            matrix[loop1][loop2] = new int[z];

            for(int loop3 = 0; loop3 < z; loop3++)
            {
                matrix[loop1][loop2][loop3] = C_LevelEditor::DONT_CLEAR;
            }
        }
    }
    return matrix;
}

void TilePattern::RotateOneTileRight(int *** matrix, int x, int y, int z)
{
    bool mirrorX = matrix[x][y][z] & 0x80000000;
    bool mirrorY = matrix[x][y][z] & 0x40000000;
    int rotationCount = 1;
    if(!mirrorX &&  mirrorY ||
        mirrorX && !mirrorY)
        rotationCount = 3;

    //for(int loop1 = 0; loop1 < rotationCount; loop1++)
    {
        int rotation = matrix[x][y][z] & 0x30000000;
        rotation >>= 28;
        rotation = (rotation + rotationCount) & 3;
        rotation <<= 28;
        matrix[x][y][z] &= 0xCFFFFFFF;
        matrix[x][y][z] |= rotation;
    }
}

void TilePattern::RotateRight()
{
    int *** matrix = CreateMatrix3D(height, width, layer);
    int rotation;

    for(int loop1 = 0; loop1 < width; loop1++)
        for(int loop2 = 0; loop2 < height; loop2++)
            for(int loop3 = 0; loop3 < layer; loop3++)
            {
                matrix[loop2][loop1][loop3] = pattern[loop1][height - loop2 - 1][loop3];

                if(matrix[loop2][loop1][loop3] != C_LevelEditor::DONT_CLEAR)
                {
                    RotateOneTileRight(matrix, loop2, loop1, loop3);
                }
            }

    Set(matrix, height, width, layer);
}



void TilePattern::MirrorHorizontal()
{
    //fprintf(stderr, "Mirrorh start\n");
    MirrorHorizontalGlobal();
    MirrorHorizontalLocal();
    //fprintf(stderr, "Mirrorh end\n");
}

void TilePattern::MirrorHorizontalGlobal()
{
    int temp;
    for(int loop1 = 0; loop1 < width / 2; loop1++)
        for(int loop2 = 0; loop2 < height; loop2++)
            for(int loop3 = 0; loop3 < layer; loop3++)
            {
                temp = pattern[loop1][loop2][loop3];
                pattern[loop1][loop2][loop3] = pattern[width - loop1 - 1][loop2][loop3];
                pattern[width - loop1 - 1][loop2][loop3] = temp;
            }
}
void TilePattern::MirrorHorizontalLocal()
{
    for(int loop1 = 0; loop1 < width; loop1++)
        for(int loop2 = 0; loop2 < height; loop2++)
            for(int loop3 = 0; loop3 < layer; loop3++)
            {
                if(pattern[loop1][loop2][loop3] != C_LevelEditor::DONT_CLEAR)
                    pattern[loop1][loop2][loop3] ^= 0x80000000;
            }
}

void TilePattern::MirrorVertical()
{
    int temp;
    for(int loop1 = 0; loop1 < width; loop1++)
        for(int loop2 = 0; loop2 < height / 2; loop2++)
            for(int loop3 = 0; loop3 < layer; loop3++)
            {
                temp = pattern[loop1][loop2][loop3];
                pattern[loop1][loop2][loop3] = pattern[loop1][height - loop2 - 1][loop3];
                pattern[loop1][height - loop2 - 1][loop3] = temp;
            }

    for(int loop1 = 0; loop1 < width; loop1++)
        for(int loop2 = 0; loop2 < height; loop2++)
            for(int loop3 = 0; loop3 < layer; loop3++)
            {
                if(pattern[loop1][loop2][loop3] != C_LevelEditor::DONT_CLEAR)
                    pattern[loop1][loop2][loop3] ^= 0x40000000;
            }
}
