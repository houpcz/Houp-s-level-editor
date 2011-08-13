#ifndef _XMLATTRIBUTE_H_
#define _XMLATTRIBUTE_H_

#include <string>

using namespace std;

class XMLAttribute
{
    public :
        XMLAttribute(string n_name, string n_value)
        {
          name = n_name;
          value = n_value;
        };
        string name;
        string value;
};

#endif
