#ifndef _SAXINTERFACE_H_
#define _SAXINTERFACE_H_

#include <string>
#include <vector>
#include "XMLAttribute.h"

using namespace std;

class SaxInterface
{
    private :
    public :
        virtual void StartDocument() {};
        virtual void EndDocument() {};
        virtual void StartElement(string elementName, vector<XMLAttribute> attributeList) {};
        virtual void EndElement(string elementName) {};
};

#endif
