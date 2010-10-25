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
 Use Case Document Name      :EC-GEN-USE-2005-07-20-Photo Backup V1.0.doc                          :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Handle photo upload, photo setting ... etc.. (source)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#include "PhotoForm.h"
#include "AEECheckWidget.h"
#include "AEEViewportWidget.h"
#include "AEEListForm.h"
#include "PhotoSettings.h"

#include "..\SyncEng\SyncEng.h"
#include "..\SyncEng\SyncEng.bid"

#define RIPPLEVAULT_RES_FILE "RippleVault.bar"

#define IMGTEXT_LEN		100

// ================================================================================
// FUNCTION		: PhotoForm_Delete
// DESCRIPTION	: Delete resources associated with Photo form.
// ================================================================================
void PhotoForm_Delete(PhotoForm* pMe) 
{
	int i=0;

	FREEIF(pMe->userSettings);

	if (NULL!=pMe->m_pSyncExt)
	{
		ISYNCENG_Release(pMe->m_pSyncExt);
		pMe->m_pSyncExt = NULL;
	}

	if ( pMe->pShareAlbumsAr )
	{
		//free text and image resources
		for (i=0;i<pMe->m_noOfAlbumSharedContacts;i++)
		{
			FREEIF(	pMe->pShareAlbumsAr[i]->pwText );
			RELEASEIF(	pMe->pShareAlbumsAr[i]->piImage );
		}

		for (i=pMe->m_noOfAlbumSharedContacts-1;i>=0;i--)
		{
			FREEIF(pMe->pShareAlbumsAr[i]);
		}
		
		FREEIF(pMe->pShareAlbumsAr);

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

	if ( pMe->PhotoSettings )
	{
		IFORM_Release(pMe->PhotoSettings);
		pMe->PhotoSettings = NULL;
	}

	if ( pMe->PhotoList )
	{
		IFORM_Release(pMe->PhotoList);
		pMe->PhotoList = NULL;
	}

	if ( pMe->pAlbumSelectForm )
	{
		IFORM_Release(pMe->pAlbumSelectForm);
		pMe->pAlbumSelectForm = NULL;
	}

	if(pMe->mainContainer)
	{
		IPROPCONTAINER_Release(pMe->mainContainer);
		pMe->mainContainer = NULL;
	}

	if (pMe->pShareAlbumMenuModel)
	{
		IMENUMODEL_Release(pMe->pShareAlbumMenuModel);
		pMe->pShareAlbumMenuModel = NULL;
	}

	if (pMe->pIConstraintContainer)
	{
		ICONSTRAINTCONTAINER_Release(pMe->pIConstraintContainer);
	}

	if (pMe->pAlbumListContainer)
	{
		IPROPCONTAINER_Release(pMe->pAlbumListContainer);
		pMe->pAlbumListContainer = NULL;
	}

	if (pMe->shareAlbumPopupMenu)
	{
		IPOPUPMENU_Release(pMe->shareAlbumPopupMenu);
		pMe->shareAlbumPopupMenu = NULL;
	}

	if (pMe->selectContactPopupMenu)
	{
		IPOPUPMENU_Release(pMe->selectContactPopupMenu);
		pMe->selectContactPopupMenu = NULL;
	}

	if(pMe->m_messageWidget)
	{
		IWIDGET_Release(pMe->m_messageWidget);
		pMe->m_messageWidget=NULL;
	}

	if(pMe->m_messageDlg)
	{
		IFORM_Release(pMe->m_messageDlg);
		pMe->m_messageDlg=NULL;
	}

	if (pMe->m_pDialogContainer)
	{
		IXYCONTAINER_Release(pMe->m_pDialogContainer);
		pMe->m_pDialogContainer = NULL;
	}

	if (pMe->m_photoShareProgressDlg)
	{
		IDIALOG_Release(pMe->m_photoShareProgressDlg);
		pMe->m_photoShareProgressDlg=NULL;
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
	if (pMe->m_pIImage6)
	{
		RELEASEIF(pMe->m_pIImage6);
		pMe->m_pIImage6 = NULL;
	}

	FREEIF(pMe->str1);
	FREEIF(pMe->str2);
	FREEIF(pMe->str3);
	FREEIF(pMe->str4);
	FREEIF(pMe->str5);
	FREEIF(pMe->str6);

	FREEIF(pMe->m_pwzDialogTitle);

	if(pMe)
		FREE(pMe);
}

// ================================================================================
// FUNCTION		: ShowSettings
// DESCRIPTION	: Show settings form
// ================================================================================
void ShowSettings(PhotoForm *pMe)
{
	if (NULL==pMe->PhotoSettings)
		PhotoSettings_New(&pMe->PhotoSettings, pMe->pIShell, pMe->rootForm);

	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->PhotoSettings);
}

// ================================================================================
// FUNCTION		: PhototForm_FreePrevForm
// DESCRIPTION	: This will free the momory taken to previous form
// ================================================================================
void PhototForm_FreePrevForm(PhotoForm *pMe)
{
	if ( pMe->PhotoSettings )
	{
		IFORM_Release(pMe->PhotoSettings);
		pMe->PhotoSettings = NULL;
	}

	if ( pMe->PhotoList )
	{
		IFORM_Release(pMe->PhotoList);
		pMe->PhotoList = NULL;
	}
}

// ================================================================================
// FUNCTION		: PhototForm_ShowSubForm
// DESCRIPTION	: Show new form
// ================================================================================
void PhototForm_ShowSubForm(PhotoForm *pMe, int nCode)
{
	switch (nCode)	//user has selected sent to vault in photo
	{
	case 1:
		PhototForm_FreePrevForm(pMe);
		if(pMe->PhotoList == NULL)
			PhotoList_New(&pMe->PhotoList, pMe->pIShell, pMe->rootForm);
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->PhotoList);
		break;

	case 3:
		PhotoForm_ShareAlbumMenu(pMe);		
		break;
	case 5:
		PhototForm_FreePrevForm(pMe);
		ShowSettings(pMe);
		break;
	}
}

