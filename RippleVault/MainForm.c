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
#include "MainForm.h"
#include "../SyncEng/SMEvents.h"

#define RIPPLEVAULT_RES_FILE "RippleVault.bar"
#define SCROLLRATE		5
#define HELPTEXTSIZE	800

// ================================================================================
// FUNCTION		: MainForm_Delete
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THEI MODULE FROM HERE
// ================================================================================

void MainForm_Delete(MainForm* pMe) 
{
	
	Delete_PhotoFormSettigs(pMe);
	Delete_SettingsAutoStart(pMe);
	deletePhotoForm_HelpForm(pMe);
	deleteMainFormDialog(pMe);
	deletePhotoForm_HelpFormPage(pMe);
	if(pMe->contactForm) 
	{
		IFORM_Release((IForm*)pMe->contactForm);
		pMe->contactForm = NULL;
	}
	if(pMe->photoForm) 
	{
		IFORM_Release((IForm*)pMe->photoForm);
		pMe->photoForm = NULL;
	}
	if(pMe->videoForm) 
	{
		IFORM_Release((IForm*)pMe->videoForm);
		pMe->videoForm = NULL;
	}
	if(pMe->messageForm) 
	{
		IFORM_Release((IForm*)pMe->messageForm);
		pMe->messageForm = NULL;
	}
	if(pMe->optionsMenu)
	{
		IPOPUPMENU_Release(pMe->optionsMenu);
		pMe->optionsMenu=NULL;
	}
	if(pMe->helpMenu)
	{
		IPOPUPMENU_Release(pMe->helpMenu);
		pMe->helpMenu=NULL;
	}

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
	if (pMe->m_pIImage3)
	{
		RELEASEIF(pMe->m_pIImage3);
		pMe->m_pIImage3 = NULL;
	}
	if (pMe->m_pIImage4)
	{
		RELEASEIF(pMe->m_pIImage4);
		pMe->m_pIImage4 = NULL;
	}
	if (pMe->m_pIImage5)
	{
		RELEASEIF(pMe->m_pIImage5);
		pMe->m_pIImage5 = NULL;
	}	
	FREEIF(pMe->aeStr1);
	FREEIF(pMe->aeStr2);
	FREEIF(pMe->aeStr3);
	FREEIF(pMe->aeStr4);

   if(pMe)
      FREE(pMe);
}

// ================================================================================
// FUNCTION		: MainForm_FreePrevForm
// DESCRIPTION	: 
// ================================================================================

void MainForm_FreePrevForm(MainForm *pMe)
{
	if(pMe->contactForm) 
	{
		IFORM_Release((IForm*)pMe->contactForm);
		pMe->contactForm = NULL;
	}
	if(pMe->photoForm) 
	{
		IFORM_Release((IForm*)pMe->photoForm);
		pMe->photoForm = NULL;
	}
	if(pMe->videoForm) 
	{
		IFORM_Release((IForm*)pMe->videoForm);
		pMe->videoForm = NULL;
	}
	if(pMe->messageForm) 
	{
		IFORM_Release((IForm*)pMe->messageForm);
		pMe->messageForm = NULL;
	}
}

// ================================================================================
// FUNCTION		: ShowSelectedForm
// DESCRIPTION	: 
// ================================================================================

int ShowSelectedForm(MainForm *pMe,int param)
{
	//free memory taken to previous form
	MainForm_FreePrevForm(pMe);
	
	switch(param)
	{
		case (0):
		{
			if(pMe->contactForm == NULL)
				ContactForm_New(&pMe->contactForm, pMe->pIShell, pMe->rootForm);
			IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->contactForm);
			break;
		}
		case(1):
		{
			if(pMe->photoForm == NULL)
				PhotoForm_New(&pMe->photoForm, pMe->pIShell, pMe->rootForm);
			IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->photoForm);
			break;
		}
		case(2):
		{
			if(pMe->videoForm == NULL)
				VideoForm_New(&pMe->videoForm, pMe->pIShell, pMe->rootForm);
			IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->videoForm);
			break;
		}
		case(3):
		{

			if(pMe->messageForm == NULL)
				MessageForm_New(&pMe->messageForm, pMe->pIShell, pMe->rootForm);
			IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->messageForm);
			break;
		}
	}
	pMe->optionselected = 0;
	return TRUE;
}

//update the operator display when an operator is selected from the pickwidget
//pev->dwParam is the index of the selected item

// ================================================================================
// FUNCTION		: MenuModal_EventHandler
// DESCRIPTION	: LISTEN FOR CHANGES ON MAIN PAGE
// ================================================================================

static void MenuModal_EventHandler(MainForm *pMe, ModelEvent *pev) 
{

	if(pev->evCode == EVT_MDL_FOCUS_SELECT) 
		ShowSelectedForm(pMe,pev->dwParam);
	if (pev->evCode == EVT_MDL_FOCUS_CHANGE)
		pMe->selectedItem = (int)pev->dwParam;

}

// ================================================================================
// FUNCTION		: MainForm_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM MAIN FORM 
// ================================================================================

static boolean MainForm_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   MainForm* pMe = (MainForm*) po;

	switch (evt) 
	{
		case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT1:
				{
					pMe->optionselected = 1;
					Main_CreateOptionsMenu(pMe);
					return (TRUE);
					break;
				}
			case AVK_SOFT2:
				{
					ISHELL_CloseApplet(pMe->pIShell, FALSE);
					return (TRUE);
					break;
				}
			case AVK_CLR:
				ISHELL_CloseApplet(pMe->pIShell, FALSE);
				return (TRUE);
				break;
			}
		}break;


		default:
				
			break;

	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->mainFormHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: 
// ================================================================================

static void PickAdapter(void *pUnused, void **ppValueIn, int nLen, void **ppValueOut, int *pnLenOut)
{
   *pnLenOut = 1;  // force length to be 1, instead of calculating
}

// ================================================================================
// FUNCTION		: Main_OptionSelectionHandler
// DESCRIPTION	: HANDLE OPTION SELECTION FROM Main_CreateOptionsMenu
// ================================================================================

static void Main_OptionSelectionHandler(MainForm* pMe, int idOption) 
{

	AECHAR* sstr=NULL;
	AECHAR* title=NULL;
    

	IROOTFORM_PopForm(pMe->rootForm);
	if(pMe->optionsMenu){
		IPOPUPMENU_Release(pMe->optionsMenu);
		pMe->optionsMenu=NULL;
	}
	
	switch(idOption)
	{
	case 0:
		
		break;
	
	case 1:
		PhotoForm_SettingsForm(&pMe->mainForm, pMe->pIShell,pMe->rootForm,pMe);
		break;
	case 2:
		PhotoForm_HelpFormMain(&pMe->mainForm, pMe->pIShell,pMe->rootForm,pMe->DeviceInfo,pMe);
		break;

	case 3:
		ISHELL_PostEvent(pMe->pIShell, AEECLSID_RIPPLEVAULT, EVT_USER_EXIT, 0, 0);
		break;
	
	}
	
}

