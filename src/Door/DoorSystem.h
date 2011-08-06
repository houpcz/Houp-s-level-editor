#ifndef __DOORSYSTEM_H__
#define __DOORSYSTEM_H__

#include "Door.h"
#include "..\Form\Button.h"
#include "InformDoor.h"

#include <vector>

using namespace std;

enum E_Language {
    CZECH_CLASSIC,
    ENGLISH_CLASSIC,
    N_LANGUAGE
};

class C_DoorSystem
{
    private :
        enum {
            BUTTON_BAR_HEIGHT = 20,     // vyska v px dolni listy
            BUTTON_BAR_MAX = 15,         // maximalni pocet oken v ni
            BUTTON_BAR_BUTTON_WIDTH = 80
        };

        int language;
        bool accent;        // carka ´
        bool wedge;         // hacek

        bool changingWorldPos;
        int worldRelX, worldRelY; // scroll sveta, kam je posunuta plocha editoru

        vector<C_Door*> door;
        C_InformDoor helpDoor;      // na ukazovani napovedy k jednotlivym tlacitkum, aktivni vzdy jen jeden frame, kdo chce zobrazit napovedu, musi svou zadost opakovat
        C_Button start;             // tlacitko, ktere mi otevre nabidku moznych dveri

        static C_DoorSystem * inst;
        C_DoorSystem();
        ~C_DoorSystem();
    public :
        static C_DoorSystem * Inst();
        void ButtonBar();       // postara se o button bar
        bool IsMouseInButtonBar();  // je mys v dolni liste?
        void MainLoop();
        void Init();
        void Draw();
        void OpenNewDoor(C_Door * newDoor);
        void OpenNewDoor(int kind, int temp = 0);       // otevre nove dvere, kind urcuje jake, v temp muze byt parametr
        void OpenNewHelp(string help);
        void CloseDoor(D_DOORKIND doorKind);
        void OneCopyOfDoor(int kind);  // zavre dvere stejneho typu, jestli jsou otevrene
        int GetWorldRelX() {return worldRelX;};
        int GetWorldRelY() {return worldRelY;};
        int GetLanguage() {return language;};       // jaky jazyk je aktivni
        bool GetAccent() {return accent;};
        bool GetWedge() {return wedge;};
        void SetAccent(bool n_accent) {accent = n_accent;};
        void SetWedge(bool n_wedge) {wedge = n_wedge;};

        void ResizeWindow();

        void Save();    // ulozi rozmistneni dveri
        bool Load();    // nacte, ktera dvere jsou otevrena a kde
};

#endif
