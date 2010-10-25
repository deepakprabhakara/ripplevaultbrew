
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
 Module Name				: StartForm.c
 Language used				: C and Brew APIs
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 07-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :

***************************************************************/

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
#include "StartForm.h"
#include "LoginForm.h"
#include "RippleVault.bid"
#include "RippleVault.brh"

#define RIPPLEVAULT_RES_FILE "RippleVault.bar"

// ================================================================================
// FUNCTION		: StartForm_Delete
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THEI MODULE FROM HERE
// ================================================================================
void StartForm_Delete(StartForm* pMe) 
{
	if(pMe->mainContainer)
	{
		IXYCONTAINER_Release(pMe->mainContainer);
		pMe->mainContainer = NULL;
	}
	if (pMe->m_pIImage1)
	{
		RELEASEIF(pMe->m_pIImage1);
		pMe->m_pIImage1 = NULL;
	}
	if (pMe->m_pIImage2)
	{
		RELEASEIF(pMe->m_pIImage2);
		pMe->m_pIImage2 = NULL;
	}
	if(pMe->optionsMenu)
	{
		IPOPUPMENU_Release(pMe->optionsMenu);
		pMe->optionsMenu = NULL;
	}
	if(pMe->loginForm) 
	{
		IFORM_Release((IForm*)pMe->loginForm);
		pMe->loginForm = NULL;
	}

	FREEIF(pMe->str1);
	FREEIF(pMe->str2);
	if(pMe)
	  FREE(pMe);
}
// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: HANDLE OPTION SELECTION FROM POPUP MENU
// ================================================================================
static void OptionSelectionHandler(StartForm* pMe, int idOption) 
{

	if( 0==idOption)
	{
		IROOTFORM_PopForm(pMe->rootForm);
		IPOPUPMENU_Release(pMe->optionsMenu);
		pMe->optionsMenu = NULL;
		LoginForm_New(&pMe->loginForm,pMe->pIShell,pMe->rootForm, FALSE);
		IROOTFORM_PushForm(pMe->rootForm, pMe->loginForm);

	}else if ( 1==idOption)
	{
		IROOTFORM_PopForm(pMe->rootForm);
		IPOPUPMENU_Release(pMe->optionsMenu);
		pMe->optionsMenu = NULL;
		LoginForm_New(&pMe->loginForm,pMe->pIShell,pMe->rootForm, TRUE);
		IROOTFORM_PushForm(pMe->rootForm, pMe->loginForm);
	}


}
// ================================================================================
// FUNCTION		: Start_OptionsEventHandler
// DESCRIPTION	: HANDLE EVENTS FROM POPUP MENU
// ================================================================================
static boolean Start_OptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   StartForm* pMe = (StartForm*) po;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		AEERect rc;
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cxScreen - pMe->DeviceInfo.cyScreen/7;
		rc.dx = pMe->DeviceInfo.cxScreen;
		rc.dy = pMe->DeviceInfo.cyScreen/5;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}
	
	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		if(pMe->optionsMenu) 
		{
			IROOTFORM_PopForm(pMe->rootForm);
			IPOPUPMENU_Release(pMe->optionsMenu);
			pMe->optionsMenu = NULL;
			return TRUE;
		}
	}
	return HANDLERDESC_Call(&pMe->optionsMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: MenuModal_EventHandler
// DESCRIPTION	: LISTEN EVENT FROM POPUP MENU
// ================================================================================
static void MenuModal_EventHandler(StartForm *pMe, ModelEvent *pev) 
{
	if(pev->evCode == EVT_MDL_FOCUS_SELECT) 
		if(pev->dwParam == 0)	
		{

		}
}

// ================================================================================
// FUNCTION		: StartForm_HandleEvent
// DESCRIPTION	: DANDLE EVENT FROM START FORM
// ================================================================================

static boolean StartForm_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	StartForm* pMe = (StartForm*) po;
    switch (evt) 
	{
	case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT2:
				ISHELL_CloseApplet(pMe->pIShell, TRUE);
				return (TRUE);
				break;
			case AVK_SOFT1:
				Start_CreateOptionsMenu(pMe);
				return (TRUE);
				break;
			case AVK_CLR:
				ISHELL_CloseApplet(pMe->pIShell, TRUE);
				break;
			}
			return TRUE;
		}

	case EVT_USER_REG_SUCCESS:
		{
			return TRUE;
			break;
		}
	}
	return HANDLERDESC_Call(&pMe->mainFormHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PickAdapter
// DESCRIPTION	: 
// ================================================================================

static void PickAdapter(void *pUnused, void **ppValueIn, int nLen, void **ppValueOut, int *pnLenOut)
{
   *pnLenOut = 1;  
}

// ================================================================================
// FUNCTION		: Start_CreateOptionsMenu
// DESCRIPTION	: CREATE OPTION MENU FOR START FORM
// ================================================================================

int Start_CreateOptionsMenu(StartForm* pMe) 
{
	int result = 0;
	IWidget* backDropWidget;
	IFont *piFont = 0;

	ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, &piFont);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
	   return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->optionsMenu) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_NEWUSER, 0,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_EXTUSER, 1,MMF_ENABLED) != 0)
	   return (FALSE);
	IFORM_SetSoftkeys((IForm*)pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);
	IFORM_SetSelectHandler((IForm*)pMe->optionsMenu, (PFNSELECT)OptionSelectionHandler, pMe);
	HANDLERDESC_Init(&pMe->optionsMenuHandler, Start_OptionsEventHandler, pMe, 0);
	IFORM_SetHandler((IForm*)pMe->optionsMenu, &pMe->optionsMenuHandler);

	//wrap the pop-up form in a scrollbar decorator
	{
		IWidget* backWidget = NULL;

		IFORM_GetWidget((IForm*)pMe->optionsMenu, WID_LISTITEM, &backWidget);
		IWIDGET_SetItemHeight(backWidget, 47);

		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IWIDGET_SetBorderWidth(backWidget, 0);
		IFORM_SetWidget((IForm*)pMe->optionsMenu, WID_LISTITEM, backWidget);

		IWIDGET_Release(backWidget);

		IFORM_GetWidget((IForm*)pMe->optionsMenu, WID_LIST, &backWidget);
		IWIDGET_SetItemHeight(backWidget, 27);
		IWIDGET_SetItemWidth(backWidget, 15);
		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IWIDGET_SetBorderWidth(backWidget, 0);
		IWIDGET_SetFont(backWidget, piFont);
		IFORM_SetWidget((IForm*)pMe->optionsMenu, WID_LIST, backWidget);

		IFORM_SetWidget((IForm*)pMe->optionsMenu, WID_BACKDROP, backDropWidget);
		IWIDGET_Release(backWidget);
	}
	IWIDGET_Release(backDropWidget);
	RELEASEIF(piFont);
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->optionsMenu);
	return (TRUE);
}

