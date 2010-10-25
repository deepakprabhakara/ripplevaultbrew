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
 Module Name				: LoginForm.cpp
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
#include "LoginForm.h"
#include "MainForm.h"
#include "RippleVault.bid"
#include "RippleVault.brh"
#include "AEEBorderWidget.h"

#define INPUTTEXT_LENGTH	50
#define WIDGET_HEIGHT		15
#define WIDGET_GAP			2
#define LEFT_GAP			4
#define TOP_GAP				2

#define RIPPLEVAULT_RES_FILE "RippleVault.bar"

//================================================================================
// FUNCTION		: LoginForm_Delete
// DESCRIPTION	: Delete resources associated with login form.
// ================================================================================
void LoginForm_Delete(LoginForm* pMe) {
   
	if ( NULL==pMe ) return;

	if(pMe->mainForm)
	{
		IFORM_Release(pMe->mainForm);
		pMe->mainForm = NULL;
	}

	if(pMe->mainContainer)
	{
		IXYCONTAINER_Release(pMe->mainContainer);
		pMe->mainContainer = NULL;
	}

	if(pMe->UsenameStaticText)
	{
		IWIDGET_Release(pMe->UsenameStaticText);
		pMe->UsenameStaticText=NULL;
	}
	if(pMe->UsenameTextWidget)
	{
		IWIDGET_Release(pMe->UsenameTextWidget);
		pMe->UsenameTextWidget = NULL;
	}
	if(pMe->PhoneNoStaticText)
	{
		IWIDGET_Release(pMe->PhoneNoStaticText);
		pMe->PhoneNoStaticText = NULL;
	}
	if(pMe->PhoneNoTextWidget)
	{
		IWIDGET_Release(pMe->PhoneNoTextWidget);
		pMe->PhoneNoTextWidget = NULL;
	}
	if ( pMe->m_bExistingUser )
	{
		if ( pMe->m_passwordStaticWgt )
		{
			IWIDGET_Release(pMe->m_passwordStaticWgt);
			pMe->m_passwordStaticWgt = NULL;
		}
		if ( pMe->m_passwordTextWgt )
		{
			IWIDGET_Release(pMe->m_passwordTextWgt);
			pMe->m_passwordTextWgt = NULL;
		}
	}
	if(pMe->progressDialog)
	{
		IDIALOG_Release(pMe->progressDialog);	
		pMe->progressDialog = NULL;
	}
	

	if (NULL!=pMe->m_pSyncExt)
	{
		ISYNCENG_Release(pMe->m_pSyncExt);
		pMe->m_pSyncExt = NULL;
	}
	
	if(pMe->unSelectDialog)
	{
		IDIALOG_Release(pMe->unSelectDialog);
		pMe->unSelectDialog = NULL;
	}

	FREEIF(pMe->str2)
	FREEIF(pMe->uname);
	FREEIF(pMe->uphone);
	FREEIF(pMe->m_pszPassword);

   if(pMe)
      FREE(pMe);
}

