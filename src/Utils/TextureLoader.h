#ifndef _TEXTURELOADER_H_
#define _TEXTURELOADER_H_

#include <string>
#include <GL\GL.h>

using namespace std;

struct S_Texture
{
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;											// Image Type (GL_RGB, GL_RGBA)
};

class C_TextureLoader
{
    private :
        string sufix;           /// ex. .bmp, .png, .tga
    public :
        C_TextureLoader(string sufix) : sufix(sufix) {};
        virtual bool Load(S_Texture * texture, const char * filename) = 0; /// Loads texture to *texture from file on filename* adress
        string GetSufix() { return sufix; }
};

#endif
