
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
#include "MessageForm.h"
#include "UserData.h"
#include "..\SyncEng\SyncEng.h"
#include "..\SyncEng\SyncEng.bid"

#define		RIPPLEVAULT_RES_FILE "RippleVault.bar"
#define		PS_TEXT_LENGTH_SMALL	40
#define		PS_TEXT_LENGTH_MEDIUM	30
#define		PS_TEXT_LENGTH			50
#define		SELECTCONTACT_FILE		"SelectedContact.txt"

// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THEI MODULE FROM HERE
// ================================================================================

void MessageForm_Delete(MessageForm* pMe) 
{
	int i=0;

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
	if(pMe->messageForm_menu)
	{
		IPOPUPMENU_Release(pMe->messageForm_menu);
		pMe->messageForm_menu=NULL;
	}
	if(pMe->smsSettingsForm)
	{
		IFORM_Release(pMe->smsSettingsForm);
		pMe->smsSettingsForm=NULL;
	}
	if(pMe->smsSettingsForm_menu)
	{
		IPOPUPMENU_Release(pMe->smsSettingsForm_menu);
		pMe->smsSettingsForm_menu=NULL;
	}

	if(pMe->valueModelEnableRdo)
	{
		IVALUEMODEL_Release(pMe->valueModelEnableRdo);
		pMe->valueModelEnableRdo=NULL;
	}

	if(pMe->valueModelDisableRdo)
	{
		IVALUEMODEL_Release(pMe->valueModelDisableRdo);
		pMe->valueModelDisableRdo=NULL;
	}

	if (pMe->pEnableRadioWgt)
	{
		IWIDGET_Release(pMe->pEnableRadioWgt);
		pMe->pEnableRadioWgt=NULL;
	}

	if (pMe->pDisableRadioWgt)
	{
		IWIDGET_Release(pMe->pDisableRadioWgt);
		pMe->pDisableRadioWgt=NULL;
	}

	if (pMe->enableStaticWgt)
	{
		IWIDGET_Release(pMe->enableStaticWgt);
		pMe->enableStaticWgt=NULL;
	}

	if (pMe->disableStaticWgt)
	{
		IWIDGET_Release(pMe->disableStaticWgt);
		pMe->disableStaticWgt=NULL;
	}

	if(pMe->selectPopup)
	{
		IFORM_Release(pMe->selectPopup);  
		pMe->selectPopup=NULL;
	}
	
	if (pMe->containerWidget)
	{
		IWIDGET_Release(pMe->containerWidget);
		pMe->containerWidget=NULL;
	}

	if (pMe->selectContainer)
	{
		IXYCONTAINER_Release(pMe->selectContainer);
		pMe->selectContainer=NULL;
	}

	if(pMe->uploadTimeSettingPopupFrom)
	{
		IFORM_Release(pMe->uploadTimeSettingPopupFrom);  
		pMe->uploadTimeSettingPopupFrom=NULL;
	}
	if (pMe->uploadTimeSettingContainer)
	{
		IXYCONTAINER_Release(pMe->uploadTimeSettingContainer);
		pMe->uploadTimeSettingContainer=NULL;
	}
	if (pMe->timeWgt)
	{
		IWIDGET_Release(pMe->timeWgt);
		pMe->timeWgt=NULL;
	}
	if (pMe->uploadTimeSettingContainerWidget)
	{
		IWIDGET_Release(pMe->uploadTimeSettingContainerWidget);
		pMe->uploadTimeSettingContainerWidget=NULL;
	}
	if(pMe->uploadTime)
	{
		IVALUEMODEL_Release(pMe->uploadTime);
		pMe->uploadTime=NULL;
	}

	if(pMe->pContactSelectForm)
	{
		IFORM_Release(pMe->pContactSelectForm);  
		pMe->pContactSelectForm=NULL;
	}
	
	if (pMe->pContactListContainer)
	{
		IPROPCONTAINER_Release(pMe->pContactListContainer);
		pMe->pContactListContainer=NULL;
	}

	if ( pMe->pTmpContAr )
	{
		for (i=0;i<pMe->m_noOfAlbumSharedContacts;i++)
		{
			FREEIF(	pMe->pTmpContAr[i]->imageInfo.pwText );
			RELEASEIF(	pMe->pTmpContAr[i]->imageInfo.piImage );
			
		}

		for (i=pMe->m_noOfAlbumSharedContacts-1;i>=0;i--)
		{
			FREEIF(pMe->pTmpContAr[i]);
		}

		FREEIF(pMe->pTmpContAr);

	}

	if(pMe->contactHandleMenuForSMSUpload_menu)
	{
		IPOPUPMENU_Release(pMe->contactHandleMenuForSMSUpload_menu);  
		pMe->contactHandleMenuForSMSUpload_menu=NULL;
	}
	
	FREEIF(pMe->pszOption1);
	FREEIF(pMe->pszOption2);
	FREEIF(pMe->pszHeading);
	FREEIF(pMe->str1);
	FREEIF(pMe->str2);
	FREEIF(pMe->str3);
	FREEIF(pMe->str4);

	if(pMe)
	  FREE(pMe);
}


// ================================================================================
// FUNCTION		: MenuModal_EventHandler
// DESCRIPTION	: HANDLE EVENT FROM MAIN CONTAINER SPECIALY LISTEN FOR EVENT FROM THE FORM
// ================================================================================
static void MenuModal_EventHandler(MessageForm *pMe, ModelEvent *pev) 
{

	if(pev->evCode == EVT_MDL_FOCUS_SELECT){ 
		if(pev->dwParam == 0)	
		{
			//user has selected contacts	
		}
		if(pev->dwParam == 1){
	
		
		}
		if(pev->dwParam == 2)	
		{
			//user has selected contacts	
		}
		if(pev->dwParam == 3)
		{
			//call Message Settings PopupForm
			MessageForm_SMSSettingsFormMain(pMe);
		}
	}
	if (pev->evCode == EVT_MDL_FOCUS_CHANGE)
		pMe->messagesSelectedItem = (int)pev->dwParam;

}


// ================================================================================
// FUNCTION		: MessageForm_HandleEvent
// DESCRIPTION	: HANDLE EEVENT FROM MAIN CONTAINER SPECIALY KEY EVENTS
// ================================================================================
static boolean MessageForm_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   MessageForm* pMe = (MessageForm*) po;
    switch (evt) 
	{
	case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT2:
				IROOTFORM_PopForm(pMe->rootForm);
				break;
			case AVK_SOFT1:
				MessageForm_Menu(pMe);
				break;
			}
		}
	}
	
	
	//the  default form handler is swapped with the AppForm handler
	// calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->mainFormHandler, evt, wParam, dwParam);
}



// ================================================================================
// FUNCTION		: MessageForm_PopulateMainContainer
// DESCRIPTION	: POPULATE MAIN FORM FROM HERE
// ================================================================================
int MessageForm_PopulateMainContainer(MessageForm* pMe) 
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

	result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->mainContainer);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGESTATICWIDGET, (void**)&imageWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MENUMODEL, (void**)&iMenuModal);
  	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&wItemList);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**)&pScrollBar);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);

	//load the strings
	pMe->str1 = (AECHAR*) MALLOC(30);
	pMe->str2 = (AECHAR*) MALLOC(40);
	pMe->str3 = (AECHAR*) MALLOC(30);
	pMe->str4 = (AECHAR*) MALLOC(40);

	result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_MVIEW, pMe->str1, 30);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_MSENTVAULT, pMe->str2, 40);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_MGETVAULT, pMe->str3, 30);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_MSETTING, pMe->str4, 40);

	//load the images
	pMe->m_pIImage1 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_MVIEW);
	pMe->m_pIImage2 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_MSENTVAULT);
	pMe->m_pIImage3 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_MGETVAULT);
	pMe->m_pIImage4 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_SETTING);

	pMe->imageInfo1.pwText = (AECHAR*)pMe->str1;
	pMe->imageInfo1.piImage = pMe->m_pIImage1;
	pMe->imageInfo2.pwText = (AECHAR*)pMe->str2;
	pMe->imageInfo2.piImage = pMe->m_pIImage2;
	pMe->imageInfo3.pwText = (AECHAR*)pMe->str3;
	pMe->imageInfo3.piImage = pMe->m_pIImage3;
	pMe->imageInfo4.pwText = (AECHAR*)pMe->str4;
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
		IFORM_SetWidget((IForm*)pMe->MessageForm, WID_FORM, containerWidget);
		IWIDGET_MoveFocus(containerWidget, wItemList);
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
// FUNCTION		: MessageForm_New
// DESCRIPTION	: FORM INITIALIZATION METHOD
// ================================================================================
int MessageForm_New(IForm** ppo, IShell *piShell,IRootForm *pRootForm) 
{
	int result=0;
	//allocate space for the form
	MessageForm *pMe = MALLOCREC(MessageForm);
	IImage* m_pIImage;

	if(!pMe)
		return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->MessageForm) == 0)
	{
		*ppo = pMe->MessageForm;
		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->MessageForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->MessageForm, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_MESSAGES);
		result =  IFORM_SetSoftkeys((IForm*)pMe->MessageForm, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);

		result = MessageForm_PopulateMainContainer(pMe);	//add items to the form

		HANDLERDESC_Init(&pMe->mainFormHandler, MessageForm_HandleEvent, pMe, MessageForm_Delete);
		IFORM_SetHandler((IForm*)pMe->MessageForm, &pMe->mainFormHandler);
	}
	RELEASEIF(m_pIImage);
	return result;
}


