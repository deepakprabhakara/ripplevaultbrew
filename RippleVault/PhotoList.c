
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
 Module Name				: MessageForm.c
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : LKA,PDP.SRI
 Date Created               : 20-08-2005
 Rational Rose Model Name   :                          
 Use Case Document Name     :                          
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :
 
 Recommendations                    :
 

***************************************************************/
#include "PhotoList.h"
#include "../SyncEng/synceng.bid"
#include "../SyncEng/SMEvents.h"
#include "AEEFile.h"
#include "UserData.h"

#define RIPPLEVAULT_RES_FILE "RippleVault.bar"

// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THIS MODULE FROM HERE
// ================================================================================

void PhotoList_Delete(PhotoList* pMe) 
{
	
	int i;
	if (pMe->userSettings)
	{
		FREEIF(pMe->userSettings);
		pMe->userSettings=NULL;
	}

	if ( pMe->photoUploadMsgDlg)
	{
		IFORM_Release(pMe->photoUploadMsgDlg);
		pMe->photoUploadMsgDlg=NULL;
	}
	if ( pMe->msgDlgContainer )
	{
		IXYCONTAINER_Release(pMe->msgDlgContainer);
		pMe->msgDlgContainer = NULL;
	}
	
	if (pMe->uploadMsgWgt)
	{
		IWIDGET_Release(pMe->uploadMsgWgt);
		pMe->uploadMsgWgt = NULL;
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
	if(pMe->optionsMenu)
	{
		IPOPUPMENU_Release(pMe->optionsMenu);
		pMe->optionsMenu = NULL;
	}

	if(pMe->albumCreateContainer)
	{
		IXYCONTAINER_Release(pMe->albumCreateContainer);
		pMe->albumCreateContainer=NULL;
	}

	if(pMe->AlbumCreateTextWidget)
	{
		IWIDGET_Release(pMe->AlbumCreateTextWidget);
		pMe->AlbumCreateTextWidget=NULL;
	}
	if(pMe->albumMenu)
	{
		IPOPUPMENU_Release(pMe->albumMenu);
		pMe->albumMenu = NULL;
	}

	FREEIF(pMe->album);

	if(pMe->confirmDialog)
	{
		IDIALOG_Release(pMe->confirmDialog);
		pMe->confirmDialog=NULL;
	}

	if(pMe->unSelectDialog)
	{
		IDIALOG_Release(pMe->unSelectDialog);
		pMe->unSelectDialog=NULL;
	}

	if(pMe->newAlbumWidget)
	{
		IWIDGET_Release(pMe->newAlbumWidget);
		pMe->newAlbumWidget=NULL;
	}
	if(pMe->progressDialog)
	{
		IDIALOG_Release(pMe->progressDialog);
		pMe->progressDialog=NULL;
	}

	if(pMe->createNewAlbumDialog)
	{
		IDIALOG_Release(pMe->createNewAlbumDialog);
		pMe->createNewAlbumDialog=NULL;
	}

	if(pMe->newAlbumCreateDialog)
	{
		IFORM_Release(pMe->newAlbumCreateDialog);
		pMe->newAlbumCreateDialog=NULL;
	}
	
	if(pMe->adstr!=NULL)
		FREEIF(pMe->adstr);

	for (i = 0;i<pMe->totalItems;i++)
	{
		FREEIF(pMe->strAr[i]);
	}
	FREEIF(pMe->strAr);

	if (NULL!=pMe->m_pSyncExt) 
	{
		ISYNCENG_Release(pMe->m_pSyncExt);
		pMe->m_pSyncExt=NULL;
	}

	FREEIF(pMe->imageInfoAr);
	FREEIF(pMe->str2);
	FREEIF(pMe->str3);
	FREEIF(pMe->newAlbumName);
	FREEIF(pMe->pszPhotoUploadMsg);
	if(pMe)
		FREE(pMe);
}

/// ================================================================================
// FUNCTION		: MenuModal_EventHandler
// DESCRIPTION	: LISTEN FOR CHANGE ON PHOTOLIST MAIN FORM
// ================================================================================

static void MenuModal_EventHandler(PhotoList *pMe, ModelEvent *pev) 
{

	if(pev->evCode == EVT_MDL_FOCUS_SELECT) 
		if(pev->dwParam == 0)	//user has selected contacts
		{

		}
	if (pev->evCode == EVT_MDL_FOCUS_CHANGE)
		pMe->selectedItem = (int)pev->dwParam;

}


// ================================================================================
// FUNCTION		: PhotoList_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM PHOTOLIST MAIN FORM
// ================================================================================

static boolean PhotoList_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   PhotoList* pMe = (PhotoList*) po;
    switch (evt) 
	{
		case EVT_KEY:
			{
				switch (wParam) 
				{
				case AVK_SOFT2:
						IROOTFORM_PopForm(pMe->rootForm);
						if(pMe->optionsMenu)
						{
							IPOPUPMENU_Release(pMe->optionsMenu);
							pMe->optionsMenu = NULL;
						}
						return (TRUE);
						break;
				
				case AVK_SOFT1:
						PhotoListOptionsMenu(pMe);
						return (TRUE);
						break;

				case AVK_CLR:
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

		case EVT_USER_PHOTOUPLOAD:
			
			break;
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
// FUNCTION		: PhotoList_PopulateMainContainer
// DESCRIPTION	: POPULATE PHOTOLIST MAIN FORM
// ================================================================================

int PhotoList_PopulateMainContainer(PhotoList* pMe) 
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
	AECHAR* filename = NULL;
	char* pszTmp = NULL;
	int count;
	FileInfo* pFileInfo = NULL;
	IFileMgr* pFileMgr = NULL;
	char* pzFileName = NULL;
	AECHAR* psFileName = NULL;
	AECHAR format[]={'%','s',' ','\0'};
	
	pMe->totalItems = 8;
	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_IMAGESTATICWIDGET, (void**)&imageWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MENUMODEL, (void**)&iMenuModal);
  	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&wItemList);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**)&pScrollBar);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FILEMGR, (void**)(&pFileMgr));

	pMe->imageInfoAr = (ImageStaticInfo*) MALLOC(pMe->totalItems*sizeof(ImageStaticInfo));
	pMe->strAr = (AECHAR**) MALLOC(pMe->totalItems*sizeof(AECHAR*));

	pzFileName = (char*) MALLOC( MAX_FILE_NAME * sizeof( char ) );
	psFileName = (AECHAR*) MALLOC( MAX_FILE_NAME * sizeof( AECHAR ) );
	IFILEMGR_EnumInit( pFileMgr, "", FALSE );
	
	// Get each file name in the aplication's root directory.
	pFileInfo = (FileInfo*) MALLOC( sizeof( FileInfo ) );
	if(!pFileInfo)
		return FALSE;

	pMe->m_pIImage1 = ISHELL_LoadResImage(pMe->pIShell,RIPPLEVAULT_RES_FILE,IDI_CVIEW);
	count = 0;
	while( IFILEMGR_EnumNext( pFileMgr, pFileInfo ) )
	{
		if(STRENDS( BMP_EXTENSION, pFileInfo->szName)|| STRENDS( JPG_EXTENSION, pFileInfo->szName))
		{
			//this will load the file to the image
			STRCPY( pzFileName, pFileInfo->szName );
			( STRRCHR( pzFileName, '.' ) )[ 0 ] = '\0';
			STRTOWSTR( pzFileName, psFileName, MAX_FILE_NAME );
			WSTRUPPER( psFileName );

			pMe->strAr[count] = (AECHAR*) MALLOC(30);

			WSPRINTF(pMe->strAr[count], 30, format,psFileName);//, count);
			pMe->imageInfoAr[count].pwText = (AECHAR*)pMe->strAr[count];
			pMe->imageInfoAr[count].piImage = pMe->m_pIImage1;
			IMENUMODEL_Add(iMenuModal,&pMe->imageInfoAr[count],(uint16)count,MMF_ENABLED);
			count += 1;
		}
	}
	pMe->totalItems = count;

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
	IWIDGET_SetProperty(pScrollBar, PROP_SCROLLPAD, 0);
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
		IFORM_SetWidget((IForm*)pMe->PhotoList, WID_FORM, containerWidget);

	}

	IWIDGET_Release((IWidget*)imageWidget);
	IMENUMODEL_Release(iMenuModal);
	IWIDGET_Release(wItemList);
	IWIDGET_Release(pScrollBar);
	if (containerWidget)
		IWIDGET_Release(containerWidget);
	RELEASEIF(piFont);
	FREEIF(filename);
	FREEIF(pszTmp);


	IFILEMGR_Release( pFileMgr );
	FREEIF( pFileInfo );
	FREEIF( pzFileName );
	FREEIF( psFileName );

	if(result != 0)
		return EFAILED;

	return result;
}


