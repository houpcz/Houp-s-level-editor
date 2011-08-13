#ifndef _SAXPARSER_H_
#define _SAXPARSER_H_

#include "SaxInterface.h"

class SaxParser {
    private :
        SaxParser();
        ~SaxParser();
        static SaxParser * inst;
        void ParseLine(string line, SaxInterface * handler);
    public :
        static SaxParser * Inst();
        void Parse(string src, SaxInterface * handler);
};

#endif
