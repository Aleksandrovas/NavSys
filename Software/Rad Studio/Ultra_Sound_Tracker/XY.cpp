//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "XY.h"
#include "Unit2.h"
#include "Main.h"
#include "YZ.h"
#include "XZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button1Click(TObject *Sender)
{
Form3->Visible=false;
Form1->XY=false;
Form1->Button4->Caption="XY - Isjungta" ;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject *Sender)
{
Form3->Visible=false;
Form3->PaintBox1->Width=Form1->Plotiss/2;
Form3->PaintBox1->Height=Form1->Ilgiss/2;
}
//---------------------------------------------------------------------------