// ================================================================================
// FUNCTION		: MessageForm_MenuOptionSelectionHandler
// DESCRIPTION	: OPTION SELECTION FROM MENU HANDLED FROM HERE
// ================================================================================
static void MessageForm_MenuOptionSelectionHandler(MessageForm* pMe, int idOption) 
{
	IROOTFORM_PopForm(pMe->rootForm);
	if(pMe->messageForm_menu)
	{
		IPOPUPMENU_Release(pMe->messageForm_menu);
		pMe->messageForm_menu=NULL;
	}

	switch(idOption)
	{
		case 0:
			MessageForm_SMSSettingsFormMain(pMe);
			break;
	}


}


 
// ================================================================================
// FUNCTION		: MessageForm_MenuOptionsEventHandler
// DESCRIPTION	: SELECTION AND OTHER KEYE EVENTS FROM MessageForm_Menu HANDLE FROM HERE
// ================================================================================
static boolean MessageForm_MenuOptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   MessageForm* pMe = (MessageForm*) po;
   
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{    
		AEERect rc; 
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen*8/10;
		rc.dx = pMe->DeviceInfo.cxScreen;
		rc.dy = pMe->DeviceInfo.cyScreen/10;
		*(AEERect*) dwParam = rc;

      return TRUE;
	}

	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
		IROOTFORM_PopForm(pMe->rootForm);
		if(pMe->messageForm_menu)
		{
			IPOPUPMENU_Release(pMe->messageForm_menu);
			pMe->messageForm_menu=NULL;
		}
		if(pMe->messagesSelectedItem==3)
		{
			MessageForm_SMSSettingsFormMain(pMe);	
		}
		return TRUE;
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		IROOTFORM_PopForm(pMe->rootForm);
		if(pMe->messageForm_menu)
		{
			IPOPUPMENU_Release(pMe->messageForm_menu);
			pMe->messageForm_menu=NULL;
		}	
		return TRUE;
	}
	
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->messageSettingsMenuHandler, evt, wParam, dwParam);
}


// ================================================================================
// FUNCTION		: MessageForm_Menu
// DESCRIPTION	: MAIN POPUP MENU OF MESSAGE FORM GENERATE FROM HERE
// ================================================================================
int MessageForm_Menu(MessageForm* pMe) 
{
	int result = 0;
	IWidget* backDropWidget=NULL;
   
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
	   return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->messageForm_menu) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->messageForm_menu, RIPPLEVAULT_RES_FILE, IDS_CHANGE, 0,MMF_ENABLED) != 0)
	   return (FALSE);
   
	IFORM_SetSoftkeys((IForm*)pMe->messageForm_menu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);
	
	IFORM_SetSelectHandler((IForm*)pMe->messageForm_menu, (PFNSELECT)MessageForm_MenuOptionSelectionHandler, pMe);
	HANDLERDESC_Init(&pMe->messageSettingsMenuHandler, MessageForm_MenuOptionsEventHandler, pMe, 0);
	IFORM_SetHandler((IForm*)pMe->messageForm_menu, &pMe->messageSettingsMenuHandler);

	//wrap the pop-up form in a scrollbar decorator
	{
		IWidget* piScrollWidget = NULL;
		IWidget* mainFormWidget = NULL;
		IWidget* backWidget = NULL;
		if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**) &piScrollWidget) != 0)
			return (FALSE);

		// change the scroll pad
		IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);

		IFORM_GetWidget((IForm*)pMe->messageForm_menu, WID_FORM, &mainFormWidget);
		IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
		IFORM_SetWidget((IForm*)pMe->messageForm_menu, WID_FORM, piScrollWidget);

		IFORM_GetWidget((IForm*)pMe->messageForm_menu, WID_LISTITEM, &backWidget);
		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->messageForm_menu, WID_LISTITEM, backWidget);
		IFORM_SetWidget((IForm*)pMe->messageForm_menu, WID_BACKDROP, backDropWidget);

		IWIDGET_Release(mainFormWidget);
		IWIDGET_Release(piScrollWidget);
		IWIDGET_Release(backWidget);
	}
	IWIDGET_Release(backDropWidget);
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->messageForm_menu);
	return (TRUE);
}


// ==================================================================================================
// FUNCTION		: SMSSettings_EventHandler
// DESCRIPTION	: LISTEN EVENTS FROM MessageForm_SMSSettingsForm AND IDENTIFY CURRENLY SELECTED INDEX
// ==================================================================================================
static void SMSSettings_EventHandler(MessageForm *pMe, ModelEvent *pev) 
{
	if(pev->evCode == EVT_MDL_FOCUS_SELECT)
	{ 
		if(pev->dwParam == 0)	//user has selected contacts
		{
			MessageForm_Select(pMe);	
			
		}
		if(pev->dwParam == 1)
		{
			MessageFrom_UploadTimeSettingForm(pMe);
		
		}
		if(pev->dwParam == 2)	//user has selected contacts
		{
			MessageForm_Select(pMe);	
			
		}
		if(pev->dwParam == 3)
		{
			//call Message Settings PopupForm
		
		}
	}
	if (pev->evCode == EVT_MDL_FOCUS_CHANGE)
		pMe->nSelectedCategory = (int)pev->dwParam;

}

 
// ================================================================================
// FUNCTION		: MessageFormSMSSettings_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM MessageForm_SMSSettingsForm
// ================================================================================
static boolean MessageFormSMSSettings_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	MessageForm* pMe = (MessageForm*) po;

	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
		MessageForm_SMSSettingsMenu(pMe);			
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		IROOTFORM_PopForm(pMe->rootForm);
		smsSettingsForm_Delete(pMe);	
		if(pMe->messageForm_menu)
		{
			IPOPUPMENU_Release(pMe->messageForm_menu);
			pMe->messageForm_menu=NULL;
		}

	return TRUE;
	}
   return HANDLERDESC_Call(&pMe->smsSettingsFormHandler, evt, wParam, dwParam);
}


// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: DELETE RESOURCES USED IN MessageForm_SMSSettingsForm
// ================================================================================
void smsSettingsForm_Delete(MessageForm* pMe) 
{
   if(pMe->smsSettingsFormContainer)
   {
      IPROPCONTAINER_Release(pMe->smsSettingsFormContainer);
		pMe->smsSettingsFormContainer = NULL;
   }
   if(pMe->smsSettingsForm)
   {
		IFORM_Release(pMe->smsSettingsForm);
		pMe->smsSettingsForm=NULL;
   }
   FREEIF(pMe->str5);
   FREEIF(pMe->str6);
   FREEIF(pMe->str7);  
}


// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: POPULATE SMS SETTINGS FROM HERE
// ================================================================================
int MessageForm_SMSSettingsForm(MessageForm* pMe) 
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

	result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->smsSettingsFormContainer);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGESTATICWIDGET, (void**)&imageWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MENUMODEL, (void**)&iMenuModal);
  	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&wItemList);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**)&pScrollBar);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);

	//load the strings
	pMe->str5 = (AECHAR*) MALLOC(30);
	pMe->str6 = (AECHAR*) MALLOC(40);
	pMe->str7 = (AECHAR*) MALLOC(30);

	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SMSUPLOAD, pMe->str5, 30);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_UPLOADTIME, pMe->str6, 40);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROAMINGUPLOAD, pMe->str7, 30);
	
	//load the images		
	pMe->imageInfo5.pwText = (AECHAR*)pMe->str5;
	pMe->imageInfo6.pwText = (AECHAR*)pMe->str6;
	pMe->imageInfo7.pwText = (AECHAR*)pMe->str7;

	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo5,1,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo6,2,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo7,3,MMF_ENABLED);
	
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
		IMODEL_AddListenerEx(pickListViewModel, &pMe->smsSettingsMenuListener, (PFNLISTENER)SMSSettings_EventHandler, pMe);
		IMODEL_Release(pickListViewModel);
	}
	wp.bVisible = TRUE;
	wp.prop = 1;
	IPROPCONTAINER_Insert(pMe->smsSettingsFormContainer, pScrollBar, WIDGET_ZNORMAL, &wp);

	//set WID_FORM to the container
	result = IPROPCONTAINER_QueryInterface(pMe->smsSettingsFormContainer, AEEIID_WIDGET, (void**)&containerWidget);
	if(result ==0) 
	{
		IFORM_SetWidget((IForm*)pMe->smsSettingsForm, WID_FORM, containerWidget);
		IWIDGET_MoveFocus(containerWidget, wItemList);
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
// FUNCTION		: MessageForm_SMSSettingsFormMain
// DESCRIPTION	: INITIALIZATION METHOD FOR SMS SETTINGS DISPLAY FORM
// ================================================================================
int MessageForm_SMSSettingsFormMain(MessageForm* pMe) 
{
	int result=0;
	IImage* m_pIImage;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->smsSettingsForm) == 0)
	{
		m_pIImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_SMSSETTINGS);
		IFORM_SetTitleImage((IForm*)pMe->smsSettingsForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->smsSettingsForm, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_SMSSETTINGS);
		result =  IFORM_SetSoftkeys((IForm*)pMe->smsSettingsForm, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
		result = MessageForm_SMSSettingsForm(pMe);	

		HANDLERDESC_Init(&pMe->smsSettingsFormHandler, MessageFormSMSSettings_HandleEvent, pMe,smsSettingsForm_Delete);
		IFORM_SetHandler((IForm*)pMe->smsSettingsForm, &pMe->smsSettingsFormHandler);

		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->smsSettingsForm);
	}
	RELEASEIF(m_pIImage);
	return result;
}

 
// ================================================================================
// FUNCTION		: MEssageForm_SMSSettingsMenuOptionSelectionHandler
// DESCRIPTION	: HANDLE OPTION SELECTION FROM MessageForm_SMSSettingsMenu
// ================================================================================
static void MEssageForm_SMSSettingsMenuOptionSelectionHandler(MessageForm* pMe, int idOption) 
{  
	IROOTFORM_PopForm(pMe->rootForm);
	if(pMe->smsSettingsForm_menu)
	{
			IPOPUPMENU_Release(pMe->smsSettingsForm_menu);
			pMe->smsSettingsForm_menu=NULL;
	}
	
	switch(pMe->nSelectedCategory)
	{
	case 0:
		MessageForm_Select(pMe);
		break;
	case 1:
		MessageFrom_UploadTimeSettingForm(pMe);
		break;
	case 2:
		MessageForm_Select(pMe);
		break;
	}
}

// ================================================================================
// FUNCTION		: HANDLE EVENTS FROM MessageForm_SMSSettingsMenu
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THEI MODULE FROM HERE
// ================================================================================
static boolean MessageForm_SMSSettingsMenuOptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   MessageForm* pMe = (MessageForm*) po;
 
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		AEERect rc;  
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen*8/10;
		rc.dx = pMe->DeviceInfo.cxScreen;
		rc.dy = pMe->DeviceInfo.cyScreen/10;
		*(AEERect*) dwParam = rc;

      return TRUE;
   }

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->smsSettingsMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: MessageForm_SMSSettingsMenu
// DESCRIPTION	: PUPULATE MESSAGE SETTINGS FORM OPTION MENU
// ================================================================================
int MessageForm_SMSSettingsMenu(MessageForm* pMe) 
{
	int result = 0;
	IWidget* backDropWidget=NULL;
   
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
	   return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->smsSettingsForm_menu) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->smsSettingsForm_menu, RIPPLEVAULT_RES_FILE, IDS_CHANGE, 0,MMF_ENABLED) != 0)
	   return (FALSE);
   
	IFORM_SetSoftkeys((IForm*)pMe->smsSettingsForm_menu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);
	
	IFORM_SetSelectHandler((IForm*)pMe->smsSettingsForm_menu, (PFNSELECT)MEssageForm_SMSSettingsMenuOptionSelectionHandler, pMe);
	HANDLERDESC_Init(&pMe->smsSettingsMenuHandler, MessageForm_SMSSettingsMenuOptionsEventHandler, pMe, 0);
	IFORM_SetHandler((IForm*)pMe->smsSettingsForm_menu, &pMe->smsSettingsMenuHandler);

	//wrap the pop-up form in a scrollbar decorator
	{
		IWidget* piScrollWidget = NULL;
		IWidget* mainFormWidget = NULL;
		IWidget* backWidget = NULL;
		if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**) &piScrollWidget) != 0)
			return (FALSE);

		// change the scroll pad
		IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);

		IFORM_GetWidget((IForm*)pMe->smsSettingsForm_menu, WID_FORM, &mainFormWidget);
		IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
		IFORM_SetWidget((IForm*)pMe->smsSettingsForm_menu, WID_FORM, piScrollWidget);

		IFORM_GetWidget((IForm*)pMe->smsSettingsForm_menu, WID_LISTITEM, &backWidget);
		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->smsSettingsForm_menu, WID_LISTITEM, backWidget);
		IFORM_SetWidget((IForm*)pMe->smsSettingsForm_menu, WID_BACKDROP, backDropWidget);

		IWIDGET_Release(mainFormWidget);
		IWIDGET_Release(piScrollWidget);
		
		IWIDGET_Release(backWidget);
	}
	IWIDGET_Release(backDropWidget);
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->smsSettingsForm_menu);
	return (TRUE);
}

 
// ================================================================================
// FUNCTION		: MessageForm_SelectEvent
// DESCRIPTION	: HANDLE EVENT FROM MessageForm_Select FORM
// ================================================================================
static boolean MessageForm_SelectEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	MessageForm* pMe = (MessageForm*) po;
	boolean bStatus=TRUE;
	user* pUser;
	char* pszContactInfo=NULL;
	ISyncEngExt* pSyncExt;
	AECHAR *pszContactsAr=NULL;
	int nLen=0;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		
		AEEDeviceInfo  DeviceInfo;
		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &DeviceInfo);
		rc.x = 0;
		rc.y = DeviceInfo.cyScreen*2/5;
		rc.dx = DeviceInfo.cyScreen*3;
		rc.dy = DeviceInfo.cyScreen-DeviceInfo.cyScreen/2;

		return TRUE;
	}

	if(evt == EVT_KEY)
	{	
		switch (wParam)
		{
		case AVK_CLR:
			IROOTFORM_PopForm(pMe->rootForm);
			break;
		case AVK_SOFT2:
			IROOTFORM_PopForm(pMe->rootForm);
			break;
		case AVK_SOFT1:
			IROOTFORM_PopForm(pMe->rootForm);
			//save the settings
			bStatus = IVALUEMODEL_GetBool(pMe->valueModelEnableRdo);
			if ( GetUserData(pMe->pIShell, &pUser))
			{			
				if(pMe->nSelectedCategory==0)
				{
					if(bStatus)
					{
				
						STRCPY(pUser->smsuploadType,"YES");

						//copy all contact to SelectedContact.txt file
						if ( SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pSyncExt))
							goto error;
					
						pMe->m_noOfAlbumSharedContacts = ISYNCENG_GetContacts(pSyncExt, &pszContactsAr);				
						pszContactInfo=(char*)MALLOC(pMe->m_noOfAlbumSharedContacts*PS_TEXT_LENGTH_SMALL);
						WSTRTOSTR(pszContactsAr, pszContactInfo, pMe->m_noOfAlbumSharedContacts*PS_TEXT_LENGTH_SMALL);
						
						nLen = STRLEN(pszContactInfo);

						if ( WriteFileStream(pMe->pIShell, SELECTCONTACT_FILE, pszContactInfo, nLen, FALSE) >0)
						{
							FREEIF(pszContactInfo);
						}
						FREEIF(pszContactInfo);

						error:
						
						FREEIF(pszContactsAr);
						if(pSyncExt)
						{
							ISYNCENG_Release(pSyncExt);
							pSyncExt=NULL;
						}
						
					}else
					{
							
						STRCPY(pUser->smsuploadType,"NO");
						
						if (ReadFileStream(pMe->pIShell, SELECTCONTACT_FILE, &pszContactInfo)>0 )
						{
							char* start=NULL;
							char* end=NULL;
							int nLen=0;
							start=pszContactInfo;

							pMe->m_noOfAlbumSharedContacts=0;
							while( NULL!=(end=STRSTR(start,";")))
							{
								nLen = STRLEN(start)-STRLEN(end);
								start = start+nLen+1;
								pMe->m_noOfAlbumSharedContacts++;
							}
							MessageForm_SelectContactToShare(pMe, pszContactInfo);
						}else
						{
							pMe->m_noOfAlbumSharedContacts=0;
							MessageForm_SelectContactToShare(pMe, "");
						}
						FREEIF(pszContactInfo);
					}
				}
				if(pMe->nSelectedCategory==2)
				{
					if(bStatus)
					{
				
						STRCPY(pUser->roamingUpload,"YES");
						
					}else
					{
						
						STRCPY(pUser->roamingUpload,"NO");
					}	
				}
				
				SetUserData(pMe->pIShell, pUser);

			}
			FREEIF(pUser);			
			break;			
		case AVK_DOWN:
			IWIDGET_MoveFocus(pMe->containerWidget, pMe->pDisableRadioWgt);
			break;
		case AVK_UP:
			IWIDGET_MoveFocus(pMe->containerWidget, pMe->pEnableRadioWgt);
			break;
		}
		return TRUE;
	}	 
	

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->selectPopupHandler, evt, wParam, dwParam);
}


