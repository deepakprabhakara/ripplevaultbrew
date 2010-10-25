
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
#include "AEEDialog.h"
#include "UserData.h"

#include "ContactForm.h"
#include "CSettings.h"

#include "RippleVault.bid"
#include "RippleVault.brh"




#define RIPPLEVAULT_RES_FILE "RippleVault.bar"
#define VIEWCONTACTURLSIZE   50

// ================================================================================
// FUNCTION		: ContactForm_Delete
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THEI MODULE FROM HERE
// ================================================================================

void ContactForm_Delete(ContactForm* pMe) 
{
	if(pMe->mainContainer)
	{
		IPROPCONTAINER_Release(pMe->mainContainer);
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
	if(pMe->csettings)
	{
		IFORM_Release(pMe->csettings);
		pMe->csettings = NULL;
	}

   FREEIF(pMe->str1);
   FREEIF(pMe->str4);
   if(pMe)
      FREE(pMe);
}

// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THEI MODULE FROM HERE
// ================================================================================

static void Contact_OptionSelectionHandler(ContactForm* pMe, int idOption) 
{
	AECHAR* sstr=NULL;
	char* text=NULL;
	char* url=NULL;
	user *pUser=NULL;

	switch(pMe->selectedItem)
	{
		case (0):
	

			IROOTFORM_PopForm(pMe->rootForm);
			if(pMe->optionsMenu)
			{
			IPOPUPMENU_Release(pMe->optionsMenu);
			pMe->optionsMenu = NULL;
			}
			if (GetUserData(pMe->pIShell, &pUser))
			{
			
				url=pUser->url;
				
			}

			sstr = (AECHAR*)MALLOC(300);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VIEWCONTACTTEXT,sstr , 300);
			text=(char*)MALLOC(350);

			WSTRTOSTR(sstr, text, 350);
			STRCAT( text, url );
			
			FREEIF(sstr);
			ContactForm_Dialog(pMe,text);
			FREEIF(sstr);
			FREEIF(text);
			FREEIF(pUser);

			break;
		case (1):
	
			IROOTFORM_PopForm(pMe->rootForm);
			if(pMe->optionsMenu)
			{
			IPOPUPMENU_Release(pMe->optionsMenu);
			pMe->optionsMenu = NULL;
			}
			if(pMe->csettings == NULL)
			CSettings_New(&pMe->csettings, pMe->pIShell, pMe->rootForm);//
			IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->csettings);
			
			break;

   }

}

// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THEI MODULE FROM HERE
// ================================================================================

static boolean Contact_OptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   ContactForm* pMe = (ContactForm*) po;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen*8/10;
		rc.dx = pMe->DeviceInfo.cyScreen;
		rc.dy = pMe->DeviceInfo.cyScreen/10;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		if(pMe->optionsMenu) 
		{
			IROOTFORM_PopForm(pMe->rootForm);
			if(pMe->csettings)
			{
				IFORM_Release(pMe->csettings);
				pMe->csettings = NULL;
			}
			return TRUE;
		}
	}


   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->optionsMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: MenuModal_EventHandler
// DESCRIPTION	: LISTEN FOR CHANGES ON MAIN PAGE
// ================================================================================

static void MenuModal_EventHandler(ContactForm *pMe, ModelEvent *pev) 
{
	char* url;
	user *pUser=NULL;
	AECHAR* sstr=NULL;
	char* text;
	if(pev->evCode == EVT_MDL_FOCUS_SELECT) {
		
		if(pev->dwParam == 0)
		{

			if (GetUserData(pMe->pIShell, &pUser))
			{
			
				url=pUser->url;
				
			}
			
				
			sstr = (AECHAR*)MALLOC(300);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VIEWCONTACTTEXT,sstr , 300);
			text=(char*)MALLOC(350);

			WSTRTOSTR(sstr, text, 350);
			STRCAT( text, url );
					
			ContactForm_Dialog(pMe,text);
			FREEIF(sstr);
			FREEIF(text);
			FREEIF(pUser);
		
		}
		if(pev->dwParam == 1)	
		{
			if(pMe->csettings == NULL)
				CSettings_New(&pMe->csettings, pMe->pIShell, pMe->rootForm);
			
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->csettings);

		}
	}
	if (pev->evCode == EVT_MDL_FOCUS_CHANGE)
		pMe->selectedItem = (int)pev->dwParam;	

}