// ================================================================================
// FUNCTION		: Main_OptionsEventHandler
// DESCRIPTION	: HANDLE EVENR FROM Main_CreateOptionsMenu
// ================================================================================

static boolean Main_OptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   MainForm* pMe = (MainForm*) po;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;

		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);

		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen/2;
		rc.dx = pMe->DeviceInfo.cxScreen;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*3/5;
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

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->optionsMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: Main_CreateOptionsMenu
// DESCRIPTION	: POPULATE OPTION MENU FOR MAIN PAGE
// ================================================================================

int Main_CreateOptionsMenu(MainForm* pMe) 
{
	int result = 0;
	IWidget* backDropWidget;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
		return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->optionsMenu) != 0)
		return (FALSE);
    if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_FRIEND, 0,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_SETTINGS, 1,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_HELP, 2,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_EXIT, 3,MMF_ENABLED) != 0)
	   return (FALSE);


	IFORM_SetSoftkeys((IForm*)pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);
	IFORM_SetSelectHandler((IForm*)pMe->optionsMenu, (PFNSELECT)Main_OptionSelectionHandler, pMe);
	HANDLERDESC_Init(&pMe->optionsMenuHandler, Main_OptionsEventHandler, pMe, 0);
	IFORM_SetHandler((IForm*)pMe->optionsMenu, &pMe->optionsMenuHandler);

	//wrap the pop-up form in a scrollbar decorator
	{
		IWidget* piScrollWidget = NULL;
		IWidget* mainFormWidget = NULL;
		IWidget* backWidget = NULL;
		if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**) &piScrollWidget) != 0)
			return (FALSE);
		// change the scroll pad
		IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);

		IFORM_GetWidget((IForm*)pMe->optionsMenu, WID_FORM, &mainFormWidget);
		IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
		IFORM_SetWidget((IForm*)pMe->optionsMenu, WID_FORM, piScrollWidget);

		IFORM_GetWidget((IForm*)pMe->optionsMenu, WID_LISTITEM, &backWidget);
		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->optionsMenu, WID_LISTITEM, backWidget);

		IWIDGET_Release(mainFormWidget);
		IWIDGET_Release(piScrollWidget);
		IWIDGET_Release(backDropWidget);
		IWIDGET_Release(backWidget);
	}

	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->optionsMenu);
	return (TRUE);
}

// ================================================================================
// FUNCTION		: MainForm_PopulateMainContainer
// DESCRIPTION	: POPULATE MAIN FORM MAIN PAGE
// ================================================================================

int MainForm_PopulateMainContainer(MainForm* pMe) 
{
	int result = 0;
	ImageStaticWidget* imageWidget = NULL;
	IMenuModel * iMenuModal = NULL;
	IWidget* wItemList = NULL;
	IWidget* containerWidget = NULL;
	IWidget* pScrollBar = NULL;
	WExtent we;
	WidgetProp wp;
	IFont *piFont = NULL;

	result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->mainContainer);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGESTATICWIDGET, (void**)&imageWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MENUMODEL, (void**)&iMenuModal);
  	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&wItemList);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**)&pScrollBar);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);

	//load the strings
	pMe->aeStr1 = (AECHAR*) MALLOC(30);
	pMe->aeStr2 = (AECHAR*) MALLOC(17);
	pMe->aeStr3 = (AECHAR*) MALLOC(17);
	pMe->aeStr4 = (AECHAR*) MALLOC(20);

	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_CONTACT, pMe->aeStr1, 30);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PHOTOS, pMe->aeStr2, 17);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VIDEOS, pMe->aeStr3, 17);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_MESSAGES, pMe->aeStr4, 20);

	//load the images
	pMe->m_pIImage1 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CONTACT);
	pMe->m_pIImage2 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_PHOTO);
	pMe->m_pIImage3 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_VIDEO);
	pMe->m_pIImage4 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_MESSAGE);

	pMe->imageInfo1.pwText = (AECHAR*)pMe->aeStr1;
	pMe->imageInfo1.piImage = pMe->m_pIImage1;
	pMe->imageInfo2.pwText = (AECHAR*)pMe->aeStr2;
	pMe->imageInfo2.piImage = pMe->m_pIImage2;
	pMe->imageInfo3.pwText = (AECHAR*)pMe->aeStr3;
	pMe->imageInfo3.piImage = pMe->m_pIImage3;
	pMe->imageInfo4.pwText = (AECHAR*)pMe->aeStr4;
	pMe->imageInfo4.piImage = pMe->m_pIImage4;

	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo1,1,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo2,2,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo3,3,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo4,4,MMF_ENABLED);

	//set the vidget to act as a menu modal vidget
	IWIDGET_SetModel(wItemList, IMENUMODEL_TO_IMODEL(iMenuModal));

	//set the picklist widget properties
	IWIDGET_SetBorderWidth(wItemList, 0);
	IWIDGET_SetItemHeight(wItemList, 25);
	IWIDGET_SetItemWidth(wItemList, 15);
	IWIDGET_SetHintCols(wItemList, 5);
	IWIDGET_GetPreferredExtent(wItemList, &we);
	IWIDGET_SetExtent(wItemList, &we);
	IWIDGET_SetBorderStyle(wItemList, 5);

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
		IFORM_SetWidget((IForm*)pMe->mainForm, WID_FORM, containerWidget);
	}

	IWIDGET_Release((IWidget*)imageWidget);
	IMENUMODEL_Release(iMenuModal);
	IWIDGET_Release(wItemList);
	IWIDGET_Release(pScrollBar);
	RELEASEIF(piFont);
	if (containerWidget)
		IWIDGET_Release(containerWidget);

	if(result != 0)
		return EFAILED;

	return result;
}

// ================================================================================
// FUNCTION		: MainForm_New
// DESCRIPTION	: INITIATE POPULATE MAIN FORM MAIN PAGE
// ================================================================================

int MainForm_New(IForm** ppo, IShell *piShell, IRootForm *pRootForm, IDisplay *piDisplay) 
{
	int result=0;

	//allocate space for the form
	MainForm *pMe = MALLOCREC(MainForm);
	IImage* m_pIImage;
	if(!pMe)
		return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;
	pMe->pIDisplay = piDisplay;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->mainForm) == 0)
	{
		*ppo = pMe->mainForm;
		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->mainForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->mainForm, FID_TITLE, RIPPLEVAULT_RES_FILE, MAINFORMTITLE);
		result =  IFORM_SetSoftkeys((IForm*)pMe->mainForm, RIPPLEVAULT_RES_FILE, MAINSK1, MAINSK2);

		result = MainForm_PopulateMainContainer(pMe);
		HANDLERDESC_Init(&pMe->mainFormHandler, MainForm_HandleEvent, pMe, MainForm_Delete);
		IFORM_SetHandler((IForm*)pMe->mainForm, &pMe->mainFormHandler);

	}

	RELEASEIF(m_pIImage);

	return result;
}