// ================================================================================
// FUNCTION		: MessageForm_Select
// DESCRIPTION	: POPULATE CONTACT FOR SELECT TO SMS UPLOAD
// ================================================================================
int MessageForm_Select(MessageForm* pMe) 
{
	int result=0;
	IWidget* backdropWidget=NULL;
	AECHAR* sstr=NULL;
	WidgetPos wpos;
	user *pUser=NULL;
	WExtent we;
	AEEDeviceInfo  deviceInfo;
	IWidget* list=NULL;
	char* type;
	char* roamimgUploading;
	IModel *iModal = NULL;
    int nWdtHeight=0;
	int nWdtWidth=0;
	
	if (NULL==pMe->selectPopup)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->selectPopup);	
		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->selectContainer);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->pEnableRadioWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->pDisableRadioWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&list);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->titleStaticWgt);
		//create Enable Text Widget
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->enableStaticWgt);
		//create Disable text widget
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->disableStaticWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MODEL, (void**)&iModal);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModelEnableRdo);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModelDisableRdo);
	
		IFORM_SetResText((IForm*)pMe->selectPopup, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
		IFORM_SetSoftkeys((IForm*)pMe->selectPopup, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
  
		IWIDGET_SetProperty(pMe->pEnableRadioWgt, PROP_VIEWMODEL, (uint32)iModal);
		IWIDGET_SetProperty(pMe->pDisableRadioWgt, PROP_VIEWMODEL, (uint32)iModal);

		IWIDGET_SetModel(pMe->pEnableRadioWgt, IVALUEMODEL_TO_IMODEL(pMe->valueModelEnableRdo));
		IWIDGET_SetModel(pMe->pDisableRadioWgt, IVALUEMODEL_TO_IMODEL(pMe->valueModelDisableRdo));
		
		pMe->pszOption1 = (AECHAR*) MALLOC(PS_TEXT_LENGTH_SMALL);
		pMe->pszOption2 = (AECHAR*) MALLOC(PS_TEXT_LENGTH_SMALL);
		pMe->pszHeading = (AECHAR*)MALLOC(PS_TEXT_LENGTH);

		if (NULL==pMe->pszOption1 || NULL==pMe->pszOption2 || NULL==pMe->pszHeading) goto error;

		
		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ENABLE, pMe->pszOption1, PS_TEXT_LENGTH_SMALL) ) goto error;
		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_DISABLE, pMe->pszOption2, PS_TEXT_LENGTH_SMALL) ) goto error;
		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB,pMe->pszHeading,PS_TEXT_LENGTH) ) goto error;
		
		//if we set last parameter true then no need to free the text associated with widget
		IWIDGET_SetText(pMe->enableStaticWgt, pMe->pszOption1, FALSE);
		IWIDGET_SetText(pMe->disableStaticWgt, pMe->pszOption2, FALSE);
		IWIDGET_SetText(pMe->titleStaticWgt, pMe->pszHeading, FALSE);

		ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
		nWdtHeight = (deviceInfo.cyScreen/12)*8;
		nWdtHeight = nWdtHeight/6;
		nWdtHeight = nWdtHeight;

		nWdtWidth = deviceInfo.cxScreen;

		IWIDGET_SetExtent(pMe->pEnableRadioWgt,&we);
		IWIDGET_SetFlags(pMe->pEnableRadioWgt, IDF_ALIGN_LEFT);
		IWIDGET_SetBorderColor(pMe->pEnableRadioWgt, MAKE_RGB(252, 252, 252));

		IWIDGET_SetExtent(pMe->pDisableRadioWgt,&we);
		IWIDGET_SetFlags(pMe->pDisableRadioWgt, IDF_ALIGN_LEFT);
		IWIDGET_SetBorderColor(pMe->pDisableRadioWgt, MAKE_RGB(252, 252, 252));
		
		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(pMe->titleStaticWgt,&we);
		IWIDGET_SetFlags(pMe->titleStaticWgt, IDF_ALIGN_MIDDLE);
		IWIDGET_SetBorderWidth(pMe->titleStaticWgt, 1);
		IWIDGET_SetBorderColor(pMe->titleStaticWgt, MAKE_RGB(0, 0, 0));
		
		IWIDGET_SetExtent(pMe->pEnableRadioWgt,&we);
		IWIDGET_SetBorderColor(pMe->pEnableRadioWgt, MAKE_RGB(100, 100, 100));
		IWIDGET_SetBorderWidth(pMe->pEnableRadioWgt, 1);
		IWIDGET_SetFlags(pMe->pEnableRadioWgt, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);
		
		
		IWIDGET_SetExtent(pMe->enableStaticWgt,&we);
		IWIDGET_SetBorderColor(pMe->enableStaticWgt, MAKE_RGB(100, 100, 100));
		
		IWIDGET_SetExtent(pMe->disableStaticWgt,&we);
		IWIDGET_SetBorderColor(pMe->disableStaticWgt, MAKE_RGB(100, 100, 100));
    		
		IWIDGET_SetExtent(pMe->pDisableRadioWgt,&we);
		IWIDGET_SetBorderColor(pMe->pDisableRadioWgt, MAKE_RGB(100, 100, 100));
		IWIDGET_SetBorderWidth(pMe->pDisableRadioWgt, 1);
		IWIDGET_SetFlags(pMe->pDisableRadioWgt, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);

		IFORM_SetWidget((IForm*)pMe->selectPopup, WID_BACKDROP, backdropWidget);
		IWIDGET_SetFlags((IWidget*)pMe->selectPopup, IDF_ALIGN_FILL);

		//add the title to form
		wpos.x = 0;
		wpos.y = 0;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->selectContainer, pMe->titleStaticWgt, WIDGET_ZNORMAL, &wpos);

		//Adding the text field to the container
		wpos.x = 4;
		wpos.y = 30;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->selectContainer, pMe->pEnableRadioWgt, WIDGET_ZNORMAL, &wpos);

		wpos.x = 30;
		wpos.y = 30;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->selectContainer, pMe->enableStaticWgt, WIDGET_ZNORMAL, &wpos);

		wpos.x = 4;
		wpos.y = 50;
		wpos.bVisible = TRUE;	     
		IXYCONTAINER_Insert(pMe->selectContainer, pMe->pDisableRadioWgt, WIDGET_ZNORMAL, &wpos);

		wpos.x = 30;
		wpos.y = 50;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->selectContainer, pMe->disableStaticWgt, WIDGET_ZNORMAL, &wpos);

		// Attach the contaner to the dialog
		result = IXYCONTAINER_QueryInterface(pMe->selectContainer, AEEIID_WIDGET, (void**)&pMe->containerWidget);
		if(result ==0) 
		{
			IFORM_SetWidget((IForm*)pMe->selectPopup, WID_FORM, pMe->containerWidget);
			IWIDGET_SetProperty(pMe->containerWidget, PROP_BGCOLOR, MAKE_RGB(225,225,225));
			IWIDGET_SetBorderWidth(pMe->containerWidget, 1);
		}

		HANDLERDESC_Init(&pMe->selectPopupHandler, MessageForm_SelectEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->selectPopup, &pMe->selectPopupHandler);
				
	error:			
		IWIDGET_Release(pMe->titleStaticWgt);
		IWIDGET_Release(list);
		IMODEL_Release(iModal);
		
	}

	if (0==result)
	{
		MEMSET(pMe->pszOption1, 0, PS_TEXT_LENGTH_SMALL);
		MEMSET(pMe->pszOption2, 0, PS_TEXT_LENGTH_SMALL);
		MEMSET(pMe->pszHeading, 0, PS_TEXT_LENGTH_SMALL);
		
		if (GetUserData(pMe->pIShell, &pUser))
		{
			
			type=pUser->smsuploadType;
			roamimgUploading=pUser->roamingUpload;
				
			//set the titles and selection texts
			switch (pMe->nSelectedCategory)
			{
			case 0:
				if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ALLSMS, pMe->pszOption1, PS_TEXT_LENGTH_SMALL) ) return FALSE;
				if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SELECTIVESMS, pMe->pszOption2, PS_TEXT_LENGTH_SMALL) ) return FALSE;
				if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SMSUPLOAD,pMe->pszHeading,PS_TEXT_LENGTH_SMALL) ) return FALSE;

				if (0==STRCMP("YES", type))
				{
					IVALUEMODEL_SetBool(pMe->valueModelEnableRdo,TRUE);
					IVALUEMODEL_SetBool(pMe->valueModelDisableRdo,FALSE);				
				}
				else
				{
					IVALUEMODEL_SetBool(pMe->valueModelDisableRdo,TRUE);
					IVALUEMODEL_SetBool(pMe->valueModelEnableRdo,FALSE);
				}

				break;

			case 2:
				if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ENABLE, pMe->pszOption1, PS_TEXT_LENGTH_SMALL) ) return FALSE;
				if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_DISABLE, pMe->pszOption2, PS_TEXT_LENGTH_SMALL) ) return FALSE;
				if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROAMINGUPLOAD,pMe->pszHeading,PS_TEXT_LENGTH_SMALL) ) return FALSE;

				if (0==STRCMP("YES", roamimgUploading))
				{
					IVALUEMODEL_SetBool(pMe->valueModelEnableRdo,TRUE);
					IVALUEMODEL_SetBool(pMe->valueModelDisableRdo,FALSE);				
				}
				else
				{
					IVALUEMODEL_SetBool(pMe->valueModelDisableRdo,TRUE);
					IVALUEMODEL_SetBool(pMe->valueModelEnableRdo,FALSE);
				}
				break;	
			}

			IWIDGET_SetText(pMe->enableStaticWgt, pMe->pszOption1, FALSE);
			IWIDGET_SetText(pMe->disableStaticWgt, pMe->pszOption2, FALSE);
			IWIDGET_SetText(pMe->titleStaticWgt, pMe->pszHeading, FALSE);
			FREEIF(pUser);
		}
			
		IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->selectPopup);

	}
	
	return TRUE;
}