// ================================================================================
// FUNCTION		: LoginForm_HandleEvent
// DESCRIPTION	: Hadndel event of login form
// ================================================================================
static boolean LoginForm_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	LoginForm* pMe = (LoginForm*) po;
	IWidget* containerWidget = NULL; 
	int result;
	
    switch (evt) 
	{
	case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT1:
				if (0==LoadSyncExt(pMe))
					LoginForm_CreateProgressDialog(pMe, IDS_REGUSER);
				else
				{
					//todo: show error message.
					//ISHELL_MessageBox(pMe->pIShell, RIPPLEVAULT_RES_FILE, MAINFORMTITLE, IDS_EMPTYUSER); 
				}

				break;
			case AVK_SOFT2:
				IROOTFORM_PopForm(pMe->rootForm);
				break;
			case AVK_CLR:
				IROOTFORM_PopForm(pMe->rootForm);
				break;
			case AVK_UP:
				result = IXYCONTAINER_QueryInterface(pMe->mainContainer, AEEIID_WIDGET, (void**)&containerWidget);
				if(result ==0) 
				{
					if (2==pMe->nFocusWdt)
					{
						IFORM_SetWidget(pMe->loginmainForm, WID_FORM, containerWidget);
						IWIDGET_MoveFocus(containerWidget, pMe->PhoneNoTextWidget);
						IWIDGET_Release(containerWidget);
						pMe->nFocusWdt--;
					}else if (1==pMe->nFocusWdt)
					{
						IFORM_SetWidget(pMe->loginmainForm, WID_FORM, containerWidget);
						IWIDGET_MoveFocus(containerWidget, pMe->UsenameTextWidget);
						IWIDGET_Release(containerWidget);
						pMe->nFocusWdt--;
					}
				}				
				break;
			case AVK_DOWN:
				result = IXYCONTAINER_QueryInterface(pMe->mainContainer, AEEIID_WIDGET, (void**)&containerWidget);
				if(result ==0) 
				{
					if (0==pMe->nFocusWdt)
					{
						IFORM_SetWidget(pMe->loginmainForm, WID_FORM, containerWidget);
						IWIDGET_MoveFocus(containerWidget, pMe->PhoneNoTextWidget);
						IWIDGET_Release(containerWidget);
						pMe->nFocusWdt++;
					}else if (1==pMe->nFocusWdt)
					{
						IFORM_SetWidget(pMe->loginmainForm, WID_FORM, containerWidget);
						IWIDGET_MoveFocus(containerWidget, pMe->m_passwordTextWgt);
						IWIDGET_Release(containerWidget);
						pMe->nFocusWdt++;
					}
				}
				break;
			}
		}
		break;

	default:
		break;
	}
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->loginFormHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: LoginForm_PopulateMainContainer
// DESCRIPTION	: Create widgets of login form
// ================================================================================
int LoginForm_PopulateMainContainer(LoginForm* pMe) 
{
	WExtent we;
	WidgetPos wpos;
	IWidget* containerWidget = NULL;
	int nWdtHeight=0;
	int nWdtWidth=0;
	AEEDeviceInfo  deviceInfo;
	int result = 0;
	
	if ( SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->mainContainer) ) ;
	
	//static widget to load the username
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->UsenameStaticText);
	//create Username text widget
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->UsenameTextWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->PhoneNoStaticText);
    result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->PhoneNoTextWidget);
	//create password static and text widgets
	if ( pMe->m_bExistingUser )
	{
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->m_passwordStaticWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->m_passwordTextWgt);
	}
	if ( result>0 ) return -1; //failed.

	//Load the string to the static widget
	pMe->UsernameText = (AECHAR*) MALLOC(INPUTTEXT_LENGTH);
	pMe->PhoneNoText = (AECHAR*) MALLOC(INPUTTEXT_LENGTH);
	

	if ( (NULL==pMe->UsernameText)||(NULL==pMe->PhoneNoText) ) return -1;

    if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_NAME, pMe->UsernameText, INPUTTEXT_LENGTH) ) return -1;
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PHONENO, pMe->PhoneNoText, INPUTTEXT_LENGTH) ) return -1;
	if ( pMe->m_bExistingUser )
	{
		pMe->m_pwzPassword = (AECHAR*) MALLOC(INPUTTEXT_LENGTH);
		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PASSWORD, pMe->m_pwzPassword, INPUTTEXT_LENGTH) ) return -1;
		IWIDGET_SetText(pMe->m_passwordStaticWgt, pMe->m_pwzPassword, TRUE);
	}

	IWIDGET_SetText(pMe->UsenameStaticText, pMe->UsernameText, TRUE);
	IWIDGET_SetText(pMe->PhoneNoStaticText, pMe->PhoneNoText, TRUE);
	

	
	ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
	nWdtHeight = (deviceInfo.cyScreen/12)*8 -WIDGET_GAP*3;
	nWdtHeight = nWdtHeight/6;
	nWdtHeight = nWdtHeight;

	nWdtWidth = (deviceInfo.cyScreen/10)*8;

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->UsenameStaticText,&we);
	IWIDGET_SetFlags(pMe->UsenameStaticText, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->UsenameStaticText, MAKE_RGB(252, 252, 252));

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->UsenameTextWidget,&we);
	IWIDGET_SetBorderColor(pMe->UsenameTextWidget, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->UsenameTextWidget, 1);
    IWIDGET_SetFlags(pMe->UsenameTextWidget, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->PhoneNoStaticText,&we);
	IWIDGET_SetFlags(pMe->PhoneNoStaticText, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->PhoneNoStaticText, MAKE_RGB(252, 252, 252));

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->PhoneNoTextWidget,&we);
	IWIDGET_SetBorderColor(pMe->PhoneNoTextWidget, MAKE_RGB(100, 100, 100));
	IWIDGET_SetBorderWidth(pMe->PhoneNoTextWidget, 1);

	//set password static and textwidget
	if ( pMe->m_bExistingUser )
	{
		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(pMe->m_passwordStaticWgt,&we);
		IWIDGET_SetFlags(pMe->m_passwordStaticWgt, IDF_ALIGN_LEFT);
		IWIDGET_SetBorderColor(pMe->m_passwordStaticWgt, MAKE_RGB(252, 252, 252));

		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(pMe->m_passwordTextWgt,&we);
		IWIDGET_SetBorderColor(pMe->m_passwordTextWgt, MAKE_RGB(100, 100, 100));
		IWIDGET_SetBorderWidth(pMe->m_passwordTextWgt, 1);
	}

	//insert the username static text to the XYcontainer
	wpos.x = LEFT_GAP;
    wpos.y = 0;
    wpos.bVisible = TRUE;	     
	IXYCONTAINER_Insert(pMe->mainContainer, pMe->UsenameStaticText, WIDGET_ZNORMAL, &wpos);

	wpos.x = LEFT_GAP;
    wpos.y = nWdtHeight;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->mainContainer, pMe->UsenameTextWidget, WIDGET_ZNORMAL, &wpos);

	wpos.x = LEFT_GAP;
    wpos.y = 2*nWdtHeight+WIDGET_GAP;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->mainContainer, pMe->PhoneNoStaticText, WIDGET_ZNORMAL, &wpos);

	wpos.x = LEFT_GAP;
    wpos.y = 3*nWdtHeight+WIDGET_GAP;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->mainContainer, pMe->PhoneNoTextWidget, WIDGET_ZNORMAL, &wpos);
	if ( pMe->m_bExistingUser )
	{
		//password
		wpos.x = LEFT_GAP;
		wpos.y = 4*nWdtHeight+2*WIDGET_GAP;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->mainContainer, pMe->m_passwordStaticWgt, WIDGET_ZNORMAL, &wpos);

		wpos.x = LEFT_GAP;
		wpos.y = 5*nWdtHeight+2*WIDGET_GAP;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->mainContainer, pMe->m_passwordTextWgt, WIDGET_ZNORMAL, &wpos);
	}

	//set WID_FORM to the container
    result = IXYCONTAINER_QueryInterface(pMe->mainContainer, AEEIID_WIDGET, (void**)&containerWidget);
	if( result ==0) 
	{
		IFORM_SetWidget(pMe->loginmainForm, WID_FORM, containerWidget);
		IWIDGET_MoveFocus(containerWidget, pMe->UsenameTextWidget);
		
	}

	IWIDGET_Release(containerWidget);
	if(result != 0)
		return EFAILED;

	return result;
}