// ================================================================================
// FUNCTION		: PhotoList_New
// DESCRIPTION	: INITIATE PHOTOLIST MAIN FORM
// ================================================================================

int PhotoList_New(IForm** ppo, IShell *piShell,IRootForm *pRootForm) 
{
	int result=0;
	//allocate space for the form
	PhotoList *pMe = MALLOCREC(PhotoList);
	IImage* m_pIImage;
	if(!pMe)
		return ENOMEMORY;
	if ( !GetUserData(piShell, &pMe->userSettings )) return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;
	pMe->itemCount = 0;
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->PhotoList) == 0)
	{
		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->mainContainer);
		*ppo = pMe->PhotoList;
		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->PhotoList,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->PhotoList, FID_TITLE, RIPPLEVAULT_RES_FILE, MAINFORMTITLE);

		result =  IFORM_SetSoftkeys((IForm*)pMe->PhotoList, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);
		result = PhotoList_PopulateMainContainer(pMe);	//add items to the form

		HANDLERDESC_Init(&pMe->mainFormHandler, PhotoList_HandleEvent, pMe, PhotoList_Delete);
		IFORM_SetHandler((IForm*)pMe->PhotoList, &pMe->mainFormHandler);

	}

	RELEASEIF(m_pIImage);
	pMe->progressDialog = NULL;
	pMe->m_pSyncExt = NULL;
	pMe->m_nCurrentUploadPhotoNo = 0;
	return result;
}

// ================================================================================
// FUNCTION		: PhotoList_OptionSelectionHandler
// DESCRIPTION	: HANDLE OPTION FROM PhotoListOptionsMenu
// ================================================================================

static void PhotoList_OptionSelectionHandler(PhotoList* pMe, int idOption) 
{
	//	dismiss the pop up menu
	IROOTFORM_PopForm(pMe->rootForm);

	switch(idOption)
	{
	case 0:
		SelectListItems(pMe,idOption);
		break;
	case 1:
		SelectListItems(pMe,idOption);
		break;
	case 2:
		SelectListItems(pMe,idOption);
		break;
	case 3:
		if(pMe->itemCount<1)
		{
			PhotoList_UnSelectInformDialog(pMe);
			break;
		}
		PhotoList_CreateConfirmDialog(pMe);
		break;
	}
	//release photo album list popup menu
	if ( pMe->optionsMenu )
	{
		IPOPUPMENU_Release(pMe->optionsMenu);
		pMe->optionsMenu = NULL;
	}
}


// ================================================================================
// FUNCTION		: PhotoList_OptionsEventHandler
// DESCRIPTION	: HANDLE EVENT FROM PhotoListOptionsMenu
// ================================================================================

