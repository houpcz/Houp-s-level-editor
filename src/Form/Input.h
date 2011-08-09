#ifndef _INPUT_H_
#define _INPUT_H_

#include <string>
#include <iostream>
#include <sstream>
#include "..\Utils\Color.h"
using namespace std;

/*
 * Jednoøádkové vstupní prvky formuláøe
 *                                      */

class C_Input : public C_FormElement
{
    protected :
        enum {
          LINE_HEIGHT = 20
        };

        int x, y;                           // pozice v ramci dveri
        unsigned int lineLength;            // delka radku v poctu znaku
        unsigned int nLines;                // pocet radek input prvku
        unsigned int width;                 // sirka input prvku v px, vypocitava se dle lineLength
        unsigned int height;                // vyska input prvku v px, vypocitava se dle nLines
        C_Color colorFocus;                 // barva form. prvku po najeti mysi
        C_Color colorNormal;                // barva form. prvku normalne
        C_Color colorActive;                // barva form. prvku, kdyz je aktivni
        C_Color colorTitle;                  // barva textu inputu
        string title;
        string help;

        int pos;   // pozice kurzoru v inputu

        bool visible;       // zobrazuje se
        bool focus;         // je mys nad inputem
        bool active;        // je dany input aktivni

        bool editable;      // lze menit hodnoto v formulari

        bool autoresize;    // upravuje se pocet radku dle poctu znaku v inputu?
        bool updateNLines;       // u autosize muze se zmenit pocet radek

        static unsigned int nInput; // pocet formularovych input prvku od otevreni programu
        static unsigned int indexActive;        // index prave aktivniho prvku
        unsigned int index;         // cislo aktualniho input prvku

        bool IsPushed(unsigned int mouseX, unsigned int mouseY);    // bylo dane tlacitko zmacknuto?
        bool IsMouseOver(unsigned int mouseX, unsigned int mouseY); // nachazi se mys nad tlacitkem.

        void NewChar(char c);       // prida novy znak na misto pos
        void AddNumber();
        void AddLetter();
        void AddVisibleChar();

        // There is no dynamic structure
        // C_Input(const C_Input &);
        // C_Input & operator=(const C_Input &);
    public :
        enum {
            AUTORESIZE = -1
        };
        C_Input();
        C_Input(int n_x, int n_y, int n_lineLength, int n_nLines, string n_title, string n_help = "nohelp", char n_tR = 33, char n_tG = 66, char n_tB = 33, char n_pR = 180, char n_pG = 255, char n_pB = 180, char n_fR = 180, char n_fG = 180, char n_fB = 180, char n_aR = 255, char n_aG = 111, char n_aB = 111);

        virtual ~C_Input();
        void MouseClick(int mouseX, int mouseY);
        void MainLoop(int mouseX, int mouseY);
        void Draw();
        void Set(int n_x, int n_y, int n_lineLength, int n_nLines, string n_title, string n_help = "nohelp", char n_tR = 33, char n_tG = 66, char n_tB = 33, char n_pR = 180, char n_pG = 255, char n_pB = 180, char n_fR = 180, char n_fG = 180, char n_fB = 180, char n_aR = 255, char n_aG = 111, char n_aB = 111);
        void SetActive(bool n_active);
        void SetPos(int n_pos) {pos = n_pos;};
        void SetVisible(bool n_visible) {visible = n_visible;};
        void SetTitle(string n_title) { title = n_title;}
        void SetEditable(bool n_editable) {editable = n_editable;};
        virtual void AddChar();
        string GetString() {return title;};
        int GetInteger();
        double GetDouble();
        unsigned int GetNLines() {return nLines;};
        bool GetUpdateNLines() {return updateNLines;};
        bool GetActive() {return active;};
        int GetPos() {return pos;};
        virtual void CheckValue() {};
};

class C_Label : public C_Input
{
    private :
        void InitLabel();
    public :
        C_Label();
        C_Label(int n_x, int n_y, int n_lineLength, int n_nLines, string n_title, string n_help = "nohelp", char n_tR = 0, char n_tG = 0, char n_tB = 0, char n_pR = 155, char n_pG = 155, char n_pB = 155, char n_fR = 155, char n_fG = 155, char n_fB = 155, char n_aR = 155, char n_aG = 155, char n_aB = 155);
        ~C_Label();
};

class C_InputInteger : public C_Input
{
    private :
        int min, max;       // minimalni a maximalni hodnoty ulozene v integeru. kdyz oboje nula, tak tam muze byt cokoliv
    public :
        C_InputInteger(int n_x, int n_y, int n_lineLength, int n_nLines, string n_title, string n_help = "nohelp",
                       char n_tR = 33, char n_tG = 66, char n_tB = 33,
                       char n_pR = 180, char n_pG = 255, char n_pB = 180,
                       char n_fR = 180, char n_fG = 180, char n_fB = 180,
                       char n_aR = 255, char n_aG = 111, char n_aB = 111) : C_Input(n_x, n_y, n_lineLength, n_nLines, n_title, n_help, n_tR, n_tG, n_tB, n_pR, n_pG, n_pB, n_fR , n_fG, n_fB, n_aR, n_aG, n_aB)
                       {
                            SetMinMax();
                       };
        C_InputInteger(int n_min = -1, int n_max = -1) {min = n_min; max = n_max;};
        ~C_InputInteger() {};
        void SetMinMax(int n_min = -1, int n_max = -1) {min = n_min; max = n_max;};

        void AddChar();
        void CheckValue();
};

class C_InputDouble : public C_Input
{
    public :
        C_InputDouble() {};
        ~C_InputDouble() {};

        void AddChar();
};

#endif