// ================================================================================
// FUNCTION		: PhotMenu_EventHandler
// DESCRIPTION	: Handle events from Photo form main list selection/change
// ================================================================================
static void PhotMenu_EventHandler(PhotoForm *pMe, ModelEvent *pev) 
{

	if (EVT_MDL_FOCUS_CHANGE==pev->evCode)
	{
		pMe->nSubForm = pev->dwParam;
	}

	
	if(pev->evCode == EVT_MDL_FOCUS_SELECT) 
	{
		PhototForm_ShowSubForm(pMe, pev->dwParam);
	
	}
}

// ================================================================================
// FUNCTION		: PhotoForm_HandleEvent
// DESCRIPTION	: Handle soft key events 
// ================================================================================
static boolean PhotoForm_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   PhotoForm* pMe = (PhotoForm*) po;
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
					
					break;
			case AVK_SOFT1:
			{
				PhotoFormOptionsMenu(pMe);
				break;
			}
			}
		}break;
	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->mainFormHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PickAdapter
// DESCRIPTION	: At draw time, the value model gets set from the list model but 
//				: the length of the data is set to -1, which means, for a StaticWidget, 
//				: that the string length must be calculated with WSTRLEN(). We need to override
//				:the length
// ================================================================================
static void PickAdapter(void *pUnused, void **ppValueIn, int nLen, void **ppValueOut, int *pnLenOut)
{
   *pnLenOut = 1;  // force length to be 1, instead of calculating
}

