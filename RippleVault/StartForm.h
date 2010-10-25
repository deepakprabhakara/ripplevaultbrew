/*******************************************************************
 
 Copyright(c) 2000-2005 
 
 PurpleACE Wireless Solutions Pvt. Ltd.
 www.purpleace.com
 #708, A Wing, Carlton Towers, Airport Road, Bangalore - 560017,
 India
 All rights reserved.
 
 This software is the confidential and proprietary information of
 PurpleACE Wireless Solutions Pvt. Ltd. ("Confidential Information").
 You shall not disclose such Confidential Information and
 shall use it only in accordance with the terms of the license
 Agreement you entered into with PurpleACE Wireless Solutions Pvt. Ltd..
 
 Project Name               : Ripple Vault BREW Client
 Module Name				: StartForm.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF
 Date Created               : 20-08-2005
 Rational Rose Model Name   :                          
 Use Case Document Name     :                          
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By           :
 Code Reviewed Date			:
 
 Recommendations            :
 

***************************************************************/

#ifndef __STARTFORM_H__
#define __STARTFORM_H__

#include "AEE.h"
#include "AEEForm.h"
#include "AEEModGen.h"          // Module interface definitions
#include "AEEAppGen.h"          // Applet interface definitions
#include "AEEShell.h"           // Shell interface definitions
#include "AEERootForm.h"
#include "FormUtil.h"
#include "AEEPopUp.h"
#include "AEEPropContainer.h"
#include "AEEStaticWidget.h"
#include "AEEListWidget.h"
#include "AEEImageWidget.h"
#include "ImageStaticWidget.h"
#include "AEEVectorModel.h"
#include "AEEScrollWidget.h"
#include "RippleVault.bid"
#include "RippleVault.brh"

typedef struct _StartForm {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IShell        *pIShell;    // give a standard way to access the Shell interface
	AEEDeviceInfo  DeviceInfo;
	IXYContainer * mainContainer;
	IRootForm* rootForm;
	IForm* startForm;
	IForm* loginForm;
	IPopupMenu* optionsMenu;
	HandlerDesc mainFormHandler;
	HandlerDesc optionsMenuHandler;
	AECHAR* str1;
	AECHAR* str2;
	IImage* m_pIImage1;
	IImage* m_pIImage2;	
	ImageStaticInfo imageInfo1;// = NULL;
	ImageStaticInfo imageInfo2;// = NULL;
	ModelListener menuListener; //listner for the menu model
	ModelListener popListener; //listner for the pop up menu
} StartForm;

int StartForm_New(IForm **ppo, IShell *piShell, IRootForm *pRootForm,AEEDeviceInfo  DInfo);
int StartForm_PopulateMainContainer(StartForm* pMe) ;
int Start_CreateOptionsMenu(StartForm * pMe);

#endif