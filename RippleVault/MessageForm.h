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
 Module Name				: MessageForm.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF,LKA,SRI
 Date Created               : 20-08-2005
 Rational Rose Model Name   :                          
 Use Case Document Name     : EC-GEN-USE-2005-07-20-Update Message Settings V1.0                         
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By           :
 Code Reviewed Date			:
 
 Recommendations            :
 

***************************************************************/

#ifndef __MESSAGEFORM_H__
#define __MESSAGEFORM_H__

#include "AEE.h"
#include "AEEForm.h"
#include "AEEModGen.h"          // Module interface definitions
#include "AEEAppGen.h"          // Applet interface definitions
#include "AEEShell.h"           // Shell interface definitions
#include "AEEDateTimeWidget.h"
#include "AEERootForm.h"
#include "AEECheckWidget.h"
#include "FormUtil.h"
#include "AEEPopUp.h"
#include "AEEPropContainer.h"
#include "AEEStaticWidget.h"
#include "AEEListWidget.h"
#include "AEEImageWidget.h"
#include "ImageStaticWidget.h"
#include "AEEVectorModel.h"
#include "AEEScrollWidget.h"
#include "../SyncEng/SMEvents.h"
#include "MessageForm.h"
#include "RippleVault.bid"
#include "RippleVault.brh"

typedef struct _ContactShareForSMS
{
	ImageStaticInfo imageInfo;
	AECHAR pwzPhoneNo[20];
	boolean status;
}ContactShareSMS;

//typedef struct MainForm MainForm;
typedef struct _MessageForm {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IShell        *pIShell;    // give a standard way to access the Shell interface
	IPropContainer * mainContainer;
	IPropContainer * smsSettingsFormContainer;
	IValueModel* uploadTime;
	IForm* selectPopup;
	IForm* uploadTimeSettingPopup;
	IForm* uploadTimeSettingPopupFrom;
	HandlerDesc selectPopupHandler;
	IXYContainer *selectContainer;
	IXYContainer *uploadTimeSettingContainer;
	IWidget	*timeWgt;
	IWidget* containerWidget;
	IWidget* uploadTimeSettingContainerWidget;
	IWidget	*pEnableRadioWgt;
	IWidget	*pDisableRadioWgt;
	IWidget* titleStaticWgt;
	IValueModel* valueModelEnableRdo;
	IValueModel* valueModelDisableRdo;
	IWidget* enableStaticWgt;
	IWidget* disableStaticWgt;
	AECHAR *pszOption1;
	AECHAR *pszOption2;
	AECHAR *pszHeading;
	AECHAR *pszTimeHeading;
	int nSelectedCategory;
	int SelectedContactIndex;
	user *userSettings;
	IRootForm* rootForm;
	IForm* MessageForm;
	IForm* smsSettingsForm;
	IPopupMenu* messageForm_menu;
	IPopupMenu* smsSettingsForm_menu;
	AEEDeviceInfo  DeviceInfo;
	HandlerDesc mainFormHandler;
	HandlerDesc smsSettingsFormHandler;
	HandlerDesc messageSettingsMenuHandler;
	HandlerDesc smsSettingsMenuHandler;
	HandlerDesc uploadTimeSettingPopupHandler;
	HandlerDesc ContactHandleForSMSUploadHandler;
	AECHAR* str1;
	AECHAR* str2;
	AECHAR* str3;
	AECHAR* str4;
	AECHAR* str5;
	AECHAR* str6;
	AECHAR* str7;
	AECHAR* text1;
	AECHAR* text2;
	IImage* m_pIImage1;
	IImage* m_pIImage2;
	IImage* m_pIImage3;
	IImage* m_pIImage4;
	IImage* m_pIImage5;
	IImage* m_pIImage6;
	IImage* m_pIImage7;
	IImage* m_pIImage8;	
	ImageStaticInfo imageInfo1;// = NULL;
	ImageStaticInfo imageInfo2;// = NULL;
	ImageStaticInfo imageInfo3;// = NULL;
	ImageStaticInfo imageInfo4;// = NULL;
	ImageStaticInfo imageInfo5;// = NULL;
	ImageStaticInfo imageInfo6;// = NULL;
	ImageStaticInfo imageInfo7;// = NULL;
	ImageStaticInfo imageInfo8;// = NULL;
	int messagesSelectedItem;
	ModelListener menuListener; //listner for the menu model
	ModelListener smsSettingsMenuListener;
	IPropContainer *pContactListContainer;
	IForm *pContactSelectForm;
	ContactShareSMS **pTmpContAr;
	int m_noOfAlbumSharedContacts;
	int m_nNoOfAlbums;
	ModelListener ContactSelectMenuListener;
	IPopupMenu* contactHandleMenuForSMSUpload_menu;
	HandlerDesc NoContactHandleForSMSUploadHandler;
	HandlerDesc SelectContactToShareHandler;

} MessageForm;
void MessageForm_SelectContactToShareDelete(MessageForm* pMe);
int MessageForm_Select(MessageForm* pMe);
int MessageForm_New(IForm **ppo, IShell *piShell, IRootForm *pRootForm);
int MessageForm_Menu(MessageForm* pMe);
int MessageForm_SMSSettingsFormMain(MessageForm* pMe);
void smsSettingsForm_Delete(MessageForm* pMe);
int MessageForm_SMSSettingsMenu(MessageForm* pMe);
int MessageFrom_UploadTimeSettingForm(MessageForm* pMe);
boolean MessageForm_SelectContactToShare(MessageForm* pMe, char* contactAr);
int MessageForm_ContactHandleFGorSMSUploadMenu(MessageForm* pMe);

#endif