// ================================================================================
// FUNCTION		: PhotoFormSettingsMenuModal_EventHandler
// DESCRIPTION	: LISTEN FOR CHANGES ON PhotoForm_Settings
// ================================================================================
static void PhotoFormSettingsMenuModal_EventHandler(MainForm *pMe, ModelEvent *pev) 
{
	AECHAR* sstr=NULL;
	AECHAR* title=NULL;
	if(pev->evCode == EVT_MDL_FOCUS_SELECT)
	{ 
		if( pMe->PhotoFormSettingsSelectedItem==0)	//user has selected contacts
		{

		  	title=(AECHAR*)MALLOC(30);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_AUTOSTART,title , 30);

			PhotoForm_Dialog(pMe,NULL,title);
			//FREEIF(sstr);
			FREEIF(title);

			
			PhotoForm_SettingsAutoStart(pMe); 	
			
		}
		
	}
	if (pev->evCode == EVT_MDL_FOCUS_CHANGE)
		pMe->PhotoFormSettingsSelectedItem = (int)pev->dwParam;

}

// ================================================================================
// FUNCTION		: PhotoFormSettings_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM PhotoForm_Settings FORM
// ================================================================================

static boolean PhotoFormSettings_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	MainForm* pMe = (MainForm*) po;	
	AECHAR* sstr=NULL;
	AECHAR* title=NULL;


   switch (evt) 
	{
	case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT2:
				 
					IROOTFORM_PopForm(pMe->rootForm);
					Delete_PhotoFormSettigs(pMe);
					
					return (TRUE);
					break;
			
			case AVK_SOFT1:
				if(pMe->PhotoFormSettingsSelectedItem==0)
				{

					title=(AECHAR*)MALLOC(30);
					ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_AUTOSTART,title , 30);

					PhotoForm_Dialog(pMe,NULL,title);
					FREEIF(title);					
					PhotoForm_SettingsAutoStart(pMe); 

				}

					return (TRUE);
					break;

			case AVK_CLR:
					IROOTFORM_PopForm(pMe->rootForm);				  
					Delete_PhotoFormSettigs(pMe);
					return (TRUE);
					break;					
			}
		}

	}
    
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->PhotoFormSettingsHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: Delete_PhotoFormSettigs
// DESCRIPTION	: DELETE RESOURCES USED IN PhotoForm_Settings FORM
// ================================================================================

void Delete_PhotoFormSettigs(MainForm* pMe)
{
	FREEIF(pMe->str1);
	if(pMe->settingsContainer)
	{
		IPROPCONTAINER_Release(pMe->settingsContainer);
		pMe->settingsContainer=NULL;
	}
	if(pMe->settingsContainerWidget)
	{
		IWIDGET_Release(pMe->settingsContainerWidget);
		pMe->settingsContainerWidget=NULL;
	}

	if(pMe->PhotoForm_SettingsForm)
	{
		IFORM_Release(pMe->PhotoForm_SettingsForm);
		pMe->PhotoForm_SettingsForm=NULL;
	}
	
}

// ================================================================================
// FUNCTION		: PhotoForm_Settings
// DESCRIPTION	: POPULATE SETTINGS PAGE
// ================================================================================

int PhotoForm_Settings(MainForm* pMe) 
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

	FREEIF(pMe->str1);
	FREEIF(pMe->str2);
	FREEIF(pMe->str3);
	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGESTATICWIDGET, (void**)&imageWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MENUMODEL, (void**)&iMenuModal);
  	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&wItemList);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**)&pScrollBar);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);
	ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
	
	//load the strings
	pMe->str1 = (AECHAR*) MALLOC(20);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_AUTOSTART, pMe->str1, 20);
	pMe->imageInfo5.pwText = (AECHAR*)pMe->str1;

	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo5,1,MMF_ENABLED);
	//set the vidget to act as a menu modal vidget
	IWIDGET_SetModel(wItemList, IMENUMODEL_TO_IMODEL(iMenuModal));
	//set the picklist widget properties
	IWIDGET_SetBorderWidth(wItemList, 0);
	IWIDGET_SetItemHeight(wItemList, 30);
	IWIDGET_SetItemWidth(wItemList, pMe->DeviceInfo.cxScreen);
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
		IMODEL_AddListenerEx(pickListViewModel, &pMe->PhotoForm_menuListener, (PFNLISTENER)PhotoFormSettingsMenuModal_EventHandler, pMe);
		IMODEL_Release(pickListViewModel);
	}
	wp.bVisible = TRUE;
	wp.prop = 1;
	IPROPCONTAINER_Insert(pMe->settingsContainer, pScrollBar, WIDGET_ZNORMAL, &wp);

	//set WID_FORM to the container
	result = IPROPCONTAINER_QueryInterface(pMe->settingsContainer, AEEIID_WIDGET, (void**)&pMe->settingsContainerWidget);

	HANDLERDESC_Init(&pMe->PhotoFormSettingsHandler, PhotoFormSettings_HandleEvent, pMe, Delete_PhotoFormSettigs);
	IFORM_SetHandler((IForm*)pMe->mainForm, &pMe->PhotoFormSettingsHandler);	
	if(result ==0) 
	{
		IFORM_SetWidget((IForm*)pMe->PhotoForm_SettingsForm, WID_FORM, pMe->settingsContainerWidget);
		IWIDGET_MoveFocus(pMe->settingsContainerWidget, wItemList);
	}

	IWIDGET_Release((IWidget*)imageWidget);
	IMENUMODEL_Release(iMenuModal);
	IWIDGET_Release(wItemList);
	IWIDGET_Release(pScrollBar);
	RELEASEIF(piFont);
	if(result != 0)
		return EFAILED;
	return result;
}

// ================================================================================
// FUNCTION		: PhotoForm_SettingsForm
// DESCRIPTION	: INITIATE SETTINGS HOME
// ================================================================================

