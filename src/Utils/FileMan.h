#ifndef _FILEMAN_H_
#define _FILEMAN_H_

#include <SDL/SDL.h>
#include <string>
#include <vector>

using namespace std;

class FileMan
{
    private :
        static FileMan * inst;                      /// Singleton instance

        FileMan();
        ~FileMan();
    public :
        static FileMan * Inst();                    /// Gets singleton pointer
        bool SaveString(string str, FILE *fw);      /// Saves string to *fw file
        bool LoadString(string & str, FILE *fr);    /// Loads string from *fr file, string is saved to str
        bool SaveInteger(int i, FILE *fw);          /// Saves integer to *fw file
        bool LoadInteger(int & i, FILE *fr);        /// Loads integer from *fr file, integer is saved to i
        bool SaveDouble(double d, FILE *fw);        /// Saves double to *fw file
        bool LoadDouble(double & d, FILE *fr);      /// Loads double from *fr file, double is saved to d

        /** \brief
         *  Returns vector of files in target directory.
         *  It can returns it with sufixes or without them.
         * \param src string Source of target directory
         * \param sufix bool Return strings containing sufixes?
         * \return vector<string>
         *
         */
        vector<string> GetFileList(string src, bool sufix);
        vector<string> GetDirectoryList(string src);
        bool MakeDirectory(string src);
};


#endif

