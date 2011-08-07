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

bool FileMan::LoadString(string & str, FILE * fr)
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

#include <windows.h>
vector<string> FileMan::GetFileList(string src, bool sufix)
{
    WIN32_FIND_DATA f;
    HANDLE h = FindFirstFile(src.c_str(), &f);
    vector<string> fileName;

    size_t found;
    string temp;
    if(h != INVALID_HANDLE_VALUE)
    {
        do
        {
            temp = f.cFileName;
            if(!sufix)
            {
                found = temp.find_last_of(".");
                temp = temp.substr(0, found);
            }
            fileName.push_back(temp);
        } while(FindNextFile(h, &f));
    }
    else
    {
        fprintf(stderr, "Error opening directory %s\n", src.c_str());
    }

    return fileName;
}

vector<string> FileMan::GetDirectoryList(string src)
{
    WIN32_FIND_DATA f;
    HANDLE h = FindFirstFile(src.c_str(), &f);
    vector<string> fileName;

    size_t found;
    string temp;
    if(h != INVALID_HANDLE_VALUE)
    {
        do
        {
            temp = f.cFileName;
            if (f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if(temp.compare(".") != 0 &&
                   temp.compare("..") != 0)
                    fileName.push_back(temp);
            }

        } while(FindNextFile(h, &f));
    }
    else
    {
        fprintf(stderr, "Error opening directory %s\n", src.c_str());
    }

    return fileName;
}

bool FileMan::MakeDirectory(string src)
{
    return CreateDirectory(src.c_str(), NULL);
}