int PhotoForm_SettingsForm(IForm** ppo, IShell *piShell,IRootForm *pRootForm,MainForm* pMe) 
{
	int result=0;
	IImage* m_pIImage;
	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->PhotoForm_SettingsForm) == 0)
	{
		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->settingsContainer);
		*ppo = pMe->PhotoForm_SettingsForm;
		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->PhotoForm_SettingsForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->PhotoForm_SettingsForm, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_CONTACTSETTINGS);
		result =  IFORM_SetSoftkeys((IForm*)pMe->PhotoForm_SettingsForm, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);
		result = PhotoForm_Settings(pMe);	//add items to the form
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->PhotoForm_SettingsForm);
	RELEASEIF(m_pIImage);

	}
	return result;
}

// ================================================================================
// FUNCTION		: PhotoForm_AutoStartHandleEvent
// DESCRIPTION	: HANDLE EVENT FROM PhotoForm_SettingsAutoStart
// ================================================================================

static boolean PhotoForm_AutoStartHandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	MainForm* pMe = (MainForm*) po;
	AECHAR* a_uname=NULL;
   	char* state=NULL;
	user *pUser=NULL;
	AECHAR* sstr=NULL;
	AECHAR* title=NULL;
	int nLen=0;
	int i=0;
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT)
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;

		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);

		rc.x = pMe->DeviceInfo.cyScreen/8;
		rc.y = pMe->DeviceInfo.cyScreen/2;
		rc.dx = pMe->DeviceInfo.cyScreen*3;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen/2;
		*(AEERect*) dwParam = rc;

		if (GetUserData(pMe->pIShell, &pUser))
		{
		
			state=pUser->autostart;
			
		}

		if ( 0==STRCMP("ON", state))		
			IVALUEMODEL_SetBool(pMe->valueModelAutoStartOn,TRUE);
		if ( 0==STRCMP("OFF", state))
			IVALUEMODEL_SetBool(pMe->valueModelAutoStartOff,TRUE);

		FREEIF(pUser);
		return TRUE;
   }
     	 
   if(evt==EVT_KEY)
   {
	      
		if(wParam == AVK_DOWN)
		{		
			IWIDGET_MoveFocus(pMe->autoStartStateContainerWidget, pMe->rdisable);
		}
		if(wParam == AVK_UP)
		{
			IWIDGET_MoveFocus(pMe->autoStartStateContainerWidget, pMe->renable);
		}
			

		
		if(wParam == AVK_SOFT1)
		{
			
			if ( GetUserData(pMe->pIShell, &pUser))
			{			
				if(IVALUEMODEL_GetBool(pMe->valueModelAutoStartOn))
				{
			
					STRCPY(pUser->autostart,"ON");
					
				}if(IVALUEMODEL_GetBool(pMe->valueModelAutoStartOff))
				{
					
					STRCPY(pUser->autostart,"OFF");
				}
				SetUserData(pMe->pIShell, pUser);
			}
			IROOTFORM_PopForm(pMe->rootForm);
			Delete_SettingsAutoStart(pMe);
			IROOTFORM_PopForm(pMe->rootForm);
			deleteMainFormDialog(pMe);
			//deleteDialog(pMe);

			FREEIF(pUser);
			return TRUE;
				
		}

		if(wParam == AVK_CLR || wParam == AVK_SOFT2)
		{
			IROOTFORM_PopForm(pMe->rootForm);
			Delete_SettingsAutoStart(pMe);
			IROOTFORM_PopForm(pMe->rootForm);
			deleteMainFormDialog(pMe);

			return TRUE;		
		}
	
   }	
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->PhotoFormStateHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: Delete_SettingsAutoStart
// DESCRIPTION	: DELETE ALL RESOURCES USED IN PhotoForm_SettingsAutoStart FORM
// ================================================================================

void Delete_SettingsAutoStart(MainForm* pMe){

	if(pMe->autoStartState)
	{
		IFORM_Release(pMe->autoStartState);
		pMe->autoStartState=NULL;
	}
	if(pMe->radioListContainer)
	{
		IXYCONTAINER_Release(pMe->radioListContainer);
		pMe->radioListContainer=NULL;
	}	
	if(pMe->renable)
	{
		IWIDGET_Release(pMe->renable);
		pMe->renable=NULL;
	}
	if(pMe->rdisable)
	{
		IWIDGET_Release(pMe->rdisable);
		pMe->rdisable=NULL;
	}
	if(pMe->enable)
	{
		IWIDGET_Release(pMe->enable);
		pMe->enable=NULL;
	}
	if(pMe->disable)
	{
		IWIDGET_Release(pMe->disable);
		pMe->disable=NULL;
	}
	if(pMe->valueModelAutoStartOn)
	{
		IVALUEMODEL_Release(pMe->valueModelAutoStartOn);
		pMe->valueModelAutoStartOn=NULL;
	}
	if(pMe->valueModelAutoStartOff)
	{
		IVALUEMODEL_Release(pMe->valueModelAutoStartOff);
		pMe->valueModelAutoStartOff=NULL;
	}
	if(pMe->autoStartStateContainerWidget)
	{
		IWIDGET_Release(pMe->autoStartStateContainerWidget);
		pMe->autoStartStateContainerWidget=NULL;
	}
	FREEIF(pMe->str3);
}

// ================================================================================
// FUNCTION		: PhotoForm_SettingsAutoStart
// DESCRIPTION	: POPULATE SETTINGS ITEMS
// ================================================================================

