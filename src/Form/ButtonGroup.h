#ifndef _BUTTONGROUP_H_
#define _BUTTONGROUP_H_

#include <string>
#include <vector>

#include "..\Utils\FontMan.h"
#include "Button.h"

using namespace std;

class C_ButtonGroup : public C_FormElement
{
    private :
        enum {
            ONLY_ONE_PUSHED     // zmacknute jen jedno tlacitko ze vsech
        };
        unsigned int kind;      // kolik tlacitek muze byt zmacknuto
        vector<C_Button> button;

        C_ButtonGroup(const C_ButtonGroup &);
        C_ButtonGroup & operator=(const C_ButtonGroup &);
    public :
        C_ButtonGroup();
        ~C_ButtonGroup();
        void AddButton(int n_x, int n_y, int n_width, int n_height, string n_title, char n_textAlign = FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER,string n_help = "nohelp", C_Color n_buttonColor = C_Color::blueDark, C_Color n_titleColor = C_Color::blueLight);
        void Draw();
        void MainLoop(int mouseX, int mouseY);
        void MouseClick(int mouseX, int mouseY);
        void SetPushed(bool n_pushed, int iButton);
        void Clear() {button.clear();};
        void SetVisible(bool n_visible, int iButton) {button[iButton].SetVisible(n_visible);};
        unsigned int GetNButton() {return button.size();};
        bool GetIsNewUnpushed(int iButton) {return button[iButton].GetNewUnpushed();};
        bool GetIsNewPushed(int iButton) {return button[iButton].GetNewPushed();};
        bool GetIsPushedAndUnpush(int iButton) {return button[iButton].GetIsPushedAndUnpush();};
        C_Button * GetButton(int iButton) {return &button[iButton];};
};

#endif


