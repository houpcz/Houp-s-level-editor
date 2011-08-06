#include <stdio.h>

#include "engine.h"

int main(int argc, char **argv)
{
    App::Inst()->Init();
    App::Inst()->InitSDL();

    if (!App::Inst()->InitGL())
	{
	    fprintf(stderr, "Nelze inicializovat OpenGL.\n");
	    App::Inst()->Quit();
	}


    App::Inst()->ResizeWindow(App::Inst()->GetScreenWidth(), App::Inst()->GetScreenHeight());
    App::Inst()->MainLoop();
    App::Inst()->Quit();

    /* Sem by se to nikdy nemelo dostat */
    return(0);
}