// ================================================================================
// FUNCTION		: MessageForm_UploadTimeSettingFormEvent
// DESCRIPTION	: HADLED EVENT FROM MessageFrom_UploadTimeSettingForm
// ================================================================================
static boolean MessageForm_UploadTimeSettingFormEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	MessageForm* pMe = (MessageForm*) po;
	user* pUser;
	uint32 uploadTimeGet;
	uint32 uploadTimeSet;
	unsigned long dataLength;
	

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT)
	{
		AEEDeviceInfo  DeviceInfo;
		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &DeviceInfo);
		rc.x = 0;
		rc.y = DeviceInfo.cyScreen*2/5;
		rc.dx = DeviceInfo.cyScreen*3;
		rc.dy = DeviceInfo.cyScreen-DeviceInfo.cyScreen/2;
		*(AEERect*) dwParam = rc;
	
		//set current time to the time field
		if ( GetUserData(pMe->pIShell, &pUser))
		{				
			uploadTimeGet=pUser->uploadTimeSeconds;
			IVALUEMODEL_SetValue(pMe->uploadTime, (void*)uploadTimeGet, 500, NULL);
		}
		FREEIF(pUser);
		IWIDGET_SetProperty(pMe->timeWgt, PROP_FLAGS, (uint32)DTWF_USEUPDOWNKEYS);
		return TRUE;
	}


	if(evt == EVT_KEY)
	{
		
		switch (wParam)
		{
		case AVK_CLR:
		case AVK_SOFT2:
			IROOTFORM_PopForm(pMe->rootForm);

			break;

		case AVK_SOFT1:
			IROOTFORM_PopForm(pMe->rootForm);
	
			//save the settings
			IWIDGET_MoveFocus(pMe->uploadTimeSettingContainerWidget, (IWidget*)WIDGET_FOCUS_NONE);
			uploadTimeSet = (uint32)IVALUEMODEL_GetValue(pMe->uploadTime,&dataLength);

			if ( GetUserData(pMe->pIShell, &pUser))
			{							
				pUser->uploadTimeSeconds=uploadTimeSet;
				SetUserData(pMe->pIShell, pUser);
			}

			FREEIF(pUser);
			break;
		
		}
		return TRUE;
	}	 
  	
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->uploadTimeSettingPopupHandler, evt, wParam, dwParam);
}


// ================================================================================
// FUNCTION		: MessageFrom_UploadTimeSettingForm
// DESCRIPTION	: POPULATE POPUP FORM FOR SET SMS UPLOAD TIME
// ================================================================================
int MessageFrom_UploadTimeSettingForm(MessageForm* pMe) 
{
	int result=0;
	IWidget* backdropWidget=NULL;
	IWidget* titleStaticWgt=NULL;
	IWidget* containerWidget = NULL;
	AECHAR* sstr=NULL;
	WidgetPos wpos;
	user *pUser=NULL;
	WExtent we;
	AEEDeviceInfo  deviceInfo;
	IWidget* list=NULL;
	IModel *iModal = NULL;
    int nWdtHeight=0;
	int nWdtWidth=0;
	const AECHAR wszFormat[] = {'h','m','s','T',0}; 
	
	if (NULL==pMe->uploadTimeSettingPopupFrom)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->uploadTimeSettingPopupFrom);		
		result +=  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->uploadTimeSettingContainer);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TIMEWIDGET, (void**)&pMe->timeWgt);		
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&titleStaticWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->uploadTime);
			
		IFORM_SetResText((IForm*)pMe->uploadTimeSettingPopupFrom, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_UPLOADTIME);
		IFORM_SetSoftkeys((IForm*)pMe->uploadTimeSettingPopupFrom, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
		
		IWIDGET_SetModel(pMe->timeWgt, IVALUEMODEL_TO_IMODEL(pMe->uploadTime));
		IWIDGET_SetProperty(pMe->timeWgt, PROP_FORMATSTRING, (uint32)wszFormat);	
		
		FREEIF(pMe->pszHeading);
		pMe->pszHeading = (AECHAR*)MALLOC(PS_TEXT_LENGTH);

		if (NULL==pMe->pszHeading) goto error;		
		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_UPLOADTIME,pMe->pszHeading,PS_TEXT_LENGTH) ) goto error;
		
		IWIDGET_SetText(titleStaticWgt, pMe->pszHeading, FALSE);

		ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
		nWdtHeight = (deviceInfo.cyScreen/12)*8;
		nWdtHeight = nWdtHeight/6;
		nWdtHeight = nWdtHeight;
		nWdtWidth = deviceInfo.cxScreen;

		IWIDGET_SetExtent(pMe->timeWgt,&we);
		IWIDGET_SetFlags(pMe->timeWgt, IDF_ALIGN_CENTER);
		IWIDGET_SetBorderColor(pMe->timeWgt, MAKE_RGB(252, 252, 252));

		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(titleStaticWgt,&we);
		IWIDGET_SetFlags(titleStaticWgt, IDF_ALIGN_CENTER);
		IWIDGET_SetBorderWidth(titleStaticWgt, 1);
		IWIDGET_SetBorderColor(titleStaticWgt, MAKE_RGB(0, 0, 0));
		
		we.height = nWdtHeight;
		we.width = deviceInfo.cxScreen*3/4;
		IWIDGET_SetExtent(pMe->timeWgt,&we);
		IWIDGET_SetBorderColor(pMe->timeWgt, MAKE_RGB(100, 100, 100));
		IWIDGET_SetBorderWidth(pMe->timeWgt, 1);
		IWIDGET_SetFlags(pMe->timeWgt, IDF_ALIGN_RIGHT);
				
		IFORM_SetWidget((IForm*)pMe->uploadTimeSettingPopupFrom, WID_BACKDROP, backdropWidget);
		IWIDGET_SetFlags((IWidget*)pMe->uploadTimeSettingPopupFrom, IDF_ALIGN_FILL);

		//add the title to form
		wpos.x = 0;
		wpos.y = 0;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->uploadTimeSettingContainer, titleStaticWgt, WIDGET_ZNORMAL, &wpos);

		//Adding the text field to the container
		wpos.x = deviceInfo.cxScreen/6;
		wpos.y = deviceInfo.cyScreen/4;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->uploadTimeSettingContainer, pMe->timeWgt, WIDGET_ZNORMAL, &wpos);

		// Attach the contaner to the dialog
		result = IXYCONTAINER_QueryInterface(pMe->uploadTimeSettingContainer, AEEIID_WIDGET, (void**)&pMe->uploadTimeSettingContainerWidget);
		if(result ==0) 
		{			
			IFORM_SetWidget((IForm*)pMe->uploadTimeSettingPopupFrom, WID_FORM, pMe->uploadTimeSettingContainerWidget);
			IWIDGET_SetProperty(pMe->uploadTimeSettingContainerWidget, PROP_BGCOLOR, MAKE_RGB(225,225,225));
			IWIDGET_SetBorderWidth(pMe->uploadTimeSettingContainerWidget, 1);
		}

		HANDLERDESC_Init(&pMe->uploadTimeSettingPopupHandler, MessageForm_UploadTimeSettingFormEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->uploadTimeSettingPopupFrom, &pMe->uploadTimeSettingPopupHandler);
				
	error:			
		IWIDGET_Release(titleStaticWgt);
	
	}
	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->uploadTimeSettingPopupFrom);
	return TRUE;
}


