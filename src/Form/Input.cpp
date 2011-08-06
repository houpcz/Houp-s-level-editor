#include "Form.h"
#include "Input.h"
#include "..\Engine.h"
#include "..\Door\DoorSystem.h"

#include <GL/GL.h>
#include "..\Utils\FontMan.h"

unsigned int C_Input::nInput = 0;
unsigned int C_Input::indexActive = 0;

C_Input::C_Input()
{
     Set(5, 100, 10, 1, "TEMP", "temp");
}

C_Input::~C_Input()
{

}

C_Input::C_Input(int n_x, int n_y, int n_lineLength, int n_nLines, string n_title, string n_help, char n_tR, char n_tG, char n_tB, char n_pR, char n_pG, char n_pB, char n_fR, char n_fG, char n_fB, char n_aR, char n_aG, char n_aB)
{
    Set(n_x, n_y, n_lineLength, n_nLines, n_title, n_help, n_tR, n_tG, n_tB, n_pR, n_pG, n_pB, n_fR, n_fG, n_fB, n_aR, n_aG, n_aB);
}

void C_Input::Set(int n_x, int n_y, int n_lineLength, int n_nLines, string n_title, string n_help, char n_tR, char n_tG, char n_tB, char n_pR, char n_pG, char n_pB, char n_fR, char n_fG, char n_fB, char n_aR, char n_aG, char n_aB)
{
    x = n_x;
    y = n_y;
    lineLength = n_lineLength;
    nLines = n_nLines;
    title = n_title;
    help = n_help;
    colorNormal = C_Color(n_pR, n_pG, n_pB);
    colorFocus = C_Color(n_fR, n_fG, n_fB);
    colorActive = C_Color(n_aR, n_aG, n_aB);
    colorTitle = C_Color(n_tR, n_tG, n_tB);

    visible = true;
    focus = false;
    active = false;

    index = nInput++;

    if(n_nLines == AUTORESIZE)
    {
        nLines = (title.size() / lineLength) + 1;
        autoresize = true;
    } else
        autoresize = false;

    width = lineLength * LUCIDA_CHAR_WIDTH_PX16 + 6;
    height = nLines * LINE_HEIGHT;
    editable = true;

    pos = title.size();
}

int C_Input::GetInteger()
{
    int temp;
    if (FromString<int>(temp, title, std::dec))
        return temp;
    else
    {
        if(title.size() == 0)
            return 0;
        else
            fprintf(stderr, "C_InputInteger::GetInteger chyba. Zadny integer tam nebyl, místo toho %s", title.c_str());
        return 0;
    }
}

double C_Input::GetDouble()
{
    double temp;
    if (FromString<double>(temp, title, std::dec))
        return temp;
    else
    {
        if(title.size() == 0)
            return 0.0;
        else
            fprintf(stderr, "C_InputDouble::GetDouble chyba. Zadny double tam nebyl");
        return 0.0;
    }
}

void C_Input::MouseClick(int mouseX, int mouseY)
{
    if (IsPushed(mouseX, mouseY) && editable)
    {
        active = true;
        pos = title.size();
        indexActive = index;
    }
}

