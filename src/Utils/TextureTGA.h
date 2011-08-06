#ifndef _TEXTURETGA_H_
#define _TEXTURETGA_H_

#include <stdio.h>										// Standard I/O header
#include <gl\gl.h>										// Header for OpenGL32 library

#include "TextureLoader.h"

class C_TextureTGA : public C_TextureLoader
{
    private :
        typedef struct
        {
            GLubyte Header[12];									// TGA File Header
        } TGAHeader;

        typedef struct
        {
            GLubyte		header[6];								// First 6 Useful Bytes From The Header
            GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
            GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
            GLuint		temp;									// Temporary Variable
            GLuint		type;
            GLuint		Height;									//Height of Image
            GLuint		Width;									//Width ofImage
            GLuint		Bpp;									// Bits Per Pixel
        } TGA;


        TGAHeader tgaheader;									// TGA header
        TGA       tga;										    // TGA image data

        static const GLubyte uTGAcompare[12];	// Uncompressed TGA Header
        static const GLubyte cTGAcompare[12];	// Compressed TGA Header

        bool LoadUncompressedTGA(S_Texture * texture, const char * filename, FILE * fTGA);
        bool LoadCompressedTGA(S_Texture * texture, const char * filename, FILE * fTGA);
    public :
        bool Load(S_Texture * texture, const char * filename);
        C_TextureTGA();
};

#endif