// ================================================================================
// FUNCTION		: LoginForm_New
// DESCRIPTION	: Create  login form
// ================================================================================
int LoginForm_New(IForm **ppo,IShell *pIShell, IRootForm *pRootForm, boolean bExistingUser)
{
	int result=0;
	IImage* m_pIImage=NULL;

	//allocate space for the form
	LoginForm *pMe = MALLOCREC(LoginForm);
	if ( NULL==pMe ) return -1;	
	

	pMe->pIShell = pIShell;
	pMe->rootForm = pRootForm;
	pMe->m_bExistingUser = bExistingUser;
		
	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->loginmainForm);

	if( SUCCESS==result) 
	{
		*ppo = pMe->loginmainForm;
		if ( NULL==(m_pIImage = ISHELL_LoadResImage(pIShell,RIPPLEVAULT_RES_FILE,IDI_TITLE))) return -1;
		IFORM_SetTitleImage((IForm*)pMe->loginmainForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->loginmainForm, FID_TITLE, RIPPLEVAULT_RES_FILE, MAINFORMTITLE);

		result =  IFORM_SetSoftkeys(pMe->loginmainForm, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);

		HANDLERDESC_Init(&pMe->loginFormHandler, LoginForm_HandleEvent, pMe, LoginForm_Delete);
		IFORM_SetHandler(pMe->loginmainForm, &pMe->loginFormHandler);	
	}
	
	if(!pMe)
		return ENOMEMORY;

	if( SUCCESS==result )
		result = LoginForm_PopulateMainContainer(pMe);
	RELEASEIF(m_pIImage);
	
	pMe->m_pSyncExt=NULL;

	return result;
}