int PhotoForm_SettingsAutoStart(MainForm* pMe) 
{
	int result=0;
	IWidget* backdropWidget=NULL;
	AECHAR* sstr=NULL;
	WidgetPos wpos;
	WExtent we;
	AEEDeviceInfo  deviceInfo;
	IWidget* rlist=NULL;
	IModel *iModel = NULL;
    int nWdtHeight=0;
	int nWdtWidth=0;
	AECHAR format[]={'%','s',' ','\0'};

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->autoStartState);
	result +=  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->radioListContainer);
    result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->renable);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->rdisable);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&rlist);
	//create Enable Text Widget
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->enable);
	//create Disable text widget
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->disable);

	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MODEL, (void**)&iModel);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModelAutoStartOn);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModelAutoStartOff);
	
	IFORM_SetResText((IForm*)pMe->autoStartState, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
	IFORM_SetSoftkeys((IForm*)pMe->autoStartState, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
  
	IWIDGET_SetProperty(pMe->renable, PROP_VIEWMODEL, (uint32)iModel);
	IWIDGET_SetProperty(pMe->rdisable, PROP_VIEWMODEL, (uint32)iModel);
	
	IWIDGET_SetModel(pMe->renable, IVALUEMODEL_TO_IMODEL(pMe->valueModelAutoStartOn));
	IWIDGET_SetModel(pMe->rdisable, IVALUEMODEL_TO_IMODEL(pMe->valueModelAutoStartOff));
	
	pMe->enableText = (AECHAR*) MALLOC(120);
	pMe->disableText = (AECHAR*) MALLOC(120);

	if ( (NULL==pMe->enableText) ) return -1;
	if ( (NULL==pMe->disableText) ) return -1;
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ENABLE, pMe->enableText, 30) ) return -1;
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_DISABLE, pMe->disableText, 30) ) return -1;
	
	IWIDGET_SetText(pMe->enable, pMe->enableText, TRUE);
	IWIDGET_SetText(pMe->disable, pMe->disableText, TRUE);
   
    ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
	nWdtHeight = (deviceInfo.cyScreen/12)*8;
	nWdtHeight = nWdtHeight/6;
	nWdtHeight = nWdtHeight;

	nWdtWidth = (deviceInfo.cyScreen/10)*8;

	IWIDGET_SetExtent(pMe->enable,&we);
	IWIDGET_SetFlags(pMe->enable, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->enable, MAKE_RGB(252, 252, 252));

	IWIDGET_SetExtent(pMe->disable,&we);
	IWIDGET_SetFlags(pMe->disable, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->disable, MAKE_RGB(252, 252, 252));

	pMe->str3 = (AECHAR*)MALLOC(120);
	sstr = (AECHAR*)MALLOC(110);
	result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB,sstr , 110);
	WSPRINTF(pMe->str3, 120, format,sstr);
    IFORM_SetText((IForm*)pMe->autoStartState,FID_TITLE, pMe->str3);
	
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->renable,&we);
	IWIDGET_SetBorderColor(pMe->renable, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->renable, 1);
    IWIDGET_SetFlags(pMe->renable, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);
	
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->enable,&we);
	IWIDGET_SetBorderColor(pMe->enable, MAKE_RGB(100, 100, 100));
	
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->disable,&we);
	IWIDGET_SetBorderColor(pMe->disable, MAKE_RGB(100, 100, 100));

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->rdisable,&we);
	IWIDGET_SetBorderColor(pMe->rdisable, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->rdisable, 1);
    IWIDGET_SetFlags(pMe->rdisable, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);

	IFORM_SetWidget((IForm*)pMe->autoStartState, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->autoStartState, IDF_ALIGN_FILL);

	//Adding the text field to the container
	wpos.x = 4;
    wpos.y = 0;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->radioListContainer, pMe->renable, WIDGET_ZNORMAL, &wpos);

	wpos.x = 30;
    wpos.y = 0;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->radioListContainer, pMe->enable, WIDGET_ZNORMAL, &wpos);

	wpos.x = 4;
    wpos.y = 20;
    wpos.bVisible = TRUE;	     
	IXYCONTAINER_Insert(pMe->radioListContainer, pMe->rdisable, WIDGET_ZNORMAL, &wpos);

	wpos.x = 30;
    wpos.y = 20;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->radioListContainer, pMe->disable, WIDGET_ZNORMAL, &wpos);

	// Attach the contaner to the dialog
	result = IXYCONTAINER_QueryInterface(pMe->radioListContainer, AEEIID_WIDGET, (void**)&pMe->autoStartStateContainerWidget);
	if(result ==0) 
	{	
		IFORM_SetWidget((IForm*)pMe->autoStartState, WID_FORM, pMe->autoStartStateContainerWidget);
	}

	HANDLERDESC_Init(&pMe->PhotoFormStateHandler, PhotoForm_AutoStartHandleEvent, pMe, Delete_SettingsAutoStart);
	IFORM_SetHandler((IForm*)pMe->autoStartState, &pMe->PhotoFormStateHandler);

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->autoStartState);
	IWIDGET_Release(rlist);
	IMODEL_Release(iModel);
	FREEIF(sstr);
		
	return TRUE;
}

// ================================================================================
// FUNCTION		: HelpForm_HandleEvent
// DESCRIPTION	: HANDLE EVENTS FROM HELP HOME PAGE
// ================================================================================

static boolean HelpForm_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   MainForm* pMe = (MainForm*) po;
    switch (evt) 
	{
	case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT2:
				IROOTFORM_PopForm(pMe->rootForm);
				deletePhotoForm_HelpForm(pMe);
				DBGPRINTF("TEST");
				return (TRUE);
				break;
			case AVK_SOFT1:
				PhotoForm_HelpMenu(pMe);
				return (TRUE);
				break;
			case AVK_CLR:
				IROOTFORM_PopForm(pMe->rootForm);
				deletePhotoForm_HelpForm(pMe);
				break;
			}
		return TRUE;
		}

	
	}
	
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->helpHomeHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: DELETE ALL RESOURCES USED IN PhotoForm_HelpForm AND PhotoForm_HelpFormMain
// ================================================================================

void deletePhotoForm_HelpForm(MainForm* pMe)
{
	if(pMe->ripplevault)
	{
		IWIDGET_Release(pMe->ripplevault);
		pMe->ripplevault=NULL;

	}
	if(pMe->purpleace)
	{
		IWIDGET_Release(pMe->purpleace);
		pMe->purpleace=NULL;

	}
	if(pMe->PhotoForm_SettingsContainer)
	{
		IXYCONTAINER_Release(pMe->PhotoForm_SettingsContainer);
		pMe->PhotoForm_SettingsContainer=NULL;
	}
	if(pMe->helpForm)
	{
		IFORM_Release(pMe->helpForm);
		pMe->helpForm=NULL;
	}
}

// ================================================================================
// FUNCTION		: PhotoForm_HelpForm
// DESCRIPTION	: POPULATE HELP HOME PAGE
// ================================================================================

