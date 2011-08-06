#include "TexturePNG.h"
#include "AlienCode\LodePNG.h"

C_TexturePNG::C_TexturePNG() : C_TextureLoader("png")
{
}

bool C_TexturePNG::Load(S_Texture * texture, const char * filename)
{
      std::vector<unsigned char> buffer, image;
      LodePNG::loadFile(buffer, filename);
      LodePNG::Decoder decoder;
      decoder.decode(image, buffer);

      if(decoder.hasError())
        return false;

      texture->width = decoder.getWidth();
      texture->height = decoder.getHeight();
      texture->bpp = decoder.getBpp();
      texture->imageData = new GLubyte[image.size()];
      std::copy(image.begin(), image.end(), texture->imageData);
      texture->type = GL_RGBA;

      return true;
}
