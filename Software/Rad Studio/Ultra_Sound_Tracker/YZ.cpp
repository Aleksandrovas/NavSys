//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "YZ.h"
#include "Unit2.h"
#include "XY.h"
#include "Main.h"
#include "XZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{
Form4->Visible=false;
Form1->YZ=false;
Form1->Button6->Caption="XZ - Isjungta" ;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormCreate(TObject *Sender)
{
Form4->Visible=false;
Form4->PaintBox1->Width=Form1->Ilgiss/2;
Form4->PaintBox1->Height=Form1->Aukstiss/2;
}
//---------------------------------------------------------------------------
