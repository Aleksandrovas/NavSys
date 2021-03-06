//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "winuser.h"
#include "stdio.h"
#include "math.h"
#include "time.h"
#include "AtUsbHid.h"
#include "Unit2.h"
#include "XY.h"
#include "YZ.h"
#include "XZ.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


#define wait_delay 12
TForm1 *Form1;
unsigned char buferis[64];
bool not_connected,bol=false;
unsigned char alivecnt=0;
static unsigned char pos0=0;
int xb,yb,zb,i,s,x,y,z;
int Xs[10],Ys[10],Zs[10],Bs=0;


int xv, yv,zv,zv_old;
int xofs,yofs,zofs,yv_old,xv_old;
float xp,yp,zp;
float mast; //lango dydzio mastelio daugiklis
TDateTime today;



//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
//Trilateracijos algoritmas grazina x,y,z koordinates
//---------------------------------------------------------------------------
bool skaiciuot(int *t,int *temp,unsigned char count)
{
	bool ta1,ta2,ta3,ta4;
	float xvid4,yvid4,zvid,xt,yt;
	float xt11,xt12,xt13,xt22,xt23,xt33;
	float r[4],rb[4],greitis[4];
	float g;
	float d,x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4,i1,i2,i3,i4,j1,j2,j3,j4,xg,yg,zg;

	for (int i=0; i < count; i++)
	{
		if ((t[i]!=0))
		   {
			r[i]=(t[i]*3.387*10000/61379+12.108)*Form1->Kp[i];
			pos0=1;
			r[i]=r[i]*Form1->k;
		   }
		else
			r[i]=0;
	}

	i1=Form1->Ilgiss;
	j1=Form1->Plotiss;
	i2=Form1->Plotiss;
	j2=Form1->Ilgiss;
	i3=Form1->Ilgiss;
	j3=Form1->Plotiss;
	i4=Form1->Plotiss;
	j4=Form1->Ilgiss;
	//T1
	if ((r[0]>0)&&(r[1]>0)&&(r[2]>0))
	{
		ta1=true;
		x1=(r[0]*r[0]-r[1]*r[1]+i1*i1)/(2*i1);
		y1=((r[0]*r[0]-r[2]*r[2]+i1*i1+j1*j1)/(2*j1))-(i1/j1)*x1;
		if (((r[0]*r[0])-(x1*x1)-(y1*y1))>0)
		{
			z1=sqrt((r[0]*r[0])-(x1*x1)-(y1*y1));
		}
		else
			z1=0;
	}
	//T2
	if((r[3]>0)&&(r[1]>0)&&(r[2]>0))
	{
		ta2=true;
		y2=(r[1]*r[1]-r[2]*r[2]+i2*i2)/(2*i2);
		x2=(i2*i2+j2*j2+r[1]*r[1]-r[3]*r[3]-(i2*(((r[1]*r[1])-(r[2]*r[2])+(i2*i2))/i2)))/(2*j2);
		if  (((r[1]*r[1])-(x2*x2)-(y2*y2))>0)
		{
			z2=sqrt((r[1]*r[1])-(x2*x2)-(y2*y2));
		}
		else
			z2=0;
	}
	//T3

	if((r[3]>0)&&(r[0]>0)&&(r[2]>0))
	{
		ta3=true;
		x3=(r[2]*r[2]-r[3]*r[3]+i3*i3)/(2*i3);
		y3=(i3*i3+j3*j3+r[2]*r[2]-r[0]*r[0]-(i3*(((r[2]*r[2])-(r[3]*r[3])+(i3*i3))/i3)))/(2*j3);
		if (((r[2]*r[2])-(x3*x3)-(y3*y3))>0)
			z3=sqrt((r[2]*r[2])-(x3*x3)-(y3*y3));
		else
			z3=0;
	}

	//T4
	if((r[0]>0)&&(r[1]>0)&&(r[3]>0))
	{
		ta4=true;
		y4=(r[3]*r[3]-r[0]*r[0]+i4*i4)/(2*i4);
		x4=(i4*i4+j4*j4+r[3]*r[3]-r[1]*r[1]-(i4*(((r[3]*r[3])-(r[0]*r[0])+(i4*i4))/i4)))/(2*j4);
		if (((r[3]*r[3])-(x4*x4)-(y4*y4))>0)
			z4=sqrt((r[3]*r[3])-(x4*x4)-(y4*y4));
		else
			z4=0;
	}
	x2=-x2+Form1->Ilgiss;
	x3=-x3+Form1->Ilgiss;
	y3=-y3+Form1->Plotiss;
	y4=-y4+Form1->Plotiss;

	if((ta1==false)&&(ta2==false)&&(ta3==false)&&(ta4==false))
	{
		x=xb,yb=y,zb=z;
		Form1->Label1->Caption="Visi Neveikia";
		Form1->Label1->Font->Color=clRed;
	}
	if((ta1==true)&&(ta2==true)&&(ta3==true)&&(ta4==true))
	{
		ta1=false;
		ta2=false;
		ta3=false;
		ta4=false;

		xt11=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
		xt12=sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
		xt13=sqrt((x1-x4)*(x1-x4)+(y1-y4)*(y1-y4));
		xt22=sqrt((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3));
		xt23=sqrt((x2-x4)*(x2-x4)+(y2-y4)*(y2-y4));
		xt33=sqrt((x3-x4)*(x3-x4)+(y3-y4)*(y3-y4));
		xt=x2-xt11/2;
		yt=y2-xt13/2;
		xvid4=(x1+x2+x3+x4+xt)/5;
		yvid4=(y1+y2+y3+y4+yt)/5;
		x=xvid4;
		y=yvid4;
		z=(z1+z2+z3+z4)/4;
		xb=x;yb=y;zb=z;
		Form1->Label1->Caption="Visi Veikia";
		Form1->Label1->Font->Color=clGreen;
		}
	else if (ta1==true)
	{
		x=x1;
		y=y1;
		z=z1;
		xb=x;yb=y;zb=z;
		Form1->Label1->Caption="Vienas neveikia";
		Form1->Label1->Font->Color=clOlive;
	}
	else if (ta2==true)
	{
		x=x2;
		y=y2;
		z=z2;
		xb=x;yb=y;zb=z;
		Form1->Label1->Caption="Vienas neveikia";
		Form1->Label1->Font->Color=clOlive;
	}
	else if (ta3==true)
	{
		x=x3;
		y=y3;
		z=z3;
		xb=x;yb=y;zb=z;
		Form1->Label1->Caption="Vienas neveikia";
		Form1->Label1->Font->Color=clOlive;
	}
	else if (ta4==true)
	{
		x=x4;
		y=y4;
		z=z4;
		xb=x;yb=y;zb=z;
		Form1->Label1->Caption="Kazkuris neveikia";
		Form1->Label1->Font->Color=clOlive;
	}


	xb=x;
	yb=y;
	zb=z;
	for (i = 0; i < 4; i++)
	rb[i]=r[i];

	 // Form1->Memo5->Lines->Add(FloatToStr(r[0]/Form1->k)+"\t"+FloatToStr(r[1]/Form1->k)+"\t"+FloatToStr(r[2]/Form1->k));
	//Form1->Memo5->Lines->Add(FloatToStr(r[0]/Form1->k)+"\t"+FloatToStr(r[1]/Form1->k)+"\t"+FloatToStr(r[2]/Form1->k)+"\t"+FloatToStr(r[3]/Form1->k)+"\t");
	Form1->Memo5->Lines->Add("R1="+FloatToStr(r[0]/Form1->k));
	Form1->Memo5->Lines->Add("R2="+FloatToStr(r[1]/Form1->k));
	Form1->Memo5->Lines->Add("R3="+FloatToStr(r[2]/Form1->k));
	Form1->Memo5->Lines->Add("R4="+FloatToStr(r[3]/Form1->k));
	Form1->Memo5->Lines->Add("-----------------") ;   
	 xv=(int)x/Form1->k;
	 yv=(int)y/Form1->k;

	 zv_old=zv;


	 zv=(int)z/Form1->k;
	 zv=-0.0021*zv*zv + 2.4737*zv - 305.99;

	 zv=zv_old*0.8+zv*0.2;
	 //Form1->Memo5->Lines->Add("X="+IntToStr(x)+" Y="+IntToStr(y)+" Z="+IntToStr(z));
	 //Form1->Memo5->Lines->Add("t1="+IntToStr(temp[0])+" t2="+IntToStr(temp[1])+" t3="+IntToStr(temp[3]));
	 //Form1->Memo5->Lines->Add("-----------------") ;
	 Form1->Label3->Caption=("X=" + IntToStr(xv));
	 Form1->Label4->Caption=("Y=" + IntToStr(yv));
	 Form1->Label5->Caption=("Z=" + IntToStr(zv));
   //	 Form1->Memo5->Lines->Add(IntToStr(xv)+"\t"+IntToStr(yv)+"\t"+IntToStr(zv));
}
//--------------------------------------------------------------
//Ieskoma ir jungiamasi prie USB
//--------------------------------------------------------------
bool connect(const UINT VendorID, const UINT ProductID)
{
  bool ans;
  ans= findHidDevice(VendorID, ProductID);
  if (!ans)
  {
		not_connected=true;
		Form1->VID=VendorID ;
		Form1->PID= ProductID;
		Form1->Label2->Caption="USB device disconnected";
		Form1->Label2->Font->Color=clRed;
  }
  else
  {
		Form1->Label2->Caption="USB device connected";
		Form1->Label2->Font->Color=clGreen;
  }
  return  ans;
}
//---------------------------------------
//Atjungiamas USB
//---------------------------------------
void disconnect()
{
    closeDevice();
}
//---------------------------------------
//Tikrinama ar USB prijungtas
//---------------------------------------
bool test_alive()
{
	unsigned char buf[64];
	int k=0;
	buf[0]=254;
	writeData(buf);
		while((readData(buf)==false) && (k++<wait_delay)) Sleep(1);
	return (k<wait_delay);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	ConTimer->Enabled=false;
	disconnect();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	static unsigned char pos0=0;
	int kkk=0,i=0,j=0;
	int laikas[4],temp[4],nr[4];
   //	float atstumas=0;
	buferis[0]=0;
	buferis[1]=0;
	kkk=0;


	// initialize variables
	for (i = 0; i < 4; i++) {
	 laikas[i]=0;
	 temp[i]=0;
	 nr[i]=0;
	}

	//read data from USB, with wait_delay timeout in ms
	while((readData(buferis)==false) && (kkk++<wait_delay))
	{
		Sleep(1);
	}
	  //no timeout?
	  if (kkk<wait_delay)
	  {   // check if first byte is 12
		  if (buferis[0]==12)
		  {
				//read data from the packet
				for (int i=0;i<buferis[1];i++)
				{
					 //	atstumas=(buferis[2+i*5+1]*256+buferis[2+i*5+2])*3.387*10000/61379+12.108;

						   if (buferis[2+i*5]==0) {
							nr[0]= buferis[2+i*5];
							laikas[0]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
							temp[0]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
						   }
							if (buferis[2+i*5]==1) {
							nr[1]= buferis[2+i*5];
							laikas[1]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
							temp[1]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
						   }
						   if (buferis[2+i*5]==2) {
							nr[2]= buferis[2+i*5];
							laikas[2]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
							temp[2]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
						   }
						   if (buferis[2+i*5]==3) {
							nr[3]= buferis[2+i*5];
							laikas[3]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
							temp[3]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
						   }
				}
		  }


		Form3->PaintBox1->Canvas->Pen->Color=clWhite;
		Form4->PaintBox1->Canvas->Pen->Color=clWhite;
		Form5->PaintBox1->Canvas->Pen->Color=clWhite;
		if ((x>0)&&(y>0)&&(pos0>0))
		{
			Form3->PaintBox1->Canvas->MoveTo(y/2,(x)/2);
			Form4->PaintBox1->Canvas->MoveTo(x/2,(z)/2);
			Form5->PaintBox1->Canvas->MoveTo(y/2,(z)/2);
		}

		skaiciuot(laikas,temp,4);
		//XY koordinatemis
		Form3->PaintBox1->Canvas->Rectangle(0,0,Plotiss/2,Ilgiss/2);  //-x           //-x
		Form3->PaintBox1->Canvas->Ellipse((y-10)/2,(x+10)/2,(y+10)/2,(x-10)/2);
		Form3->PaintBox1->Canvas->TextOutA((y+20)/2,(x-30)/2,"X="+IntToStr((int)((x/Form1->k)))) ;
		Form3->PaintBox1->Canvas->TextOutA((y+20)/2,(x)/2,"Y="+IntToStr((int)((y/Form1->k)))) ;
		if ((x>0)&&(y>0)&&(pos0>0))
			Form3->PaintBox1->Canvas->LineTo(y/2,(x)/2);
		//YZ koordinatemis
		Form4->PaintBox1->Canvas->Rectangle(0,0,Ilgiss/2,Aukstiss/2);
		Form4->PaintBox1->Canvas->Ellipse((x-10)/2,(z+10)/2,(x+10)/2,(z-10)/2);
		Form4->PaintBox1->Canvas->TextOutA((x+20)/2,(z-30)/2,"X="+IntToStr((int)((x/Form1->k)))) ;
		Form4->PaintBox1->Canvas->TextOutA((x+20)/2,(z)/2,"Z="+IntToStr((int)((z/Form1->k)))) ;
		if ((x>0)&&(y>0)&&(pos0>0))
			Form4->PaintBox1->Canvas->LineTo(x/2,(z)/2);
		//XZ koordinatemis
		Form5->PaintBox1->Canvas->Rectangle(0,0,Plotiss/2,Aukstiss/2);
		Form5->PaintBox1->Canvas->Ellipse((y-10)/2,(z+10)/2,(y+10)/2,(z-10)/2);
		Form5->PaintBox1->Canvas->TextOutA((y+20)/2,(z-30)/2,"Y="+IntToStr((int)((y/Form1->k)))) ;
		Form5->PaintBox1->Canvas->TextOutA((y+20)/2,(z)/2,"Z="+IntToStr((int)((z/Form1->k)))) ;
		if ((x>0)&&(y>0)&&(pos0>0))
			Form5->PaintBox1->Canvas->LineTo(y/2,(z)/2);

		if (pos0!=1)
			pos0=1;

      Xs[9]=x/2;
	   Ys[9]=y/2;
	   Zs[9]=z/2;
       if (Bs<9) {

		  for (j = 0; j <= Bs; j++) {
				Xs[j]=Xs[j+1];
				Ys[j]=Ys[j+1];
				Zs[j]=Zs[j+1];
               if (j==Bs)
				{
					Xs[Bs]=x/2;
					Ys[Bs]=y/2;
					Zs[Bs]=z/2;
				}
		  }
		  Bs++;
	  }
       Xs[9]=x/2;
	   Ys[9]=y/2;
	   Zs[9]=z/2;

	  if (Bs==9)
		 {
		  for (j = 0; j < Bs; j++)
			{
				Xs[j]=Xs[j+1];
				Ys[j]=Ys[j+1];
				Zs[j]=Zs[j+1];
			if (j==Bs)
				{
					Xs[Bs]=x/2;
					Ys[Bs]=y/2;
					Zs[Bs]=z/2;
				}
		  }

	  }        

	  if (Bs==9) {
		  Form3->PaintBox1->Canvas->MoveTo(Ys[0],Xs[0]);
		  Form4->PaintBox1->Canvas->MoveTo(Xs[0],Zs[0]);
		  Form5->PaintBox1->Canvas->MoveTo(Ys[0],Zs[0]);
	  }
	  //Form1->Memo5->Lines->Add("=====================   ");
	  if (Bs==9)
	   {
		for (j = 0; j < 10; j++) {

		  Form3->PaintBox1->Canvas->LineTo(Ys[j],Xs[j]);
		  Form4->PaintBox1->Canvas->LineTo(Xs[j],Zs[j]);
		  Form5->PaintBox1->Canvas->LineTo(Ys[j],Zs[j]);
		  Form3->PaintBox1->Canvas->MoveTo(Ys[j],Xs[j]);
		  Form4->PaintBox1->Canvas->MoveTo(Xs[j],Zs[j]);
		  Form5->PaintBox1->Canvas->MoveTo(Ys[j],Zs[j]);
		 // Form1->Memo5->Lines->Add(IntToStr(Xs[j])+"   ");
	   }
	  }

//-------------------------------
//

	today = Now();

	   Form1->Memo1->Lines->Add("data: " +DateToStr(today)
								+" laikas : "+TimeToStr(today)
								+ " X: " +IntToStr((int)((x/Form1->k)))
								+ " Y: "+IntToStr((int)((y/Form1->k)))
								+" Z: "+IntToStr((int)((z/Form1->k))));

	  }


	 // scale the values
	 xv_old=xp;
	 yv_old=yp;
	 xp=(float)(xv-xofs)/100;
	 yp=(float)(yv-yofs)/100;
	 zp=(float)(zv-zofs)/100;

	 zp=0;

	 if ((abs(xv_old)-abs(xp))>0.2) {
	  xp=xv_old;
	 }

	 if ((abs(yv_old)-abs(yp))>0.2) {
	  yp=yv_old;
	 }

	 FILE *fp;
	// char buff[100];
	// strcpy(buff,pos_file,pos_File.Length());
	 //	  pos_File.c_str()
	 fp = fopen ("C:\\web\\pos.txt", "w");
	 fprintf(fp, "%4.2f %4.2f %4.2f\n", xp,yp,zp);
	 fflush(fp);
	 fclose(fp);


}