// ================================================================================
// FUNCTION		: PhotoForm_PopulateMainContainer
// DESCRIPTION	: Create widgets and menues in main form
// ================================================================================
int PhotoForm_PopulateMainContainer(PhotoForm* pMe) 
{
	int result = 0;

	ImageStaticWidget* imageWidget = NULL;
	IMenuModel * iMenuModal = NULL;
	IWidget* wItemList =NULL;
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
	pMe->str1 = (AECHAR*) MALLOC(IMGTEXT_LEN);
	pMe->str2 = (AECHAR*) MALLOC(IMGTEXT_LEN);
	pMe->str3 = (AECHAR*) MALLOC(IMGTEXT_LEN);
	pMe->str4 = (AECHAR*) MALLOC(IMGTEXT_LEN);
	pMe->str5 = (AECHAR*) MALLOC(IMGTEXT_LEN);
	pMe->str6 = (AECHAR*) MALLOC(IMGTEXT_LEN);

	result =  ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PVIEWGAL, pMe->str1, IMGTEXT_LEN);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PSENTVAULT, pMe->str2, IMGTEXT_LEN);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PGETVAULT, pMe->str3, IMGTEXT_LEN);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PSHARE, pMe->str4, IMGTEXT_LEN);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PUNSHARE, pMe->str5, IMGTEXT_LEN);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_PSETTINGS, pMe->str6, IMGTEXT_LEN);

	//load the images
	pMe->m_pIImage1 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_PVIEWGAL);
	pMe->m_pIImage2 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_PSENTVAULT);
	pMe->m_pIImage3 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_PGETVAULT);
	pMe->m_pIImage4 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_PSHARE);
	pMe->m_pIImage5 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_PUNSHARE);
	pMe->m_pIImage6 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_SETTING);

	pMe->imageInfo1.pwText = (AECHAR*)pMe->str1;
	pMe->imageInfo1.piImage = pMe->m_pIImage1;
	pMe->imageInfo2.pwText = (AECHAR*)pMe->str2;
	pMe->imageInfo2.piImage = pMe->m_pIImage2;
	pMe->imageInfo3.pwText = (AECHAR*)pMe->str3;
	pMe->imageInfo3.piImage = pMe->m_pIImage3;
	pMe->imageInfo4.pwText = (AECHAR*)pMe->str4;
	pMe->imageInfo4.piImage = pMe->m_pIImage4;
	pMe->imageInfo5.pwText = (AECHAR*)pMe->str5;
	pMe->imageInfo5.piImage = pMe->m_pIImage5;
	pMe->imageInfo6.pwText = (AECHAR*)pMe->str6;
	pMe->imageInfo6.piImage = pMe->m_pIImage6;

	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo1,1,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo2,2,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo3,3,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo4,4,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo5,5,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo6,6,MMF_ENABLED);

	//set the vidget to act as a menu modal vidget
	IWIDGET_SetModel(wItemList, IMENUMODEL_TO_IMODEL(iMenuModal));

	//set the picklist widget properties
	IWIDGET_SetBorderWidth(wItemList, 0);
	IWIDGET_SetItemHeight(wItemList, 25);
	IWIDGET_SetItemWidth(wItemList, 15);
	IWIDGET_SetHintCols(wItemList, 5);
	IWIDGET_GetPreferredExtent(wItemList, &we);
	IWIDGET_SetExtent(wItemList, &we);

	IWIDGET_SetSelectedShadowOffset((IWidget *)imageWidget,2);
	IWIDGET_SetShadowColor((IWidget *)imageWidget,MAKE_RGB(122,122,122));
	IWIDGET_SetSelectedActiveBGColor((IWidget*)imageWidget, MAKE_RGB(128,128,255));

	IWIDGET_SetFont((IWidget *)imageWidget, piFont);
	IWIDGET_SetProperty(pScrollBar, PROP_ACTIVE_SCROLLCOLOR, MAKE_RGB(128,128,255));
	IDECORATOR_SetWidget((IDecorator*)pScrollBar, (IWidget *)wItemList);
	IDECORATOR_SetWidget((IDecorator*)wItemList, (IWidget *)imageWidget);

	// get the view model of the wItemList and register a listener 
	// in order to to pick up operator selection
	{
		IModel* pickListViewModel = NULL;
		IWIDGET_GetViewModel(wItemList, &pickListViewModel);
		IMODEL_AddListenerEx(pickListViewModel, &pMe->menuListener, (PFNLISTENER)PhotMenu_EventHandler, pMe);
		IMODEL_Release(pickListViewModel);
	}
	wp.bVisible = TRUE;
	wp.prop = 1;
	
	IPROPCONTAINER_Insert(pMe->mainContainer, pScrollBar, WIDGET_ZNORMAL, &wp);

	//set WID_FORM to the container
	result = IPROPCONTAINER_QueryInterface(pMe->mainContainer, AEEIID_WIDGET, (void**)&containerWidget);
	if(result ==0) 
	{
		IFORM_SetWidget((IForm*)pMe->photoViewForm, WID_FORM, containerWidget);
		//IWIDGET_MoveFocus(containerWidget, wItemList);
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
// FUNCTION		: PhotoForm_New
// DESCRIPTION	: Create resources for photoform form
// ================================================================================
int PhotoForm_New(IForm** ppo, IShell *piShell,IRootForm *pRootForm) 
{
	int result=0;

	//allocate space for the form
	PhotoForm *pMe = MALLOCREC(PhotoForm);
	IImage* m_pIImage;

	if(!pMe)
		return ENOMEMORY;

	//get user settings
	if ( !GetUserData(piShell, &pMe->userSettings) ) return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->photoViewForm) == 0)
	{
		*ppo = pMe->photoViewForm;
		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->photoViewForm, m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->photoViewForm, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_PHOTOS);
		result =  IFORM_SetSoftkeys((IForm*)pMe->photoViewForm, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);

		result = PhotoForm_PopulateMainContainer(pMe);	//add items to the form

		HANDLERDESC_Init(&pMe->mainFormHandler, PhotoForm_HandleEvent, pMe, PhotoForm_Delete);
		IFORM_SetHandler((IForm*)pMe->photoViewForm, &pMe->mainFormHandler);
	}
	RELEASEIF(m_pIImage);
	return result;
}

// ================================================================================
// FUNCTION		: PhotoForm_OptionSelectionHandler
// DESCRIPTION	: Option menu selection handler
// ================================================================================
static void PhotoForm_OptionSelectionHandler(PhotoForm* pMe, int idOption)
{
	IROOTFORM_PopForm(pMe->rootForm);
	IPOPUPMENU_Release(pMe->optionsMenu);
	pMe->optionsMenu = NULL;
	PhototForm_ShowSubForm(pMe, pMe->nSubForm);
}

