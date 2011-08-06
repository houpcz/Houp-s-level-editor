#ifndef _TILEPATTERN_H_
#define _TILEPATTERN_H_

class TilePattern
{
  private :
        int ***pattern;
        int width;
        int height;
        int layer;

        void RotateOneTileRight(int *** matrix, int x, int y, int z);
        void MirrorHorizontalGlobal();
        void MirrorHorizontalLocal();
        void Clear();
  public :
        enum {
            DONT_CLEAR = -1
        };
        TilePattern();
        ~TilePattern();
        int GetWidth() { return width; };
        int GetHeight() { return height; };
        int GetLayer() { return layer; };
        int GetTileID(int startX, int startY, int layer);
        void RotateRight();
        void MirrorHorizontal();
        void MirrorVertical();
        void Set(int *** matrix, int n_width, int n_height, int n_layer);
        static int *** CreateMatrix3D(int x, int y, int z);
};

#endif