static boolean PhotoList_OptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   PhotoList* pMe = (PhotoList*) po;

   
   
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		
		//catch FID_PREFRECT and set preferred extent of menu
		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen/2;
		rc.dx = pMe->DeviceInfo.cyScreen*5/6;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*3/5;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
		if(pMe->optionsMenu) 
		{
			IROOTFORM_PopForm(pMe->rootForm);
			if ( pMe->optionsMenu )
			{
				IPOPUPMENU_Release(pMe->optionsMenu);
				pMe->optionsMenu = NULL;
			}
			return TRUE;
		}
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
	
		IROOTFORM_PopForm(pMe->rootForm);
		if ( pMe->optionsMenu )
		{
			IPOPUPMENU_Release(pMe->optionsMenu);
			pMe->optionsMenu = NULL;
		}
        return TRUE;
	}
		

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->optionsMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PhotoListOptionsMenu
// DESCRIPTION	: POPULATE OPTION MENU FOR PHOTOLIST 
// ================================================================================

int PhotoListOptionsMenu(PhotoList* pMe) 
{
	int result = 0;
	IWidget* backDropWidget;
   
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
	   return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->optionsMenu) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_POPSELECT, 0,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_POPSELECTALL, 1,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_POPUNSELALL, 2,MMF_ENABLED) != 0)
	   return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_PSENTVAULT, 3,MMF_ENABLED) != 0)
	   return (FALSE);

	IFORM_SetSoftkeys((IForm*)pMe->optionsMenu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);

	IFORM_SetSelectHandler((IForm*)pMe->optionsMenu, (PFNSELECT)PhotoList_OptionSelectionHandler, pMe);

	HANDLERDESC_Init(&pMe->optionsMenuHandler, PhotoList_OptionsEventHandler, pMe, NULL);

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
// FUNCTION		: SelectListItems
// DESCRIPTION	: HANDLE ITEM SELECTION OF PHOTO
// ================================================================================

int SelectListItems(PhotoList* pMe,int iIndex)
{
	char *pszTmp=NULL; 
	AECHAR format[]={'%','s',' ','\0'};
	int result,i;
	
	switch (iIndex)
	{
	case 0:
		{
			pszTmp = (char*)MALLOC(30+1);
			if ( NULL==pszTmp )
				return TRUE;
			WSTRTOSTR(pMe->strAr[pMe->selectedItem], pszTmp, 30);
			if (STRSTR(pszTmp,&" *") == NULL)
			{
				pMe->adstr = (AECHAR*) MALLOC(6);
				result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIGHTSIGN, pMe->adstr, 6);
				WSTRCAT(pMe->strAr[pMe->selectedItem],pMe->adstr);
				pMe->itemCount +=1;
			}
			else
			{
				pMe->adstr = (AECHAR*) MALLOC(30);			                 
				WSTRNCOPYN(pMe->strAr[pMe->selectedItem], WSTRLEN(pMe->strAr[pMe->selectedItem]), pMe->strAr[pMe->selectedItem], WSTRLEN(pMe->strAr[pMe->selectedItem])-2);
				WSTRCPY(pMe->adstr,pMe->strAr[pMe->selectedItem]);

				if (pMe->selectedItem > 0)
					WSPRINTF(pMe->strAr[pMe->selectedItem], 30, format,pMe->adstr);//, pMe->selectedItem);
				else
					WSTRCPY(pMe->strAr[pMe->selectedItem],pMe->adstr);
				pMe->itemCount -=1;
			}


			break;
		}
	case 1:
		pszTmp = (char*)MALLOC(30+1);
		pMe->adstr = (AECHAR*) MALLOC(6);
		for(i =0;i<pMe->totalItems;i++)
		{
			if ( NULL==pszTmp )
				return TRUE;
			WSTRTOSTR(pMe->strAr[i], pszTmp, 30);

			if (STRSTR(pszTmp," *") == NULL)
			{
				result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIGHTSIGN, pMe->adstr, 6);
				WSTRCAT(pMe->strAr[i],pMe->adstr);
				pMe->itemCount +=1;
			}
		}
		break;

	case 2:
		pszTmp = (char*)MALLOC(30+1);

		for(i =0;i<pMe->totalItems;i++)
		{
			if ( NULL==pszTmp )
				return TRUE;
			WSTRTOSTR(pMe->strAr[i], pszTmp, 30);

			if (STRSTR(pszTmp," *") != NULL)
			{
				STRSTR(pszTmp,&" *")[ 0 ] = '\0';
				STRTOWSTR(pszTmp,pMe->strAr[i],30);
				pMe->itemCount -=1;
				
			}
		}
		break;
	}

	FREEIF(pMe->adstr);
	FREEIF(pszTmp);
	
	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoList_UploadPhoto
// DESCRIPTION	: UPLOAD PHOTO
// ================================================================================

static void PhotoList_UploadPhoto(PhotoList* pMe)
{
	char *pszPhotoName=NULL;
	char *pSelected=NULL;
	int nLen=0;

	if ( pMe->m_nCurrentUploadPhotoNo>=pMe->totalItems ) 
	{
		ISYNCENG_SyncAlbum(pMe->m_pSyncExt);
		return;
	}

	nLen = WSTRLEN(pMe->strAr[pMe->m_nCurrentUploadPhotoNo])+1;
	if ( NULL==(pszPhotoName=(char*)MALLOC(nLen+5)) ) return; 
	WSTRTOSTR(pMe->strAr[pMe->m_nCurrentUploadPhotoNo], pszPhotoName, nLen+1);
	
	//get the next selected item
	while ( NULL == (pSelected=STRSTR(pszPhotoName, "  *")))
	{
		FREEIF(pszPhotoName);
		if ( pMe->m_nCurrentUploadPhotoNo==pMe->totalItems ) 
		{
			//synchronise server albums
			ISYNCENG_SyncAlbum(pMe->m_pSyncExt);
			return;
		}
		nLen = WSTRLEN(pMe->strAr[pMe->m_nCurrentUploadPhotoNo])+1;
		if ( NULL==(pszPhotoName=(char*)MALLOC(nLen+5)) ) return; //room for image extension
		WSTRTOSTR(pMe->strAr[pMe->m_nCurrentUploadPhotoNo], pszPhotoName, nLen+1);
		pMe->m_nCurrentUploadPhotoNo++;
	}

	if (NULL!=pSelected)
	{
		pSelected[0]='\0';
	}
	pMe->m_nCurrentUploadPhotoNo++;
	STRCAT(pszPhotoName, ".jpg");
	
	ISYNCENG_UploadPhoto(pMe->m_pSyncExt, pszPhotoName, pMe->newAlbumName, pMe->pszPhotoUploadMsg);

	FREEIF(pszPhotoName);
}