void C_Input::MainLoop(int mouseX, int mouseY)
{
    updateNLines = false;

    focus = IsMouseOver(mouseX, mouseY);

    if(focus && help != "nohelp")
        C_DoorSystem::Inst()->OpenNewHelp(help);

    if (active && (indexActive != index || App::Inst()->GetKey(SDLK_RETURN, false)))
    {
        active = false;
        CheckValue();
    }

    if (active)
    {
        if(App::Inst()->GetKey(SDLK_LEFT))
        {
            pos--;
        }
        if(App::Inst()->GetKey(SDLK_UP))
        {
            pos -= lineLength;
        }
        if(App::Inst()->GetKey(SDLK_RIGHT))
        {
            pos++;
        }
        if(App::Inst()->GetKey(SDLK_DOWN))
        {
            pos += lineLength;
        }
        if(pos < 0) pos = 0;
        if(pos >= (signed) title.size()) pos = title.size();

        if (App::Inst()->GetKey(SDLK_BACKSPACE))
        {
            if (pos > 0) {
                title.erase(title.begin() + pos - 1, title.begin() + pos);
                pos--;
            }
            if(App::Inst()->GetKey(SDLK_LSHIFT, false) || App::Inst()->GetKey(SDLK_RSHIFT, false))
            {
                title.clear();
                pos = 0;
            }
        }

        if (App::Inst()->GetKey(SDLK_DELETE))
        {
            if (pos < (signed) title.size()) {
                title.erase(title.begin() + pos, title.begin() + pos + 1);
            }
        }

        AddChar();
    }

    if (title.size() > lineLength * nLines)
    {   if(autoresize && title.size() < title.max_size() - 1)
        {
            nLines++;
            height = nLines * LINE_HEIGHT;
            updateNLines = true;
        } else
            title.erase(title.size() - 1);
    }
}

void C_Input::NewChar(char c)
{
    string s;
    s += c;
    title.insert(pos++, s);
    C_DoorSystem::Inst()->SetAccent(false);
    C_DoorSystem::Inst()->SetWedge(false);
}

void C_Input::AddLetter()
{
    int loop1;
    for (loop1 = SDLK_a; loop1 <= SDLK_z; loop1++)
    {
        if (App::Inst()->GetKey(loop1))
        {
            if (App::Inst()->GetKey(SDLK_LSHIFT, false) || App::Inst()->GetKey(SDLK_RSHIFT, false))
                NewChar(loop1+'A' - 'a');
            else
                NewChar(loop1);
        }
    }
}

