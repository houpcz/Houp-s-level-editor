#ifndef _MANXML_H_
#define _MANXML_H_

#include <string>
#include "XMLNode.h"

using namespace std;

class XMLDoc {
  private :
        XMLNode *rootNode;

  public :
        XMLDoc(string rootName);
        ~XMLDoc();
        bool Save(string src);
        XMLNode * GetRootNode() { return rootNode; }
};

#endif
