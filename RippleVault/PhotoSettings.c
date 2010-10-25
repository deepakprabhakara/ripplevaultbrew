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

 Description                   : Handle photo setting change (source)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#include "PhotoSettings.h"
#include "AEECheckWidget.h"
#include "UserData.h"
#include "AEEDialog.h"
#include "AEETextWidget.h"

#define RIPPLEVAULT_RES_FILE "RippleVault.bar"
#define PS_TEXT_LENGTH		50
#define PS_TEXT_LENGTH_SMALL		20

// ================================================================================
// FUNCTION		: PhotoSettings_Delete
// DESCRIPTION	: Delete PhotoSettings form and release its resources.
// ================================================================================
void PhotoSettings_Delete(PhotoSettings* pMe) {

	//set user settings
	SetUserData(pMe->pIShell, pMe->userSettings);

	FREEIF(pMe->userSettings);

	if ( pMe->msgDlg)
	{
		IFORM_Release(pMe->msgDlg);
		pMe->msgDlg=NULL;
	}
	if ( pMe->msgDlgContainer )
	{
		IXYCONTAINER_Release(pMe->msgDlgContainer);
		pMe->msgDlgContainer = NULL;
	}
	
	if (pMe->msgWgt)
	{
		IWIDGET_Release(pMe->msgWgt);
		pMe->msgWgt = NULL;
	}

	if (pMe->infoDlg)
	{
		IDIALOG_Release(pMe->infoDlg);
		pMe->infoDlg=NULL;
	}

	if (pMe->selectAlbumMenuModel)
	{
	   IMENUMODEL_Release(pMe->selectAlbumMenuModel);
	   pMe->selectAlbumMenuModel = NULL;
	}

	if (pMe->selectAlbumPopupMenu)
	{
		IPOPUPMENU_Release(pMe->selectAlbumPopupMenu);
		pMe->selectAlbumPopupMenu=NULL;
	}

	if(pMe->mainContainer)
	{
		IPROPCONTAINER_Release(pMe->mainContainer);
		pMe->mainContainer = NULL;
	}

	if(pMe->optionsMenu)
	{
		IPOPUPMENU_Release(pMe->optionsMenu);  
		pMe->optionsMenu=NULL;
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

	if (pMe->titleStaticWgt)
	{
		IWIDGET_Release(pMe->titleStaticWgt);
		pMe->titleStaticWgt=NULL;
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

	
	FREEIF(pMe->pszAlbum);
	FREEIF(pMe->pszAlbumValue);
	FREEIF(pMe->pszDescription);
	FREEIF(pMe->pszDescriptionValue);
	FREEIF(pMe->pszRoaming);
	FREEIF(pMe->pszRoamingValue);

	if (pMe->pAlbumWgt)				IWIDGET_Release(pMe->pAlbumWgt);
	if (pMe->pAlbumValueWgt)		IWIDGET_Release(pMe->pAlbumValueWgt);
	if (pMe->pDescriptionWgt)		IWIDGET_Release(pMe->pDescriptionWgt);
	if (pMe->pDescriptionValueWgt)	IWIDGET_Release(pMe->pDescriptionValueWgt);

	FREEIF(pMe->pszOption1);
	FREEIF(pMe->pszOption2);
	FREEIF(pMe->pszHeading);

	if(pMe)
		FREE(pMe);
}


// ================================================================================
// FUNCTION		: PhotoSettings_MenuModalEvent
// DESCRIPTION	: Handle user select/change main list item
// ================================================================================
static void PhotoSettings_MenuModalEvent(PhotoSettings *pMe, ModelEvent *pev) 
{

	if(pev->evCode == EVT_MDL_FOCUS_SELECT)
	{
		pMe->nSelectedCategory = pev->dwParam;

		PhotoSettings_Select(pMe);
	}

	if(pev->evCode == EVT_MDL_FOCUS_CHANGE)
	{
		pMe->nSelectedCategory = pev->dwParam;
	}
	

}

// ================================================================================
// FUNCTION		: PhotoSettings_HandleEvent
// DESCRIPTION	: Photo settings main event handler
// ================================================================================
static boolean PhotoSettings_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   PhotoSettings* pMe = (PhotoSettings*) po;

	switch (evt) 
	{
		case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT1:
				PhotoSettings_CreateOptionsMenu(pMe);
				return (TRUE);
				break;
			case AVK_CLR:
				IROOTFORM_PopForm(pMe->rootForm);
				return (TRUE);
				
			case AVK_SOFT2:
				IROOTFORM_PopForm(pMe->rootForm);
				return (TRUE);

			case AVK_DOWN:
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


static void PickAdapter(void *pUnused, void **ppValueIn, int nLen, void **ppValueOut, int *pnLenOut)
{
   *pnLenOut = 1;  // force length to be 1, instead of calculating
}

// ================================================================================
// FUNCTION		: PhotoSettings_OptionSelectionHandler
// DESCRIPTION	: Photo settings option selection handler
// ================================================================================
static void PhotoSettings_OptionSelectionHandler(PhotoSettings* pMe, int idOption)
{
	//pop option menu
	IROOTFORM_PopForm(pMe->rootForm);

	PhotoSettings_Select(pMe);
}

// ================================================================================
// FUNCTION		: PhotoSettings_OptionsEventHandler
// DESCRIPTION	: Photo settings option menu event handler
// ================================================================================
static boolean PhotoSettings_OptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   PhotoSettings* pMe = (PhotoSettings*) po;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		
		//catch FID_PREFRECT and set preferred extent of menu
		AEEDeviceInfo  DeviceInfo;
		AEERect rc;
		int nStatusbarHeight=0; 
		ISHELL_GetDeviceInfo(pMe->pIShell, &DeviceInfo);

		nStatusbarHeight = 	DeviceInfo.cyScreen/8;
		rc.x = 0;
		rc.y = DeviceInfo.cyScreen-2*nStatusbarHeight;
		rc.dx = DeviceInfo.cxScreen;
		rc.dy = nStatusbarHeight;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if(evt == EVT_KEY)
	{
		if(pMe->optionsMenu) 
		{
			switch (wParam)
			{
			case AVK_CLR:
			case AVK_SOFT2:
				IROOTFORM_PopForm(pMe->rootForm);
			break;

			case AVK_SOFT1:
				//PhotoSettings_Select(pMe);
				break;
			}
		}
	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->optionsMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PhotoSettings_OptionsEventHandler
// DESCRIPTION	: Photo settings option menu event handler
// ================================================================================
int PhotoSettings_CreateOptionsMenu(PhotoSettings* pMe) 
{
	int result = 0;
	IWidget* backDropWidget;

	if ( NULL==pMe->optionsMenu )
	{
		if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
		   return (FALSE);
		if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->optionsMenu) != 0)
		   return (FALSE);

		if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_CHANGE, 0,MMF_ENABLED) != 0)
		   return (FALSE);

		IFORM_SetSoftkeys((IForm*)pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);

		IFORM_SetSelectHandler((IForm*)pMe->optionsMenu, (PFNSELECT)PhotoSettings_OptionSelectionHandler, pMe);
		HANDLERDESC_Init(&pMe->optionsMenuHandler, PhotoSettings_OptionsEventHandler, pMe, 0);
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
			IFORM_SetWidget((IForm*)pMe->optionsMenu, WID_BACKDROP, backDropWidget);

			IWIDGET_Release(mainFormWidget);
			IWIDGET_Release(piScrollWidget);
			IWIDGET_Release(backDropWidget);
			IWIDGET_Release(backWidget);
		}
	}

	if (pMe->optionsMenu)
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->optionsMenu);
	return (TRUE);
}

