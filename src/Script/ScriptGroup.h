#ifndef _SCRIPTGROUP_H_
#define _SCRIPTGROUP_H_

class C_ScriptGroup
{
    public :
        enum {
            MAIN_SCRIPT_GROUP = 0,
            NO_PARENT = 65000
        };

    private :
        unsigned int iParent;   // index rodice     // zatim bez vyznamu
        unsigned int i;         // index skupiny
        static unsigned int amount;          // mnozstvi skupin
        bool visible;           // je tato skupina rozkliknuta?
        string name;
    public :
        C_ScriptGroup() {name = "No name";};
        C_ScriptGroup(string n_name, int n_iParent, bool n_visible);
        ~C_ScriptGroup() {};
        void Save(FILE *fw);
        void Load(FILE *fr);
        unsigned int GetIndex() {return i;};
        string       GetName() {return name;};
        bool         GetVisible() {return visible;};
        unsigned int GetIParent() {return iParent;};

        void         SetIParent(unsigned int n_iParent) {iParent = n_iParent;};
        void         SetVisible(bool n_visible) {visible = n_visible;};
        void         SetName(string n_name) {name = n_name;};
};

#endif