// ================================================================================
// FUNCTION		: PhotoList_StartUpload
// DESCRIPTION	: START THE UPLOAD PROCESS FROM HERE
// ================================================================================

boolean PhotoList_StartUpload(PhotoList *pMe)
{
	IROOTFORM_PopForm(pMe->rootForm);

	if ( NULL==pMe->m_pSyncExt )
	{

		if (SUCCESS==ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pMe->m_pSyncExt))
		{
			pMe->m_nCurrentUploadPhotoNo = 0;
			PhotoList_CreateProgressDialog(pMe, IDS_SYNC);
			PhotoList_UploadPhoto(pMe);
			return TRUE;
		}
	}

	return FALSE;
}

// ================================================================================
// FUNCTION		: PhotoList_GetUploadMessage
// DESCRIPTION	: RETREIVE UPLOADED MESSAGE FROM HERE
// ================================================================================

void PhotoList_GetUploadMessage(PhotoList *pMe)
{
	int nLen=0;
	if ( 0==STRCMP(pMe->userSettings->message, "YES"))
	{
		PhotoList_PhotoUploadMsgDlg(pMe);
		
	}
	else
	{
		FREEIF(pMe->pszPhotoUploadMsg);
		nLen = STRLEN(pMe->userSettings->message)+1;
		pMe->pszPhotoUploadMsg = (char*)MALLOC(nLen);
		STRCPY(pMe->pszPhotoUploadMsg, pMe->userSettings->message);
		PhotoList_StartUpload(pMe);
	}
}

// ================================================================================
// FUNCTION		: PhotoList_AlbumSelectionHandler
// DESCRIPTION	: HANDLE SELECTION FROM PhotoListAlbumMenu
// ================================================================================

static void PhotoList_AlbumSelectionHandler(PhotoList* pMe, int idOption) 
{
    int nLen=0;

	if(pMe->nAlbum==idOption || pMe->nAlbum==0)
	{

		IROOTFORM_PopForm(pMe->rootForm);
		IPOPUPMENU_Release(pMe->albumMenu);
		pMe->albumMenu =NULL;
		PhotoList_CreateNewAlbumDialog(pMe);
		NewAlbumCreate(pMe);

	}
	else
	{
		IROOTFORM_PopForm(pMe->rootForm);
		IPOPUPMENU_Release(pMe->albumMenu);
		pMe->albumMenu =NULL;
		DBGPRINTF("%S", pMe->album[idOption]);
		nLen= WSTRLEN(pMe->album[idOption])+1;
		FREEIF(pMe->newAlbumName);
		pMe->newAlbumName = (char*)MALLOC(nLen);
		WSTRTOSTR(pMe->album[idOption], pMe->newAlbumName, nLen);
		PhotoList_GetUploadMessage(pMe);
		
	}	

}

// ================================================================================
// FUNCTION		: PhotoList_AlbumEventHandler
// DESCRIPTION	: HANDLE EVENT FROM PhotoListAlbumMenu
// ================================================================================
 
static boolean PhotoList_AlbumEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   PhotoList* pMe = (PhotoList*) po;
   
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT)
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.dx = pMe->DeviceInfo.cyScreen*5/6;
		if(pMe->nAlbum>4)
		{
			rc.y = pMe->DeviceInfo.cyScreen/2;
			rc.dy = pMe->DeviceInfo.cyScreen*2/5;
		}else{
			rc.y = (pMe->DeviceInfo.cyScreen/2)+(pMe->DeviceInfo.cyScreen*(4-(pMe->nAlbum+1))/10);
			rc.dy = pMe->DeviceInfo.cyScreen*(pMe->nAlbum+1)/10;

		}
		*(AEERect*) dwParam = rc;
		return TRUE;
	}


	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		if(pMe->albumMenu) 
		{
			IROOTFORM_PopForm(pMe->rootForm);
			IPOPUPMENU_Release(pMe->albumMenu);
			pMe->albumMenu =NULL;

			return TRUE;
		}
	}


    if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{				        
		IROOTFORM_PopForm(pMe->rootForm);
		IPOPUPMENU_Release(pMe->albumMenu);
		pMe->albumMenu =NULL;

		return TRUE;
	}



   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->albumMenuHandler, evt, wParam, dwParam);
}


// ================================================================================
// FUNCTION		: PhotoListAlbumMenu
// DESCRIPTION	: POPULATE ALBUM MENU
// ================================================================================

