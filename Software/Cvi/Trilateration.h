//==============================================================================
//
// Title:		Trilateration.h
// Purpose:		A short description of the interface.
//
// Created on:	2016.05.14 at 20:53:23 by Vytautas Deksnys.
// Copyright:	KTU. All Rights Reserved.
//
//==============================================================================

#ifndef __Trilateration_H__
#define __Trilateration_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
#include "Nav_CVI.h"
//==============================================================================
// Constants

//==============================================================================
// Types
/* refernce point and trilateration data stryctures definitions*/
typedef struct S_point
	{		
		unsigned char number; 
		int flight_time;
		float radius;
		float error_coef;
		float x_p;
		float y_p;
		float z_p;
	} S_point;

typedef struct Trilateration
	{
	  S_point point[8];
	  float x;
	  float y;
	  float z;
	}Trilateration;

static Trilateration T_data[3]; 
//==============================================================================
// External variables

//==============================================================================
// Global functions

int Declare_Your_Functions_Here (int x);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Trilateration_H__ */