void C_Input::AddVisibleChar()
{
    switch(C_DoorSystem::Inst()->GetLanguage())
    {
        case ENGLISH_CLASSIC :
            if (App::Inst()->GetKey(SDLK_LSHIFT, false) || App::Inst()->GetKey(SDLK_RSHIFT, false)) {
                if(App::Inst()->GetKey(SDLK_SLASH))       NewChar('?'); else
                if(App::Inst()->GetKey(SDLK_1))           NewChar('!'); else
                if(App::Inst()->GetKey(SDLK_QUOTE))       NewChar('"'); else
                if(App::Inst()->GetKey(SDLK_PERIOD))      NewChar(':'); else
                if(App::Inst()->GetKey(SDLK_9))           NewChar('('); else
                if(App::Inst()->GetKey(SDLK_0))           NewChar(')');
            }
            break;

        case CZECH_CLASSIC :
            if(App::Inst()->GetKey(SDLK_LSHIFT, false) || App::Inst()->GetKey(SDLK_RSHIFT, false)) {
                if(C_DoorSystem::Inst()->GetAccent())
                {
                    if(App::Inst()->GetKey(SDLK_a))     NewChar('Á'); else
                    if(App::Inst()->GetKey(SDLK_e))     NewChar('É'); else
                    if(App::Inst()->GetKey(SDLK_i))     NewChar('Í'); else
                    if(App::Inst()->GetKey(SDLK_o))     NewChar('Ó'); else
                    if(App::Inst()->GetKey(SDLK_u))     NewChar('Ú'); else
                    if(App::Inst()->GetKey(SDLK_y))     NewChar('Ý');
                }
                if(C_DoorSystem::Inst()->GetWedge())
                {
                    if(App::Inst()->GetKey(SDLK_c))     NewChar('È'); else
                    if(App::Inst()->GetKey(SDLK_d))     NewChar('Ï'); else
                    if(App::Inst()->GetKey(SDLK_n))     NewChar('Ò'); else
                    if(App::Inst()->GetKey(SDLK_r))     NewChar('Ø'); else
                    if(App::Inst()->GetKey(SDLK_s))     NewChar('Š'); else
                    if(App::Inst()->GetKey(SDLK_t))     NewChar(''); else
                    if(App::Inst()->GetKey(SDLK_z))     NewChar('Ž');
                }

                if(App::Inst()->GetKey(SDLK_COMMA))     NewChar('?'); else
                if(App::Inst()->GetKey('}'))            NewChar('('); else
                if(App::Inst()->GetKey(SDLK_SLASH))     NewChar('_');
            } else {    // pro mala pismena
                if(C_DoorSystem::Inst()->GetAccent())
                {
                    if(App::Inst()->GetKey(SDLK_a))     NewChar('á'); else
                    if(App::Inst()->GetKey(SDLK_e))     NewChar('é'); else
                    if(App::Inst()->GetKey(SDLK_i))     NewChar('í'); else
                    if(App::Inst()->GetKey(SDLK_o))     NewChar('ó'); else
                    if(App::Inst()->GetKey(SDLK_u))     NewChar('ú'); else
                    if(App::Inst()->GetKey(SDLK_y))     NewChar('ý');
                }
                if(C_DoorSystem::Inst()->GetWedge())
                {
                    if(App::Inst()->GetKey(SDLK_c))     NewChar('è'); else
                    if(App::Inst()->GetKey(SDLK_d))     NewChar('ï'); else
                    if(App::Inst()->GetKey(SDLK_n))     NewChar('ò'); else
                    if(App::Inst()->GetKey(SDLK_r))     NewChar('ø'); else
                    if(App::Inst()->GetKey(SDLK_s))     NewChar('š'); else
                    if(App::Inst()->GetKey(SDLK_t))     NewChar(''); else
                    if(App::Inst()->GetKey(SDLK_z))     NewChar('ž');
                }

                if(App::Inst()->GetKey(']'))         NewChar(')'); else
                if(App::Inst()->GetKey('`'))         NewChar(';'); else
                if(App::Inst()->GetKey(SDLK_SLASH))  NewChar('-'); else
                if(App::Inst()->GetKey(SDLK_2))      NewChar('ì'); else
                if(App::Inst()->GetKey(SDLK_3))      NewChar('š'); else
                if(App::Inst()->GetKey(SDLK_4))      NewChar('è'); else
                if(App::Inst()->GetKey(SDLK_5))      NewChar('ø'); else
                if(App::Inst()->GetKey(SDLK_6))      NewChar('ž'); else
                if(App::Inst()->GetKey(SDLK_7))      NewChar('ý'); else
                if(App::Inst()->GetKey(SDLK_8))      NewChar('á'); else
                if(App::Inst()->GetKey(SDLK_9))      NewChar('í'); else
                if(App::Inst()->GetKey(SDLK_0))      NewChar('é'); else
                if(App::Inst()->GetKey('['))         NewChar('ú'); else
                if(App::Inst()->GetKey(';'))         NewChar('ù');
            }
            break;
    }
    if(App::Inst()->GetKey(' '))         NewChar(' '); else
    if(App::Inst()->GetKey(','))         NewChar(','); else
    if(App::Inst()->GetKey('.'))         NewChar('.'); else
    if(App::Inst()->GetKey(SDLK_KP0))    NewChar('0'); else
    if(App::Inst()->GetKey(SDLK_KP1))    NewChar('1'); else
    if(App::Inst()->GetKey(SDLK_KP2))    NewChar('2'); else
    if(App::Inst()->GetKey(SDLK_KP3))    NewChar('3'); else
    if(App::Inst()->GetKey(SDLK_KP4))    NewChar('4'); else
    if(App::Inst()->GetKey(SDLK_KP5))    NewChar('5'); else
    if(App::Inst()->GetKey(SDLK_KP6))    NewChar('6'); else
    if(App::Inst()->GetKey(SDLK_KP7))    NewChar('7'); else
    if(App::Inst()->GetKey(SDLK_KP8))    NewChar('8'); else
    if(App::Inst()->GetKey(SDLK_KP9))    NewChar('9');
}