// ================================================================================
// FUNCTION		: PhotoForm_OptionsEventHandler
// DESCRIPTION	: Option menu event handler
// ================================================================================
static boolean PhotoForm_OptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   PhotoForm* pMe = (PhotoForm*) po;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {

		//catch FID_PREFRECT and set preferred extent of menu
		AEERect rc;
		AEEDeviceInfo  DeviceInfo;
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

	if(evt==EVT_KEY)
	{
		switch (wParam)
		{
		case AVK_CLR:
		case AVK_SOFT2:
			if(pMe->optionsMenu) 
			{
				IROOTFORM_PopForm(pMe->rootForm);
				IPOPUPMENU_Release(pMe->optionsMenu);
				pMe->optionsMenu = NULL;
				return TRUE;
			}
			break;
		case AVK_SOFT1:
			IROOTFORM_PopForm(pMe->rootForm);
			IPOPUPMENU_Release(pMe->optionsMenu);
			pMe->optionsMenu = NULL;
			PhototForm_ShowSubForm(pMe, pMe->nSubForm);
			return TRUE;
		}
	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->optionsMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PhotoFormOptionsMenu
// DESCRIPTION	: Create option menu
// ================================================================================
int PhotoFormOptionsMenu(PhotoForm* pMe) 
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

	IFORM_SetSelectHandler((IForm*)pMe->optionsMenu, (PFNSELECT)PhotoForm_OptionSelectionHandler, pMe);
	HANDLERDESC_Init(&pMe->optionsMenuHandler, PhotoForm_OptionsEventHandler, pMe, 0);
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

	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->optionsMenu);
	return (TRUE);
}

// ================================================================================
// FUNCTION		: PhotoForm_ShareAlbumMenu
// DESCRIPTION	: Create Album share form
// ================================================================================
boolean PhotoForm_ShareAlbumMenu(PhotoForm* pMe)
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
	if ( !pMe->shareAlbumPopupMenu)
	{
		
		if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
		   return (FALSE);

		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM_SCROLL, (void**) &pMe->shareAlbumPopupMenu);
		if(result == 0) 
		{
	
			IPOPUPMENU_GetMenuModel(pMe->shareAlbumPopupMenu, &pMe->pShareAlbumMenuModel);
			
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

					IMENUMODEL_Add(pMe->pShareAlbumMenuModel,pwzTemp,(uint16)nCount,MMF_ENABLED);

					pStart = pStart+nLen+1;
					nCount++;					
					
				}
			}
			pMe->m_nNoOfAlbums = nCount;
			FREEIF(pszAlbumList);

			IFORM_SetSoftkeys((IForm*)pMe->shareAlbumPopupMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);
			IFORM_SetSelectHandler((IForm*)pMe->shareAlbumPopupMenu, (PFNSELECT)PhotoForm_AlbumShareSelectHandle, pMe);

			HANDLERDESC_Init(&pMe->shareAlbumHandler, PhotoForm_AlbumShareHandleEvent, pMe, 0);
			IFORM_SetHandler((IForm*)pMe->shareAlbumPopupMenu, &pMe->shareAlbumHandler);

			//wrap the pop-up form in a scrollbar decorator
			{
			
				IFORM_SetWidget((IForm*)pMe->shareAlbumPopupMenu, WID_BACKDROP, backDropWidget);
			
				IWIDGET_Release(backDropWidget);
				
			}

			
		}
	}

	if ( pMe->shareAlbumPopupMenu )
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->shareAlbumPopupMenu);

	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoForm_AlbumShareHandleEvent
// DESCRIPTION	: Event handler for album share form
// ================================================================================
boolean PhotoForm_AlbumShareHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;
	int nHeight=0;
	if(eCode == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		//catch FID_PREFRECT and set preferred extent of menu
		AEEDeviceInfo  DeviceInfo;
		AEERect rc;
		
		ISHELL_GetDeviceInfo(pThis->pIShell, &DeviceInfo);
		nHeight=pThis->m_nNoOfAlbums*(DeviceInfo.cyScreen/8);
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
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;
		case AVK_SOFT2:
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:
			PhotoForm_SelectAlbumToShare(pThis);
			break;
		}
	}

	if ( !result )
		result = HANDLERDESC_Call( &pThis->shareAlbumHandler, eCode, wParam, dwParam );

	result = TRUE;
	return result;
}

static void PhotoForm_AlbumShareSelectHandle(PhotoForm* pMe, int idOption) 
{
	pMe->m_nShareAlbum = idOption;
}

