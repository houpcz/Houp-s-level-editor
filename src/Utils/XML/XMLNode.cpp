#include "XMLNode.h"

XMLNode::XMLNode(string n_name)
{
    name = n_name;
    for(int loop1 = 0; loop1 < name.size(); loop1++)
    {
        if(name[loop1] == ' ')
            name[loop1] = '_';
    }
    value = "";
}

XMLNode::XMLNode(string n_name, string n_value)
{
    name = n_name;
    for(int loop1 = 0; loop1 < name.size(); loop1++)
    {
        if(name[loop1] == ' ')
            name[loop1] = '_';
    }
    value = n_value;
}

XMLNode::~XMLNode()
{
    for(int loop1 = 0; loop1 < children.size(); loop1++)
        delete children[loop1];
    children.clear();
}

void XMLNode::SaveOffset(ofstream & fw, int depth)
{
    for(int loop1 = 0; loop1 < depth; loop1++)
    {
        fw << "  ";
    }
}

void XMLNode::SaveAttributes(ofstream & fw)
{
    vector<XMLAttribute>::iterator it;
    for(it = attribute.begin(); it != attribute.end(); ++it)
    {
        fw << " " << it->name << "=\"" << it->value << "\"";
    }
}

bool XMLNode::Save(ofstream & fw, int depth)
{
    bool singleton = value.size() == 0 && children.size() == 0;

    SaveOffset(fw, depth);
    fw << "<" << name;
    SaveAttributes(fw);
    if(singleton)
    {
        fw << " />" << endl;
        return true;
    }
    else
        fw <<  ">";

    if(value.size() == 0 && !singleton)
    {
        fw << endl;
        for(int loop1 = 0; loop1 < children.size(); loop1++)
            children[loop1]->Save(fw, depth + 1);
        SaveOffset(fw, depth);
    }
    else
        fw << value;

    if(!singleton)
        fw << "</" << name <<  ">" << endl;
}

bool XMLNode::Save(ofstream & fw)
{
    Save(fw, 0);
}

void XMLNode::AddChild(XMLNode * child)
{
    children.push_back(child);
}

void XMLNode::AddAttribute(string attrName, string attrValue)
{
    XMLAttribute attr = XMLAttribute(attrName, attrValue);
    attribute.push_back(attr);
}
