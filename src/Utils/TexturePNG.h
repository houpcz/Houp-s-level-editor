#ifndef _TEXTUREPNG_H_
#define _TEXTUREPNG_H_

#include <stdio.h>										// Standard I/O header
#include <gl\gl.h>										// Header for OpenGL32 library

#include "TextureLoader.h"

class C_TexturePNG : public C_TextureLoader
{
    private :

    public :
        bool Load(S_Texture * texture, const char * filename);
        C_TexturePNG();
};

#endif