// ================================================================================
// FUNCTION		: ContactForm_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM CONTACT MAON PAGE
// ================================================================================

static boolean ContactForm_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   ContactForm* pMe = (ContactForm*) po;
   int abc;
    switch (evt) 
	{
	case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT2:
				 abc = 1;
					IROOTFORM_PopForm(pMe->rootForm);
				   if(pMe->optionsMenu)
				   {
					  IPOPUPMENU_Release(pMe->optionsMenu);
					  pMe->optionsMenu = NULL;
					}
					return (TRUE);
					break;
			
			case AVK_SOFT1:
					Contact_CreateOptionsMenu(pMe);
					return (TRUE);
					break;
			case AVK_CLR:
				 abc = 1;
					IROOTFORM_PopForm(pMe->rootForm);
				   if(pMe->optionsMenu)
				   {
					  IPOPUPMENU_Release(pMe->optionsMenu);
					  pMe->optionsMenu = NULL;
					}
					return (TRUE);
					break;
			}
		}
	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->mainFormHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PickAdapter
// DESCRIPTION	: 
// ================================================================================
static void PickAdapter(void *pUnused, void **ppValueIn, int nLen, void **ppValueOut, int *pnLenOut)
{
   *pnLenOut = 1;  // force length to be 1, instead of calculating
}

// ================================================================================
// FUNCTION		: ContactForm_PopulateMainContainer
// DESCRIPTION	: POPULATE CONTACT FORM MAIN PAGE
// ================================================================================

int ContactForm_PopulateMainContainer(ContactForm* pMe) 
{
	int result = 0;

	ImageStaticWidget* imageWidget = NULL;
	IMenuModel * iMenuModal = NULL;
	IWidget* wItemList;
	IWidget* containerWidget = NULL;
	IWidget* pScrollBar = NULL;
	WExtent we;
	WidgetProp wp;
	IFont *piFont = 0;

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGESTATICWIDGET, (void**)&imageWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MENUMODEL, (void**)&iMenuModal);
  	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&wItemList);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**)&pScrollBar);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);

	//load the strings
	pMe->str1 = (AECHAR*) MALLOC(30);
	pMe->str4 = (AECHAR*) MALLOC(20);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_CVIEW, pMe->str1, 30);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_CSETTINGS, pMe->str4, 20);

	//load the images
	pMe->m_pIImage1 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CVIEW);
	pMe->m_pIImage2 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_SETTING);

	pMe->imageInfo1.pwText = (AECHAR*)pMe->str1;
	pMe->imageInfo1.piImage = pMe->m_pIImage1;
	pMe->imageInfo2.pwText = (AECHAR*)pMe->str4;
	pMe->imageInfo2.piImage = pMe->m_pIImage2;


	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo1,1,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo2,2,MMF_ENABLED);

	//set the vidget to act as a menu modal vidget
	IWIDGET_SetModel(wItemList, IMENUMODEL_TO_IMODEL(iMenuModal));

	//set the picklist widget properties
	IWIDGET_SetBorderWidth(wItemList, 0);
	IWIDGET_SetItemHeight(wItemList, 25);
	IWIDGET_SetItemWidth(wItemList, 15);
	IWIDGET_SetHintCols(wItemList, 5);
	IWIDGET_GetPreferredExtent(wItemList, &we);
	IWIDGET_SetExtent(wItemList, &we);

	IWIDGET_SetFont((IWidget *)imageWidget, piFont);
	IWIDGET_SetSelectedShadowOffset((IWidget *)imageWidget,2);
	IWIDGET_SetShadowColor((IWidget *)imageWidget,MAKE_RGB(122,122,122));
	IWIDGET_SetSelectedActiveBGColor((IWidget*)imageWidget, MAKE_RGB(128,128,255));

	IWIDGET_SetProperty(pScrollBar, PROP_ACTIVE_SCROLLCOLOR, MAKE_RGB(128,128,255));

	IDECORATOR_SetWidget((IDecorator*)pScrollBar, (IWidget *)wItemList);
	IDECORATOR_SetWidget((IDecorator*)wItemList, (IWidget *)imageWidget);

	// get the view model of the wItemList and register a listener 
	// in order to to pick up operator selection
	{
		IModel* pickListViewModel = NULL;
		IWIDGET_GetViewModel(wItemList, &pickListViewModel);
		IMODEL_AddListenerEx(pickListViewModel, &pMe->menuListener, (PFNLISTENER)MenuModal_EventHandler, pMe);
		IMODEL_Release(pickListViewModel);
	}
	wp.bVisible = TRUE;
	wp.prop = 1;
	IPROPCONTAINER_Insert(pMe->mainContainer, pScrollBar, WIDGET_ZNORMAL, &wp);

	//set WID_FORM to the container
	result = IPROPCONTAINER_QueryInterface(pMe->mainContainer, AEEIID_WIDGET, (void**)&containerWidget);
	if(result ==0) 
	{
		IFORM_SetWidget((IForm*)pMe->contactForm, WID_FORM, containerWidget);
	}

	IWIDGET_Release((IWidget*)imageWidget);
	IMENUMODEL_Release(iMenuModal);
	IWIDGET_Release(wItemList);
	IWIDGET_Release(pScrollBar);
	if (containerWidget)
		IWIDGET_Release(containerWidget);
	RELEASEIF(piFont);
	if(result != 0)
		return EFAILED;

	return result;
}

