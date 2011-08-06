#include "ManXML.h"

#include <fstream>
#include <iostream>

XMLDoc::XMLDoc(string rootName)
{
    rootNode = new XMLNode(rootName);
}

XMLDoc::~XMLDoc()
{
    delete rootNode;
}

bool XMLDoc::Save(string src)
{
    ofstream fw;
    fw.open(src.c_str());
    fw << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    rootNode->Save(fw);
    fw.close();
}
