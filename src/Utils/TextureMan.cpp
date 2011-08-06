#include "TextureMan.h"
#include "TexturePNG.h"
#include "TextureTGA.h"
#include "TextureBMP.h"
#include <stdlib.h>
#include <iterator>

C_TextureMan * C_TextureMan::inst = NULL;
const string C_TextureMan::srcPrefix = "Data/texture/";

C_TextureMan* C_TextureMan::Inst()
{
    if (inst == NULL)
    {
        inst = new C_TextureMan;
    }
    return inst;
}

C_TextureMan::C_TextureMan()
{
    loader[C_TextureTGA().GetSufix()] = new C_TextureTGA();
    loader[C_TexturePNG().GetSufix()] = new C_TexturePNG();
    loader[C_TextureBMP().GetSufix()] = new C_TextureBMP();
}

C_TextureMan::~C_TextureMan()
{
    map<string, C_TextureLoader *>::iterator itVec;
    for(itVec = loader.begin(); itVec != loader.end(); itVec++)
    {
        delete itVec->second;
    }

    ClearTextures();
}

void C_TextureMan::ClearTextures()
{
    map<string, S_Texture *>::iterator it;
    for(it = texture.begin(); it != texture.end(); it++)
    {
        delete it->second;
    }
    texture.clear();
}

S_Texture * C_TextureMan::GetTexture(string src)
{
    if(texture.find(src) == texture.end())
    {
        S_Texture * s_texture = new S_Texture;
        if(LoadTexture(s_texture, (srcPrefix + src).c_str()))
        {
            texture[src] = s_texture;
        } else {
            delete s_texture;
            return NULL;
        }
    }

    return texture[src];
}


void C_TextureMan::MakeTexture(S_Texture * texture) {
	glGenTextures(1, &(*texture).texID);
	glBindTexture(GL_TEXTURE_2D, (*texture).texID);
	glTexImage2D(GL_TEXTURE_2D, 0, (*texture).bpp / 8, (*texture).width, (*texture).height, 0, (*texture).type, GL_UNSIGNED_BYTE, (*texture).imageData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	if ((*texture).imageData)	{
		free((*texture).imageData);
	}
}

bool C_TextureMan::LoadTexture(S_Texture * text, const char * src)
{
    C_TextureLoader * load = NULL;

    string srcString = string(src);
    string srcSufix = srcString.substr(srcString.length() - 3, 3);

    load = loader[srcSufix];
    if(load == NULL)
    {
        fprintf(stderr, "There is no loader for images with sufix %s", srcSufix.c_str());
        return false;
    }

    if(load->Load(text, src))
    {
        MakeTexture(text);
        return true;
    }
    else
        return false;
}