// ================================================================================
// FUNCTION		: Contact_CreateOptionsMenu
// DESCRIPTION	: CREATE OPTION MENU FOR CONTACT FORM 
// ================================================================================

int Contact_CreateOptionsMenu(ContactForm* pMe) 
{
	int result = 0;
	IWidget* backDropWidget;
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
		return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->optionsMenu) != 0)
		return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, 0,MMF_ENABLED) != 0)
		return (FALSE);
	IFORM_SetSoftkeys((IForm*)pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);

	IFORM_SetSelectHandler((IForm*)pMe->optionsMenu, (PFNSELECT)Contact_OptionSelectionHandler, pMe);
	HANDLERDESC_Init(&pMe->optionsMenuHandler, Contact_OptionsEventHandler, pMe, 0);
	IFORM_SetHandler((IForm*)pMe->optionsMenu, &pMe->optionsMenuHandler);

	//wrap the pop-up form in a scrollbar decorator
	{
		IWidget* piScrollWidget = NULL;
		IWidget* mainFormWidget = NULL;

		if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**) &piScrollWidget) != 0)
			return (FALSE);

		// change the scroll pad
		IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);
		IFORM_GetWidget((IForm*)pMe->optionsMenu, WID_FORM, &mainFormWidget);
		IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
		IFORM_SetWidget((IForm*)pMe->optionsMenu, WID_FORM, piScrollWidget);
		
		IFORM_SetWidget((IForm*)pMe->optionsMenu, WID_BACKDROP, backDropWidget);
		IWIDGET_Release(mainFormWidget);
		IWIDGET_Release(piScrollWidget);
		IWIDGET_Release(backDropWidget);
	}
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->optionsMenu);
	return (TRUE);
}


// ================================================================================
// FUNCTION		: ContactForm_New
// DESCRIPTION	: INITIATE CONTACT FORM 
// ================================================================================

int ContactForm_New(IForm** ppo, IShell *piShell,IRootForm *pRootForm) 
{
	int result=0;
	ContactForm *pMe = MALLOCREC(ContactForm);
	IImage* m_pIImage;
	if(!pMe)
		return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->contactForm) == 0)
	{
		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->mainContainer);
		*ppo = pMe->contactForm;
		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->contactForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->contactForm, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_CONTACT);

		result =  IFORM_SetSoftkeys((IForm*)pMe->contactForm, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);
		result = ContactForm_PopulateMainContainer(pMe);	//add items to the form
		HANDLERDESC_Init(&pMe->mainFormHandler, ContactForm_HandleEvent, pMe, ContactForm_Delete);
		IFORM_SetHandler((IForm*)pMe->contactForm, &pMe->mainFormHandler);

	}

	RELEASEIF(m_pIImage);
	return result;
}