// ================================================================================
// FUNCTION		: PhotoForm_SelectAlbumToShare
// DESCRIPTION	: Select contact to share the album
// ================================================================================
boolean PhotoForm_SelectAlbumToShare(PhotoForm* pMe)
{
	int result = 0;
	IWidget *containerWidget=NULL;	
	IWidget* pScrollBar = NULL;
	IWidget *pListWgt = NULL;
	IMenuModel *pMenuModel=NULL;
	ImageStaticWidget* imageWidget = NULL;
	AECHAR *pszContactsAr=NULL;
	ISyncEngExt* pSyncExt;
	AECHAR *pStart=NULL;
	AECHAR *pEnd=NULL;
	AECHAR *pPhoneNo=NULL;
	int nLen=0;
	int nSize=0;

	WidgetProp wp;
	WExtent we;
	AECHAR *pwsTemp=NULL;
	int nScreenWidth=0;
	int i=0;

	if ( NULL!=pMe->pAlbumSelectForm )
	{
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->pAlbumSelectForm);
		return TRUE;
	}

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->pAlbumSelectForm) == 0)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->pAlbumListContainer);
		result += IFORM_SetResText((IForm*)pMe->pAlbumSelectForm, FID_TITLE, RIPPLEVAULT_RES_FILE, MAINFORMTITLE);
		result += IFORM_SetSoftkeys((IForm*)pMe->pAlbumSelectForm, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);

		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**)&pScrollBar);
		
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&pListWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MENUMODEL, (void**)&pMenuModel);
				
		//get contacts from addressbook
		//create sync module
	
		if ( SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pSyncExt))
			goto error;
		
		pMe->m_noOfAlbumSharedContacts = ISYNCENG_GetContacts(pSyncExt, &pszContactsAr);
		DBGPRINTF("%S", pszContactsAr);
		
		///////////add albums to from///////////
		pMe->pTmpContAr = (ContactShare**)MALLOC(pMe->m_noOfAlbumSharedContacts*sizeof(ContactShare*));
		if ( NULL==pMe->pTmpContAr ) goto error;

		//add contacts to list
		pStart=pszContactsAr;
		while( NULL!=(pEnd=WSTRCHR(pStart,';')))
		{
			nLen = WSTRLEN(pStart)-WSTRLEN(pEnd);
			
			nSize=(nLen+1)*sizeof(AECHAR);
			
			pwsTemp = (AECHAR*) MALLOC(nSize);//this memory will release with the pTmpContAr release
			pMe->pTmpContAr[i] = (ContactShare*) MALLOC(sizeof(ContactShare));
			if ( NULL==pMe->pTmpContAr[i] || NULL==pwsTemp )
				goto error;

			WSTRLCPY(pwsTemp, pStart, nLen+1);
			
			//extract the phone number
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
		IPROPCONTAINER_Insert(pMe->pAlbumListContainer, pScrollBar, WIDGET_ZNORMAL, &wp);

		// in order to to pick up operator selection
		{
			IModel* pickListViewModel = NULL;
			IWIDGET_GetViewModel(pListWgt, &pickListViewModel);
			IMODEL_AddListenerEx(pickListViewModel, &pMe->AlbumSelectMenuListener, (PFNLISTENER)PhotForm_AlbumSelectEH, pMe);
			IMODEL_Release(pickListViewModel);
		}

		//set WID_FORM to the container
		result = IPROPCONTAINER_QueryInterface(pMe->pAlbumListContainer, AEEIID_WIDGET, (void**)&containerWidget);
		if(result ==0) 
		{
			IFORM_SetWidget((IForm*)pMe->pAlbumSelectForm, WID_FORM, containerWidget);
			//IWIDGET_MoveFocus(containerWidget, wItemList);
		}
		////////////////////////////////////////

		HANDLERDESC_Init(&pMe->selectAlbumHandler, PhotoForm_SelectAlbumHandleEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->pAlbumSelectForm, &pMe->selectAlbumHandler);

		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->pAlbumSelectForm);

		IWIDGET_Release((IWidget*)imageWidget);
		
	}
	
error:
	//release module.
	ISYNCENG_Release(pSyncExt);
	FREEIF(pszContactsAr);
	IWIDGET_Release(containerWidget);
	IWIDGET_Release(pScrollBar);

	IWIDGET_Release(pListWgt);
	
	IMENUMODEL_Release(pMenuModel);
	
	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoForm_SelectAlbumHandleEvent