// ================================================================================
// FUNCTION		: PhotoSettings_PopulateMainContainer
// DESCRIPTION	: Photo settings main widgets and form creator
// ================================================================================
int PhotoSettings_PopulateMainContainer(PhotoSettings* pMe) 
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

	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->pAlbumWgt);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->pAlbumValueWgt);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->pDescriptionWgt);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->pDescriptionValueWgt);

	if (result) goto error;

	//load the strings
	pMe->pszAlbum				= (AECHAR*) MALLOC(PS_TEXT_LENGTH);
	pMe->pszAlbumValue			= (AECHAR*) MALLOC(PS_TEXT_LENGTH);
	pMe->pszDescription			= (AECHAR*) MALLOC(PS_TEXT_LENGTH);
	pMe->pszDescriptionValue	= (AECHAR*) MALLOC(PS_TEXT_LENGTH);
	pMe->pszRoaming				= (AECHAR*) MALLOC(PS_TEXT_LENGTH);

	if ( NULL==pMe->pszAlbum || NULL==pMe->pszAlbumValue || NULL==pMe->pszDescription 
		|| NULL==pMe->pszDescriptionValue ) goto error;

	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ALBUMPROMPT, pMe->pszAlbum, PS_TEXT_LENGTH);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PROMPT, pMe->pszAlbumValue, PS_TEXT_LENGTH);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_DESCPROMPT, pMe->pszDescription, PS_TEXT_LENGTH);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ENABLE, pMe->pszDescriptionValue, PS_TEXT_LENGTH);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROAMINGPROMPT, pMe->pszRoaming, PS_TEXT_LENGTH);

	IMENUMODEL_Add(iMenuModal,pMe->pszAlbum,0,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,pMe->pszDescription,1,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,pMe->pszRoaming,2,MMF_ENABLED);

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

	IWIDGET_SetFont(pMe->pAlbumWgt, piFont);
	IWIDGET_SetSelectedShadowOffset(pMe->pAlbumWgt,2);
	IWIDGET_SetShadowColor(pMe->pAlbumWgt,MAKE_RGB(122,122,122));
	IWIDGET_SetSelectedActiveBGColor(pMe->pAlbumWgt, MAKE_RGB(128,128,255));

	IWIDGET_SetProperty(pScrollBar, PROP_ACTIVE_SCROLLCOLOR, MAKE_RGB(128,128,255));
	IDECORATOR_SetWidget((IDecorator*)pScrollBar, (IWidget *)wItemList);
	IDECORATOR_SetWidget((IDecorator*)wItemList, pMe->pAlbumWgt);

	// get the view model of the wItemList and register a listener 
	// in order to to pick up operator selection
	{
		IModel* pickListViewModel = NULL;
		IWIDGET_GetViewModel(wItemList, &pickListViewModel);
		IMODEL_AddListenerEx(pickListViewModel, &pMe->menuListener, (PFNLISTENER)PhotoSettings_MenuModalEvent, pMe);
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
		IFORM_SetSoftkeys((IForm*)pMe->mainForm, RIPPLEVAULT_RES_FILE, IDS_SELECT, PHOTOSK2);
	}

