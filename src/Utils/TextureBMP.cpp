#include "TextureBMP.h"
#include "AlienCode\KalyttaBMP.h"

#include <gl\gl.h>

C_TextureBMP::C_TextureBMP() : C_TextureLoader("bmp")
{
}

bool C_TextureBMP::Load(S_Texture * texture, const char * filename)
{
      CBitmap loader;
      if(!loader.Load(filename))
        return false;

      texture->width = loader.GetWidth();
      texture->height = loader.GetHeight();
      texture->bpp = loader.GetBitCount();
      GLubyte *bitmap = (GLubyte*) loader.GetBits();
      int size = texture->width * texture->height * 4;//RGBA
      texture->imageData = new GLubyte[size];
      std::copy(bitmap, bitmap + size, texture->imageData);
      texture->type = GL_RGBA;
      return true;
}