void C_Input::AddNumber()
{
    int loop1;

    //if(title.length() == 0)   // na prvni pozici muze byt minus
    {
        /*
        if(App::Inst()->GetKey(SDLK_MINUS) || App::Inst()->GetKey(SDLK_KP_MINUS))
        {
            title.clear();
            pos = 0;
            NewChar(SDLK_MINUS);
        }*/
    }

    for (loop1 = SDLK_0; loop1 <= SDLK_9; loop1++)
    {
        if (App::Inst()->GetKey(loop1))
        {
            NewChar(loop1);
        }
    }

    for (loop1 = SDLK_KP0; loop1 <= SDLK_KP9; loop1++)
    {
        if (App::Inst()->GetKey(loop1))
        {
            NewChar(loop1 - SDLK_KP0 + SDLK_0);
        }
    }
}

void C_Input::AddChar()
{
    AddVisibleChar();
    AddLetter();
    AddNumber();
}

void C_InputInteger::CheckValue()
{
    if(!(min == max && max == -1))
    {
        if(GetInteger() < min)
            title = ToString(min);
        else if(GetInteger() > max && max != -1)
            title = ToString(max);
    }
}

void C_InputInteger::AddChar()
{
    AddNumber();
}

void C_InputDouble::AddChar()
{
    AddNumber();

    if(App::Inst()->GetKey(SDLK_PERIOD) && title.find('.') == string::npos)
    {
        if(title.size() == 0)
            title.push_back('0');
        title.push_back(SDLK_PERIOD);
    }
}

void C_Input::Draw()
{
    if(visible)
    {

        if (!editable)
            glColor3ubv(colorNormal);
        else if (active)
            glColor3ubv(colorActive);
        else if (focus)
            glColor3ubv(colorFocus);
        else
            glColor3ubv(colorNormal);

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glVertex2i(x, y);
        glVertex2i(x + width, y);
        glVertex2i(x + width, y + height);
        glVertex2i(x, y + height);
        glEnd();

        glColor3ub(0, 33, 33);
        glBegin(GL_LINES);
        glVertex2i(x, y);
        glVertex2i(x + width, y);

        glVertex2i(x + width, y);
        glVertex2i(x + width, y + height);

        glVertex2i(x + width, y + height);
        glVertex2i(x, y + height);

        glVertex2i(x, y + height);
        glVertex2i(x, y);
        glEnd();
        glEnable(GL_TEXTURE_2D);

        glColor3ubv(colorTitle);

        string temp1, temp2;
        temp1 = title;
        if(active && (unsigned) pos < nLines * lineLength)
            temp1.insert(pos, string("^"));
        if(title.size() == nLines * lineLength && pos != (signed) title.size())
            temp1.erase(temp1.end() - 1, temp1.end());
        unsigned int line = 0;
        do
        {
            if(line % 2 == 0)
                glColor3ubv(colorTitle);
            else
                glColor3ubv(colorTitle - 20);

            temp2 = temp1.substr(lineLength * line, lineLength);
            FontMan::Inst()->Print("lucida16", x + 3, y + (LINE_HEIGHT * (line + 1)) - 4, "%s", temp2.c_str());
            line++;
        } while(temp2.size() == lineLength);
    }
}

bool C_Input::IsMouseOver(unsigned int mouseX, unsigned int mouseY)
{
    if (visible &&
        mouseX >= (unsigned) x && mouseX <= (unsigned) (x + width) &&
        mouseY >= (unsigned) y && mouseY <= (unsigned) (y + height))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool C_Input::IsPushed(unsigned int mouseX, unsigned int mouseY)
{
    if (App::Inst()->mouseNewLeftDown && IsMouseOver(mouseX, mouseY))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void C_Input::SetActive(bool n_active)
{
    active = n_active;
    if(active)
        indexActive = index;
}