int PhotoForm_HelpForm(MainForm* pMe) 
{
	int result;
	IWidget* containerWidget = NULL;
	IWidget* imageWidget = NULL;
	IImage* m_pIImage;
	WidgetPos wpos;
	WExtent we;	
    int nWdtHeight=0;
	int nWdtWidth=0;

	wpos.x = pMe->DeviceInfo.cxScreen/2 - 35;
	wpos.y = pMe->DeviceInfo.cyScreen/2 - 45;
	wpos.bVisible = TRUE;

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGEWIDGET, (void**)&imageWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->ripplevault);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->purpleace);

	pMe->rippleVault = (AECHAR*) MALLOC(30);
	pMe->purpleAce = (AECHAR*) MALLOC(50);
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, MAINFORMTITLE, pMe->rippleVault, 30) ) return -1;
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PURPLEACE, pMe->purpleAce, 50) ) return -1;

	IWIDGET_SetText(pMe->ripplevault, pMe->rippleVault, TRUE);
	IWIDGET_SetText(pMe->purpleace, pMe->purpleAce, TRUE);

	nWdtHeight = (pMe->DeviceInfo.cyScreen/12)*8;
	nWdtHeight = nWdtHeight/6;
	nWdtHeight = nWdtHeight;

	nWdtWidth = (pMe->DeviceInfo.cyScreen/10)*8;

	IWIDGET_SetExtent(pMe->ripplevault,&we);
	IWIDGET_SetFlags(pMe->ripplevault, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->ripplevault, MAKE_RGB(252, 252, 252));

	IWIDGET_SetExtent(pMe->purpleace,&we);
	IWIDGET_SetFlags(pMe->purpleace, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->purpleace, MAKE_RGB(252, 252, 252));

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->ripplevault,&we);
	IWIDGET_SetBorderColor(pMe->ripplevault, MAKE_RGB(100, 100, 100));

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->purpleace,&we);
	IWIDGET_SetBorderColor(pMe->purpleace, MAKE_RGB(100, 100, 100));

	m_pIImage  = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_MAIN);
	IWIDGET_SetImage(imageWidget,m_pIImage);

	wpos.x = pMe->DeviceInfo.cxScreen/4;
    wpos.y = pMe->DeviceInfo.cyScreen/7;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->PhotoForm_SettingsContainer, pMe->ripplevault, WIDGET_ZNORMAL, &wpos);

	wpos.x = pMe->DeviceInfo.cxScreen/4;
    wpos.y = pMe->DeviceInfo.cyScreen/4;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->PhotoForm_SettingsContainer, (IWidget*)imageWidget, WIDGET_ZNORMAL, &wpos);
	
	wpos.x = pMe->DeviceInfo.cxScreen/9;
    wpos.y = pMe->DeviceInfo.cyScreen*4/8;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->PhotoForm_SettingsContainer, pMe->purpleace, WIDGET_ZNORMAL, &wpos);

	result = IXYCONTAINER_QueryInterface(pMe->PhotoForm_SettingsContainer, AEEIID_WIDGET, (void**)&containerWidget);
	if(result ==0)
	IFORM_SetWidget((IForm*)pMe->helpForm, WID_FORM, containerWidget);

	IWIDGET_Release(containerWidget);
	IWIDGET_Release((IWidget*)imageWidget);
	RELEASEIF(m_pIImage);
	return result;
}

// ================================================================================
// FUNCTION		: PhotoForm_HelpFormMain
// DESCRIPTION	: INITIATE HELP HOME PAGE
// ================================================================================

int PhotoForm_HelpFormMain(IForm** ppo, IShell *piShell,IRootForm *pRootForm,AEEDeviceInfo  DInfo,MainForm* pMe) 
{
	int result=0;
	IImage* m_pIImage;
	IFont *piFont = 0;
	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;
	pMe->DeviceInfo = DInfo;
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->helpForm) == 0)
	{
		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->PhotoForm_SettingsContainer);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);

		*ppo = pMe->helpForm;
		result =  IFORM_SetSoftkeys((IForm*)pMe->helpForm, RIPPLEVAULT_RES_FILE, IDS_ABOUT,IDS_CANCEL);

		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->helpForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->helpForm, FID_TITLE, RIPPLEVAULT_RES_FILE, MAINFORMTITLE);

		RELEASEIF(m_pIImage);

		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_MAIN);
		
		{
				IWidget* piScrollWidget = NULL;
			IWidget* mainFormWidget = NULL;
			
			if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLINDICATORWIDGET, (void**) &piScrollWidget) != 0)
				return (FALSE);

			// change the scroll pad
			IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);

			IFORM_GetWidget((IForm*)pMe->helpForm, WID_FORM, &mainFormWidget);
			IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
			IFORM_SetWidget((IForm*)pMe->helpForm, WID_FORM, piScrollWidget);

			IWIDGET_Release(piScrollWidget);	
		}
		result = PhotoForm_HelpForm(pMe);	//add items to the form
		
		HANDLERDESC_Init(&pMe->helpHomeHandler, HelpForm_HandleEvent, pMe, 0);
		IFORM_SetHandler((IForm*)pMe->helpForm, &pMe->helpHomeHandler);
	}

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->helpForm);
	RELEASEIF(m_pIImage);
	RELEASEIF(piFont);
	return result;
}

// ================================================================================
// FUNCTION		: PhotoForm_HelpMenuOptionSelectionHandler
// DESCRIPTION	: HANDLE OPTION SELECTION FROM PhotoForm_HelpMenu
// ================================================================================

static void PhotoForm_HelpMenuOptionSelectionHandler(MainForm* pMe, int idOption) 
{  
   	AECHAR* sstr=NULL;
	AECHAR* title=NULL;
    
	IROOTFORM_PopForm(pMe->rootForm);
	if(pMe->helpMenu)
	{
		IPOPUPMENU_Release(pMe->helpMenu);
		pMe->helpMenu=NULL;
	}
	
	switch(idOption)
	{
	case 0:
		pMe->helpChartext= (AECHAR*) MALLOC(HELPTEXTSIZE);
		pMe->helpTextTitleChar= (AECHAR*) MALLOC(50);
		
		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLETEXT, pMe->helpChartext, HELPTEXTSIZE) ;

		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, MAINFORMTITLE, pMe->helpTextTitleChar, 50) ;
	
		PhotoForm_HelpFormPage(pMe,pMe->helpChartext,pMe->helpTextTitleChar);	
		break;
	case 1:
		pMe->helpChartext= (AECHAR*) MALLOC(HELPTEXTSIZE);
		pMe->helpTextTitleChar= (AECHAR*) MALLOC(50);
		
		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_CONTACTBACKUP, pMe->helpChartext, HELPTEXTSIZE) ;

		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_CONTACTBACKUPTITLE, pMe->helpTextTitleChar, 50) ;
	
		PhotoForm_HelpFormPage(pMe,pMe->helpChartext,pMe->helpTextTitleChar);
		break;
	
	case 2:
		pMe->helpChartext= (AECHAR*) MALLOC(1500);
		pMe->helpTextTitleChar= (AECHAR*) MALLOC(50);
		
		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PHOTOVIDEO, pMe->helpChartext, 1500) ;

		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PHOTOVIDEOTITLE, pMe->helpTextTitleChar, 50) ;
	
		PhotoForm_HelpFormPage(pMe,pMe->helpChartext,pMe->helpTextTitleChar);
		break;
	case 3:
		pMe->helpChartext= (AECHAR*) MALLOC(HELPTEXTSIZE);
		pMe->helpTextTitleChar= (AECHAR*) MALLOC(50);
		
		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE,IDS_SMSBACKUP, pMe->helpChartext, HELPTEXTSIZE) ;

		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE,IDS_SMSBACKUPTITLE, pMe->helpTextTitleChar, 50) ;
	
		PhotoForm_HelpFormPage(pMe,pMe->helpChartext,pMe->helpTextTitleChar);
		break;
	case 4:
		pMe->helpChartext= (AECHAR*) MALLOC(HELPTEXTSIZE);
		pMe->helpTextTitleChar= (AECHAR*) MALLOC(50);
		
		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE,IDS_RESTORINGCONTACTS , pMe->helpChartext, HELPTEXTSIZE) ;
		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RESTORINGCONTACTSTITLE, pMe->helpTextTitleChar, 50) ;
	
		PhotoForm_HelpFormPage(pMe,pMe->helpChartext,pMe->helpTextTitleChar);
	
		break;
	case 5:
		pMe->helpChartext= (AECHAR*) MALLOC(HELPTEXTSIZE);
		pMe->helpTextTitleChar= (AECHAR*) MALLOC(50);
		
		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE,IDS_ABOUTVIEWING, pMe->helpChartext, HELPTEXTSIZE) ;

		ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ABOUTVIEWINGTITLE, pMe->helpTextTitleChar, 50) ;
	
		PhotoForm_HelpFormPage(pMe,pMe->helpChartext,pMe->helpTextTitleChar);
		break;
	
	}
	

}


