#ifndef _XMLNODE_H_
#define _XMLNODE_H_

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class XMLNode
{
    private :
        string name;
        string value;

        map<string, string> attribute;
        vector<XMLNode *> children;
        bool Save(ofstream & fw, int depth);
        void SaveOffset(ofstream & fw, int depth);
        void SaveAttributes(ofstream &fw);
    public :
        XMLNode(string n_name, string n_value);
        XMLNode(string n_name);
        ~XMLNode();
        bool Save(ofstream & fw);
        void AddChild(XMLNode * child);
        void AddAttribute(string attrName, string attrValue);
};

#endif
