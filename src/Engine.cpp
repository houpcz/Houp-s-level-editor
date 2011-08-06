
#include <GL/gl.h>
#include <SDL/SDL.h>

#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "Engine.h"

#include "Map.h"

#include "Door\Doorsystem.h"
#include "LevelEditor.h"
#include "Script\Scriptsystem.h"
#include "Utils\FontMan.h"
#include "Utils\TextureMan.h"

App * App::inst = NULL;

App* App::Inst()
{
    if (inst == NULL)
    {
        inst = new App;
    }
    return inst;
}

App::App() {
    done = false;
    isActive = true;
    screenWidth = 800;
    screenHeight = 600;

    for(int loop1 = 0; loop1 < MAX_KEY; loop1++)
        key[loop1] = false;

    mouseLeft = false;
    mouseRight = false;
    mouseNewLeftDown = false;
    mouseNewRightDown = false;
    mouseNewLeftUp = false;
    mouseNewRightUp = false;
    mouseWheelDown = false;
    mouseWheelUp = false;
    mouseRelX = 0;
    mouseRelY = 0;
}

App::~App() {
    if(inst != NULL)
        delete inst;
    inst  = NULL;
}

bool App::ResizeWindow(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    /* Ochrana proti deleni nulou */
    if (height == 0)
	height = 1;

    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    C_DoorSystem::Inst()->ResizeWindow();

    return(true);
}

int App::InitGL()
{
    BuildFont();
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D);// Povoli textury
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return(true);
}

void App::Init()
{
    C_LevelEditor::Inst()->Load();
    C_DoorSystem::Inst()->Init();
    C_ScriptSystem::Inst()->LoadScriptKind();
}



bool App::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
	    fprintf(stderr, "Inicializace SDL selhala: %s\n",
		     SDL_GetError());
	    Quit();
	}
	SDL_WM_SetCaption("Level editor", NULL);

    videoInfo = SDL_GetVideoInfo( );

    if (!videoInfo)
	{
	    fprintf(stderr, "Selhalo zjistovani video informaci: %s\n",
		     SDL_GetError());
	    Quit();
	}

    videoFlags  = SDL_OPENGL;          /* Povoli OpenGL v SDL */
    //videoFlags |= SDL_NOFRAME;
    videoFlags |= SDL_RESIZABLE;
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Povoli double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Ulozi paletu v hardwaru */
    //videoFlags |= SDL_FULLSCREEN;      /* Fullscreen mode*/

    if (videoInfo->hw_available)
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    /* Nastavi OpenGL double buffering */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_WM_SetCaption("Level editor", NULL);
	SDL_Surface *icon = SDL_LoadBMP("data/texture/icon.bmp");
    if(icon != NULL)
    {
        SDL_WM_SetIcon(icon, NULL);
        SDL_FreeSurface(icon);
    }

    /* ziska SDL surface */
    surface = SDL_SetVideoMode(screenWidth, screenHeight, 16,
				videoFlags);

    if (!surface)
	{
	    fprintf(stderr,  "Nastaveni video modu selhalo: %s\n", SDL_GetError());
	    Quit();
	}

	SDL_ShowCursor(1);

	return true;
}

void App::BuildFont()
{
    FontMan * fontMan = FontMan::Inst();
    fontMan->MakeFont("impact12", "data/font/impact.TTF", 12);
    fontMan->MakeFont("lucida8", "data/font/lucida.TTF", 8);
    //fontMan->MakeFont("lucida12", "data/font/lucida.TTF", 12);
    fontMan->MakeFont("lucida16", "data/font/lucida.TTF", 16);
}

void App::ProcedeEvent()
{
    switch( event.type )
    {
        case SDL_ACTIVEEVENT:
            if ( event.active.gain == 0 )
                isActive = false;
            else
                isActive = true;
            break;

        case SDL_KEYDOWN:
            keyPress++;
            key[event.key.keysym.sym] = true;
            break;

        case SDL_KEYUP:
            keyPress--;
            key[event.key.keysym.sym] = false;
            break;

        case SDL_QUIT:
            done = true;
            break;

        case SDL_MOUSEMOTION:
            mouseLastX = mouseX;
            mouseLastY = mouseY;
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            mouseRelX = mouseX - mouseLastX;
            mouseRelY = mouseY - mouseLastY;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseLastX = mouseX;
            mouseLastY = mouseY;
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            mouseRelX = mouseX - mouseLastX;
            mouseRelY = mouseY - mouseLastY;

            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseLeft = true;
                mouseNewLeftDown = true;
                timeMouseLeft = thisTime;
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                mouseRight = true;
                mouseNewRightDown = true;
                timeMouseRight = thisTime;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            switch(event.button.button)
            {
                case SDL_BUTTON_LEFT :
                    mouseLeft = false;
                    mouseNewLeftUp = true;
                    break;

                case SDL_BUTTON_RIGHT :
                    mouseRight = false;
                    mouseNewRightUp = true;
                    break;

                case SDL_BUTTON_WHEELDOWN :
                    mouseWheelDown = true;
                    break;

                case SDL_BUTTON_WHEELUP :
                    mouseWheelUp = true;
                    break;
            }
            break;

        case SDL_VIDEORESIZE:
            SDL_FreeSurface(surface);
            surface = SDL_SetVideoMode(event.resize.w, event.resize.h, 16,
                videoFlags);

            InitGL();
            ResizeWindow(event.resize.w, event.resize.h);
            C_TextureMan::Inst()->ClearTextures();
            break;

        default:
            break;
    }
}

void App::GetEvents()
{
    mouseNewLeftDown = false;
    mouseNewRightDown = false;
    mouseNewLeftUp = false;
    mouseNewRightUp = false;
    mouseWheelDown = false;
    mouseWheelUp = false;
    mouseRelX = 0;
    mouseRelY = 0;

    if(keyPress == 0) {
        SDL_WaitEvent( &event );
        ProcedeEvent();
    }
    else
        while ( SDL_PollEvent( &event ) )
        {
            ProcedeEvent();
        }
}

void App::AppKeys()
{
    if(key[SDLK_ESCAPE])
	    Quit();
}

void App::MainLoop()
{
    while (!done)
	{
        GetEvents();
        AppKeys();
	    //if(isActive)
		  DrawGLScene();
        //_sleep(50);         // tahle supr vec neovlivni beh programu, ale utisi notebook neskutecne
	}
}

void App::Quit()
{
  C_LevelEditor::Inst()->Save();
  C_DoorSystem::Inst()->Save();

  SDL_Quit();
  exit(0);
}

bool App::GetKey(unsigned int c, bool unpress)
{
    bool temp = key[c];

    if(unpress)
        key[c] = false;

    return temp;
}

double App::GetDiffTime()
{
    if(diffTime > 0.2)
        return 0.0;
    else
        return diffTime;
}