// ================================================================================
// FUNCTION		: MessageForm_ContactSelectEH
// DESCRIPTION	: LISTEN EVENTS FROM MessageForm_SelectContactToShare FORM
// ================================================================================
static void MessageForm_ContactSelectEH(MessageForm *pMe, ModelEvent *pev) 
{
	int nPos=0;
	if(pev->evCode == EVT_MDL_FOCUS_SELECT) 
	{
		nPos = pev->dwParam;
		pMe->pTmpContAr[nPos]->status = !pMe->pTmpContAr[nPos]->status;

		RELEASEIF(pMe->pTmpContAr[nPos]->imageInfo.piImage);

		if ( pMe->pTmpContAr[nPos]->status )
			pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_SELECT);
		else
			pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_UNSELECT);

		IROOTFORM_Activate(pMe->rootForm);	
	}
	if (pev->evCode == EVT_MDL_FOCUS_CHANGE)
		pMe->SelectedContactIndex = (int)pev->dwParam;	
}


// ===================================================================================================================
// FUNCTION		: MessageForm_SelectContactCommandDisplayHandleEvent
// DESCRIPTION	: HANDLE EVENT FROM MessageForm_SelectContactToShare AND MessageForm_SelectContactCommandDisplay FORMS
// ===================================================================================================================
static boolean MessageForm_SelectContactCommandDisplayHandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   MessageForm* pMe = (MessageForm*) po;
   char *pszTmp=NULL;
   int len=0;
   int nPos=0;
   int nLen=0;
   char* name=NULL;
   char* phoneNo=NULL;
    switch (evt) 
	{
	case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT2:

				if(0==pMe->m_noOfAlbumSharedContacts)
				{
					IROOTFORM_PopForm(pMe->rootForm);
					if(pMe->pContactSelectForm)
					{
						IFORM_Release(pMe->pContactSelectForm);
						pMe->pContactSelectForm=NULL;
					}
					return TRUE;
				}		
				//write selected contacts to SelectedContact.txt file

				pszTmp=(char*)MALLOC(30*pMe->m_noOfAlbumSharedContacts);
				for(nPos=0;nPos<pMe->m_noOfAlbumSharedContacts;nPos++)
				{
					if(pMe->pTmpContAr[nPos]->status)
					{
						len = WSTRLEN(pMe->pTmpContAr[nPos]->imageInfo.pwText)+1;
						name=(char*)MALLOC(PS_TEXT_LENGTH_SMALL);
						WSTRTOSTR(pMe->pTmpContAr[nPos]->imageInfo.pwText, name, len+1);

						STRCAT( pszTmp, name );
						FREEIF(name);
						STRCAT( pszTmp, ":" );

						len = WSTRLEN(pMe->pTmpContAr[nPos]->pwzPhoneNo)+1;
						phoneNo=(char*)MALLOC(PS_TEXT_LENGTH_SMALL);
						WSTRTOSTR(pMe->pTmpContAr[nPos]->pwzPhoneNo, phoneNo, len+1);

						STRCAT( pszTmp, phoneNo );
						FREEIF(phoneNo);
						STRCAT( pszTmp, ";" );

					}
				}				
				nLen = STRLEN(pszTmp);

				if ( WriteFileStream(pMe->pIShell, SELECTCONTACT_FILE, pszTmp, nLen, FALSE) >0)
				{
					FREEIF(pszTmp);
				}
				FREEIF(pszTmp);
				IROOTFORM_PopForm(pMe->rootForm);
				MessageForm_SelectContactToShareDelete(pMe);				
				return (TRUE);
				break;
			case AVK_SOFT1:
				MessageForm_ContactHandleFGorSMSUploadMenu(pMe);
				return (TRUE);
				break;
			case AVK_CLR:
				IROOTFORM_PopForm(pMe->rootForm);
				MessageForm_SelectContactToShareDelete(pMe);
				break;
			}
			return TRUE;
		}

	
	}
	

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->SelectContactToShareHandler, evt, wParam, dwParam);
}


// ================================================================================
// FUNCTION		: MessageForm_SelectContactToShareDelete
// DESCRIPTION	: DELETE RESOURCES USED IN MessageForm_SelectContactToShare FORM
// ================================================================================
void MessageForm_SelectContactToShareDelete(MessageForm* pMe)
{
	int i=0;
	if(pMe->pContactSelectForm)
	{
		IFORM_Release(pMe->pContactSelectForm);  
		pMe->pContactSelectForm=NULL;
	}
	
	if (pMe->pContactListContainer)
	{
		IPROPCONTAINER_Release(pMe->pContactListContainer);
		pMe->pContactListContainer=NULL;
	}

	if ( pMe->pTmpContAr )
	{
		//free text and image resources
		for (i=0;i<pMe->m_noOfAlbumSharedContacts;i++)
		{
			FREEIF(	pMe->pTmpContAr[i]->imageInfo.pwText );
			RELEASEIF(	pMe->pTmpContAr[i]->imageInfo.piImage );		
		}

		for (i=pMe->m_noOfAlbumSharedContacts-1;i>=0;i--)
		{
			FREEIF(pMe->pTmpContAr[i]);
		}

		FREEIF(pMe->pTmpContAr);

	}
}


// ======================================================================================================================
// FUNCTION		: MessageForm_SelectContactCommandDisplay
// DESCRIPTION	: IF THERE IS NO ANY CONTACT TO SELECT FOR SMS UPLOAD THEN USER IS VIEWED COMMAND FORM IS BUILT FROM HERE.
// ======================================================================================================================
boolean MessageForm_SelectContactCommandDisplay(MessageForm* pMe){
	
	IWidget* addContactsText1=NULL;
	IWidget* addContactsText2=NULL;
	IWidget *containerWidget=NULL;
	IXYContainer* container;
	AEEDeviceInfo  deviceInfo;
	WExtent we;
	WidgetPos wpos;
	int nWdtHeight=0;
	int nWdtWidth=0;
	int result=0;
	
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->pContactSelectForm) == 0){
		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &container);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&addContactsText1);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&addContactsText2);
	
		result += IFORM_SetResText((IForm*)pMe->pContactSelectForm, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_CONTACT);
		result += IFORM_SetSoftkeys((IForm*)pMe->pContactSelectForm, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);

		pMe->text1 = (AECHAR*) MALLOC(50);
		pMe->text2 = (AECHAR*) MALLOC(50);
		
		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ADDCONTACTSTEXT1, pMe->text1, 50) ) return -1;
		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ADDCONTACTSTEXT2, pMe->text2, 50) ) return -1;

		IWIDGET_SetText(addContactsText1, pMe->text1, TRUE);
		IWIDGET_SetText(addContactsText2, pMe->text2, TRUE);

		IWIDGET_SetExtent(addContactsText1,&we);
		IWIDGET_SetFlags(addContactsText1, IDF_ALIGN_LEFT);
		IWIDGET_SetBorderColor(addContactsText1, MAKE_RGB(252, 252, 252));

		IWIDGET_SetExtent(addContactsText2,&we);
		IWIDGET_SetFlags(addContactsText2, IDF_ALIGN_LEFT);
		IWIDGET_SetBorderColor(addContactsText2, MAKE_RGB(252, 252, 252));

		ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
		nWdtHeight = (deviceInfo.cyScreen/12)*8;
		nWdtHeight = nWdtHeight/6;
		nWdtWidth = deviceInfo.cxScreen;

		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(addContactsText1,&we);
		IWIDGET_SetBorderColor(addContactsText1, MAKE_RGB(100, 100, 100));

		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(addContactsText2,&we);
		IWIDGET_SetBorderColor(addContactsText2, MAKE_RGB(100, 100, 100));

		wpos.x = deviceInfo.cxScreen/6;
		wpos.y = deviceInfo.cyScreen/4;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(container, addContactsText1, WIDGET_ZNORMAL, &wpos);

		wpos.x = deviceInfo.cxScreen/6;
		wpos.y = deviceInfo.cyScreen/3;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(container, addContactsText2, WIDGET_ZNORMAL, &wpos);
	
		result = IXYCONTAINER_QueryInterface(container, AEEIID_WIDGET, (void**)&containerWidget);
		
		HANDLERDESC_Init(&pMe->SelectContactToShareHandler, MessageForm_SelectContactCommandDisplayHandleEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->pContactSelectForm, &pMe->SelectContactToShareHandler);

		if(result ==0) 
		{
			IFORM_SetWidget((IForm*)pMe->pContactSelectForm, WID_FORM, containerWidget);
		}
			IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->pContactSelectForm);

		if (containerWidget)
		{
			IWIDGET_Release(containerWidget);
			containerWidget=NULL;
		}
		if (addContactsText1)
		{
			IWIDGET_Release(addContactsText1);
			addContactsText1=NULL;
		}
		if (addContactsText2)
		{
			IWIDGET_Release(addContactsText2);
			addContactsText2=NULL;
		}
		if (container)
		{
			IXYCONTAINER_Release(container);
			container=NULL;
		}				
	}
	return TRUE;
}