// DESCRIPTION	: Event handler for contact selection for share album
// ================================================================================
boolean PhotoForm_SelectAlbumHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;
	IWidget *containerWidget=NULL;

	if ( eCode == EVT_KEY )
	{
		switch ( wParam )
		{
		case AVK_CLR:
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT2:
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:		
			PhotoForm_ShowSelectContactPopupMenu(pThis);
			break;

		}
	}

	if ( !result )
		result = HANDLERDESC_Call( &pThis->selectAlbumHandler, eCode, wParam, dwParam );

	result = TRUE;
	
	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoForm_SelectAlbumHandleEvent
// DESCRIPTION	: Event handler for contact selection for share album
// ================================================================================
static void PhotForm_AlbumSelectEH(PhotoForm *pMe, ModelEvent *pev) 
{
	int nPos=0;
	if(pev->evCode == EVT_MDL_FOCUS_SELECT) 
	{
		//toggle the checkbox image
		nPos = pev->dwParam;
		pMe->pTmpContAr[nPos]->status = !pMe->pTmpContAr[nPos]->status;
		
		//release previous image
		RELEASEIF(pMe->pTmpContAr[nPos]->imageInfo.piImage);

		if ( pMe->pTmpContAr[nPos]->status )
			pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_SELECT);
		else
			pMe->pTmpContAr[nPos]->imageInfo.piImage = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CB_UNSELECT);

		IROOTFORM_Activate(pMe->rootForm);
	}

	
}

// ================================================================================
// FUNCTION		: PhotoForm_ShowSelectContactPopupMenu
// DESCRIPTION	: Select contact form popup menu
// ================================================================================
boolean PhotoForm_ShowSelectContactPopupMenu(PhotoForm* pMe)
{

	int result = 0;
	IWidget* backDropWidget;
	IMenuModel *pMenuModel=NULL;
	AECHAR* pwzTemp=NULL; 
	//release if any popup menu is created
	if ( !pMe->selectContactPopupMenu)
	{
		
		if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
		   return (FALSE);

		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->selectContactPopupMenu);
		if(result == 0) 
		{
	
			IPOPUPMENU_GetMenuModel(pMe->selectContactPopupMenu, &pMenuModel);
		
			pwzTemp = (AECHAR*)MALLOC(40);
			result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ADDSHARE, pwzTemp, 40);
			IMENUMODEL_Add(pMenuModel,pwzTemp,(uint16)0,MMF_ENABLED);
			pwzTemp = (AECHAR*)MALLOC(20);
			result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_CANCEL, pwzTemp, 20);
			IMENUMODEL_Add(pMenuModel,pwzTemp,(uint16)1,MMF_ENABLED);

			IFORM_SetSoftkeys((IForm*)pMe->selectContactPopupMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);
			IFORM_SetSelectHandler((IForm*)pMe->selectContactPopupMenu, (PFNSELECT)PhotoForm_SelectContactHandleEvent, pMe);

			HANDLERDESC_Init(&pMe->selectContactHandler, PhotoForm_SelectContactHandleEvent, pMe, 0);
			IFORM_SetHandler((IForm*)pMe->selectContactPopupMenu, &pMe->selectContactHandler);

			//wrap the pop-up form in a scrollbar decorator
			{	
				IFORM_SetWidget((IForm*)pMe->selectContactPopupMenu, WID_BACKDROP, backDropWidget);
				IWIDGET_Release(backDropWidget);	
			}

			
		}

		IMENUMODEL_Release(pMenuModel);
	}

	if ( pMe->selectContactPopupMenu )
		IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->selectContactPopupMenu);

	
	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoForm_SelectContactHandleEvent
