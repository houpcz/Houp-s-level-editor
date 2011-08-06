
#include <GL/GL.h>
#include "Engine.h"
#include "Door\DoorSystem.h"

#include "LevelEditor.h"
#include "Utils\FontMan.h"
#include "Map.h"

bool App::DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity( );

    lastTime=thisTime;
    thisTime=SDL_GetTicks();
    diffTime=(thisTime - lastTime) / 1000.0;
	fps = 1 / diffTime;

    C_DoorSystem::Inst()->MainLoop();
    C_LevelEditor::Inst()->MainLoop();
    C_DoorSystem::Inst()->Draw();

    glLoadIdentity( );
    glColor3ub(22, 22, 44);

    /*
    glDisable(GL_TEXTURE_2D);
    glRecti((App::Inst()->GetScreenWidth() >> 1) - 10, (App::Inst()->GetScreenHeight() >> 1) - 10, (App::Inst()->GetScreenWidth() >> 1) + 10, (App::Inst()->GetScreenHeight() >> 1) + 10);
    glRecti((App::Inst()->GetScreenWidth() >> 1) - 40, (App::Inst()->GetScreenHeight() >> 1) - 1, (App::Inst()->GetScreenWidth() >> 1) + 40, (App::Inst()->GetScreenHeight() >> 1) + 1);
    glRecti((App::Inst()->GetScreenWidth() >> 1) - 1, (App::Inst()->GetScreenHeight() >> 1) - 40, (App::Inst()->GetScreenWidth() >> 1) + 1, (App::Inst()->GetScreenHeight() >> 1) + 40);

    glColor3ub(255, 255, 255);
    //map.Debug();
    FontMan::Inst()->SetAnchor(FontMan::ALIGN_HCENTER | FontMan::ALIGN_VCENTER);
    FontMan::Inst()->Print("impact12", App::Inst()->GetScreenWidth() >> 1, App::Inst()->GetScreenHeight() >> 1, "_aolotokopppppppppppKOQZKppppppppppppokotoloa_\n_aolotokafds\n ahoj, jak \nv KOP dg \ngfdodruhyrade\nfdjsaflkjsa\nkdruhyzkotoloa_");
    //FontMan::Inst()->Print("lucida16", App::Inst()->GetScreenWidth() >> 1, App::Inst()->GetScreenHeight() >> 1, "HolaHou");
    FontMan::Inst()->SetAnchor(FontMan::ALIGN_LEFT | FontMan::ALIGN_BOTTOM);
    */
    SDL_GL_SwapBuffers();

    return(true);
}