error:

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
// FUNCTION		: PhotoSettings_New
// DESCRIPTION	: Allocate resources for Photo settings.
// ================================================================================
int PhotoSettings_New(IForm** ppo, IShell *piShell, IRootForm *pRootForm) 
{
	int result=0;

	//allocate space for the form
	PhotoSettings *pMe = MALLOCREC(PhotoSettings);
	IImage* pIImage;

	if(!pMe)
		return ENOMEMORY;

	//get user settings
	if ( !GetUserData(piShell, &pMe->userSettings) ) return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->mainForm) == 0)
	{
		*ppo = pMe->mainForm;
		pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->mainForm,pIImage);
		result = IFORM_SetResText((IForm*)pMe->mainForm, FID_TITLE, RIPPLEVAULT_RES_FILE, MAINFORMTITLE);
		result =  IFORM_SetSoftkeys((IForm*)pMe->mainForm, RIPPLEVAULT_RES_FILE, MAINSK1, MAINSK2);

		result = PhotoSettings_PopulateMainContainer(pMe);	//add items to the form

		HANDLERDESC_Init(&pMe->mainFormHandler, PhotoSettings_HandleEvent, pMe, PhotoSettings_Delete);
		IFORM_SetHandler((IForm*)pMe->mainForm, &pMe->mainFormHandler);

	}

	RELEASEIF(pIImage);

	return result;
}