// ================================================================================
// FUNCTION		: PhotoForm_HelpMenuOptionsEventHandler
// DESCRIPTION	: HANDLE EVENT FROM PhotoForm_HelpMenu
// ================================================================================

static boolean PhotoForm_HelpMenuOptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   MainForm* pMe = (MainForm*) po;

   
   
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen*2/5;
		rc.dx = pMe->DeviceInfo.cyScreen*5/6;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*5/10;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		IROOTFORM_PopForm(pMe->rootForm);
		if(pMe->helpMenu)
		{
			IPOPUPMENU_Release(pMe->helpMenu);
			pMe->helpMenu=NULL;
		}
		return TRUE;
	}
		

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->helpHomeMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PhotoForm_HelpMenu
// DESCRIPTION	: POPULATE POPUP MENU INCLUDING HELP TOPIC
// ================================================================================

int PhotoForm_HelpMenu(MainForm* pMe) 
{
	int result = 0;
	IWidget* backDropWidget=NULL;
   
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
	   return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->helpMenu) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->helpMenu, RIPPLEVAULT_RES_FILE, MAINFORMTITLE, 0,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->helpMenu, RIPPLEVAULT_RES_FILE, IDS_CONTACTBACKUPTITLE, 1,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->helpMenu, RIPPLEVAULT_RES_FILE, IDS_PHOTOVIDEOTITLE, 2,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->helpMenu, RIPPLEVAULT_RES_FILE, IDS_SMSBACKUPTITLE, 3,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->helpMenu, RIPPLEVAULT_RES_FILE, IDS_RESTORINGCONTACTSTITLE, 4,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->helpMenu, RIPPLEVAULT_RES_FILE, IDS_ABOUTVIEWINGTITLE, 5,MMF_ENABLED) != 0)
	   return (FALSE);
   
	IFORM_SetSoftkeys((IForm*)pMe->helpMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);

	IFORM_SetSelectHandler((IForm*)pMe->helpMenu, (PFNSELECT)PhotoForm_HelpMenuOptionSelectionHandler, pMe);

	HANDLERDESC_Init(&pMe->helpHomeMenuHandler, PhotoForm_HelpMenuOptionsEventHandler, pMe, 0);
	
	IFORM_SetHandler((IForm*)pMe->helpMenu, &pMe->helpHomeMenuHandler);

	//wrap the pop-up form in a scrollbar decorator
	{
		IWidget* piScrollWidget = NULL;
		IWidget* mainFormWidget = NULL;
		IWidget* backWidget = NULL;
		if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**) &piScrollWidget) != 0)
			return (FALSE);

		// change the scroll pad
		IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);

		IFORM_GetWidget((IForm*)pMe->helpMenu, WID_FORM, &mainFormWidget);
		IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
		IFORM_SetWidget((IForm*)pMe->helpMenu, WID_FORM, piScrollWidget);

		IFORM_GetWidget((IForm*)pMe->helpMenu, WID_LISTITEM, &backWidget);
		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->helpMenu, WID_LISTITEM, backWidget);
		//IFORM_SetWidget((IForm*)pMe->helpMenu, WID_BACKDROP, backDropWidget);

		IWIDGET_Release(mainFormWidget);
		IWIDGET_Release(piScrollWidget);	
		IWIDGET_Release(backWidget);
	}
	IWIDGET_Release(backDropWidget);
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->helpMenu);
	return (TRUE);
}

// ================================================================================
// FUNCTION		: HelpFormPage_HandleEvent
// DESCRIPTION	: HANDLE EEVENT FROM PhotoForm_HelpFormPage
// ================================================================================
static boolean HelpFormPage_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	MainForm* pMe = (MainForm*) po;
	user *pUser=NULL;
	AECHAR *rangetext=NULL;
	char* charrangetext=NULL;
	unsigned long time=0;
   
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu
		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = pMe->DeviceInfo.cxScreen/20;
		rc.y = pMe->DeviceInfo.cyScreen/4;
		rc.dx = pMe->DeviceInfo.cyScreen*15/20;
		rc.dy = pMe->DeviceInfo.cyScreen*2/3;
		*(AEERect*) dwParam = rc;

		return TRUE;

	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT1))
	{
		IROOTFORM_PopForm(pMe->rootForm);
		deletePhotoForm_HelpFormPage(pMe);	
		return TRUE;
	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->HelpFormPageHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: deleteMainFormDialog
// DESCRIPTION	: DELETE ALL  RESOURCES USED IN DIALOG
// ================================================================================

void deleteMainFormDialog(MainForm* pMe)
{
	if(pMe->dialog)
	{
		IDIALOG_Release(pMe->dialog);
		pMe->dialog=NULL;
	}
	FREEIF(pMe->str2);
}

// ================================================================================
// FUNCTION		: PhotoForm_Dialog
// DESCRIPTION	: POPULATE DIALOG FOR USED WITHI THIS MODULE
// ================================================================================

int PhotoForm_Dialog(MainForm* pMe,AECHAR* text,AECHAR* title)
{
	int result=0;
	int nLen=0;
	int size=0;
	

	IWidget* backdropWidget=NULL;
	IWidget* backWidget = NULL;
	AECHAR format[]={'%','s',' ',' ','\0'};
	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_INFODIALOG, (void**) &pMe->dialog);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);

	IFORM_SetResText((IForm*)pMe->dialog, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
	IFORM_SetSoftkeys((IForm*)pMe->dialog, RIPPLEVAULT_RES_FILE, IDS_OK, 0);
	
	pMe->str2 = (AECHAR*)MALLOC(120);
	IFORM_SetText((IForm*)pMe->dialog,FID_TITLE, title);
	IFORM_GetWidget((IForm*)pMe->dialog, WID_TITLE, &backWidget);
	IWIDGET_SetBorderWidth(backWidget, 1);
	IWIDGET_SetBorderColor(backWidget,MAKE_RGB(100, 100, 100));
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IFORM_SetWidget((IForm*)pMe->dialog, WID_TITLE, backWidget);
	IWIDGET_Release(backWidget);

	IFORM_GetWidget((IForm*)pMe->dialog, WID_BACKGROUND, &backWidget);
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IWIDGET_SetBorderWidth(backWidget, 1);
	IWIDGET_SetBorderColor(backWidget,MAKE_RGB(100, 100, 100));
	IFORM_SetWidget((IForm*)pMe->dialog, WID_BACKGROUND, backWidget);
	
	IFORM_SetWidget((IForm*)pMe->dialog, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->dialog, IDF_ALIGN_FILL);
	
	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->dialog);
	
	IWIDGET_Release(backdropWidget);
	IWIDGET_Release(backWidget);
	return TRUE;
}

