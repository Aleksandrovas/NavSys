//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "Main.h"
#include "XY.h"
#include "YZ.h"
#include "XZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
	//ilgis=StrToInt(Edit1->Text);
	//plotis=StrToInt(Edit2->Text);
	//aukstis=StrToInt(Edit3->Text);
	ilgis=StrToInt(MaskEdit1->Text);
	plotis=StrToInt(MaskEdit2->Text);
	aukstis=StrToInt(MaskEdit3->Text);

   /*
	if ((plotis>700)||(ilgis>700))
	{
		Form1->Ilgiss=ilgis/2;
		Form1->Plotiss=plotis/2;
		Form1->Aukstiss=aukstis/2;
		Form1->k=0.5;
	}  */

	if ((400<plotis<=700)||(400<ilgis<=700))
	{
		Form1->Ilgiss=ilgis*1.5;
		Form1->Plotiss=plotis*1.5;
		Form1->Aukstiss=aukstis*1.5;
		Form1->k=1.5;
	}
	   /*
	if ((1<plotis<=400)||(1<ilgis<=400))
	{
		Form1->Ilgiss=ilgis*2;
		Form1->Plotiss=plotis*2;
		Form1->Aukstiss=aukstis*2 ;
		Form1->k=2;

	}  */

Form3->PaintBox1->Width=Form1->Plotiss/2;
Form3->PaintBox1->Height=Form1->Ilgiss/2;

Form4->PaintBox1->Width=Form1->Ilgiss/2;
Form4->PaintBox1->Height=Form1->Aukstiss/2;

Form5->PaintBox1->Width=Form1->Plotiss/2;
Form5->PaintBox1->Height=Form1->Aukstiss/2;


}
//---------------------------------------------------------------------------









void __fastcall TForm2::Button2Click(TObject *Sender)
{
   switch (Form2->ComboBox1->ItemIndex)
   {
		case 0 :
				Form1->Kp[0]=StrToFloat(Form2->Edit4->Text);
				break;
		case 1 :
				Form1->Kp[1]=StrToFloat(Form2->Edit4->Text);
				break;
		case 2 :
				Form1->Kp[2]=StrToFloat(Form2->Edit4->Text);
				break;
		case 3 :
				Form1->Kp[3]=StrToFloat(Form2->Edit4->Text);
				break;
		default :
				ShowMessage("Pasirinkite taska");

				break;
   }

}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button3Click(TObject *Sender)
{
Form2->Visible=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm2::Edit1KeyPress(TObject *Sender, char &Key)
{

if ((Key!=	VK_NUMPAD1)||Key!=	VK_NUMPAD0) {
ShowMessage("Veskite tik skaicius");
    
}
/*switch (Key)
   {
		case 0 : break;
		case 1 : break;
		case 2 : break;
		case 3 : break;
		case 4 : break;
		case 5 : break;
		case 6 : break;             k
		case 7 : break;
		case 8 : break;
		case 9 : break;
		default :
        	Form2->Edit1->Text="";
			Form2->Edit1->Text=IntToStr(Form1->Ilgiss);
			ShowMessage("Veskite tik skaicius");
				break;  

}   */
}
//---------------------------------------------------------------------------