// ================================================================================
// FUNCTION		: PhototSettings_SelectEvent
// DESCRIPTION	: Select event handler
// ================================================================================
static boolean PhototSettings_SelectEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	PhotoSettings* pMe = (PhotoSettings*) po;
	boolean bStatus=TRUE;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		//catch FID_PREFRECT and set preferred extent of menu
		AEEDeviceInfo  DeviceInfo;
		AEERect rc;

		ISHELL_GetDeviceInfo(pMe->pIShell, &DeviceInfo);

		rc.x = 0;
		rc.y = DeviceInfo.cyScreen*2/5;
		rc.dx = DeviceInfo.cxScreen;
		rc.dy = DeviceInfo.cyScreen-DeviceInfo.cyScreen/2;
		*(AEERect*) dwParam = rc;

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
			bStatus = IVALUEMODEL_GetBool(pMe->valueModelEnableRdo);
			
			switch (pMe->nSelectedCategory)
			{
			case 0:
				if (!bStatus)
					STRCPY(pMe->userSettings->album, "YES");
				else
				{
					//select a default album
					PhotoSettings_SelectAlbumMenu(pMe);
				}
				break;
			case 1:
				if (bStatus)
					STRCPY(pMe->userSettings->message, "YES");
				else
				{
					PhotoList_MsgDlg(pMe);
				}
				break;
			case 2:
				if (bStatus)
					STRCPY(pMe->userSettings->roaming, "YES");
				else
					STRCPY(pMe->userSettings->roaming, "NO");
				
				PhotoSettings_InfoDialog(pMe);
				break;
			}
			
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
// FUNCTION		: PhotoSettings_Select
// DESCRIPTION	: Opion selection form
// ================================================================================
int PhotoSettings_Select(PhotoSettings* pMe) 
{
	int result=0;

	IWidget* backdropWidget=NULL;
	AECHAR* sstr=NULL;
	WidgetPos wpos;
	
	WExtent we;
	AEEDeviceInfo  deviceInfo;
	IWidget* list=NULL;
	
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

		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_AUTO, pMe->pszOption1, PS_TEXT_LENGTH_SMALL) ) goto error;
		if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PROMPT, pMe->pszOption2, PS_TEXT_LENGTH_SMALL) ) goto error;
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
		
	
		
		//IFORM_SetText((IForm*)pMe->selectPopup,FID_TITLE, pMe->pszHeading);
		//title widget
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

		HANDLERDESC_Init(&pMe->selectPopupHandler, PhototSettings_SelectEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->selectPopup, &pMe->selectPopupHandler);
	
	error:	
				
		IWIDGET_Release(list);
		IMODEL_Release(iModal);
		
	}

	if (0==result)
	{
		MEMSET(pMe->pszOption1, 0, PS_TEXT_LENGTH_SMALL);
		MEMSET(pMe->pszOption2, 0, PS_TEXT_LENGTH_SMALL);
		MEMSET(pMe->pszHeading, 0, PS_TEXT_LENGTH);

		//set the titles and selection texts
		switch (pMe->nSelectedCategory)
		{
		case 0:
			if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_AUTO, pMe->pszOption1, PS_TEXT_LENGTH_SMALL) ) return FALSE;
			if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PROMPT, pMe->pszOption2, PS_TEXT_LENGTH_SMALL) ) return FALSE;
			if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ALBUMPROMPT,pMe->pszHeading,PS_TEXT_LENGTH) ) return FALSE;
			DBGPRINTF("%S", pMe->pszOption1);

			if (0==STRCMP("YES", pMe->userSettings->album))
			{
				IVALUEMODEL_SetBool(pMe->valueModelDisableRdo,TRUE);
				IVALUEMODEL_SetBool(pMe->valueModelEnableRdo,FALSE);
			}
			else
			{
				IVALUEMODEL_SetBool(pMe->valueModelEnableRdo,TRUE);
				IVALUEMODEL_SetBool(pMe->valueModelDisableRdo,FALSE);
			}

			break;

		case 1:
			if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ENABLE, pMe->pszOption1, PS_TEXT_LENGTH_SMALL) ) return FALSE;
			if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_DISABLE, pMe->pszOption2, PS_TEXT_LENGTH_SMALL) ) return FALSE;
			if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_DESCPROMPT,pMe->pszHeading,PS_TEXT_LENGTH) ) return FALSE;

			if (0==STRCMP("YES", pMe->userSettings->message))
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
			if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROAMINGPROMPT,pMe->pszHeading,PS_TEXT_LENGTH) ) return FALSE;

			if (0==STRCMP("YES", pMe->userSettings->roaming))
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

		IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->selectPopup);
	}
	
	return TRUE;
}