// ================================================================================
// FUNCTION		: deletePhotoForm_HelpFormPage
// DESCRIPTION	: DELETE ALL RESOURCES USED IN PhotoForm_HelpFormPage
// ================================================================================

void deletePhotoForm_HelpFormPage(MainForm* pMe)
{
	
	if(pMe->helpText)
	{
		IWIDGET_Release(pMe->helpText);
		pMe->helpText=NULL;
	}
	if(pMe->helptextTitle)
	{
		IWIDGET_Release(pMe->helptextTitle);
		pMe->helptextTitle=NULL;
	}
	if(pMe->viewPort)
	{
		IWIDGET_Release(pMe->viewPort);
		pMe->viewPort=NULL;
	}
	if(pMe->helpFormPageContainer)
	{
		IPROPCONTAINER_Release(pMe->helpFormPageContainer);
		pMe->helpFormPageContainer=NULL;
	}
	if(pMe->helpFormPage)
	{
		IFORM_Release(pMe->helpFormPage);
		pMe->helpFormPage=NULL;
	}

}

// ================================================================================
// FUNCTION		: PhotoForm_HelpFormPage
// DESCRIPTION	: POPULATE HELP FORM PAGE
// ================================================================================

int PhotoForm_HelpFormPage(MainForm* pMe,AECHAR* text, AECHAR* title) 
{
	int result=0;
	IWidget* containerWidget = NULL;
	WExtent we;
	AEEDeviceInfo  deviceInfo;
    int nWdtHeight=0;
	int nWdtWidth=0;
	WidgetProp wp;
	IWidget* pScrollBar = NULL;
	AECHAR format[]={'%','s',' ','\0'};

	result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->helpFormPageContainer); 
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->helpFormPage);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLINDICATORWIDGET, (void**)&pScrollBar);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VIEWPORTWIDGET, (void**)&pMe->viewPort);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->helpText);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->helptextTitle);

	IFORM_SetResText((IForm*)pMe->helpFormPage, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
	IFORM_SetSoftkeys((IForm*)pMe->helpFormPage, RIPPLEVAULT_RES_FILE, IDS_OK,0);
		
	IWIDGET_SetProperty(pScrollBar, PROP_SCROLLPAD, 0);
	IWIDGET_SetFlags(pScrollBar, SCROLL_VERT | SBWF_NOAUTOHIDE | SBWF_ARROWS);
	
	
	IWIDGET_SetText(pMe->helptextTitle, title, TRUE);
   
    ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
	nWdtHeight = (deviceInfo.cyScreen/12)*8;
	nWdtHeight = nWdtHeight/6;
	nWdtWidth = (deviceInfo.cyScreen/10)*8;
	
	we.height = pMe->DeviceInfo.cyScreen/20;
    we.width = deviceInfo.cxScreen*71/80;
	IWIDGET_SetExtent(pMe->helptextTitle,&we);
	IWIDGET_SetFlags(pMe->helptextTitle, SWF_WRAPTEXT); 
	IWIDGET_SetBorderWidth(pMe->helptextTitle, 1);
	IWIDGET_SetBorderColor(pMe->helptextTitle, MAKE_RGB(100, 100, 100));		
	//set extent for static widget
	we.height = pMe->DeviceInfo.cyScreen/5;
    we.width = deviceInfo.cxScreen*71/80;

	IWIDGET_SetExtent(pMe->helpText,&we);
	IWIDGET_SetFlags(pMe->helpText, SWF_WRAPTEXT);
	IWIDGET_SetText(pMe->helpText, text, TRUE);
	IWIDGET_SetBorderWidth(pMe->helpText, 1);
	IWIDGET_SetBorderColor(pMe->helpText, MAKE_RGB(100, 100, 100));
	
	//set scroll bar for static widget 

	IWIDGET_SetProperty(pMe->viewPort, PROP_INCREMENT, SCROLLRATE);
	IDECORATOR_SetWidget((IDecorator*)pMe->viewPort,pMe->helpText);
	IDECORATOR_SetWidget((IDecorator*)pScrollBar,pMe->viewPort);
	
	//Adding the text field to the container
	wp.bVisible = TRUE;
	wp.prop = 1;
	
	IPROPCONTAINER_Insert(pMe->helpFormPageContainer, pMe->helptextTitle, WIDGET_ZNORMAL, &wp);
	
	wp.bVisible = TRUE;
	wp.prop = 4;
	IPROPCONTAINER_Insert(pMe->helpFormPageContainer, pScrollBar, WIDGET_ZTOPMOST, &wp);
	// Attach the contaner to the dialog
	result = IPROPCONTAINER_QueryInterface(pMe->helpFormPageContainer, AEEIID_WIDGET, (void**)&containerWidget);
	if(result ==0) 
	{		
		IFORM_SetWidget((IForm*)pMe->helpFormPage, WID_FORM, containerWidget);
		IWIDGET_SetBorderWidth(containerWidget, 1);
		IWIDGET_SetBorderColor(containerWidget, MAKE_RGB(100, 100, 100));
		IWIDGET_SetProperty(containerWidget, PROP_BGCOLOR, MAKE_RGB( 225,225,225));
		IWIDGET_MoveFocus(containerWidget, pMe->helptextTitle);
		
	}

	HANDLERDESC_Init(&pMe->HelpFormPageHandler, HelpFormPage_HandleEvent, pMe, deletePhotoForm_HelpFormPage);
	IFORM_SetHandler((IForm*)pMe->helpFormPage, &pMe->HelpFormPageHandler);

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->helpFormPage);	
	if(containerWidget)
		IWIDGET_Release(containerWidget);
	if(pScrollBar)
		IWIDGET_Release(pScrollBar);	
	return TRUE;
}
