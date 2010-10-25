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
 Module Name				: PhotoForm.cpp
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 05-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name      :EC-GEN-USE-2005-07-20-Update Photo Video Settings V1.0.doc                       :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Handle photo setting change (header)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#ifndef __PHOTOSETTINGS_H__
#define __PHOTOSETTINGS_H__

#include "AEE.h"
#include "AEEForm.h"
#include "AEEModGen.h"       
#include "AEEAppGen.h"       
#include "AEEShell.h"        
#include "AEERootForm.h"
#include "ListForm.h"
#include "FormUtil.h"
#include "AEEPopUp.h"
#include "AEEPropContainer.h"
#include "AEEStaticWidget.h"
#include "AEEListWidget.h"
#include "AEEImageWidget.h"
#include "ImageStaticWidget.h"
#include "AEEVectorModel.h"
#include "AEEScrollWidget.h"
#include "AEEMenu.h"		// Menu Services
#include "AEEFont.h"
#include "RippleVault.brh"
#include "..\SyncEng\SMEvents.h"

typedef struct _PhotoSettings{
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IDisplay      *pIDisplay;  // give a standard way to access the Display interface
	IShell        *pIShell;    // give a standard way to access the Shell interface
	IPropContainer * mainContainer;	
	IRootForm* rootForm;	
	IForm* mainForm;
	HandlerDesc mainFormHandler;
	ModelListener menuListener; //listner for the menu model
	IPopupMenu* optionsMenu;
	HandlerDesc optionsMenuHandler;
	IForm* selectPopup;
	HandlerDesc selectPopupHandler;
	IXYContainer *selectContainer;
	IWidget* containerWidget;
	IWidget	*pEnableRadioWgt;
	IWidget	*pDisableRadioWgt;
	IValueModel* valueModelEnableRdo;
	IValueModel* valueModelDisableRdo;
	AECHAR *pszAlbum;
	AECHAR *pszAlbumValue;
	AECHAR *pszDescription;
	AECHAR *pszDescriptionValue;
	AECHAR *pszRoaming;
	AECHAR *pszRoamingValue;
	IWidget* pAlbumWgt;
	IWidget* pAlbumValueWgt;
	IWidget* pDescriptionWgt;
	IWidget* pDescriptionValueWgt;
	IWidget* enableStaticWgt;
	IWidget* disableStaticWgt;
	IWidget* titleStaticWgt;
	AECHAR *pszOption1;
	AECHAR *pszOption2;
	AECHAR *pszHeading;
	int nSelectedCategory;
	user *userSettings;
	IPopupMenu *selectAlbumPopupMenu;
	HandlerDesc selectAlbumHandler;
	IMenuModel *selectAlbumMenuModel;
	int noOfAlbums;
	int nSelectedAlbum;
	IDialog *infoDlg;
	HandlerDesc infoDlgHandler;
	IForm *msgDlg;
	IXYContainer *msgDlgContainer;
	HandlerDesc msgDlgHandler;
	IWidget *msgWgt;

} PhotoSettings;

int PhotoList_MsgDlg(PhotoSettings* pMe);
int PhotoSettings_InfoDialog(PhotoSettings* pMe);
boolean PhotoSettings_SelectAlbumMenu(PhotoSettings* pMe);
int PhotoSettings_Select(PhotoSettings* pMe);
int PhotoSettings_New(IForm **ppo, IShell *piShell, IRootForm *pRootForm);
int PhotoSettings_CreateOptionsMenu(PhotoSettings * pMe);
#endif