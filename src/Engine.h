#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL/SDL.h>
#include <string>

using namespace std;

class App
{
    private :
        enum {
            MAX_KEY = 400
        };
        int videoFlags;
        int done;                       // je hlavni smycka skoncena
        SDL_Event event;
        long keyPress;                  // how many key down - key up events (how many keys are pressed?

        const SDL_VideoInfo *videoInfo;
        int isActive;                   // je okno aktivni
        SDL_Surface *surface;
        bool key[MAX_KEY];                  // uklada informace o stisknutych klavesach
        double diffTime;              // [s] rozdil casu

        int screenWidth;
        int screenHeight;

        static App * inst;

        App();
        ~App();
    public :
        unsigned long int lastTime;              // [ms] cas minuleho framu
        unsigned long int thisTime;              // [ms] cas soucasneho framu
        double fps;                     // FPS

        int mouseX, mouseY;             // souradnice mysi
        int mouseLastX, mouseLastY;     // souradnice mysi pri minulem framu
        int mouseRelX, mouseRelY;       // zmena souradnice mysi od posledniho snimku
        bool mouseLeft, mouseRight;     // stisknuto tlacitko mysi
        bool mouseNewLeftDown, mouseNewRightDown; // stisknuto tento frame
        bool mouseNewLeftUp, mouseNewRightUp; // odstisknuto tento frame
        bool mouseWheelUp, mouseWheelDown;  // pouziva uzivatel kolecko
        long int timeMouseLeft, timeMouseRight; // kdy bylo jake tlacitko stisknuto
    public :
        static App * Inst();

        bool ResizeWindow(int width, int height);
        void Init();
        int InitGL();
        bool InitSDL();
        void BuildFont();
        void Quit();
        void MainLoop();            // hlavni smycka programu
        void GetEvents();           // ziska informace o udalostech(zmena velikosti okna, stisknute klavesy...)
        void ProcedeEvent();        // zpracuje jednu udalost
        bool DrawGLScene();
        void AppKeys();             // konec programu, fullscreen...
        bool GetKey(unsigned int c, bool unpress = true);
        double GetDiffTime();

        int GetScreenWidth() {return screenWidth;}
        int GetScreenHeight() {return screenHeight;}
        void SetScreenWidth(int width) {screenWidth = width;};
        void SetScreenHeight(int height) {screenHeight = height;};
};

#endif
