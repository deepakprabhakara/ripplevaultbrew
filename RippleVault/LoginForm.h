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
 Module Name				: LoginForm.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF,LKA,SRI
 Date Created               : 20-08-2005
 Rational Rose Model Name   : EC-GEN-USE-2005-07-20-User Registration V1.0.doc                      
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

#ifndef __LOGINFORM_H__
#define __LOGINFORM_H__

#include "AEE.h"
#include "AEEForm.h"
#include "FormUtil.h"
#include "AEEXYContainer.h"
#include "AEETextWidget.h"
#include "AEEStaticWidget.h"
#include "AEEPropContainer.h"
#include "AEEDialog.h"
#include "AEERootForm.h"
#include "..\SyncEng\SyncEng.h"
#include "..\SyncEng\SyncEng.bid"
#include "..\SyncEng\SMEvents.h"

typedef struct _LoginForm {
	IShell* pIShell;
	IRootForm* rootForm;
	IForm* loginmainForm;
	IForm* mainForm;	
	HandlerDesc loginFormHandler;
	HandlerDesc progressDlgHandler;
	HandlerDesc unSelectDlgHandler;
	IXYContainer* mainContainer;
	IWidget* UsenameStaticText;
	IWidget* UsenameTextWidget;
	IWidget* PhoneNoStaticText;
	IWidget* PhoneNoTextWidget;
	IWidget *m_passwordStaticWgt;
	IWidget *m_passwordTextWgt;
	IDialog* progressDialog;
	IDialog* unSelectDialog;
	AECHAR* UsernameText;
	AECHAR* PhoneNoText;
	AECHAR*	m_pwzPassword;
	AECHAR* str2;
	char* uname;
	char* uphone;
	char* m_pszPassword;
	ISyncEngExt* m_pSyncExt;
	int nFocusWdt;
	int ftest;
	boolean m_bExistingUser;

}LoginForm;

int LoginForm_New(IForm **ppo,IShell *piShell, IRootForm *pRootForm, boolean bExistingUser);
int LoginForm_CreateProgressDialog(LoginForm* pMe, uint16 nResId);
int ShowMainForm(LoginForm* pMe);
int AppForm_CreateExitConfDialog(LoginForm* pMe);
int UnloadProgress(LoginForm* pMe);
int LoginForm_PhoneNoDialog(LoginForm* pMe);
static boolean ProgressDlg_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam);
int LoadSyncExt(LoginForm* pMe); //from pathi.. :)

#endif

