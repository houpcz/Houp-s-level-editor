#ifndef _TEXTUREBMP_H_
#define _TEXTUREBMP_H_

#include <stdio.h>										// Standard I/O header
#include <gl\gl.h>										// Header for OpenGL32 library

#include "TextureLoader.h"

class C_TextureBMP : public C_TextureLoader
{
    private :

    public :
        bool Load(S_Texture * texture, const char * filename);
        C_TextureBMP();
};

#endif