// DESCRIPTION	: select form popup menu event handler
// ================================================================================
boolean PhotoForm_SelectContactHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;
	
	if(eCode == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		//catch FID_PREFRECT and set preferred extent of menu
		AEEDeviceInfo  DeviceInfo;
		AEERect rc;

		ISHELL_GetDeviceInfo(pThis->pIShell, &DeviceInfo);
		
		
		rc.x = 0;
		rc.y = DeviceInfo.cyScreen-60;
		rc.dx = DeviceInfo.cxScreen;
		rc.dy = 40;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if ( eCode == EVT_KEY )
	{
		switch ( wParam )
		{
		case AVK_CLR:
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT2:
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:		
			IROOTFORM_PopForm(pThis->rootForm);
			PhotoForm_GetMessageDlg(pThis);
			break;

		}
	}

	if ( !result )
		result = HANDLERDESC_Call( &pThis->selectContactHandler, eCode, wParam, dwParam );

	result = TRUE;
	
	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoForm_GetMessageDlg
// DESCRIPTION	: Information message box
// ================================================================================
int PhotoForm_GetMessageDlg(PhotoForm* pMe)
{
	int result=0;

	IWidget* backdropWidget=NULL;
	IWidget* dialogContainer = NULL;
	IWidget* containerWidget = NULL;
	IWidget* titleWidget = NULL;
	AECHAR *pwsTemp=NULL;

	WExtent we;
	AEEDeviceInfo  deviceInfo;
    int nWdtHeight=0;
	int nWdtWidth=0;
	WidgetPos wpos;

	if( !pMe->m_messageDlg)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->m_messageDlg);

		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->m_pDialogContainer);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->m_messageWidget);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&titleWidget);

		IFORM_SetResText((IForm*)pMe->m_messageDlg, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
		IFORM_SetSoftkeys((IForm*)pMe->m_messageDlg, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
  
		ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
		nWdtHeight = (deviceInfo.cyScreen/12)*8;
		nWdtHeight = nWdtHeight/6;
		nWdtHeight = nWdtHeight;

		nWdtWidth = (deviceInfo.cyScreen/10)*8;
		FREEIF(pMe->m_pwzDialogTitle);
		pMe->m_pwzDialogTitle = (AECHAR*)MALLOC(50);
		result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHARE_MSG, pMe->m_pwzDialogTitle, 50);
		
		IFORM_SetText((IForm*)pMe->m_messageDlg,FID_TITLE, pMe->m_pwzDialogTitle);
		DBGPRINTF("%S", pMe->m_pwzDialogTitle);
		
		IWIDGET_SetBorderColor(pMe->m_messageWidget, MAKE_RGB(100, 100, 100));
		IWIDGET_SetBorderWidth(pMe->m_messageWidget, 1);
		IWIDGET_SetFlags(pMe->m_messageWidget, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);
		
		IFORM_SetWidget((IForm*)pMe->m_messageDlg, WID_BACKDROP, backdropWidget);
		IWIDGET_SetFlags((IWidget*)pMe->m_messageDlg, IDF_ALIGN_FILL);
		
		we.height = nWdtHeight;
		we.width = deviceInfo.cxScreen;
		IWIDGET_SetExtent(titleWidget,&we);
		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(pMe->m_messageWidget,&we);
		IWIDGET_SetBorderWidth(titleWidget, 1);
		IWIDGET_SetText(titleWidget, pMe->m_pwzDialogTitle, FALSE);
		
		wpos.x = 0;
		wpos.y = 0;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->m_pDialogContainer, titleWidget, WIDGET_ZNORMAL, &wpos);

		// Adding the text field to the container
		wpos.x = 5;
		wpos.y = 25;
		wpos.bVisible = TRUE;
		IXYCONTAINER_Insert(pMe->m_pDialogContainer, pMe->m_messageWidget, WIDGET_ZNORMAL, &wpos);

		// Attach the contaner to the dialog
		result = IXYCONTAINER_QueryInterface(pMe->m_pDialogContainer, AEEIID_WIDGET, (void**)&containerWidget);
		if(result ==0) 
		{
			IFORM_SetWidget((IForm*)pMe->m_messageDlg, WID_FORM, containerWidget);
			IWIDGET_SetProperty(containerWidget, PROP_BGCOLOR, MAKE_RGB(225,225,225));
			IWIDGET_SetBorderWidth(containerWidget, 1);
			
			IWIDGET_MoveFocus(containerWidget, pMe->m_messageWidget);		
		}

		HANDLERDESC_Init(&pMe->messageDlgHandler, PhotoForm_MessageDlgHandleEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->m_messageDlg, &pMe->messageDlgHandler);

		IWIDGET_Release(containerWidget);
		IWIDGET_Release(titleWidget);

	}

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->m_messageDlg);

	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoForm_MessageDlgHandleEvent
// DESCRIPTION	: Dialogbox message handler
// ================================================================================
boolean PhotoForm_MessageDlgHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;
	AEEDeviceInfo  DeviceInfo;
	AEERect rc;
	
	if(eCode == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		//catch FID_PREFRECT and set preferred extent of menu
	
		ISHELL_GetDeviceInfo(pThis->pIShell, &DeviceInfo);

		rc.x = 0;
		rc.y = DeviceInfo.cyScreen*6/10;
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
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT2:
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:		
			IROOTFORM_PopForm(pThis->rootForm);
			PhotoForm_SendAlbumToServer(pThis);
			result = TRUE;
			break;

		}
	}

	if ( !result )
		result = HANDLERDESC_Call( &pThis->messageDlgHandler, eCode, wParam, dwParam );

	return result;
}

