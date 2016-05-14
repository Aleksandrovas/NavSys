#include <ansi_c.h>
//==============================================================================
//
// Title:		Trilateration.c
// Purpose:		A short description of the implementation.
//
// Created on:	2016.05.14 at 20:53:23 by Vytautas Deksnys.
// Copyright:	KTU. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "Trilateration.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?

int Trilat(Trilateration *T_data)
{
	char ta1,ta2,ta3,ta4,i;
	float xvid4,yvid4,zvid,xt,yt;
	float xt11,xt12,xt13,xt22,xt23,xt33;
	float r[4],rb[4],greitis[4];
	float g,t[5],pos0,x,y,z,xb,yb,zb,xv,yv,zv,zv_old;
	float d,x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4,i1,i2,i3,i4,j1,j2,j3,j4,xg,yg,zg;

	for (int i=0; i < 8; i++)
	{
		if ((t[i]!=0))
		   {
			r[i]=(t[i]*3.387*10000/61379+12.108);//*Form1->Kp[i];
			pos0=1;
			r[i]=r[i];//*Form1->k;
		   }
		else
			r[i]=0;
	}

//	i1=Form1->Ilgiss;
//	j1=Form1->Plotiss;
//	i2=Form1->Plotiss;
//	j2=Form1->Ilgiss;
//	i3=Form1->Ilgiss;
//	j3=Form1->Plotiss;
//	i4=Form1->Plotiss;
//	j4=Form1->Ilgiss;
	//T1

		ta1=1;//true;
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
		ta2=1;//true;
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
		ta3=1;//true;
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
		ta4=1;//true;
		y4=(r[3]*r[3]-r[0]*r[0]+i4*i4)/(2*i4);
		x4=(i4*i4+j4*j4+r[3]*r[3]-r[1]*r[1]-(i4*(((r[3]*r[3])-(r[0]*r[0])+(i4*i4))/i4)))/(2*j4);
		if (((r[3]*r[3])-(x4*x4)-(y4*y4))>0)
			z4=sqrt((r[3]*r[3])-(x4*x4)-(y4*y4));
		else
			z4=0;
	}
	x2=-x2;//+Form1->Ilgiss;
	x3=-x3;//+Form1->Ilgiss;
	y3=-y3;//+Form1->Plotiss;
	y4=-y4;//+Form1->Plotiss;

	if((ta1==0)&&(ta2==0)&&(ta3==0)&&(ta4==0))
	{
		x=xb,yb=y,zb=z;
		//Form1->Label1->Caption="Visi Neveikia";
		//Form1->Label1->Font->Color=clRed;
	}
	if((ta1==1)&&(ta2==1)&&(ta3==1)&&(ta4==1))
	{
		ta1=0;//false;
		ta2=0;//false;
		ta3=0;//false;
		ta4=0;//false;

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
	//	Form1->Label1->Caption="Visi Veikia";
	//	Form1->Label1->Font->Color=clGreen;
		}
	else if (ta1==1)
	{
		x=x1;
		y=y1;
		z=z1;
		xb=x;yb=y;zb=z;
	//	Form1->Label1->Caption="Vienas neveikia";
	//	Form1->Label1->Font->Color=clOlive;
	}
	else if (ta2==1)
	{
		x=x2;
		y=y2;
		z=z2;
		xb=x;yb=y;zb=z;
	//	Form1->Label1->Caption="Vienas neveikia";
	//	Form1->Label1->Font->Color=clOlive;
	}
	else if (ta3==1)
	{
		x=x3;
		y=y3;
		z=z3;
		xb=x;yb=y;zb=z;
	//	Form1->Label1->Caption="Vienas neveikia";
	//	Form1->Label1->Font->Color=clOlive;
	}
	else if (ta4==1)
	{
		x=x4;
		y=y4;
		z=z4;
		xb=x;yb=y;zb=z;
	//	Form1->Label1->Caption="Kazkuris neveikia";
	//	Form1->Label1->Font->Color=clOlive;
	}


	xb=x;
	yb=y;
	zb=z;
	for (i = 0; i < 4; i++)
	rb[i]=r[i];

	 // Form1->Memo5->Lines->Add(FloatToStr(r[0]/Form1->k)+"\t"+FloatToStr(r[1]/Form1->k)+"\t"+FloatToStr(r[2]/Form1->k));
	//Form1->Memo5->Lines->Add(FloatToStr(r[0]/Form1->k)+"\t"+FloatToStr(r[1]/Form1->k)+"\t"+FloatToStr(r[2]/Form1->k)+"\t"+FloatToStr(r[3]/Form1->k)+"\t");
   //	Form1->Memo5->Lines->Add("R1="+FloatToStr(r[0]/Form1->k));
   //	Form1->Memo5->Lines->Add("R2="+FloatToStr(r[1]/Form1->k));
   //	Form1->Memo5->Lines->Add("R3="+FloatToStr(r[2]/Form1->k));
   //	Form1->Memo5->Lines->Add("R4="+FloatToStr(r[3]/Form1->k));
   //	Form1->Memo5->Lines->Add("-----------------") ;
	 xv=(int)x;//Form1->k;
	 yv=(int)y;//Form1->k;

	 zv_old=zv;


	 zv=(int)z;//Form1->k;
	 zv=-0.0021*zv*zv + 2.4737*zv - 305.99;

	 zv=zv_old*0.8+zv*0.2;
	 //Form1->Memo5->Lines->Add("X="+IntToStr(x)+" Y="+IntToStr(y)+" Z="+IntToStr(z));
	 //Form1->Memo5->Lines->Add("t1="+IntToStr(temp[0])+" t2="+IntToStr(temp[1])+" t3="+IntToStr(temp[3]));
	 //Form1->Memo5->Lines->Add("-----------------") ;
	// Form1->Label3->Caption=("X=" + IntToStr(xv));
	// Form1->Label4->Caption=("Y=" + IntToStr(yv));
	// Form1->Label5->Caption=("Z=" + IntToStr(zv));
   //	 Form1->Memo5->Lines->Add(IntToStr(xv)+"\t"+IntToStr(yv)+"\t"+IntToStr(zv));
   return 1;
}
