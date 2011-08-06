#ifndef _FONTMAN_H_
#define _FONTMAN_H_

#include "Font.h"
#include <map>
#include <stdexcept>

const int LUCIDA_CHAR_WIDTH_PX16 = 13;    /// Width of one char in lucida, h = 16, font (its monospace font)
const int LUCIDA_CHAR_WIDTH_PX8 = 7;      /// Width of one char in lucida, h = 8, font (its monospace font)

class FontMan
{
    private :
        static FontMan * inst;              /// Its singleton

        std::map<string, Font *> fontMap;   /// map of font name and font itself
        char anchor;                        /// actual anchor/alignment, texts will be aligned according this (ALIGN_VCENTER, ALIGN_TOP ..)

        inline void PopProjectionMatrix();
        inline void PushScreenCoordinateMatrix();
        inline void AlignTextCoord(float & dx, float & dy, int width, int height);  /// It applies alignement

        FontMan();
        ~FontMan();
    public :
        static const char ALIGN_VCENTER = 0x01;
        static const char ALIGN_TOP     = 0x02;
        static const char ALIGN_BOTTOM  = 0x04;
        static const char ALIGN_HCENTER = 0x08;
        static const char ALIGN_LEFT    = 0x10;
        static const char ALIGN_RIGHT   = 0x20;

        static FontMan * Inst();                /// returns pointer to singleton
        wchar_t * ToWChar(const char * text);   /// converts string from const char  * to wchar_t *
        void Print(const char * fontName, float x, float y, const char *fmt, ...); /// Its print font with name fontName on screen on coord. [x, y], fmt and ... is same as in function printf
        void MakeFont(const char * name, const char * src, unsigned int h);
        int GetTextWidth(const char * name, const wchar_t * text);
        int GetTextWidth(const char * name, string text) {return GetTextWidth(name, text.c_str());};
        int GetTextWidth(const char * name, const char * text) {return GetTextWidth(name, ToWChar(text));}

        int GetFontHeight(const char * name) {return fontMap[name]->GetHeight();}; /// Returns height of font with name name
        void SetAnchor(const char align) {anchor = align;};
        char GetAnchor() { return anchor; };
};

#endif