int PhotoListAlbumMenu(PhotoList* pMe) 
{    
	
	int size=0;
	int c=0;
	char *pszUserInfo=NULL;
	char *end=NULL;
	char *start=NULL;	
	int nCount=0;
	char *pszTemp=NULL;
	int result = 0;
	int nLen=0;
	IWidget* backDropWidget;
	IMenuModel* albumModel;


	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->albumMenu) != 0)
	return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
	return (FALSE);

	IPOPUPMENU_GetMenuModel(pMe->albumMenu,&albumModel);

     if (ReadFileStream(pMe->pIShell, ALBUM_FILE, &pszUserInfo)>0 )
	 {
         start=pszUserInfo;
		 while( NULL!=(end=STRSTR(start,";")))
		{
			nLen = STRLEN(start)-STRLEN(end);
			start = start+nLen+1;
			nCount++;
		}
         
		//release any memory allocted before. memeory associated eith each pMe->album[i] will release with the menu
		//model release at the end of this function.
		FREEIF(pMe->album);

        pMe->album=(AECHAR**)MALLOC(nCount*sizeof(AECHAR*));
			 
		for(c=0;c<nCount;c++)
		{
			 pMe->album[c]=NULL;
		}        
	    nCount=0;
		start=pszUserInfo;
		
		while( NULL!=(end=STRSTR(start,";")))
		{
			
			nLen = STRLEN(start)-STRLEN(end);
			size=(nLen+1)*sizeof(AECHAR);	
			pMe->album[nCount] = (AECHAR*) MALLOC(size);
			pszTemp = (char*)MALLOC(nLen+1);
			STRLCPY(pszTemp, start, nLen+1); 	
			STRTOWSTR(pszTemp, pMe->album[nCount], size);
			DBGPRINTF("%S", pMe->album[nCount]);
			IMENUMODEL_Add(albumModel,pMe->album[nCount],(uint16)nCount,MMF_ENABLED);   
			start = start+nLen+1;
			nCount++;
			FREEIF(pszTemp); 
		}
	 }
            
	pMe->nAlbum=nCount;
	if(IPOPUPMENU_LoadMenuItem(pMe->albumMenu, RIPPLEVAULT_RES_FILE, IDS_PNEWALBUM, (uint16)nCount,MMF_ENABLED) != 0)//IDS_PNEWALBUM
		return (FALSE);

    FREEIF(pszUserInfo); 

    IFORM_SetSoftkeys((IForm*)pMe->albumMenu, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
	IFORM_SetResText((IForm*)pMe->albumMenu,FID_TITLE,RIPPLEVAULT_RES_FILE,IDS_PSELECTALBUM);

	IFORM_SetSelectHandler((IForm*)pMe->albumMenu, (PFNSELECT)PhotoList_AlbumSelectionHandler, pMe);
	HANDLERDESC_Init(&pMe->albumMenuHandler, PhotoList_AlbumEventHandler, pMe, 0);
	IFORM_SetHandler((IForm*)pMe->albumMenu, &pMe->albumMenuHandler);

	{
		IWidget* piScrollWidget = NULL;
		IWidget* mainFormWidget = NULL;
		IWidget* backWidget = NULL;
		if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**) &piScrollWidget) != 0)
			return (FALSE);

		// change the scroll pad
		IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);

		IFORM_GetWidget((IForm*)pMe->albumMenu, WID_FORM, &mainFormWidget);
		IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
		IFORM_SetWidget((IForm*)pMe->albumMenu, WID_FORM, piScrollWidget);
		
		IFORM_GetWidget((IForm*)pMe->albumMenu, WID_LISTITEM, &backWidget);
		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->albumMenu, WID_LISTITEM, backWidget);
		IFORM_SetWidget((IForm*)pMe->albumMenu, WID_BACKDROP, backDropWidget);
		
		IWIDGET_Release(mainFormWidget);
		IWIDGET_Release(piScrollWidget);
		IWIDGET_Release(backDropWidget);
		IWIDGET_Release(backWidget);
	}
	
	IMENUMODEL_Release(albumModel);
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->albumMenu);

	return (TRUE);
}

// ================================================================================
// FUNCTION		: ConfirmDialog_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM PhotoList_CreateConfirmDialog
// ================================================================================

static boolean ConfirmDialog_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	PhotoList* pMe = (PhotoList*) po;
	int nLen=0;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen/2;
		//DBGPRINTF("%",rc.y);
		rc.dx = pMe->DeviceInfo.cyScreen*5/6;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*3/5;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		if(pMe->optionsMenu) 
		{
			IROOTFORM_PopForm(pMe->rootForm);
			if ( pMe->optionsMenu )
			{
				IPOPUPMENU_Release(pMe->optionsMenu);
				pMe->optionsMenu = NULL;
			}
			return TRUE;
		}
	}
	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
		IROOTFORM_PopForm(pMe->rootForm);
		if ( pMe->optionsMenu )
		{
			IPOPUPMENU_Release(pMe->optionsMenu);
			pMe->optionsMenu = NULL;
		}

		//check user settings for prompt for select a album to upload
		if ( 0==STRCMP("YES", pMe->userSettings->album))
			PhotoListAlbumMenu(pMe);
		else
		{
			nLen = STRLEN(pMe->userSettings->album)+1;
			pMe->newAlbumName = (char*)MALLOC(nLen);
			STRCPY(pMe->newAlbumName, pMe->userSettings->album);

			PhotoList_GetUploadMessage(pMe);
		}

		return TRUE;
	}
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->confirmDlgHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: ConfirmDialog_HandleEvent2
// DESCRIPTION	: HANDLE EVENT FROM PhotoList_CreateNewAlbumDialog
// ================================================================================

static boolean ConfirmDialog_HandleEvent2(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	PhotoList* pMe = (PhotoList*) po;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT)
	{
	
		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen*6/10;
		//DBGPRINTF("%",rc.y);
		rc.dx = pMe->DeviceInfo.cyScreen*5/6;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*7/10;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->confirmDlgHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PhotoList_CreateConfirmDialog
// DESCRIPTION	: POPULATE MESSAGE DIALOG FOR CONFIRMATION 
// ================================================================================

int PhotoList_CreateConfirmDialog(PhotoList* pMe)
{
	int result=0;
	IWidget* backdropWidget;
	IWidget* backWidget = NULL;
	AECHAR* sstr;
	AECHAR format[]={'%','s',' ','(','%','d',')','\0'};

	if(pMe->confirmDialog)
	{
		IDIALOG_Release(pMe->confirmDialog);
		pMe->confirmDialog = NULL;
	}

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_INFODIALOG, (void**) &pMe->confirmDialog);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);

	IFORM_SetResText((IForm*)pMe->confirmDialog, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
	IFORM_SetSoftkeys((IForm*)pMe->confirmDialog, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
	IFORM_SetResText((IForm*)pMe->confirmDialog, FID_TEXT, RIPPLEVAULT_RES_FILE, IDS_UPCONFIRM);

	FREEIF(pMe->str2);
	pMe->str2 = (AECHAR*)MALLOC(120);
	sstr = (AECHAR*)MALLOC(110);
	result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB,sstr , 110);

	WSPRINTF(pMe->str2, 120, format,sstr, pMe->itemCount);
	IFORM_SetText((IForm*)pMe->confirmDialog,FID_TITLE, pMe->str2);
		
	IFORM_GetWidget((IForm*)pMe->confirmDialog, WID_TITLE, &backWidget);
	IWIDGET_SetBorderWidth(backWidget, 1);
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IFORM_SetWidget((IForm*)pMe->confirmDialog, WID_TITLE, backWidget);
	IWIDGET_Release(backWidget);

	IFORM_GetWidget((IForm*)pMe->confirmDialog, WID_BACKGROUND, &backWidget);
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IWIDGET_SetBorderWidth(backWidget, 1);
	IFORM_SetWidget((IForm*)pMe->confirmDialog, WID_BACKGROUND, backWidget);
	
	IFORM_SetWidget((IForm*)pMe->confirmDialog, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->confirmDialog, IDF_ALIGN_FILL);
    
	HANDLERDESC_Init(&pMe->confirmDlgHandler, ConfirmDialog_HandleEvent, pMe, NULL);
	IFORM_SetHandler((IForm*)pMe->confirmDialog, &pMe->confirmDlgHandler);

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->confirmDialog);
	FREEIF(sstr);
	IWIDGET_Release(backdropWidget);
	IWIDGET_Release(backWidget);
	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoList_CreateNewAlbumDialog
