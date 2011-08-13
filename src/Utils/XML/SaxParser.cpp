#include "SaxParser.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;

SaxParser * SaxParser::inst = NULL;

SaxParser::SaxParser()
{

}

SaxParser * SaxParser::Inst()
{
    if (inst == NULL)
    {
        inst = new SaxParser;
    }
    return inst;
}

void SaxParser::Parse(string src, SaxInterface * handler)
{
    string line;

    ifstream fr;
    fr.open(src.c_str());
    if(fr.is_open())
    {
        handler->StartDocument();
        while( fr.good() )
        {
          getline(fr,line);
          ParseLine(line, handler);
        }
        fr.close();
        handler->EndDocument();
    }
    else cout << "Unable to open file";
}

void SaxParser::ParseLine(string line, SaxInterface * handler)
{
    if(line.length() == 0)
        return;

    size_t found1, found2;

    if (line.find("<?") != string::npos)
    {
        found1 = line.find_first_of("<");
        while (found1 != string::npos)
        {
            found2 = line.find_first_of(">");

            found1 = line.find_first_of("<", found1 + 1);
        }
    }
}

