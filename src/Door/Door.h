#ifndef __DOORS_H__
#define __DOORS_H__

#include "String.h"
#include "..\Form\Button.h"
#include "..\Form\Form.h"

#include "..\Engine.h"

typedef enum E_DoorKind {
    D_MAP,              // dvere s mapou
    D_TILES,            // dvere s background tily
    D_PATTERN,          /// Doors with patterns
    D_MONSTER,          // dvere s priserami
    D_OBJECT,           // dvere s objecty
    D_ITEM,             // dvere se sebratelnymi predmety
    D_SET_SCRIPT_EDITOR,// dvere s nastavovanim druhu skriptu
    D_SET_SCRIPTS,      // dvere se vsemi skripty
    D_SET_GROUPS,       // dvere se vsemi skupinami
    D_MAIN_MENU,        // dvere pro nacteni mapy, ulozeni mapy..
    D_MENU_DOOR,        // dvere pro otevirani ostatnich dveri
    D_SET_MAP,          // dvere pro zmenu velikosti mapy
    D_SET_EDITOR,       /// Setup of map editor
    D_NEW_MAP,          // dvere pri otevirani nove mapy
    D_LOAD_MAP,         // nacitam starou mapu
    D_SAVE_MAP,          // ukladam mapu
    D_NEW_SCRIPT_KIND,   // dvere pri novem druhu skriptu
    D_NEW_SCRIPT,         // novy script
    D_INFORM_DOOR,      // help door
    D_NONAME                // anonymni dvere, jejich druh neni podstatny
} D_DOORKIND;


using namespace std;

class C_Door
{
    public:
        enum {
            TITLE_HEIGHT = 20, // vyska titulku
            MIN_WIDTH = 50,     // minimalni rozmery dveri
            MIN_HEIGHT = 20,
            LEFTBUTTON = 0,
            RIGHTBUTTON = 1,
            MAX_TITLE = 20,
            RESIZE_AREA = 15,

            UNLIMITED = -1      // maxWidth a maxHeight
        };

        string title;
        D_DOORKIND doorKind;

        vector<C_FormElement *> formElement;

        static long int topLayer;    // jake cislo ma okno na vrcholu
        static bool wasClick;       // bylo kliknuto na nejake okno jiz tento frame? (aby se nedalo klikat na spodni okna
        static bool wasOver;        // bylo najeto na nejake dvere jiz tento frame? (abychom napr. nezobrazovali napovedu spodnich oken)

        int layer;          // v jake vrstve je okno
        int x, y;           // poloha okna
        double scrollY;        // vnitrek okna o kolik je skrolovan
        double maxScrollY;
        int width, height;          // sirka, vyska dveri, bez titulku
        int maxWidth, maxHeight;    // maximalni sirka/vyska dveri, kterym se muze menit velikost
        int interMouseX, interMouseY;       // pozice mysi v ramci dveri

        bool firstTime;     // jsou dane dvere tesne po otevreni

        bool drag;          // tahneme?
        bool resize;        // menime velikost
        bool minimalize;    // je okno minimalizovane
        bool close;         // pokud true, tak priste DoorSystem se postara o zniceni dveri
        bool dontSave;      ///

        bool resizeableVertical;    // lze menit velikost dveri
        bool resizeableHorizontal;
        bool closeable;
        bool minimalizeable;
        bool isTitle;       // je titulek dveri? pokud ne, tak x, y jsou souradnice obsahu dveri
        bool scrollable;    // muzou se dane dvere skrolovat?

        bool IsMouseInMinimalize();     // je mys na minimalizacnim tlacitku
        bool IsMouseInClose();          // je mys na krizku
        bool IsMouseInTitle();
        bool IsMouseInDoor();
        bool IsMouseInResize();

        virtual void DrawContent();
        virtual void DoorActionSetTop();
        virtual void DoorActionDown(int button);
        virtual void DoorActionUp(int button) {button = LEFTBUTTON;};
        virtual void DoorActionIn() {};
        virtual void DoorActionOut() {};              // reagovat nejak na mys, ktera se pohybuje mimo dane dvere
        virtual void DoorEverTime();               // "MainLoop" pro kazde dvere zvlast
        virtual void FirstTime();                     // funkce, ktera se spusti jednou po vytvoreni okna

        C_Door(const C_Door &);
        C_Door & operator=(const C_Door &);
    public:
        C_Door();
        virtual ~C_Door();

        void Draw();
        void MainLoop();

        virtual void Save(FILE * fw);
        virtual void Load(FILE * fr);

        void AddFormElement(C_FormElement * element) {formElement.push_back(element);};
        long int GetTopLayer() {return topLayer;};
        int GetLayer() {return layer;};
        static void ResetWasClick() {wasClick = false;};
        static bool GetWasClick() {return wasClick;};
        string GetTitle() {return title;};
        bool GetIsTopDoor() {return layer == topLayer;};     // vrati jestli dane dvere jsou ty aktivni, ty navrcholu
        bool GetIsClose()   {return close;};
        void Close() { close = true; };
        D_DOORKIND GetDoorKind() {return doorKind;};
        void SetDontSave(bool save) { dontSave = save;};
        bool GetDontSave() { return dontSave;};
        void SetDoorKind(int kind) {doorKind = (D_DOORKIND) kind;};
        void SetTopDoor(); // da dane dvere do popredi
        void SetMinimalize(bool min) {minimalize = min;};
};

#endif
