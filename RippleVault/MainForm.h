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
 Module Name				: MainForm.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF,LKA,SRI
 Date Created               : 20-08-2005
 Rational Rose Model Name   :                     
 Use Case Document Name     : EC-GEN-USE-2005-07-20-Update System Settings V1.0.doc
							  EC-GEN-USE-2005-07-20- Show Help V1.0.doc
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By           :
 Code Reviewed Date			:
 
 Recommendations            :
 

***************************************************************/
#ifndef __MAINFORM_H__
#define __MAINFORM_H__

#include "AEE.h"
#include "AEEForm.h"
#include "AEEModGen.h"          // Module interface definitions
#include "AEEAppGen.h"          // Applet interface definitions
#include "AEEShell.h"           // Shell interface definitions
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
#include "AEEMenu.h"
#include "AEEViewportWidget.h"		// Menu Services
#include "AEEFont.h"
#include "ContactForm.h"
#include "PhotoForm.h"
#include "VideoForm.h"
#include "MessageForm.h"
#include "RippleVault.bid"
#include "RippleVault.brh"
#include "..\SyncEng\SyncEng.h"
#include "SyncEng.bid"
#include "..\SyncEng\SMEvents.h"


typedef struct _MainForm {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IDisplay      *pIDisplay;  // give a standard way to access the Display interface
	IShell        *pIShell;    // give a standard way to access the Shell interface
	IPropContainer * mainContainer;
	IPropContainer * settingsContainer;
	IRootForm* rootForm;
	IForm* mainForm;
	IForm* contactForm;
	IForm* photoForm;
	IForm* videoForm;
	IForm* messageForm;
	IForm* helpFormPage;
	HandlerDesc mainFormHandler;
	HandlerDesc progressHandler;
	HandlerDesc optionsMenuHandler;
	HandlerDesc HelpFormPageHandler;
	AECHAR* aeStr1;
	AECHAR* aeStr2;
	AECHAR* aeStr3;
	AECHAR* aeStr4;
	AECHAR* str1;
	AECHAR* str2;
	AECHAR* str3;
	AECHAR* rippleVault;
	AECHAR* purpleAce;
	IImage* m_pIImage1;
	IImage* m_pIImage2;
	IImage* m_pIImage3;
	IImage* m_pIImage4;
	IImage* m_pIImage5;	
	ImageStaticInfo imageInfo1;// = NULL;
	ImageStaticInfo imageInfo2;// = NULL;
	ImageStaticInfo imageInfo3;// = NULL;
	ImageStaticInfo imageInfo4;// = NULL;
	ImageStaticInfo imageInfo5;
	IPopupMenu* optionsMenu;	
	ModelListener  PhotoForm_menuListener;
	int PhotoFormSettingsSelectedItem;
	IWidget* PhotoForm_SettingsContainerWidget;
	IWidget* ripplevault;
	IWidget* purpleace;
	IForm* PhotoForm_SettingsMenu;
	IXYContainer* PhotoForm_SettingsContainer;
	IForm* autoStartState;
	IForm* helpForm;
	IValueModel* valueModelAutoStartOn;
	IValueModel* valueModelAutoStartOff;
	HandlerDesc PhotoFormSettingsHandler;
	HandlerDesc PhotoFormStateHandler;
	HandlerDesc helpFormHandler;
	HandlerDesc helpHomeHandler;
	HandlerDesc helpHomeMenuHandler;
	HandlerDesc mainFormDlgHandler;
	IPopupMenu* helpMenu;
	AEEDeviceInfo  DeviceInfo;	
	AECHAR* enableText;
	AECHAR* disableText;
	AECHAR* helpChartext;
	AECHAR* helpTextTitleChar;
	IDialog* dialog;
	IXYContainer* radioListContainer;
	IPropContainer* helpFormPageContainer;
	IWidget* renable;
	IWidget* rdisable;
	IWidget* enable;
	IWidget* disable;
	IWidget* containerWidget;
	IWidget* settingsContainerWidget;
	IWidget* autoStartStateContainerWidget;
	IWidget* viewPort;
	IForm* PhotoForm_SettingsForm;
	IWidget* helpText;
	IWidget* helptextTitle;
	int optionselected;
	int selectedItem;
	ModelListener menuListener; 

} MainForm;

int MainForm_New(IForm **ppo, IShell *piShell, IRootForm *pRootForm, IDisplay *piDisplay);
int Main_CreateOptionsMenu(MainForm * pMe);
int	MainForm_CreateProgressDialog(MainForm* pMe, uint16 nResId);
static boolean MainProgressDlg_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam);
PhotoForm_SettingsPopupMenu(MainForm * pMe);
void Delete_SettingsAutoStart(MainForm* pMe);
void deleteMainFormDialog(MainForm* pMe);
void Delete_PhotoFormSettigs(MainForm* pMe);
void deletePhotoForm_HelpFormPage(MainForm* pMe);
void deletePhotoForm_HelpForm(MainForm* pMe);
int PhotoForm_SettingsForm(IForm** ppo, IShell *piShell,IRootForm *pRootForm,MainForm* pMe);
int PhotoForm_HelpFormMain(IForm** ppo, IShell *piShell,IRootForm *pRootForm,AEEDeviceInfo  DInfo,MainForm* pMe);
int PhotoForm_Dialog(MainForm* pMe,AECHAR* text,AECHAR* title);
int PhotoForm_SettingsAutoStart(MainForm* pMe);
int PhotoForm_HelpMenu(MainForm* pMe);
void deletePhotoForm_HelpFormPage(MainForm* pMe);
int PhotoForm_HelpFormPage(MainForm* pMe,AECHAR* text, AECHAR* title);
#endif