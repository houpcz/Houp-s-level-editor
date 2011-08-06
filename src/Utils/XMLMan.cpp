#include "XMLMan.h"

XMLMan * XMLMan::inst = NULL;

XMLMan* XMLMan::Inst()
{
    if (inst == NULL)
    {
        inst = new XMLMan;
    }
    return inst;
}

XMLMan::~XMLMan()
{
    if(inst != NULL)
        delete inst;
    inst = NULL;
}

XMLMan::XMLMan()
{

}