static void PhotoSettings_AlbumSelectEvent(PhotoSettings *pMe, int idOption) 
{
	
	pMe->nSelectedAlbum = idOption;
	
}

// ================================================================================
// FUNCTION		: PhotoSettings_SelectAlbumHandleEvent
// DESCRIPTION	: Event handle when album select
// ================================================================================
boolean PhotoSettings_SelectAlbumHandleEvent(PhotoSettings *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;
	int nHeight=0;
	AECHAR *pszAlbumName=NULL;
	uint16 nFlags=0;
	uint16 nId=0;

	if(eCode == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		//catch FID_PREFRECT and set preferred extent of menu
		AEEDeviceInfo  DeviceInfo;
		AEERect rc;
		
		ISHELL_GetDeviceInfo(pMe->pIShell, &DeviceInfo);
		nHeight=pMe->noOfAlbums*(DeviceInfo.cyScreen/8);
		if ( nHeight>DeviceInfo.cyScreen/2 )
			nHeight = DeviceInfo.cyScreen/2;
		rc.x = 0;
		rc.y = DeviceInfo.cyScreen-nHeight-DeviceInfo.cyScreen/8;//
		rc.dx = DeviceInfo.cxScreen;
		rc.dy = nHeight;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if ( eCode == EVT_KEY )
	{
		switch ( wParam )
		{
		case AVK_CLR:
			IROOTFORM_PopForm(pMe->rootForm);
			result = TRUE;
		break;
		case AVK_SOFT2:
			IROOTFORM_PopForm(pMe->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:
			IROOTFORM_PopForm(pMe->rootForm);
			//set the album name in user settings
			IMENUMODEL_GetAt(pMe->selectAlbumMenuModel, (uint16)pMe->nSelectedAlbum, MENUITEM_ALL, &pszAlbumName, &nFlags, &nId);
			WSTRTOSTR(pszAlbumName, pMe->userSettings->album, WSTRLEN(pszAlbumName)+1);
			break;
		}
	}

	if ( !result )
		result = HANDLERDESC_Call( &pMe->selectAlbumHandler, eCode, wParam, dwParam );

	result = TRUE;
	return result;
}

// ================================================================================
// FUNCTION		: PhotoSettings_SelectAlbumMenu
// DESCRIPTION	: create album select menu
// ================================================================================
boolean PhotoSettings_SelectAlbumMenu(PhotoSettings* pMe)
{

	int result = 0;
	IWidget* backDropWidget;
	AECHAR *pwzTemp=NULL;
	
	char *pszAlbumList=NULL;
	char *pStart=NULL;
	char *pEnd=NULL;
	int nLen=0;
	int nSize=0;
	int nCount=0;
	
	//release if any popup menu is created
	if ( !pMe->selectAlbumPopupMenu)
	{
		
		if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
		   return (FALSE);

		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM_SCROLL, (void**) &pMe->selectAlbumPopupMenu);
		if(result == 0) 
		{
	
			IPOPUPMENU_GetMenuModel(pMe->selectAlbumPopupMenu, &pMe->selectAlbumMenuModel);
			
			//fill popup menu from album list
			if (ReadFileStream(pMe->pIShell, ALBUM_FILE, &pszAlbumList)>0 )
			{
				pStart=pszAlbumList;

				while( NULL!=(pEnd=STRSTR(pStart,";")))
				{
					nLen = STRLEN(pStart)-STRLEN(pEnd);
					
					nSize=(nLen+1)*sizeof(AECHAR);
					pwzTemp = (AECHAR*) MALLOC( nSize );//no need to free this bcos this will be freed with menumodel release
					STRTOWSTR(pStart, pwzTemp, nSize); 

					IMENUMODEL_Add(pMe->selectAlbumMenuModel,pwzTemp,(uint16)nCount,MMF_ENABLED);

					pStart = pStart+nLen+1;
					nCount++;					
					
				}
			}
			pMe->noOfAlbums = nCount;
			FREEIF(pszAlbumList);

			IFORM_SetSoftkeys((IForm*)pMe->selectAlbumPopupMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);
			IFORM_SetSelectHandler((IForm*)pMe->selectAlbumPopupMenu, (PFNSELECT)PhotoSettings_AlbumSelectEvent, pMe);

			HANDLERDESC_Init(&pMe->selectAlbumHandler, PhotoSettings_SelectAlbumHandleEvent, pMe, 0);
			IFORM_SetHandler((IForm*)pMe->selectAlbumPopupMenu, &pMe->selectAlbumHandler);

			//wrap the pop-up form in a scrollbar decorator
			{
			
				IFORM_SetWidget((IForm*)pMe->selectAlbumPopupMenu, WID_BACKDROP, backDropWidget);
			
				IWIDGET_Release(backDropWidget);
				
			}

			
		}
	}

	if ( pMe->selectAlbumPopupMenu )
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->selectAlbumPopupMenu);

	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotSettings_InfoDlgEvent
// DESCRIPTION	: Information dilog event handler
// ================================================================================
boolean PhotSettings_InfoDlgEvent(PhotoSettings *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;
	

	if(eCode == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		//catch FID_PREFRECT and set preferred extent of menu

	}

	if ( eCode == EVT_KEY )
	{
		switch ( wParam )
		{
		case AVK_CLR:
			IROOTFORM_PopForm(pMe->rootForm);
			result = TRUE;
		break;
		case AVK_SOFT2:
			IROOTFORM_PopForm(pMe->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:
			IROOTFORM_PopForm(pMe->rootForm);
			
			break;
		}
	}

	if ( !result )
		result = HANDLERDESC_Call( &pMe->infoDlgHandler, eCode, wParam, dwParam );


	return result;
}

// ================================================================================
// FUNCTION		: PhotoSettings_InfoDialog
// DESCRIPTION	: Create information dialog
// ================================================================================
int PhotoSettings_InfoDialog(PhotoSettings* pMe)
{
	int result=0;
	int nLen=0;
	int size=0;
		
	IWidget* backWidget = NULL;
	AECHAR* pszText=NULL;
	
	if (NULL==pMe->infoDlg)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_INFODIALOG, (void**) &pMe->infoDlg);
		
		IFORM_SetResText((IForm*)pMe->infoDlg, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEVAULT);
		IFORM_SetSoftkeys((IForm*)pMe->infoDlg, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
		
		pszText = (AECHAR*)MALLOC(200);
		result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROAMING_MSG, pszText, 200);
		DBGPRINTF("%S", pszText);
		IFORM_SetText((IForm*)pMe->infoDlg, FID_TEXT, pszText); 
			
		IFORM_GetWidget((IForm*)pMe->infoDlg, WID_TITLE, &backWidget);
		IWIDGET_SetBorderWidth(backWidget, 2);
		IWIDGET_SetBorderColor(backWidget,MAKE_RGB(100, 100, 100));
		IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));

		IWIDGET_SetFlags((IWidget*)pMe->infoDlg, IDF_ALIGN_FILL);
		    
		HANDLERDESC_Init(&pMe->infoDlgHandler, PhotSettings_InfoDlgEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->infoDlg, &pMe->infoDlgHandler);
		
		FREEIF(pszText);
		IWIDGET_Release(backWidget);
	}

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->infoDlg);

	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoSettings_MsgDlgHandleEvent