// ================================================================================
// FUNCTION		: LoginForm_CreateProgressDialog
// DESCRIPTION	: Create  progressbar
// ================================================================================
int LoginForm_CreateProgressDialog(LoginForm* pMe, uint16 nResId)
{
	int result=0;

	IWidget* backdropWidget;
	IWidget* backWidget = NULL;


	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROGRESSDIALOG, (void**) &pMe->progressDialog);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);

	IFORM_SetResText((IForm*)pMe->progressDialog, FID_TEXT, RIPPLEVAULT_RES_FILE, nResId);	

	IFORM_GetWidget((IForm*)pMe->progressDialog, WID_PROGRESS, &backWidget);
	IWIDGET_SetFGColor(backWidget,MAKE_RGB(128, 128, 255));
	IFORM_SetWidget((IForm*)pMe->progressDialog, WID_PROGRESS, backWidget);
	IWIDGET_Release(backWidget);

	IFORM_GetWidget((IForm*)pMe->progressDialog, WID_BACKGROUND, &backWidget);
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IWIDGET_SetBorderWidth(backWidget, 1);
	IFORM_SetWidget((IForm*)pMe->progressDialog, WID_BACKGROUND, backWidget);
	
	IFORM_SetWidget((IForm*)pMe->progressDialog, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->progressDialog, IDF_ALIGN_LEFT);

	HANDLERDESC_Init(&pMe->progressDlgHandler, ProgressDlg_HandleEvent, pMe, NULL);
	IFORM_SetHandler((IForm*)pMe->progressDialog, &pMe->progressDlgHandler);//pathi

	IFORM_SetProgress((IForm*)pMe->progressDialog,-1);

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->progressDialog);

	IWIDGET_Release(backdropWidget);
	IWIDGET_Release(backWidget);
	return TRUE;
}


int ShowMainForm(LoginForm* pMe)
{

	MainForm_New(&pMe->mainForm, pMe->pIShell, pMe->rootForm,NULL);
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->mainForm);
	return TRUE;
}

// ================================================================================
// FUNCTION		: LoadSyncExt
// DESCRIPTION	: Load synchronization modeul
// ================================================================================
int LoadSyncExt(LoginForm* pMe)
{
	int status =0;

	AECHAR* a_uname=NULL;
	AECHAR* a_uphone=NULL;
	AECHAR* pwzPassword=NULL;
	int nLen=0;
	IWIDGET_GetTextWidgetText(pMe->UsenameTextWidget,&a_uname);
	IWIDGET_GetTextWidgetText(pMe->PhoneNoTextWidget,&a_uphone);
	if ( pMe->m_bExistingUser )
	{
		IWIDGET_GetTextWidgetText(pMe->m_passwordTextWgt,&pwzPassword);
	}

	nLen = WSTRLEN(a_uname)+1;
	if ( nLen<=1 ) 
	{ 
		pMe->ftest=0;
		LoginForm_PhoneNoDialog(pMe);
		status =  -1;
		goto error;
	}
	pMe->uname = (char*)MALLOC(nLen);
	WSTRTOSTR(a_uname,pMe->uname,nLen);
	nLen = WSTRLEN(a_uphone)+1;
	if ( nLen<=1 ) 
	{
		pMe->ftest=0;
		LoginForm_PhoneNoDialog(pMe);
		status =  -1;
		goto error;
	}
	pMe->uphone = (char*)MALLOC(nLen);
	WSTRTOSTR(a_uphone,pMe->uphone, nLen);
	if ( pMe->m_bExistingUser )
	{
		nLen = WSTRLEN(pwzPassword)+1;
		if ( nLen>1 )
		{
			pMe->m_pszPassword = (char*)MALLOC(nLen);
			WSTRTOSTR(pwzPassword, pMe->m_pszPassword, nLen);
		}else
		{
			status =  -1;
			goto error;
		}
	}	
	
	//create sync module
	if ( NULL==pMe->m_pSyncExt )
		if ( SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pMe->m_pSyncExt))
		{
			status = -1; 
			goto error;
		}


	status = ISYNCENG_RegisterNewUser(pMe->m_pSyncExt,pMe->uname, pMe->uphone, pMe->m_pszPassword);

	error:
		FREEIF(pMe->uname);
		FREEIF(pMe->uphone);
		FREEIF(pMe->m_pszPassword);
	
		return status;
}