// ================================================================================
// FUNCTION		: PhotoForm_SendAlbumToServer
// DESCRIPTION	: Synchronize albums to server
// ================================================================================
boolean PhotoForm_SendAlbumToServer(PhotoForm* pMe)
{
	AECHAR *pwzText=NULL;
	AECHAR *pwzContacts=NULL;
	AECHAR *pwzAlbum=NULL;
	uint16 nFlags=0;
	uint16 nId=0;
	int i=0;
	int nLen=0;
	
	IWIDGET_GetTextWidgetText(pMe->m_messageWidget, &pwzText);
	
	if (0==WSTRLEN(pwzText)) goto error;

	IMENUMODEL_GetAt(pMe->pShareAlbumMenuModel, (uint16)pMe->m_nShareAlbum, MENUITEM_ALL, &pwzAlbum, &nFlags, &nId);
	
	if (0==WSTRLEN(pwzAlbum)) goto error;
	
	
	//get the selected contacts list
	if (pMe->m_noOfAlbumSharedContacts)
	{
		pwzContacts = (AECHAR*)MALLOC(pMe->m_noOfAlbumSharedContacts*sizeof(AECHAR)*100);//assume the length of a contact name is ~50
		if (NULL==pwzContacts) goto error;

		for (i=0;i<pMe->m_noOfAlbumSharedContacts;i++)
		{
			if ( pMe->pTmpContAr[i]->status )
			{
				WSTRCAT(pwzContacts, pMe->pTmpContAr[i]->imageInfo.pwText);
				if (pMe->pTmpContAr[i]->pwzPhoneNo)
				{
					WSTRCAT(pwzContacts, L":");
					WSTRCAT(pwzContacts, pMe->pTmpContAr[i]->pwzPhoneNo);
				}
				WSTRCAT(pwzContacts, L";");
			}
		}
		
		if (WSTRLEN(pwzContacts))
		{
			if (NULL==pMe->m_pSyncExt)
				if ( SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pMe->m_pSyncExt))
					goto error;
			
			ISYNCENG_ShareAlbum(pMe->m_pSyncExt, pwzAlbum, pwzContacts, pwzText);
		}

	}

	PhotoForm_CreateProgressDialog(pMe);
	
error:
	
	FREEIF(pwzContacts);

	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoForm_CreateProgressDialog
// DESCRIPTION	: Create progressbar dialog
// ================================================================================
int PhotoForm_CreateProgressDialog(PhotoForm* pMe)
{
	int result=0;

	IWidget* backdropWidget;

	IWidget* backWidget = NULL;

	if (NULL==pMe->m_photoShareProgressDlg)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROGRESSDIALOG, (void**) &pMe->m_photoShareProgressDlg);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);

		IFORM_SetResText((IForm*)pMe->m_photoShareProgressDlg, FID_TEXT, RIPPLEVAULT_RES_FILE, MAINFORMTITLE);	

		IFORM_GetWidget((IForm*)pMe->m_photoShareProgressDlg, WID_PROGRESS, &backWidget);
		IWIDGET_SetFGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->m_photoShareProgressDlg, WID_PROGRESS, backWidget);
		IWIDGET_Release(backWidget);

		IFORM_GetWidget((IForm*)pMe->m_photoShareProgressDlg, WID_BACKGROUND, &backWidget);
		IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
		IWIDGET_SetBorderWidth(backWidget, 1);
		IFORM_SetWidget((IForm*)pMe->m_photoShareProgressDlg, WID_BACKGROUND, backWidget);
		
		IFORM_SetWidget((IForm*)pMe->m_photoShareProgressDlg, WID_BACKDROP, backdropWidget);
		IWIDGET_SetFlags((IWidget*)pMe->m_photoShareProgressDlg, IDF_ALIGN_LEFT);

		HANDLERDESC_Init(&pMe->progressDlgHandler, PhotoForm_ProgressDlgHandleEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->m_photoShareProgressDlg, &pMe->progressDlgHandler);

		IFORM_SetProgress((IForm*)pMe->m_photoShareProgressDlg,-1);

		IWIDGET_Release(backdropWidget);
		IWIDGET_Release(backWidget);
	}
	
	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->m_photoShareProgressDlg);

	
	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoForm_ProgressDlgHandleEvent
// DESCRIPTION	: Event handler for progressbar dialog
// ================================================================================
boolean PhotoForm_ProgressDlgHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;

	if ( eCode == EVT_KEY )
	{
		switch ( wParam )
		{
		case AVK_CLR:
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT2:
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:		
			IROOTFORM_PopForm(pThis->rootForm);
			result = TRUE;
			break;

		}
	}
	else if (eCode == EVT_USER_ALBUM_SHARE)
	{
		if (NULL!=pThis->m_pSyncExt)
		{
			ISYNCENG_Release(pThis->m_pSyncExt);
			pThis->m_pSyncExt = NULL;
		}
		IROOTFORM_PopForm(pThis->rootForm);
		result = TRUE;
	}

	if ( !result )
		result = HANDLERDESC_Call( &pThis->progressDlgHandler, eCode, wParam, dwParam );

	return result;
}