// DESCRIPTION	: APPERAR WHEN NEW ALBUM CREATE
// ================================================================================

int PhotoList_CreateNewAlbumDialog(PhotoList* pMe)
{
	int result=0;

	IWidget* backdropWidget;
	IWidget* backWidget = NULL;
	IWidget* dialogContainer = NULL;
	IWidget* containerWidget = NULL;


	if(pMe->createNewAlbumDialog)
	{
		IDIALOG_Release(pMe->createNewAlbumDialog);
		pMe->createNewAlbumDialog = NULL;
	}

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_INFODIALOG, (void**) &pMe->createNewAlbumDialog);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);
    result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->newAlbumWidget);

	IFORM_SetResText((IForm*)pMe->createNewAlbumDialog, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_PNEWALBUM);
	IFORM_SetSoftkeys((IForm*)pMe->createNewAlbumDialog, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);

	IFORM_GetWidget((IForm*)pMe->createNewAlbumDialog, WID_TITLE, &backWidget);
	IWIDGET_SetBorderWidth(backWidget, 1);
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IFORM_SetWidget((IForm*)pMe->confirmDialog, WID_TITLE, backWidget);
	IWIDGET_Release(backWidget);

	IFORM_GetWidget((IForm*)pMe->createNewAlbumDialog, WID_BACKGROUND, &backWidget);
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IWIDGET_SetBorderWidth(backWidget, 1);
	IFORM_SetWidget((IForm*)pMe->createNewAlbumDialog, WID_BACKGROUND, backWidget);
	
	IFORM_SetWidget((IForm*)pMe->createNewAlbumDialog, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->createNewAlbumDialog, IDF_ALIGN_FILL);

    HANDLERDESC_Init(&pMe->confirmDlgHandler, ConfirmDialog_HandleEvent2, pMe, NULL);
	IFORM_SetHandler((IForm*)pMe->createNewAlbumDialog, &pMe->confirmDlgHandler);

	
	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->createNewAlbumDialog);
	
	IWIDGET_Release(backdropWidget);
	IWIDGET_Release(backWidget);
	return TRUE;
}

// ================================================================================
// FUNCTION		: PhotoList_CreateProgressDialog
// DESCRIPTION	: POPULATE PROGRESS DIALOG
// ================================================================================

int PhotoList_CreateProgressDialog(PhotoList* pMe, uint16 nResId)
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

	HANDLERDESC_Init(&pMe->progressHandler, PLProgressDlg_HandleEvent, pMe, NULL);
	IFORM_SetHandler((IForm*)pMe->progressDialog, &pMe->progressHandler);//pathi

	IFORM_SetProgress((IForm*)pMe->progressDialog,-1);
	
	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->progressDialog);
	
	IWIDGET_Release(backdropWidget);
	IWIDGET_Release(backWidget);
	return TRUE;
}

// ================================================================================
// FUNCTION		: PLProgressDlg_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM PROGRESS DIALOG
// ================================================================================

static boolean PLProgressDlg_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	PhotoList* pMe = (PhotoList*) po;
	switch (evt)
	{
	case EVT_KEY:
		break;

	case EVT_USER_PHOTOUPLOAD:
			PhotoList_UploadPhoto(pMe);
		break;

	case EVT_USER_ALBUM_SYNC:
		//end of photo upload and albumsync
		if (NULL!=pMe->m_pSyncExt) 
		{
			ISYNCENG_Release(pMe->m_pSyncExt);
			pMe->m_pSyncExt=NULL;
		}
		//popup progress bar and image list
		IROOTFORM_PopForm(pMe->rootForm);
		IROOTFORM_PopForm(pMe->rootForm);

		break;

	default:
		if (pMe->m_pSyncExt != NULL) 
			ISYNCENG_HandleEvent(pMe->m_pSyncExt, evt, wParam, dwParam);
		break;
	}
	return HANDLERDESC_Call(&pMe->progressHandler, evt, wParam, dwParam);
}


// ================================================================================
// FUNCTION		: newAlbumCreateText_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM NewAlbumCreate
// ================================================================================

