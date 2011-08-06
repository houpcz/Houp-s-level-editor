#ifndef _TEXTUREMAN_H_
#define _TEXTUREMAN_H_

#include <Gl\Gl.h>
#include <string>
#include <map>
#include <vector>

#include "TextureLoader.h"

using namespace std;

class C_TextureMan
{
    private :
        map<string, C_TextureLoader *> loader;      /// <prefix, loader>, ex. <"tga", C_TextureTGA>
        map<string, S_Texture *> texture;           /// <src path, texture>
        static const string srcPrefix;              /// in which folder the textures are

        static C_TextureMan * inst;                 /// singleton pattern

        C_TextureMan();
        ~C_TextureMan();

        bool LoadTexture(S_Texture * text, const char * src); /// Loads texture from disk to S_Texture
        void MakeTexture(S_Texture * texture);                /// Makes OpenGl texture from S_Texture
    public :
        static C_TextureMan * Inst();               /// returns pointer to singleton
        S_Texture  * GetTexture(string src);
        void ClearTextures();
};

#endif