//---------------------------------------------------------------------------
void __fastcall TForm1::ConTimerTimer(TObject *Sender)
{
  //Application->ProcessMessages();
  if (not_connected)
  {not_connected=false;

	connect(VID,PID);
	}
  else
  {
  if (!test_alive())
	connect(VID,PID);
  }

}
//i atskira f=ja
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
 // connect(0x03EB,0x2013);
	VID=0x03EB;
	PID=0x2013;
	connect(VID,PID);

	Form1->Kp[0]=0.99;
	Form1->Kp[1]=0.99;
	Form1->Kp[2]=1;
	Form1->Kp[3]=0.97;
	/*      Sena kalibracija
	Form1->Kp[0]=0.98969767;
	Form1->Kp[1]=0.963711971;
	Form1->Kp[2]=1;
	Form1->Kp[3]=0.996654443;
							  */


	Form1->XY=true;
	Form1->YZ=true;
	Form1->XZ=true;
	Form1->Ilgiss=455;
	Form1->Plotiss=455;
	Form1->Aukstiss=256;
	Form1->k=1.5;





   if ((Form1->Plotiss>700)&&(Form1->Ilgiss>700))
	{
		Form1->Ilgiss=Form1->Ilgiss/2;
		Form1->Plotiss=Form1->Ilgiss/2;
		Form1->Aukstiss=Form1->Aukstiss/2;
		Form1->k=0.5;
	}

	if ((400<Form1->Plotiss<=700)&&(400<Form1->Ilgiss<=700))
	{
		Form1->Ilgiss=Form1->Ilgiss*1.5;
		Form1->Plotiss=Form1->Plotiss*1.5;
		Form1->Aukstiss=Form1->Aukstiss*1.5 ;
		Form1->k=1.5;
	}

	if ((1<Form1->Plotiss<=400)&&(1<Form1->Ilgiss<=400))
	{
		Form1->Ilgiss=Form1->Ilgiss*2;
		Form1->Plotiss=Form1->Plotiss*2;
		Form1->Aukstiss=Form1->Aukstiss*2;
		Form1->k=2;
	}






   //	ConTimer->Enabled=true;


}
//---------------------------------------------------------------------------

