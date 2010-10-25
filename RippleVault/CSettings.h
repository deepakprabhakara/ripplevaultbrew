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
 Module Name				: CSettings.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF,LKA,SRI
 Date Created               : 20-08-2005
 Rational Rose Model Name   :                          
 Use Case Document Name     : EC-GEN-USE-2005-07-20-Update Contact Settings V1.0,EC-GEN-USE-2005-07-20-View Contacts V1.0                         
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By           :
 Code Reviewed Date			:
 
 Recommendations            :
 

***************************************************************/
#ifndef __CSETTINGS_H__
#define __CSETTINGS_H__

#include "AEE.h"
#include "AEEForm.h"
#include "../SyncEng/SMCommon.h"
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
#include "Dialog.h"
#include "AEETextWidget.h"
#include "AEECheckWidget.h"
#include "AEE.h"
#include "AEEFile.h"
#include "RippleVault.bid"
#include "RippleVault.brh"
#include "..\SyncEng\SyncEng.h"


typedef struct _CSettings {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IShell        *pIShell;    // give a standard way to access the Shell interface
	IPropContainer* mainContainer;
	IPropContainer* contactSettingsContainer;
    IXYContainer* radioListContainer;
	IXYContainer* shedulerContainer;
	IXYContainer* rangeContainer;
	IRootForm* rootForm;
	IForm* csettings;
	AEEDeviceInfo  DeviceInfo;
	IDialog* dialog;
	IForm* range;
	IPopupMenu* menu;
	IForm* radioList;
	IForm* shedulerList;
	IForm* PhotoForm_SettingsForm;
	IWidget* renable;
	IWidget* rdisable;
	IWidget* hourtext;
	IWidget* minutestext;
	IWidget* secondstext;
	IWidget* rangeTitle;
	IWidget* shedulerFormTitle;
	IForm* loginmainForm;
	IWidget* containerWidget;
	IValueModel* valueModel1;
	IValueModel* valueModel2;
	IValueModel* valueModel3;
	IValueModel* valueModel4;
	IValueModel* valueModel5;
	IWidget* rangeCreateTextWidget;
	IWidget* enable;
	IWidget* disable;
	IWidget* hour;
	IWidget* minutes;
	IWidget* seconds;
	ImageStaticInfo imageInfo1;
	ImageStaticInfo imageInfo2;
	ImageStaticInfo imageInfo3;
	ModelListener menuListener;
	HandlerDesc menuHandler;
	HandlerDesc	dlgHandler;
	HandlerDesc settingsMenuHandler;
	HandlerDesc	radioListHandler;
	HandlerDesc	rangeHandler;
	HandlerDesc shedulerHandler;
	IImage* m_pIImage1;
	IImage* m_pIImage2;
	IImage* m_pIImage3;
	AECHAR* str1;
	AECHAR* str2;
	AECHAR* str3;
	AECHAR* str4;
	AECHAR* enableText;
	AECHAR* disableText;
	AECHAR* hourText;
	AECHAR* minutesText;
	AECHAR* secondsText;
	AECHAR* rangeTitleText;
	AECHAR* shedulerFormTitleText;
	AECHAR* text;
	AECHAR* title;
	int selectedItem;
	int nFocusWdt;
	int count;
	int dialogEvent;
	int isRoomimgDialog;
	ModelListener  PhotoForm_menuListener;
	int PhotoFormSettingsSelectedItem;
	IWidget* PhotoForm_SettingsContainerWidget;
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
	IPopupMenu* helpMenu;

} CSettings;

CSettings_New(IForm** ppo, IShell *piShell,IRootForm *pRootForm);
int CSettings_Dialog(CSettings* pMe,AECHAR* text,AECHAR* title);
int CSettingsMenu(CSettings* pMe);
int CSettingsRadioList(CSettings* pMe);
void deleteDialog(CSettings* pMe);
int CSettingsRadioListForSheduler(CSettings* pMe);
void deleteShdulerForm(CSettings* pMe);


#endif