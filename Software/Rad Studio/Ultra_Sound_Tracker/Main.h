//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Math.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TButton *Button3;
  TTimer *ConTimer;
        TLabel *Label1;
	TOpenDialog *OpenDialog1;
	TMemo *Memo5;
	TButton *Button1;
	TTimer *Timer1;
	TButton *Button7;
	TButton *Button8;
	TLabel *Label2;
	TButton *Button2;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TMemo *Memo1;
	TButton *Button9;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall ConTimerTimer(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

	unsigned int VID,PID,Ilgiss,Plotiss,Aukstiss;
	int X,Y,Z;
	float Xxy,Yxy,k,Kp[4];
	bool XY,YZ,XZ;

	String pos_File;

	//static unsigned char pos0=0;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
