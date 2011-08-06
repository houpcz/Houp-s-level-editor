#ifndef _FORM_H_
#define _FORM_H_

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

template <class T>
bool FromString(T& t,
                const std::string& s,
                std::ios_base& (*f)(std::ios_base&))
{
    std::istringstream iss(s);
    return !(iss >> f >> t).fail();
}

template <class T>
string ToString(T toBeConverted){

 // create an out string stream
 ostringstream buffer;
 // write the value to be converted to the output stream
 buffer << toBeConverted;
 // get the string value
 return buffer.str();
};

//template bool FromString(int& t, const std::string& s, std::ios_base& (*f)(std::ios_base&));

class C_FormElement
{
    public :
        virtual void MouseClick(int mouseX, int mouseY) = 0;
        virtual void MainLoop(int mouseX, int mouseY) = 0;
        virtual void Draw() = 0;
        C_FormElement();
        virtual ~C_FormElement();
};

#endif

