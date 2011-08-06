#include "Form.h"
#include "Input.h"

void C_Label::InitLabel()
{
    editable = false;
}

C_Label::C_Label()
{
    InitLabel();
}

C_Label::C_Label(int n_x, int n_y, int n_lineLength, int n_nLines, string n_title, string n_help, char n_tR, char n_tG, char n_tB, char n_pR, char n_pG, char n_pB, char n_fR, char n_fG, char n_fB, char n_aR, char n_aG, char n_aB) : C_Input(n_x, n_y, n_lineLength, n_nLines, n_title, n_help, n_tR, n_tG, n_tB, n_pR, n_pG, n_pB, n_fR, n_fG, n_fB, n_aR, n_aG, n_aB)
{
    InitLabel();
}

C_Label::~C_Label()
{

}