static boolean newAlbumCreateText_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	PhotoList* pMe = (PhotoList*) po;
	AECHAR* a_uname=NULL;
	int nLen=0;
	int i=0;
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu
		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = -37;
		rc.y = pMe->DeviceInfo.cyScreen*3/4;
		rc.dx = pMe->DeviceInfo.cyScreen*3;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*2/5;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

   if(wParam == AVK_CLR)
   {

		IWIDGET_GetTextWidgetText(pMe->AlbumCreateTextWidget,&a_uname);

		if(WSTRLEN(a_uname)==0)
		{
			IROOTFORM_PopForm(pMe->rootForm);
			IFORM_Release(pMe->newAlbumCreateDialog);
			pMe->newAlbumCreateDialog = NULL;

			IROOTFORM_PopForm(pMe->rootForm);
			IDIALOG_Release(pMe->createNewAlbumDialog);
			pMe->createNewAlbumDialog = NULL;

		}
	
   }
  	 
	
	if(evt == EVT_KEY && wParam == AVK_SOFT2)
	{
    	if(pMe->albumMenu) 
		{
			IROOTFORM_PopForm(pMe->rootForm);
			IFORM_Release(pMe->newAlbumCreateDialog);
			pMe->newAlbumCreateDialog = NULL;

			IROOTFORM_PopForm(pMe->rootForm);
			IDIALOG_Release(pMe->createNewAlbumDialog);
			pMe->createNewAlbumDialog = NULL;

		    IPOPUPMENU_Release(pMe->albumMenu);
			pMe->albumMenu =NULL;
		
			return TRUE;
		}
	}
	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
		IROOTFORM_PopForm(pMe->rootForm);
		IROOTFORM_PopForm(pMe->rootForm);

        AlbumName(pMe);
	
		return TRUE;
	}
	
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->newAlbumDlgHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: NewAlbumCreate
// DESCRIPTION	: CREATE NEW ALBUM 
// ================================================================================


int NewAlbumCreate(PhotoList* pMe) 
{
	int result=0;
	IWidget* backdropWidget=NULL;
	IWidget* backWidget = NULL;
	AECHAR* sstr;
	WidgetPos wpos;
	IWidget* containerWidget = NULL;
	WExtent we;
	AEEDeviceInfo  deviceInfo;
    int nWdtHeight=0;
	int nWdtWidth=0;
	AECHAR format[]={'%','s',' ','\0'};

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->newAlbumCreateDialog);
	result +=  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->albumCreateContainer);
    result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->AlbumCreateTextWidget);
	
	IFORM_SetResText((IForm*)pMe->newAlbumCreateDialog, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
	IFORM_SetSoftkeys((IForm*)pMe->newAlbumCreateDialog, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
  
    ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
	nWdtHeight = (deviceInfo.cyScreen/12)*8;// - deviceInfo.cyScreen/3;// - space taken for title bar and menu bar
	nWdtHeight = nWdtHeight/6;
	nWdtHeight = nWdtHeight;
	nWdtWidth = (deviceInfo.cyScreen/10)*8;

	pMe->str3 = (AECHAR*)MALLOC(120);
	sstr = (AECHAR*)MALLOC(110);
	result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB,sstr , 110);
	WSPRINTF(pMe->str3, 120, format,sstr);//, pMe->itemCount);
    IFORM_SetText((IForm*)pMe->newAlbumCreateDialog,FID_TITLE, pMe->str3);
	
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->AlbumCreateTextWidget,&we);
	IWIDGET_SetBorderColor(pMe->AlbumCreateTextWidget, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->AlbumCreateTextWidget, 1);
    IWIDGET_SetFlags(pMe->AlbumCreateTextWidget, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);

	IFORM_SetWidget((IForm*)pMe->newAlbumCreateDialog, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->newAlbumCreateDialog, IDF_ALIGN_FILL);

	// Adding the text field to the container
	wpos.x = 40;
    wpos.y = 0;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->albumCreateContainer, pMe->AlbumCreateTextWidget, WIDGET_ZNORMAL, &wpos);

	// Attach the contaner to the dialog
	result = IXYCONTAINER_QueryInterface(pMe->albumCreateContainer, AEEIID_WIDGET, (void**)&containerWidget);
	if(result ==0) 
	{
		IFORM_SetWidget((IForm*)pMe->newAlbumCreateDialog, WID_FORM, containerWidget);
		IWIDGET_MoveFocus(containerWidget, pMe->AlbumCreateTextWidget);		
	}

	HANDLERDESC_Init(&pMe->newAlbumDlgHandler, newAlbumCreateText_HandleEvent, pMe, NULL);
	IFORM_SetHandler((IForm*)pMe->newAlbumCreateDialog, &pMe->newAlbumDlgHandler);

	FREEIF(sstr);
	IWIDGET_Release(containerWidget);

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->newAlbumCreateDialog);
	
	return TRUE;
}

// ================================================================================
// FUNCTION		: AlbumName
// DESCRIPTION	: GET ALBUM NAME ENTRED TO THE TEXT WIDGET
// ================================================================================

void AlbumName(PhotoList* pMe)
{
	AECHAR* a_uname=NULL;
	int nLen=0;

	IWIDGET_GetTextWidgetText(pMe->AlbumCreateTextWidget,&a_uname);
	nLen = WSTRLEN(a_uname)+1;

	FREEIF(pMe->newAlbumName);
	pMe->newAlbumName = (char*)MALLOC(nLen);

	WSTRTOSTR(a_uname,pMe->newAlbumName,nLen);

	PhotoList_GetUploadMessage(pMe);
	
}

// ================================================================================
// FUNCTION		: UnSelectDialog_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM PhotoList_UnSelectInformDialog
// ================================================================================

static boolean UnSelectDialog_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	PhotoList* pMe = (PhotoList*) po;

	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu
		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen/2;
		rc.dx = pMe->DeviceInfo.cyScreen*5/6;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*3/5;
		*(AEERect*) dwParam = rc;

		return TRUE;
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		if(pMe->optionsMenu) 
		{
			IROOTFORM_PopForm(pMe->rootForm);
			if ( pMe->optionsMenu )
			{
				IPOPUPMENU_Release(pMe->optionsMenu);
				pMe->optionsMenu = NULL;
			}
			return TRUE;
		}
	}
	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
		IROOTFORM_PopForm(pMe->rootForm);
		if ( pMe->optionsMenu )
		{
			IPOPUPMENU_Release(pMe->optionsMenu);
			pMe->optionsMenu = NULL;
		}

		return TRUE;
	}
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->unSelectDlgHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: PhotoList_UnSelectInformDialog
// DESCRIPTION	: DISPLAY MESSAGE WHEN USER UNSELECT PHOTOS
// ================================================================================