// DESCRIPTION	: message dialog event handler
// ================================================================================
boolean PhotoSettings_MsgDlgHandleEvent(PhotoSettings *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;
	AEEDeviceInfo  DeviceInfo;
	AEERect rc;
	AECHAR *pszTemp=NULL;
	int nLen=0;

	if(eCode == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		//catch FID_PREFRECT and set preferred extent of menu
	
		ISHELL_GetDeviceInfo(pMe->pIShell, &DeviceInfo);

		rc.x = 0;
		rc.y = DeviceInfo.cyScreen*6/10;
		//DBGPRINTF("%",rc.y);
		rc.dx = DeviceInfo.cxScreen;//*5/6
		rc.dy = DeviceInfo.cyScreen-DeviceInfo.cyScreen*7/10;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if ( eCode == EVT_KEY )
	{
		switch ( wParam )
		{
		case AVK_CLR:
			IROOTFORM_PopForm(pMe->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT2:
			IROOTFORM_PopForm(pMe->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:		
			IROOTFORM_PopForm(pMe->rootForm);
			
			IWIDGET_GetTextWidgetText(pMe->msgWgt, &pszTemp);
			nLen = WSTRLEN(pszTemp)+1;
			WSTRTOSTR(pszTemp, pMe->userSettings->message, nLen);
			result = TRUE;
			break;

		}
	}

	if ( !result )
		result = HANDLERDESC_Call( &pMe->msgDlgHandler, eCode, wParam, dwParam );

	return result;
}

// ================================================================================
// FUNCTION		: PhotoSettings_MsgDlgHandleEvent
// DESCRIPTION	: Create message dialog
// ================================================================================
int PhotoList_MsgDlg(PhotoSettings* pMe)
{
	int result=0;

	IWidget* backdropWidget=NULL;
	IWidget* dialogContainer = NULL;
	IWidget* containerWidget = NULL;
	IWidget* titleWidget = NULL;
	AECHAR *pwsTemp=NULL;
	AECHAR *pszTitle=NULL;

	WExtent we;
	AEEDeviceInfo  deviceInfo;
    int nWdtHeight=0;
	int nWdtWidth=0;
	WidgetPos wpos;

	if( !pMe->msgDlg)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->msgDlg);

		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->msgDlgContainer);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->msgWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&titleWidget);

		IFORM_SetResText((IForm*)pMe->msgDlg, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
		IFORM_SetSoftkeys((IForm*)pMe->msgDlg, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
  
		ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
		nWdtHeight = (deviceInfo.cyScreen/12)*8;// - deviceInfo.cyScreen/3;// - space taken for title bar and menu bar
		nWdtHeight = nWdtHeight/6;
		nWdtHeight = nWdtHeight;

		nWdtWidth = (deviceInfo.cyScreen/10)*8;
		
		pszTitle = (AECHAR*)MALLOC(50);//this will free when widget release.
		result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHARE_MSG, pszTitle, 50);
		
		IFORM_SetText((IForm*)pMe->msgDlg,FID_TITLE, pszTitle);
		
		
		IWIDGET_SetBorderColor(pMe->msgWgt, MAKE_RGB(100, 100, 100));
		IWIDGET_SetBorderWidth(pMe->msgWgt, 1);
		IWIDGET_SetFlags(pMe->msgWgt, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);
		

		IFORM_SetWidget((IForm*)pMe->msgDlg, WID_BACKDROP, backdropWidget);
		IWIDGET_SetFlags((IWidget*)pMe->msgDlg, IDF_ALIGN_FILL);
		
		we.height = nWdtHeight;
		we.width = deviceInfo.cxScreen;
		IWIDGET_SetExtent(titleWidget,&we);
		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(pMe->msgWgt,&we);
		IWIDGET_SetBorderWidth(titleWidget, 1);
		IWIDGET_SetText(titleWidget, pszTitle, TRUE);
		
		wpos.x = 0;
		wpos.y = 0;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->msgDlgContainer, titleWidget, WIDGET_ZNORMAL, &wpos);

		// Adding the text field to the container
		wpos.x = 5;
		wpos.y = 25;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->msgDlgContainer, pMe->msgWgt, WIDGET_ZNORMAL, &wpos);

		// Attach the contaner to the dialog
		result = IXYCONTAINER_QueryInterface(pMe->msgDlgContainer, AEEIID_WIDGET, (void**)&containerWidget);
		if(result ==0) 
		{
			IFORM_SetWidget((IForm*)pMe->msgDlg, WID_FORM, containerWidget);
			IWIDGET_SetProperty(containerWidget, PROP_BGCOLOR, MAKE_RGB(225,225,225));
			IWIDGET_SetBorderWidth(containerWidget, 1);
			
			IWIDGET_MoveFocus(containerWidget, pMe->msgWgt);		
		}

		HANDLERDESC_Init(&pMe->msgDlgHandler, PhotoSettings_MsgDlgHandleEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->msgDlg, &pMe->msgDlgHandler);

		IWIDGET_Release(containerWidget);
		IWIDGET_Release(titleWidget);

	}

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->msgDlg);

	return TRUE;
}