// ================================================================================
// FUNCTION		: MessageForm_SelectContactToShare
// DESCRIPTION	: POPULATE CONTACT TO SELECT FOR SMS UPLOAD
// ================================================================================
boolean MessageForm_SelectContactToShare(MessageForm* pMe, char* contactAr)
{
	IWidget *containerWidget=NULL;	
	IWidget* pScrollBar = NULL;
	IWidget *pListWgt = NULL;
	IMenuModel *pMenuModel=NULL;
	ImageStaticWidget* imageWidget = NULL;
	AECHAR *pszContactsAr=NULL;
	AECHAR *pStart=NULL;
	AECHAR *pEnd=NULL;
	AECHAR *pPhoneNo=NULL;
	WidgetProp wp;
	WExtent we;
	AECHAR *pwsTemp=NULL;
	IImage* m_pIImage;
	int nLen=0;
	int nSize=0;
	int size=0;	
	int nScreenWidth=0;
	int i=0;
	int result = 0;
	
	if(0==pMe->m_noOfAlbumSharedContacts)
	{
		MessageForm_SelectContactCommandDisplay(pMe);
		return TRUE;
	}
	if ( NULL!=pMe->pContactSelectForm )
	{
		int nPos=0;
		for(nPos=0;nPos<pMe->m_noOfAlbumSharedContacts;nPos++)
		{
			pMe->pTmpContAr[nPos]->status = FALSE;
			//release previous image
			RELEASEIF(pMe->pTmpContAr[nPos]->imageInfo.piImage);
			pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_UNSELECT);
		}
		
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->pContactSelectForm);
		return TRUE;
	}

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->pContactSelectForm) == 0)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->pContactListContainer);
		m_pIImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_STITLE);
		IFORM_SetTitleImage((IForm*)pMe->pContactSelectForm,m_pIImage);
		result += IFORM_SetResText((IForm*)pMe->pContactSelectForm, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_CONTACT);
		result += IFORM_SetSoftkeys((IForm*)pMe->pContactSelectForm, RIPPLEVAULT_RES_FILE, MAINSK1, IDS_DONE);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**)&pScrollBar);		
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&pListWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MENUMODEL, (void**)&pMenuModel);
				
		nLen = STRLEN(contactAr);
		size=(nLen+1)*sizeof(AECHAR);	
		pszContactsAr = (AECHAR*) MALLOC(size);
		STRTOWSTR(contactAr, pszContactsAr, size);	
		
		nLen=0;
		pMe->pTmpContAr = (ContactShareSMS**)MALLOC(pMe->m_noOfAlbumSharedContacts*sizeof(ContactShareSMS*));
		if ( NULL==pMe->pTmpContAr ) goto error;

		//add contacts to list
		pStart=pszContactsAr;
		DBGPRINTF("%S",pStart);
		while( NULL!=(pEnd=WSTRCHR(pStart,';')))
		{
			nLen = WSTRLEN(pStart)-WSTRLEN(pEnd);		
			nSize=(nLen+1)*sizeof(AECHAR);
			pwsTemp = (AECHAR*) MALLOC(nSize);//this memory will release with the pTmpContAr release
			pMe->pTmpContAr[i] = (ContactShareSMS*) MALLOC(sizeof(ContactShareSMS));
			if ( NULL==pMe->pTmpContAr[i] || NULL==pwsTemp )
				goto error;

			WSTRLCPY(pwsTemp, pStart, nLen+1);
			pPhoneNo = WSTRCHR(pwsTemp, ':');
			if (pPhoneNo)
			{
				WSTRCPY(pMe->pTmpContAr[i]->pwzPhoneNo, pPhoneNo+1);
				DBGPRINTF("%S", pMe->pTmpContAr[i]->pwzPhoneNo);
				//remove phone number from name string
				WSTRCPY(pPhoneNo, L"\0");
			}

			DBGPRINTF("%S", pwsTemp);
			pMe->pTmpContAr[i]->imageInfo.pwText = (AECHAR*)pwsTemp;
			DBGPRINTF("%S", pMe->pTmpContAr[i]->imageInfo.pwText);
			pMe->pTmpContAr[i]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_UNSELECT);
			IMENUMODEL_Add(pMenuModel, &pMe->pTmpContAr[i]->imageInfo, (uint16)i, MMF_ENABLED);
			pMe->pTmpContAr[i]->status=FALSE;

			pStart = pStart+nLen+1;
			i++;
			
		}
		
		
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGESTATICWIDGET, (void**)&imageWidget);	
	
		//set the vidget to act as a menu modal vidget
		IWIDGET_SetModel(pListWgt, IMENUMODEL_TO_IMODEL(pMenuModel));

		//set the picklist widget properties
		IWIDGET_SetBorderWidth(pListWgt, 0);
		IWIDGET_SetItemHeight(pListWgt, 25);
		IWIDGET_SetItemWidth(pListWgt, 15);
		IWIDGET_SetHintCols(pListWgt, 5);
		IWIDGET_GetPreferredExtent(pListWgt, &we);
		IWIDGET_SetExtent(pListWgt, &we);

		IWIDGET_SetSelectedShadowOffset((IWidget *)imageWidget,2);
		IWIDGET_SetShadowColor((IWidget *)imageWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor((IWidget*)imageWidget, MAKE_RGB(128,128,255));
		
		IWIDGET_SetProperty(pScrollBar, PROP_ACTIVE_SCROLLCOLOR, MAKE_RGB(128,128,255));
		IDECORATOR_SetWidget((IDecorator*)pScrollBar, (IWidget *)pListWgt);
		IDECORATOR_SetWidget((IDecorator*)pListWgt, (IWidget *)imageWidget);
		wp.bVisible = TRUE;
		wp.prop = 1;
		IPROPCONTAINER_Insert(pMe->pContactListContainer, pScrollBar, WIDGET_ZNORMAL, &wp);

		// in order to to pick up operator selection
		{
			IModel* pickListViewModel = NULL;
			IWIDGET_GetViewModel(pListWgt, &pickListViewModel);
			IMODEL_AddListenerEx(pickListViewModel, &pMe->ContactSelectMenuListener, (PFNLISTENER)MessageForm_ContactSelectEH, pMe);
			IMODEL_Release(pickListViewModel);
		}

		//set WID_FORM to the container
		result = IPROPCONTAINER_QueryInterface(pMe->pContactListContainer, AEEIID_WIDGET, (void**)&containerWidget);
		if(result ==0) 
		{
			IFORM_SetWidget((IForm*)pMe->pContactSelectForm, WID_FORM, containerWidget);
		}

		HANDLERDESC_Init(&pMe->SelectContactToShareHandler, MessageForm_SelectContactCommandDisplayHandleEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->pContactSelectForm, &pMe->SelectContactToShareHandler);

		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->pContactSelectForm);

		IWIDGET_Release((IWidget*)imageWidget);
		
	}
	

	//release module.
error:
	FREEIF(pszContactsAr);
	IWIDGET_Release(containerWidget);
	IWIDGET_Release(pScrollBar);

	IWIDGET_Release(pListWgt);
	RELEASEIF(m_pIImage);
	IMENUMODEL_Release(pMenuModel);
		
	return TRUE;
}





// ================================================================================
// FUNCTION		: MEssageForm_ContactHandleForSMSUploadSelectionHandler
// DESCRIPTION	: HANDLE OPTION SELECTION FROM MessageForm_ContactHandleFGorSMSUploadMenu MENU
// ================================================================================
static void MEssageForm_ContactHandleForSMSUploadSelectionHandler(MessageForm* pMe, int idOption) 
{
    
	int nPos=0;
	char* start=NULL;
	char* end=NULL;
	int nLen=0;
	char* pszContactInfo=NULL;
	ISyncEngExt* pSyncExt;
	AECHAR *pszContactsAr=NULL;
	
	switch(idOption){
	case 0:
		if(0==pMe->m_noOfAlbumSharedContacts)
		{
			IROOTFORM_PopForm(pMe->rootForm);
			if(pMe->contactHandleMenuForSMSUpload_menu)
			{
				IPOPUPMENU_Release(pMe->contactHandleMenuForSMSUpload_menu);  
				pMe->contactHandleMenuForSMSUpload_menu=NULL;
			}
			IROOTFORM_PopForm(pMe->rootForm);
			if(pMe->pContactSelectForm)
			{
				IFORM_Release(pMe->pContactSelectForm);
				pMe->pContactSelectForm=NULL;
			}
			//call form for selelct contacts from main contact collection
			if ( SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pSyncExt))
			goto error;
	
			pMe->m_noOfAlbumSharedContacts = ISYNCENG_GetContacts(pSyncExt, &pszContactsAr);				
			pszContactInfo=(char*)MALLOC(pMe->m_noOfAlbumSharedContacts*PS_TEXT_LENGTH_SMALL);
			WSTRTOSTR(pszContactsAr, pszContactInfo, pMe->m_noOfAlbumSharedContacts*PS_TEXT_LENGTH_SMALL);

			MessageForm_SelectContactToShare(pMe, pszContactInfo);
			FREEIF(pszContactInfo);
			error:
			ISYNCENG_Release(pSyncExt);
			FREEIF(pszContactsAr);
			break;
		}
		nPos = pMe->SelectedContactIndex;
		pMe->pTmpContAr[nPos]->status = !pMe->pTmpContAr[nPos]->status;
		
		//release previous image
		RELEASEIF(pMe->pTmpContAr[nPos]->imageInfo.piImage);

		if ( pMe->pTmpContAr[nPos]->status )
			pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_SELECT);
		else
			pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_UNSELECT);

		IROOTFORM_PopForm(pMe->rootForm);
		if(pMe->contactHandleMenuForSMSUpload_menu)
		{
			IPOPUPMENU_Release(pMe->contactHandleMenuForSMSUpload_menu);  
			pMe->contactHandleMenuForSMSUpload_menu=NULL;
		}
		IROOTFORM_Activate(pMe->rootForm);
	break;
	case 1:
		IROOTFORM_PopForm(pMe->rootForm);
		if(pMe->contactHandleMenuForSMSUpload_menu)
		{
			IPOPUPMENU_Release(pMe->contactHandleMenuForSMSUpload_menu);  
			pMe->contactHandleMenuForSMSUpload_menu=NULL;
		}

	}

}



