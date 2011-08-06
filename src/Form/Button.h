#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>
#include <vector>

#include "..\Utils\Color.h"
#include "Form.h"
#include "..\Utils\FontMan.h"

using namespace std;

/*
 * Stisknutela tlacitka do dveri *
 *                               */

class C_Button : public C_FormElement
{
    private :
        int x, y;           // pozice v ramci dveri
        int width, height;  // rozmery
        C_Color colorButton;    // barva tlacitka
        C_Color colorTitle;

        string title;
        string help;        // napoveda k tlacitku
        char textAlign;     // zarovnani textu v tlacitku

        bool visible;       // je videt tento prvek, nemuze byt ani zmacknuto
        bool focus;         // je mys nad tlacitkem
        bool pushed;        // je dane tlacitko zmacknuto
        bool newUnpushed;   // nove odkliknuto
        bool newPushed;     // nove kliknuto

        bool IsPushed(int mouseX, int mouseY);    // bylo dane tlacitko zmacknuto?
        bool IsMouseOver(int mouseX, int mouseY); // nachazi se mys nad tlacitkem.

        static unsigned int nButton;            // pocet tlacitek v celem systemu
        unsigned int iButton;                   // cislo konkretniho tlacitka(od 1)
        static unsigned int iActiveButton;      // ktere tlacitko je aktivni (zadne, tak 0)


    public :
        C_Button();
        C_Button(int n_x, int n_y, int n_width, int n_height, string n_title, char n_textAlign = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER, string n_help = "nohelp", C_Color n_buttonColor = C_Color::blueDark, C_Color n_titleColor = C_Color::blueLight);
        ~C_Button();

        C_Button(const C_Button &);
        C_Button & operator=(const C_Button &);

        void MainLoop(int mouseX, int mouseY);  // zde mouseX, mouseY bud pokud tlacitko ve dverich, tak interMouseX, interMouseY, nebo normalne App::Inst()->mouseX..
        void MouseClick(int mouseX, int mouseY);
        void Draw();
        void Set(int n_x, int n_y, int n_width, int n_height, string n_title, char n_textAlign = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER, string n_help = "nohelp", C_Color n_buttonColor = C_Color::blueDark, C_Color n_titleColor = C_Color::blueLight);
        void SetVisible(bool n_visible) {visible = n_visible;};
        bool GetIsPushed() {return pushed;};
        string GetTitle() {return title;};
        int GetX() {return x;};
        int GetY() {return y;};
        int GetHeight() {return height;};
        int GetWidth() {return width;};
        bool GetNewUnpushed() {return newUnpushed;};
        bool GetNewPushed() {return newPushed;};
        void SetIsPushed(bool n_pushed) {pushed = n_pushed;};
        bool GetIsPushedAndUnpush() {bool temp = pushed; pushed = false; return temp;};
};

#endif