//nustatymam
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	Form2->Visible=true;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
	if (YZ==false)
	{
		YZ=true;
		Form4->Visible=true;
	}
	 else
	 {
	 Form4->Visible=false;
	  YZ=false;
	 }
}
//---------------------------------------------------------

//---------------------------------------------------------------------------

//---nereiks
void __fastcall TForm1::Button6Click(TObject *Sender)
{
	if (XZ==false)
	{
		Form5->Visible=true;
		XZ=true;
	}
	 else
	 {
	  Form5->Visible=false;
	  XZ=false;
	 }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	Button3->Click();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
	Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
  if (not_connected)
  {not_connected=false;

	connect(VID,PID);
	}
  else
  {
  if (!test_alive())
	connect(VID,PID);
  }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button4Click(TObject *Sender)
{

	if (XY==false)
	{
	Form3->Visible=true;
		XY=true;
	}
	 else
	 {
	  Form3->Visible=false;
	  XY=false;
     }


}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////

void __fastcall TForm1::Button8Click(TObject *Sender)
{
 /*
//////--------------------------paisymas
int laikas[4],temp[4],nr[4],i,j ;
Timer1->Enabled=false;
unsigned short dien,met,men,val,min,sec,ss;
//char dieni[2],meti[4],mensi[2],vali[2],mini[2],seci[2];
AnsiString dieni,meti,mensi,vali,mini,seci;
unsigned char eil;

today=Now();


//data_dabar=DateToStr(today);
//DecodeDate(today, met, men, dien);
DecodeTime(today,val, min,sec,ss);
dieni=IntToStr(dien);
meti=IntToStr(met);
mensi=IntToStr(men);
vali=IntToStr(val);
mini=IntToStr(min);
seci=IntToStr(sec);

Form1->Memo1->Lines->SaveToFile(meti+"_"+mensi+"_"+dieni+"_"+vali+"_"+mini+"_"+seci+"_dat.txt");
Form1->Memo1->Lines->Clear();
   */
}


void __fastcall TForm1::Button9Click(TObject *Sender)
{
	xofs=xv;
	yofs=yv;
	zofs=zv;
}
//---------------------------------------------------------------------------




