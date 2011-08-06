#ifndef _XMLMAN_H_
#define _XMLMAN_H_

#ifndef NULL
#define NULL 0
#endif

class XMLMan
{
    private :
        static XMLMan * inst;                      /// Singleton instance

        XMLMan();
        ~XMLMan();
    public :
        static XMLMan * Inst();

};

#endif