// ================================================================================
// FUNCTION		: StartForm_PopulateMainContainer
// DESCRIPTION	: POPULATE START FORM
// ================================================================================

int StartForm_PopulateMainContainer(StartForm* pMe) 
{
	int result;
	IWidget* containerWidget = NULL;
	IWidget* imageWidget = NULL;
	IImage* m_pIImage;
	WidgetPos wpos;

	wpos.x = pMe->DeviceInfo.cxScreen/2 - 35;
	wpos.y = pMe->DeviceInfo.cyScreen/2 - 45;
	wpos.bVisible = TRUE;

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGEWIDGET, (void**)&imageWidget);
	m_pIImage  = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_MAIN);
	IWIDGET_SetImage(imageWidget,m_pIImage);

	IXYCONTAINER_Insert(pMe->mainContainer, (IWidget*)imageWidget, WIDGET_ZNORMAL, &wpos);
	result = IXYCONTAINER_QueryInterface(pMe->mainContainer, AEEIID_WIDGET, (void**)&containerWidget);
	
	if(result ==0)
		IFORM_SetWidget((IForm*)pMe->startForm, WID_FORM, containerWidget);
	IWIDGET_Release(containerWidget);
	IWIDGET_Release((IWidget*)imageWidget);
	RELEASEIF(m_pIImage);
	return result;
}

// ================================================================================
// FUNCTION		: StartForm_New
// DESCRIPTION	: INITIATE START FORM
// ================================================================================

int StartForm_New(IForm** ppo, IShell *piShell,IRootForm *pRootForm,AEEDeviceInfo  DInfo) 
{
	int result=0;
	StartForm *pMe = MALLOCREC(StartForm);
	IImage* m_pIImage;
	IFont *piFont = 0;
	if(!pMe)
		return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;
	pMe->DeviceInfo = DInfo;
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->startForm) == 0)
	{
		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->mainContainer);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);

		*ppo = pMe->startForm;
		result =  IFORM_SetSoftkeys((IForm*)pMe->startForm, RIPPLEVAULT_RES_FILE, IDS_REGISTER,IDS_CANCEL);

		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->startForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->startForm, FID_TITLE, RIPPLEVAULT_RES_FILE, MAINFORMTITLE);

		RELEASEIF(m_pIImage);

		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_MAIN);		
		result = StartForm_PopulateMainContainer(pMe);	//add items to the form
		
		HANDLERDESC_Init(&pMe->mainFormHandler, StartForm_HandleEvent, pMe, StartForm_Delete);
		IFORM_SetHandler((IForm*)pMe->startForm, &pMe->mainFormHandler);
	}
	RELEASEIF(m_pIImage);
	RELEASEIF(piFont);
	return result;
}