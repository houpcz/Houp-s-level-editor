#ifndef _FONT_H_
#define _FONT_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glu.h>

//Some STL headers
#include <vector>
#include <string>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include <string>

using namespace std;

class Font {
  private :
        static const int MAX_LETTERS = 400; ///< Its enough to use czech letters in fonts

        GLfloat height;		   ///< Holds the height of the font.
        GLuint * textures;	   ///< Holds the texture id's
        GLuint displayListID;  ///< Holds the first display list id
        GLuint * letterWidth;  ///< Pointer to array of letter widths

        int MakeDisplayList( FT_Face face, int ch, GLuint list_base, GLuint * tex_base );

        Font(const Font &);
        Font & operator=(const Font &);
  public :
        Font(const char * src, unsigned int h);
        Font();
        ~Font();

        void Init(const char * src, unsigned int h);    /// Loads font on src path with height h
        int GetTextWidth(const char * text);            /// Gets text width in px
        int GetTextWidth(const wchar_t * text);         /// Gets text width in px
        int GetTextWidth(const std::string * text);     /// Gets text width in px
        void Clean();                                   /// Cleans the memory used by font

        GLuint GetDisplayListID() {return displayListID;};
        GLfloat GetHeight() {return height;};
};

#endif