int PhotoList_UnSelectInformDialog(PhotoList* pMe)
{
	int result=0;

	IWidget* backdropWidget;
	IWidget* backWidget = NULL;
	AECHAR* sstr;
	AECHAR format[]={'%','s',' ','(','%','d',')','\0'};

	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_INFODIALOG, (void**) &pMe->unSelectDialog);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);

	IFORM_SetResText((IForm*)pMe->unSelectDialog, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_UNSELECT_TITLE);
	IFORM_SetSoftkeys((IForm*)pMe->unSelectDialog, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
	IFORM_SetResText((IForm*)pMe->unSelectDialog, FID_TEXT, RIPPLEVAULT_RES_FILE, IDS_UNSELECT_MESSAGE);

	pMe->str2 = (AECHAR*)MALLOC(120);
	sstr = (AECHAR*)MALLOC(110);
	result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB,sstr , 110);

	
	IFORM_GetWidget((IForm*)pMe->unSelectDialog, WID_TITLE, &backWidget);
	IWIDGET_SetBorderWidth(backWidget, 1);
	IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
	IFORM_SetWidget((IForm*)pMe->unSelectDialog, WID_TITLE, backWidget);
	IWIDGET_Release(backWidget);

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


// ================================================================================
// FUNCTION		: PhotoList_PhotoUploadMsgDlgHandleEvent
// DESCRIPTION	: HANDLE EVENT FROM PhotoList_PhotoUploadMsgDlg 
// ================================================================================

boolean PhotoList_PhotoUploadMsgDlgHandleEvent(PhotoList *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	boolean result = FALSE;
	AEEDeviceInfo  DeviceInfo;
	AEERect rc;
	AECHAR *pszTemp=NULL;
	int nLen=0;

	if(eCode == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu

		ISHELL_GetDeviceInfo(pMe->pIShell, &DeviceInfo);
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
			IROOTFORM_PopForm(pMe->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT2:
			IROOTFORM_PopForm(pMe->rootForm);
			result = TRUE;
		break;

		case AVK_SOFT1:		
			IROOTFORM_PopForm(pMe->rootForm);
			FREEIF(pMe->pszPhotoUploadMsg);
			IWIDGET_GetTextWidgetText(pMe->uploadMsgWgt, &pszTemp);
			nLen = WSTRLEN(pszTemp)+1;
			pMe->pszPhotoUploadMsg = (char*)MALLOC(nLen);
			WSTRTOSTR(pszTemp, pMe->pszPhotoUploadMsg, nLen);
			PhotoList_StartUpload(pMe);
			result = TRUE;
			break;

		}
	}

	if ( !result )
		result = HANDLERDESC_Call( &pMe->msgDlgHandler, eCode, wParam, dwParam );

	return result;
}


// ================================================================================
// FUNCTION		: PhotoList_PhotoUploadMsgDlg
// DESCRIPTION	: DISPLAY MESSAGE WHEN USER GOING TO UPLOAD PHOTO
// ================================================================================

int PhotoList_PhotoUploadMsgDlg(PhotoList* pMe)
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

	if( !pMe->photoUploadMsgDlg)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->photoUploadMsgDlg);
		result +=  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->msgDlgContainer);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->uploadMsgWgt);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&titleWidget);

		IFORM_SetResText((IForm*)pMe->photoUploadMsgDlg, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
		IFORM_SetSoftkeys((IForm*)pMe->photoUploadMsgDlg, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
  
		ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
		nWdtHeight = (deviceInfo.cyScreen/12)*8;// - deviceInfo.cyScreen/3;
		nWdtHeight = nWdtHeight/6;
		nWdtHeight = nWdtHeight;

		nWdtWidth = (deviceInfo.cyScreen/10)*8;
		
		pszTitle = (AECHAR*)MALLOC(50);//this will free when widget release.
		result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHARE_MSG, pszTitle, 50);
		IFORM_SetText((IForm*)pMe->photoUploadMsgDlg,FID_TITLE, pszTitle);
		
		
		IWIDGET_SetBorderColor(pMe->uploadMsgWgt, MAKE_RGB(100, 100, 100));
		IWIDGET_SetBorderWidth(pMe->uploadMsgWgt, 1);
		IWIDGET_SetFlags(pMe->uploadMsgWgt, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);
		IFORM_SetWidget((IForm*)pMe->photoUploadMsgDlg, WID_BACKDROP, backdropWidget);
		IWIDGET_SetFlags((IWidget*)pMe->photoUploadMsgDlg, IDF_ALIGN_FILL);
		
		we.height = nWdtHeight;
		we.width = deviceInfo.cxScreen;
		IWIDGET_SetExtent(titleWidget,&we);
		we.height = nWdtHeight;
		we.width = nWdtWidth;
		IWIDGET_SetExtent(pMe->uploadMsgWgt,&we);
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
		IXYCONTAINER_Insert(pMe->msgDlgContainer, pMe->uploadMsgWgt, WIDGET_ZNORMAL, &wpos);

		// Attach the contaner to the dialog
		result = IXYCONTAINER_QueryInterface(pMe->msgDlgContainer, AEEIID_WIDGET, (void**)&containerWidget);
		if(result ==0) 
		{
			IFORM_SetWidget((IForm*)pMe->photoUploadMsgDlg, WID_FORM, containerWidget);
			IWIDGET_SetProperty(containerWidget, PROP_BGCOLOR, MAKE_RGB(225,225,225));
			IWIDGET_SetBorderWidth(containerWidget, 1);
			
			IWIDGET_MoveFocus(containerWidget, pMe->uploadMsgWgt);		
		}

		HANDLERDESC_Init(&pMe->msgDlgHandler, PhotoList_PhotoUploadMsgDlgHandleEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->photoUploadMsgDlg, &pMe->msgDlgHandler);

		IWIDGET_Release(containerWidget);
		IWIDGET_Release(titleWidget);

	}

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->photoUploadMsgDlg);

	return TRUE;
}

