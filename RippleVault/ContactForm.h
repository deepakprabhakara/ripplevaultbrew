
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
 Module Name				: ContactForm..
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF,LKA,SRI
 Date Created               : 20-08-2005
 Rational Rose Model Name   :                       
 Use Case Document Name     : EC-GEN-USE-2005-07-20-Update Contact Settings V1.0                          
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By           :
 Code Reviewed Date			:
 
 Recommendations            :
 

***************************************************************/

#ifndef __CONTACTFORM_H__
#define __CONTACTFORM_H__

#include "AEE.h"
#include "AEEForm.h"
#include "AEEModGen.h"          // Module interface definitions
#include "AEEAppGen.h"          // Applet interface definitions
#include "AEEShell.h"           // Shell interface definitions
#include "AEERootForm.h"
#include "Dialog.h"
#include "FormUtil.h"
#include "AEEPopUp.h"
#include "AEEPropContainer.h"
#include "AEEStaticWidget.h"
#include "AEEListWidget.h"
#include "AEEImageWidget.h"
#include "ImageStaticWidget.h"
#include "AEEVectorModel.h"
#include "AEEScrollWidget.h"
#include "CSettings.h"
#include "ContactForm.h"
#include "RippleVault.bid"
#include "RippleVault.brh"

typedef struct _ContactForm {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IShell        *pIShell;    // give a standard way to access the Shell interface
	IPropContainer * mainContainer;
	IRootForm* rootForm;
	IForm* contactForm;
	IForm* csettings;
	IPopupMenu* optionsMenu;
	IDialog* dialog;
	AEEDeviceInfo  DeviceInfo;
	HandlerDesc mainFormHandler;
	HandlerDesc optionsMenuHandler;
	HandlerDesc	dlgHandler;
	AECHAR* str1;
	AECHAR* str2;
	AECHAR* str4;
	IImage* m_pIImage1;
	IImage* m_pIImage2;
	ImageStaticInfo imageInfo1;// = NULL;
	ImageStaticInfo imageInfo2;// = NULL;
	ModelListener menuListener; //listner for the menu model
	ModelListener popListener; //listner for the pop up menu
	int selectedItem;

} ContactForm;

int ContactForm_New(IForm **ppo, IShell *piShell, IRootForm *pRootForm);
int ContactForm_PopulateMainContainer(ContactForm* pMe) ;
int Contact_CreateOptionsMenu(ContactForm * pMe);
void deleteContactDialog(ContactForm* pMe);
int ContactForm_Dialog(ContactForm* pMe,char* text);

#endif