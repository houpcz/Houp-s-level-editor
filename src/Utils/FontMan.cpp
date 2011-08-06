#include "FontMan.h "
#include <cstring>

using namespace std;

FontMan * FontMan::inst = NULL;

FontMan * FontMan::Inst()
{
    if (inst == NULL)
    {
        inst = new FontMan;
    }
    return inst;
}

FontMan::FontMan()
{
        anchor = ALIGN_LEFT | ALIGN_BOTTOM;
}

FontMan::~FontMan()
{
    if(inst != NULL)
        delete inst;
    inst = NULL;
}

int FontMan::GetTextWidth(const char * name, const wchar_t * text)
{
    Font * font = fontMap[name];

    if(font == NULL)
        return -1;

    return font->GetTextWidth(text);
}

inline void FontMan::AlignTextCoord(float & dx, float & dy, int width, int height)
{
    if(anchor & ALIGN_BOTTOM)  dy = -height; else
    if(anchor & ALIGN_VCENTER) dy = -(height >> 1);

    if(anchor & ALIGN_RIGHT)   dx = -width; else
    if(anchor & ALIGN_HCENTER) dx = -(width >> 1);
}

void FontMan::Print(const char * fontName, float x, float y, const char *fmt, ...)  {
	// We want a coordinate system where things coresponding to window pixels.
	PushScreenCoordinateMatrix();

    char		OLDtext[256];							// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		*OLDtext=0;											// Do Nothing
	else {
        va_start(ap, fmt);									// Parses The String For Variables
            vsprintf(OLDtext, fmt, ap);						// And Converts Symbols To Actual Numbers
        va_end(ap);											// Results Are Stored In Text
	}

    Font * font = fontMap[fontName];
    if(font == NULL)
    {
        fprintf(stderr, "Font with name %s doesn't exist. I can't print %s.\n", fontName, OLDtext);
        return;
    }
	GLuint listID = font->GetDisplayListID();
	float textHeihgt = font->GetHeight();
    float lineHeight = textHeihgt / 0.63f;
	//Here is some code to split the text that we have been
	//given into a set of lines.
	//This could be made much neater by using
	//a regular expression library such as the one avliable from
	//boost.org (I've only done it out by hand to avoid complicating
	//this tutorial with unnecessary library dependencies).

	wchar_t	* NEWtext;

	NEWtext = ToWChar(OLDtext);

	const wchar_t *start_line=NEWtext;
	vector<wstring> lines;

    wchar_t *c;

	for(c=NEWtext;*c;c++) { //puvodene const wchar_t *c=NEWtext
		if(*c=='\n') {
			wstring line;
			for(const wchar_t *n=start_line;n<c;n++) line.append(1,*n);
			lines.push_back(line);
			start_line=c+1;
		}
	}
	if(start_line) {
		wstring line;
		for(const wchar_t *n=start_line;n<c;n++) line.append(1,*n);
		lines.push_back(line);
	}

	/*
	for(c=NEWtext ;*c;c++) {
		if(*c=='\n') {
			wstring line;
			for(const int *n=start_line;n<c;n++) line.append(1,*n);
			lines.push_back(line);
			start_line=c+1;
		}
	}
	if(start_line) {
		wstring line;
		for(const int *n=start_line;n<c;n++) line.append(1,*n);
		lines.push_back(line);
	}*/

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glListBase(listID);

	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//This is where the text display actually happens.
	//For each line of text we reset the modelview matrix
	//so that the line's text will start in the correct position.
	//Notice that we need to reset the matrix, rather than just translating
	//down by h. This is because when each character is
	//draw it modifies the current matrix so that the next character
	//will be drawn immediatly after it.
    float dx = 0.0f, dy = 0.0f;

	for(unsigned int loop1 = 0; loop1 < lines.size(); loop1++) {
	    AlignTextCoord(dx, dy, GetTextWidth(fontName, lines[loop1].c_str()), lineHeight * (lines.size() - 1) + textHeihgt);

		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x + dx, y + dy + (lineHeight * loop1) + textHeihgt, 0);
		glMultMatrixf(modelview_matrix);
		glCallLists(lines[loop1].length(), GL_UNSIGNED_SHORT, lines[loop1].c_str());//NEWCESTINA

		glPopMatrix();
	}
	glPopAttrib();

	PopProjectionMatrix();

	delete [] NEWtext;
}

wchar_t * FontMan::ToWChar(const char * text)
{
    wchar_t * newText;
    newText = new wchar_t[strlen(text) + 1];

    int ch;
	for(unsigned int loop1=0; loop1 < strlen(text); loop1++) {
		switch((unsigned) text[loop1]) {
		    case 'Á': ch = 193; break;
		    case 'á': ch = 225; break;
		    case 'É': ch = 201; break;
		    case 'é': ch = 233; break;
		    case 'Í': ch = 205; break;
		    case 'í': ch = 237; break;
		    case 'Ó': ch = 211; break;
		    case 'ó': ch = 243; break;
			case 'È': ch = 268; break;
			case 'è': ch = 269; break;
            case 'Ï': ch = 270; break;
			case 'ï': ch = 271; break;
			case 'Ì': ch = 282; break;
            case 'ì': ch = 283; break;
			case 'Ò': ch = 327; break;
			case 'ò': ch = 328; break;
			case 'Ø': ch = 344; break;
			case 'ø': ch = 345; break;
			case 'Š': ch = 352; break;
			case 'š': ch = 353; break;
			case '': ch = 356; break;
			case '': ch = 357; break;
            case 'Ù': ch = 366; break;
			case 'ù': ch = 367; break;
			case 'Ú': ch = 218; break;
			case 'ú': ch = 250; break;
			case 'Ý': ch = 221; break;
			case 'ý': ch = 253; break;
			case 'Ž': ch = 381; break;
			case 'ž': ch = 382; break;
			default :
				ch = (int) text[loop1];
				break;
		}

		newText[loop1]= ch;
	}
    newText[strlen(text)] = '\0';

	return newText;
}

void FontMan::MakeFont(const char * name, const char * src, unsigned int h)
{
    fontMap[string(name)] = new Font(src, h);
}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world
/// coordinates identical to window coordinates.
inline void FontMan::PushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint	viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

    glOrtho(0.0f, viewport[2], viewport[3], 0.0,-1.0f,1.0f);
    //fprintf(stderr, "%d %d %d %d\n", viewport[0],viewport[1],viewport[2],viewport[3]);
	//gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
	glPopAttrib();
}

/// Pops the projection matrix without changing the current MatrixMode.
inline void FontMan::PopProjectionMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