// ================================================================================
// FUNCTION		: Dialog_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM ContactForm_Dialog
// ================================================================================

static boolean ContactForm_Dialog_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   ContactForm* pMe = (ContactForm*) po;
   
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = pMe->DeviceInfo.cxScreen/40;
		rc.y = pMe->DeviceInfo.cyScreen/4;
		rc.dx = pMe->DeviceInfo.cyScreen*5/6;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*10/28;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}
   
	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2 || wParam == AVK_SOFT1))
	{
		IROOTFORM_PopForm(pMe->rootForm);
		deleteContactDialog(pMe);
		if(pMe->dialog)
		{
			IDIALOG_Release(pMe->dialog);
			pMe->dialog=NULL;
		}
		return TRUE;
	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->dlgHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: deleteContactDialog
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN DIALOG
// ================================================================================

void deleteContactDialog(ContactForm* pMe)
{
	if(pMe->dialog)
	{
		IDIALOG_Release(pMe->dialog);
		pMe->dialog=NULL;
	}
	FREEIF(pMe->str2);
}

// ================================================================================
// FUNCTION		: ContactForm_Dialog
// DESCRIPTION	: POPULATE DIALOG FOR CONTACT FORM
// ================================================================================

int ContactForm_Dialog(ContactForm* pMe,char* text)
{
	int result=0;
	int nLen=0;
	int size=0;
	IWidget* backdropWidget;
	IWidget* backWidget = NULL;
	AECHAR* sstr;
	AECHAR* data;
	AECHAR format[]={'%','s',' ',' ','\0'};//{'%','s',' ','(','%','d',')','\0'};

	if (NULL==pMe->dialog)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_INFODIALOG, (void**) &pMe->dialog);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);

		IFORM_SetResText((IForm*)pMe->dialog, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
		IFORM_SetSoftkeys((IForm*)pMe->dialog, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
		
		nLen = STRLEN(text);
		size=(nLen+1)*sizeof(AECHAR);	
		data = (AECHAR*) MALLOC(size);
		
		STRTOWSTR(text, data, size);
		DBGPRINTF("%S", data);
		IFORM_SetText((IForm*)pMe->dialog, FID_TEXT, data); 
		pMe->str2 = (AECHAR*)MALLOC(120);
		sstr = (AECHAR*)MALLOC(110);
		result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLEVAULT,sstr , 110);
		WSPRINTF(pMe->str2, 120, format,sstr);
		DBGPRINTF("%S",sstr);
		IFORM_SetText((IForm*)pMe->dialog,FID_TITLE, pMe->str2);
	
		IFORM_GetWidget((IForm*)pMe->dialog, WID_TITLE, &backWidget);
		IWIDGET_SetBorderWidth(backWidget, 1);
		IWIDGET_SetBorderColor(backWidget,MAKE_RGB(100, 100, 100));
		IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
		IFORM_SetWidget((IForm*)pMe->dialog, WID_TITLE, backWidget);
		IWIDGET_Release(backWidget);

		IFORM_GetWidget((IForm*)pMe->dialog, WID_BACKGROUND, &backWidget);
		IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
		IWIDGET_SetBorderWidth(backWidget, 1);
		IFORM_SetWidget((IForm*)pMe->dialog, WID_BACKGROUND, backWidget);
		
		IFORM_SetWidget((IForm*)pMe->dialog, WID_BACKDROP, backdropWidget);
		IWIDGET_SetFlags((IWidget*)pMe->dialog, IDF_ALIGN_FILL);
    
		HANDLERDESC_Init(&pMe->dlgHandler, ContactForm_Dialog_HandleEvent, pMe, deleteContactDialog);
		IFORM_SetHandler((IForm*)pMe->dialog, &pMe->dlgHandler);
		
		FREEIF(sstr);
		FREEIF(data);
		IWIDGET_Release(backdropWidget);
		IWIDGET_Release(backWidget);
	}

	if (pMe->dialog)
		IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->dialog);

	return TRUE;
}

