//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "XZ.h"
#include "Unit2.h"
#include "XY.h"
#include "YZ.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm5::Button1Click(TObject *Sender)
{
Form5->Visible=false;
Form1->XZ=false;
Form1->Button5->Caption="XZ - Isjungta" ;
}
//---------------------------------------------------------------------------
void __fastcall TForm5::FormCreate(TObject *Sender)
{
Form5->Visible=false;
Form5->PaintBox1->Width=Form1->Plotiss/2;
Form5->PaintBox1->Height=Form1->Aukstiss/2;
}
//---------------------------------------------------------------------------