// ================================================================================
// FUNCTION		: MEssageForm_ContactHandleFGorSMSUploadEventHandler
// DESCRIPTION	: DANDLE EVENT FROM MessageForm_ContactHandleFGorSMSUploadMenu MENU
// ================================================================================
static boolean MEssageForm_ContactHandleFGorSMSUploadEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	MessageForm* pMe = (MessageForm*) po;
	int nPos=0;
	char* start=NULL;
	char* end=NULL;
	int nLen=0;
	char* pszContactInfo=NULL;
	ISyncEngExt* pSyncExt;
	AECHAR *pszContactsAr=NULL;

   
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;

		if(0==pMe->m_noOfAlbumSharedContacts)
		{
			rc.y = pMe->DeviceInfo.cyScreen*8/10;
			rc.dy = pMe->DeviceInfo.cyScreen*1/10;
		}else
		{
			rc.y = pMe->DeviceInfo.cyScreen*7/10;
			rc.dy = pMe->DeviceInfo.cyScreen*2/10;
		}
		rc.dx = pMe->DeviceInfo.cxScreen;
		*(AEERect*) dwParam = rc;

      return TRUE;
   }

	switch (evt) 
		{
		case EVT_KEY:
			{
				switch (wParam) 
				{
				case AVK_SOFT2:
						IROOTFORM_PopForm(pMe->rootForm);
						if(pMe->contactHandleMenuForSMSUpload_menu)
						{
							IPOPUPMENU_Release(pMe->contactHandleMenuForSMSUpload_menu);  
							pMe->contactHandleMenuForSMSUpload_menu=NULL;
						}
						return (TRUE);
						break;
				case AVK_SOFT1:
					if(0==pMe->m_noOfAlbumSharedContacts)
					{
						IROOTFORM_PopForm(pMe->rootForm);
						if(pMe->contactHandleMenuForSMSUpload_menu)
						{
							IPOPUPMENU_Release(pMe->contactHandleMenuForSMSUpload_menu);  
							pMe->contactHandleMenuForSMSUpload_menu=NULL;
						}
						IROOTFORM_PopForm(pMe->rootForm);
						if(pMe->pContactSelectForm)
						{
							IFORM_Release(pMe->pContactSelectForm);
							pMe->pContactSelectForm=NULL;
						}

						//call form for selelct contacts from main contact collection
						if ( SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pSyncExt))
						goto error;
				
						pMe->m_noOfAlbumSharedContacts = ISYNCENG_GetContacts(pSyncExt, &pszContactsAr);				
						pszContactInfo=(char*)MALLOC(pMe->m_noOfAlbumSharedContacts*PS_TEXT_LENGTH_SMALL);
						WSTRTOSTR(pszContactsAr, pszContactInfo, pMe->m_noOfAlbumSharedContacts*PS_TEXT_LENGTH_SMALL);
						
						MessageForm_SelectContactToShare(pMe, pszContactInfo);
						FREEIF(pszContactInfo);

						error:
							ISYNCENG_Release(pSyncExt);
							FREEIF(pszContactsAr);

						return TRUE;
					}
					nPos = pMe->SelectedContactIndex;
					pMe->pTmpContAr[nPos]->status = !pMe->pTmpContAr[nPos]->status;
					
					//release previous image
					RELEASEIF(pMe->pTmpContAr[nPos]->imageInfo.piImage);

					if ( pMe->pTmpContAr[nPos]->status )
						pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_SELECT);
					else
						pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_UNSELECT);

					IROOTFORM_PopForm(pMe->rootForm);

					if(pMe->contactHandleMenuForSMSUpload_menu)
					{
						IPOPUPMENU_Release(pMe->contactHandleMenuForSMSUpload_menu);  
						pMe->contactHandleMenuForSMSUpload_menu=NULL;
					}
					IROOTFORM_Activate(pMe->rootForm);
					return (TRUE);
					break;
				case AVK_CLR:
					IROOTFORM_PopForm(pMe->rootForm);
					if(pMe->contactHandleMenuForSMSUpload_menu)
					{
						IPOPUPMENU_Release(pMe->contactHandleMenuForSMSUpload_menu);  
						pMe->contactHandleMenuForSMSUpload_menu=NULL;
					}
					break;
				}
				
				return TRUE;
			}

		
		}
   




   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->ContactHandleForSMSUploadHandler, evt, wParam, dwParam);
}


// ================================================================================
// FUNCTION		: MessageForm_ContactHandleFGorSMSUploadMenu
// DESCRIPTION	: POPULATE OPTION MENU FOR SMS UPLOAD SELECTION FORM
// ================================================================================
int MessageForm_ContactHandleFGorSMSUploadMenu(MessageForm* pMe) 
{
	int result = 0;
	IWidget* backDropWidget=NULL;
  
   if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
	   return (FALSE);
   if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->contactHandleMenuForSMSUpload_menu) != 0)
	   return (FALSE);
   if(0==pMe->m_noOfAlbumSharedContacts)
   {
		if(IPOPUPMENU_LoadMenuItem(pMe->contactHandleMenuForSMSUpload_menu, RIPPLEVAULT_RES_FILE, IDS_ADDCONTACTS, 0,MMF_ENABLED) != 0)
		   return (FALSE);
   }else
   {
		if(IPOPUPMENU_LoadMenuItem(pMe->contactHandleMenuForSMSUpload_menu, RIPPLEVAULT_RES_FILE, IDS_ADDCANCELSHARE, 0,MMF_ENABLED) != 0)
		   return (FALSE);
		if(IPOPUPMENU_LoadMenuItem(pMe->contactHandleMenuForSMSUpload_menu, RIPPLEVAULT_RES_FILE, IDS_CANCEL, 1,MMF_ENABLED) != 0)
		   return (FALSE);
   }
	
	IFORM_SetSoftkeys((IForm*)pMe->contactHandleMenuForSMSUpload_menu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);
	IFORM_SetSelectHandler((IForm*)pMe->contactHandleMenuForSMSUpload_menu, (PFNSELECT)MEssageForm_ContactHandleForSMSUploadSelectionHandler, pMe);

	HANDLERDESC_Init(&pMe->ContactHandleForSMSUploadHandler, MEssageForm_ContactHandleFGorSMSUploadEventHandler, pMe, 0);	
	IFORM_SetHandler((IForm*)pMe->contactHandleMenuForSMSUpload_menu, &pMe->ContactHandleForSMSUploadHandler);

	//wrap the pop-up menu in a scrollbar decorator
	{
		IWidget* piScrollWidget = NULL;
		IWidget* mainFormWidget = NULL;
		IWidget* backWidget = NULL;
		if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**) &piScrollWidget) != 0)
			return (FALSE);

		// change the scroll pad
		IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);

		IFORM_GetWidget((IForm*)pMe->contactHandleMenuForSMSUpload_menu, WID_FORM, &mainFormWidget);
		IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
		IFORM_SetWidget((IForm*)pMe->contactHandleMenuForSMSUpload_menu, WID_FORM, piScrollWidget);

		IFORM_GetWidget((IForm*)pMe->contactHandleMenuForSMSUpload_menu, WID_LISTITEM, &backWidget);
		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->contactHandleMenuForSMSUpload_menu, WID_LISTITEM, backWidget);
	
		IWIDGET_Release(mainFormWidget);
		IWIDGET_Release(piScrollWidget);	
		IWIDGET_Release(backWidget);
	}
	IWIDGET_Release(backDropWidget);
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->contactHandleMenuForSMSUpload_menu);
	return (TRUE);
}