// ================================================================================
// FUNCTION		: ProgressDlg_HandleEvent
// DESCRIPTION	: Handle progrssbar events
// ================================================================================
static boolean ProgressDlg_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	LoginForm* pMe = (LoginForm*) po;
	switch (evt)
	{
	case EVT_KEY:
			break;
	case EVT_USER_REG_FINISHED:

			IFORM_SetResText((IForm*)pMe->progressDialog, FID_TEXT, RIPPLEVAULT_RES_FILE, IDS_SYNCADDR);
			ISYNCENG_SyncContacts(pMe->m_pSyncExt);
			
		break;
	case EVT_USER_REG_FAILED:
			//free sync module
			if (NULL!=pMe->m_pSyncExt) 
			{
				ISYNCENG_Release(pMe->m_pSyncExt);
				pMe->m_pSyncExt=NULL;
			}
			//pop progress bar and show login form

			
			IROOTFORM_PopForm(pMe->rootForm);
			if(pMe->progressDialog)
			{
				IDIALOG_Release(pMe->progressDialog);	
				pMe->progressDialog = NULL;
			}

			pMe->ftest=1;
			LoginForm_PhoneNoDialog(pMe);

		break;
	case EVT_USER_SYN_FINISHED:
			//free sync module
			if (NULL!=pMe->m_pSyncExt) 
			{
				ISYNCENG_Release(pMe->m_pSyncExt);
				pMe->m_pSyncExt=NULL;
			}
			//pop progressbar	
			IROOTFORM_PopForm(pMe->rootForm);
			if(pMe->progressDialog)
			{
				IDIALOG_Release(pMe->progressDialog);	
				pMe->progressDialog = NULL;
			}
			//pop login form
			IROOTFORM_PopForm(pMe->rootForm);

			ISHELL_PostEvent(pMe->pIShell, AEECLSID_RIPPLEVAULT, EVT_USER_SHOWMENU, 0, 0);
			//show menu window
			//ShowMainForm(pMe);
			
		break;
		
	case EVT_USER_SYN_FAILED:
			//free sync module
			if (NULL!=pMe->m_pSyncExt) 
			{
				ISYNCENG_Release(pMe->m_pSyncExt);
				pMe->m_pSyncExt=NULL;
			}
			//pop progressbar and login form
			IROOTFORM_PopForm(pMe->rootForm);
			IROOTFORM_PopForm(pMe->rootForm);
		break;

	default:
		if (pMe->m_pSyncExt != NULL) 
			ISYNCENG_HandleEvent(pMe->m_pSyncExt, evt, wParam, dwParam);
		break;
	}

	return HANDLERDESC_Call(&pMe->progressDlgHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: UnSelectDialog_HandleEvent
// DESCRIPTION	: Handle menu select/unselect
// ================================================================================
static boolean UnSelectDialog_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   LoginForm* pMe = (LoginForm*) po;


	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
	
		IROOTFORM_PopForm(pMe->rootForm);
		return TRUE;
	
	}
	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
		IROOTFORM_PopForm(pMe->rootForm);
		return TRUE;
	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->unSelectDlgHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: LoginForm_PhoneNoDialog
// DESCRIPTION	: Dilog box to get phone number
// ================================================================================
int LoginForm_PhoneNoDialog(LoginForm* pMe)
{
	int result=0;

	IWidget* backdropWidget;
	IWidget* backWidget = NULL;
	AECHAR* sstr=NULL;
	AECHAR format[]={'%','s',' ','(','%','d',')','\0'};

	FREEIF(pMe->str2);
    if(pMe->unSelectDialog)
	{
		IDIALOG_Release(pMe->unSelectDialog);
		pMe->unSelectDialog = NULL;
	}

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_INFODIALOG, (void**) &pMe->unSelectDialog);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);
	
	IFORM_SetSoftkeys((IForm*)pMe->unSelectDialog, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);

	if(pMe->ftest==0)
		IFORM_SetResText((IForm*)pMe->unSelectDialog, FID_TEXT, RIPPLEVAULT_RES_FILE, IDS_PHONE_NO_MESSAGE);
	else
		IFORM_SetResText((IForm*)pMe->unSelectDialog, FID_TEXT, RIPPLEVAULT_RES_FILE, IDS_DUPLICATE_NAME);

	pMe->str2 = (AECHAR*)MALLOC(120);
	sstr = (AECHAR*)MALLOC(110);
	result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB,sstr , 110);

	IFORM_GetWidget((IForm*)pMe->unSelectDialog, WID_BACKGROUND, &backWidget);
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IWIDGET_SetBorderWidth(backWidget, 1);
	IFORM_SetWidget((IForm*)pMe->unSelectDialog, WID_BACKGROUND, backWidget);
	
	IFORM_SetWidget((IForm*)pMe->unSelectDialog, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->unSelectDialog, IDF_ALIGN_FILL);
    
	HANDLERDESC_Init(&pMe->unSelectDlgHandler, UnSelectDialog_HandleEvent, pMe, NULL);
	IFORM_SetHandler((IForm*)pMe->unSelectDialog, &pMe->unSelectDlgHandler);
	
	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->unSelectDialog);
	FREEIF(sstr);
	IWIDGET_Release(backdropWidget);
	IWIDGET_Release(backWidget);

	return TRUE;
}