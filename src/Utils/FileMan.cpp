#include "FileMan.h"

FileMan * FileMan::inst = NULL;

FileMan* FileMan::Inst()
{
    if (inst == NULL)
    {
        inst = new FileMan;
    }
    return inst;
}

FileMan::~FileMan()
{
    if(inst != NULL)
        delete inst;
    inst = NULL;
}

FileMan::FileMan()
{
}

bool FileMan::SaveString(string str, FILE *fw)
{
    if(fw != NULL)
    {
        int size =str.size();
        fwrite(&size, sizeof(int), 1, fw);
        fwrite(str.c_str(), sizeof(char), size, fw);

        return true;
    }
    return false;
}

bool FileMan::SaveInteger(int i, FILE *fw)
{
    if(fw != NULL)
    {
        fwrite(&i, sizeof(int), 1, fw);
        return true;
    }
    return false;
}

bool FileMan::SaveDouble(double d, FILE *fw)
{
    if(fw != NULL)
    {
        fwrite(&d, sizeof(double), 1, fw);
        return true;
    }
    return false;
}

bool FileMan::LoadString(string & str, FILE *fr)
{
    if(fr != NULL)
    {
        unsigned int size;
        char n_name[256];
        if(fread(&size, sizeof(int), 1, fr) != 1)
            return false;
        if(fread(&n_name, sizeof(char), size, fr) != size)
            return false;

        if(size>0)
            n_name[size] = '\0';
        str = n_name;

        return true;
    }
    return false;
}

bool FileMan::LoadInteger(int & i, FILE *fr)
{
    if(fr != NULL)
    {
        if(fread(&i, sizeof(int), 1, fr) != 1)
            return false;

        return true;
    }
    return false;
}

bool FileMan::LoadDouble(double & d, FILE *fr)
{
    if(fr != NULL)
    {
        if(fread(&d, sizeof(double), 1, fr) != 1)
            return false;

        return true;
    }
    return false;
}

