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
 Module Name				: CSettings.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF,LKA,SRI
 Date Created               : 20-08-2005
 Rational Rose Model Name   :                          
 Use Case Document Name     : EC-GEN-USE-2005-07-20-Update Contact Settings V1.0,EC-GEN-USE-2005-07-20-View Contacts V1.0                         
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By           :
 Code Reviewed Date			:
 
 Recommendations            :
 

***************************************************************/
#include "CSettings.h"
#include "AEEFile.h"
#include "AEEListWidget.h"
#include "AEEDialog.h"
#include "UserData.h"

#define RIPPLEVAULT_RES_FILE "RippleVault.bar"

// ================================================================================
// FUNCTION		: CSettings_Delete
// DESCRIPTION	: DELETE ALL GLOBAL RESOURCES USED IN THIS MODULE FROM HERE
// ================================================================================

void CSettings_Delete(CSettings* pMe) 
{	
	if(pMe->shedulerList)
	{
		IFORM_Release(pMe->shedulerList);
		pMe->shedulerList=NULL;
	}
	if(pMe->radioList)
	{
		IFORM_Release(pMe->radioList);
		pMe->radioList=NULL;
	}
	if(pMe->mainContainer)
	{
	IPROPCONTAINER_Release(pMe->mainContainer);
	pMe->mainContainer = NULL;
	}
	if(pMe->radioListContainer)
	{
	IXYCONTAINER_Release(pMe->radioListContainer);
	pMe->radioListContainer=NULL;
	}
	if(pMe->menu)
	{
	IPOPUPMENU_Release(pMe->menu);
	pMe->menu=NULL;
	}
	if(pMe->csettings)
	{
		IFORM_Release(pMe->csettings);
		pMe->csettings=NULL;
	}
	
	FREEIF(pMe->str1);
	FREEIF(pMe->str2);

    if(pMe)
		FREE(pMe);
}

// ================================================================================
// FUNCTION		: MenuModal_EventHandler
// DESCRIPTION	: LISTEN FOR CHANGES ON MAIN FORM
// ================================================================================
static void MenuModal_EventHandler(CSettings *pMe, ModelEvent *pev) 
{
	
	if(pev->evCode == EVT_MDL_FOCUS_SELECT)
	{ 
		if(pev->dwParam == 0)
		{
			pMe->text = (AECHAR*)MALLOC(10);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_NON,pMe->text , 10);
			pMe->title=(AECHAR*)MALLOC(30);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROOMING,pMe->title , 30);
			pMe->isRoomimgDialog=1;
			CSettings_Dialog(pMe,pMe->text,pMe->title);
			FREEIF(pMe->text);
			FREEIF(pMe->title);
			CSettingsRadioList(pMe); 
			
		}
		if(pev->dwParam == 1)
		{
	
			pMe->text = (AECHAR*)MALLOC(10);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_NON,pMe->text , 10);
			pMe->title=(AECHAR*)MALLOC(30);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHEDULER,pMe->title , 20);
			CSettings_Dialog(pMe,pMe->text,pMe->title);
			FREEIF(pMe->text);
			FREEIF(pMe->title);
			CSettingsRadioListForSheduler(pMe);
		
		}
	}
	if (pev->evCode == EVT_MDL_FOCUS_CHANGE)
		pMe->selectedItem = (int)pev->dwParam;

}

// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: HANDLE EVENT FROM MAIN FORM 
// ================================================================================

static boolean CSettings_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   CSettings* pMe = (CSettings*) po;	

   switch (evt) 
	{
	case EVT_KEY:
		{
			switch (wParam) 
			{
			case AVK_SOFT2:
					IROOTFORM_PopForm(pMe->rootForm);
					if(pMe->menu)
					{
						IPOPUPMENU_Release(pMe->menu);
						pMe->menu=NULL;
					}
											 
					return (TRUE);
					break;
			
			case AVK_SOFT1:
					CSettingsMenu(pMe);
					return (TRUE);
					break;

			case AVK_CLR:
					IROOTFORM_PopForm(pMe->rootForm);
					if(pMe->menu)
					{
						IPOPUPMENU_Release(pMe->menu);
						pMe->menu=NULL;
						IROOTFORM_PopForm(pMe->rootForm);
						return TRUE;
			
					}
					return (TRUE);
					break;			
					
			}
		}

	}
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->settingsMenuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: CSettings_PopulateMainContainer
// DESCRIPTION	: POPULATE MAIN FORM
// ================================================================================

int CSettings_PopulateMainContainer(CSettings* pMe) 
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
	ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
	
	//load the strings
	pMe->str1 = (AECHAR*) MALLOC(20);
	pMe->str2 = (AECHAR*) MALLOC(20);

	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RANGE, pMe->str1, 20);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHEDULER, pMe->str2, 20);
	
	//load the images
	pMe->imageInfo1.pwText = (AECHAR*)pMe->str1;
	pMe->imageInfo2.pwText = (AECHAR*)pMe->str2;

	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo1,1,MMF_ENABLED);
	IMENUMODEL_Add(iMenuModal,&pMe->imageInfo2,2,MMF_ENABLED);

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
		IMODEL_AddListenerEx(pickListViewModel, &pMe->menuListener, (PFNLISTENER)MenuModal_EventHandler, pMe);
		IMODEL_Release(pickListViewModel);
	}
	wp.bVisible = TRUE;
	wp.prop = 1;
	IPROPCONTAINER_Insert(pMe->mainContainer, pScrollBar, WIDGET_ZNORMAL, &wp);

	//set WID_FORM to the container
	result = IPROPCONTAINER_QueryInterface(pMe->mainContainer, AEEIID_WIDGET, (void**)&pMe->containerWidget);
	
	HANDLERDESC_Init(&pMe->settingsMenuHandler, CSettings_HandleEvent, pMe, CSettings_Delete);
	IFORM_SetHandler((IForm*)pMe->csettings, &pMe->settingsMenuHandler);	
	if(result ==0) 
	{
		IFORM_SetWidget((IForm*)pMe->csettings, WID_FORM, pMe->containerWidget);
		IWIDGET_MoveFocus(pMe->containerWidget, wItemList);
	}

	IWIDGET_Release((IWidget*)imageWidget);
	IMENUMODEL_Release(iMenuModal);
	IWIDGET_Release(wItemList);
	IWIDGET_Release(pScrollBar);
	if (pMe->containerWidget)
		IWIDGET_Release(pMe->containerWidget);
	
	RELEASEIF(piFont);
		
	if(result != 0)
		return EFAILED;
	
	return result;
}

// ================================================================================
// FUNCTION		: CSettings_New
// DESCRIPTION	: INITIATE MAIN FORM POPULATION.
// ================================================================================

int CSettings_New(IForm** ppo, IShell *piShell,IRootForm *pRootForm) 
{
	int result=0;
	IImage* m_pIImage;
	//allocate space for the form
	CSettings *pMe = MALLOCREC(CSettings);
	if(!pMe)
		return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->csettings) == 0)
	
	{
		result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROPCONTAINER, (void**) &pMe->mainContainer);
		*ppo = pMe->csettings;
		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->csettings,m_pIImage);
		result += IFORM_SetResText((IForm*)pMe->csettings, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_CONTACTSETTINGS);
		result +=  IFORM_SetSoftkeys((IForm*)pMe->csettings, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);
		result += CSettings_PopulateMainContainer(pMe);	//add items to the form
	
	RELEASEIF(m_pIImage);
	}
	return result;
}

// ================================================================================
// FUNCTION		: CSettings_Dialog_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM DIALOG
// ================================================================================

static boolean CSettings_Dialog_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	CSettings* pMe = (CSettings*) po;
	user *pUser=NULL;
	AECHAR *rangetext=NULL;
	char* charrangetext=NULL;
	unsigned long time=0;
   
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) {
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;   
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		if(pMe->isRoomimgDialog==1){
			pMe->isRoomimgDialog=0;
			rc.x = pMe->DeviceInfo.cxScreen/10;
			rc.y = pMe->DeviceInfo.cyScreen/2;
			rc.dx = pMe->DeviceInfo.cxScreen*5/6;
			rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*3/5;
			*(AEERect*) dwParam = rc;
		}else{

			rc.x = 0;
			rc.y = pMe->DeviceInfo.cyScreen/4;
			rc.dx = pMe->DeviceInfo.cxScreen;
			rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen*19/56;
			*(AEERect*) dwParam = rc;
		}
		return TRUE;
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		IROOTFORM_PopForm(pMe->rootForm);
		deleteDialog(pMe);
		deleteShdulerForm(pMe);
		if(pMe->dialog)
		{
			IDIALOG_Release(pMe->dialog);
			pMe->dialog=NULL;
		}
		return TRUE;
	}

	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
	   if ( GetUserData(pMe->pIShell, &pUser))
		{			
			if(pMe->dialogEvent==1)
			{
		
				STRCPY(pUser->roomingState,"ON");
				
			}if(pMe->dialogEvent==2)
			{
				
				STRCPY(pUser->roomingState,"OFF");
			
			}
			if(pMe->dialogEvent==3)
			{

			IWIDGET_GetTextWidgetText(pMe->rangeCreateTextWidget,&rangetext);
			charrangetext=MALLOC(30);
			WSTRTOSTR(rangetext,charrangetext,30);

			if(IVALUEMODEL_GetBool(pMe->valueModel3))
			{
				STRCPY(pUser->sheduler,"h");
				if(STRTOUL(charrangetext, NULL, 10)>1000)
					time=500*3600000;
				else
					time = STRTOUL(charrangetext, NULL, 10)*3600000;		
			}
			if(IVALUEMODEL_GetBool(pMe->valueModel4))
			{
				STRCPY(pUser->sheduler,"m");
				time = STRTOUL(charrangetext, NULL, 10)*60000;
			}
			if(IVALUEMODEL_GetBool(pMe->valueModel5))
			{
				STRCPY(pUser->sheduler,"s");
				time = STRTOUL(charrangetext, NULL, 10)*1000;
			}		
			pUser->shedulePeriod=time;
				
		}
		SetUserData(pMe->pIShell, pUser);
		pMe->dialogEvent=0;
		FREEIF(charrangetext);
		FREEIF(pUser);
	}
		
	IROOTFORM_PopForm(pMe->rootForm);
	deleteDialog(pMe);
	deleteShdulerForm(pMe);

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
// FUNCTION		: deleteDialog
// DESCRIPTION	: DELETE ALL  RESOURCES USED IN DIALOG
// ================================================================================


void deleteDialog(CSettings* pMe)
{
	if(pMe->dialog)
	{
		IDIALOG_Release(pMe->dialog);
		pMe->dialog=NULL;
	}
	
}

// ================================================================================
// FUNCTION		: CSettings_Dialog
// DESCRIPTION	: INFO DIALOG FOR CSETTINGS MODULE
// ================================================================================

int CSettings_Dialog(CSettings* pMe,AECHAR* text,AECHAR* title)
{
	int result=0;
	int nLen=0;
	int size=0;
	IWidget* backdropWidget=NULL;
	IWidget* backWidget = NULL;
	AECHAR format[]={'%','s',' ',' ','\0'};

	if(NULL==pMe->dialog)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_INFODIALOG, (void**) &pMe->dialog);		
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);

		IFORM_SetResText((IForm*)pMe->dialog, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
		IFORM_SetSoftkeys((IForm*)pMe->dialog, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
			
		IFORM_SetText((IForm*)pMe->dialog, FID_TEXT, text); 
		IFORM_SetText((IForm*)pMe->dialog,FID_TITLE, title);
				
		IFORM_GetWidget((IForm*)pMe->dialog, WID_TITLE, &backWidget);
		IWIDGET_SetBorderWidth(backWidget, 2);
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
    
		HANDLERDESC_Init(&pMe->dlgHandler, CSettings_Dialog_HandleEvent, pMe, deleteDialog);
		IFORM_SetHandler((IForm*)pMe->dialog, &pMe->dlgHandler);

		IWIDGET_Release(backdropWidget);
		IWIDGET_Release(backWidget);
	}
	if(pMe->dialog)
		IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->dialog);
	return TRUE;
}

// ================================================================================
// FUNCTION		: CSettings_OptionSelectionHandler
// DESCRIPTION	: HANDLE OPTION SFROM CSettingsMenu MENU
// ================================================================================
static void CSettings_OptionSelectionHandler(CSettings* pMe, int idOption) 
{
       
   	AECHAR* sstr=NULL;
	AECHAR* title=NULL;
    
	IROOTFORM_PopForm(pMe->rootForm);
	if(pMe->menu)
	{
		IPOPUPMENU_Release(pMe->menu);
		pMe->menu=NULL;
	}
	
	switch(pMe->selectedItem)//pMe->selectedItem
	{
	case 0:
			sstr = (AECHAR*)MALLOC(10);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_NON,sstr , 10);
			title=(AECHAR*)MALLOC(20);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROOMING,title , 20);
			pMe->isRoomimgDialog=1;
			CSettings_Dialog(pMe,sstr,title);
			FREEIF(sstr);
			FREEIF(title);
			CSettingsRadioList(pMe); 
			break;
	
	case 1:
			sstr = (AECHAR*)MALLOC(10);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_NON,sstr , 10);
			title=(AECHAR*)MALLOC(20);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHEDULER,title , 20);
			CSettings_Dialog(pMe,sstr,title);
			FREEIF(sstr);
			FREEIF(title);
			CSettingsRadioListForSheduler(pMe);			
			break;
	case 2:
		
		break;
	
	}
	

}


// ================================================================================
// FUNCTION		: CSettings_OptionsEventHandler
// DESCRIPTION	: HANDLE EVENT FROM CSettingsMenu MENU
// ================================================================================
static boolean CSettings_OptionsEventHandler(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	CSettings* pMe = (CSettings*) po;  
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT)
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;
		ISHELL_GetDeviceInfo(pMe->pIShell, &pMe->DeviceInfo);
		rc.x = 0;
		rc.y = pMe->DeviceInfo.cyScreen*8/10;
		rc.dx = pMe->DeviceInfo.cxScreen;
		rc.dy = pMe->DeviceInfo.cyScreen/8;
		*(AEERect*) dwParam = rc;
		return TRUE;
	}

	if(evt == EVT_KEY && wParam == AVK_SOFT1)
	{
		DBGPRINTF("TESTing");		
	}

	if(evt == EVT_KEY && (wParam == AVK_CLR || wParam == AVK_SOFT2))
	{
		IROOTFORM_PopForm(pMe->rootForm);
		if(pMe->menu)
		{
			IPOPUPMENU_Release(pMe->menu);
			pMe->menu=NULL;
			return TRUE;
		}
	}
		

   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->menuHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: CSettingsMenu
// DESCRIPTION	: POPULATE MENU FOR CHANE PHOHE FOR LIST ITEMS
// ================================================================================

int CSettingsMenu(CSettings* pMe) 
{
	int result = 0;
	IWidget* backDropWidget=NULL;
   
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**) &backDropWidget) != 0)
		return (FALSE);
	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPMENUFORM, (void**) &pMe->menu) != 0)
		return (FALSE);
	if(IPOPUPMENU_LoadMenuItem(pMe->menu, RIPPLEVAULT_RES_FILE, IDS_CHANGE, 0,MMF_ENABLED) != 0)
		return (FALSE);	   
	IFORM_SetSoftkeys((IForm*)pMe->menu, RIPPLEVAULT_RES_FILE, IDS_SELECT, IDS_CANCEL);


	IFORM_SetSelectHandler((IForm*)pMe->menu, (PFNSELECT)CSettings_OptionSelectionHandler, pMe);
	HANDLERDESC_Init(&pMe->menuHandler, CSettings_OptionsEventHandler, pMe, 0);	
	IFORM_SetHandler((IForm*)pMe->menu, &pMe->menuHandler);
	//wrap the pop-up form in a scrollbar decorator
	{
		IWidget* piScrollWidget = NULL;
		IWidget* mainFormWidget = NULL;
		IWidget* backWidget = NULL;
		if (ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SCROLLBARWIDGET, (void**) &piScrollWidget) != 0)
			return (FALSE);
		// change the scroll pad
		IWIDGET_SetProperty(piScrollWidget, PROP_SCROLLPAD, 0);
		IFORM_GetWidget((IForm*)pMe->menu, WID_FORM, &mainFormWidget);
		IDECORATOR_SetWidget((IDecorator*)piScrollWidget, mainFormWidget);
		IFORM_SetWidget((IForm*)pMe->menu, WID_FORM, piScrollWidget);

		IFORM_GetWidget((IForm*)pMe->menu, WID_LISTITEM, &backWidget);
		IWIDGET_SetSelectedShadowOffset((IWidget *)backWidget,2);
		IWIDGET_SetShadowColor((IWidget *)backWidget,MAKE_RGB(122,122,122));
		IWIDGET_SetSelectedActiveBGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->menu, WID_LISTITEM, backWidget);
		IFORM_SetWidget((IForm*)pMe->menu, WID_BACKDROP, backDropWidget);

		IWIDGET_Release(mainFormWidget);
		IWIDGET_Release(piScrollWidget);		
		IWIDGET_Release(backWidget);
	}
	IWIDGET_Release(backDropWidget);
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->menu);
	return (TRUE);
}


// ================================================================================
// FUNCTION		: deleteRoomingState
// DESCRIPTION	: DELETE ALL RESOURCES USED IN ROAMING UPLOAD SETTING FORM
// ================================================================================

void deleteRoomingState(CSettings* pMe)
{
	if(pMe->valueModel1){
		IVALUEMODEL_Release(pMe->valueModel1);
		pMe->valueModel1=NULL;
	}
	if(pMe->valueModel2)
	{
		IVALUEMODEL_Release(pMe->valueModel2);
		pMe->valueModel2=NULL;
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
	if(pMe->radioList)
	{
		IFORM_Release(pMe->radioList);
		pMe->radioList=NULL;
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
	if(pMe->containerWidget)
	{
		IWIDGET_Release(pMe->containerWidget);
		pMe->containerWidget=NULL;
	}	
	if(pMe->radioListContainer)
	{
		IXYCONTAINER_Release(pMe->radioListContainer);
		pMe->radioListContainer=NULL;
	}
	if(pMe->dialog)
	{
		IDIALOG_Release(pMe->dialog);
		pMe->dialog=NULL;
	}
	
}	

// ================================================================================
// FUNCTION		: radioList_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM CSettingsRadioList
// ================================================================================

static boolean radioList_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	CSettings* pMe = (CSettings*) po;
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

		rc.x = -pMe->DeviceInfo.cxScreen*2/10;
		rc.y = pMe->DeviceInfo.cyScreen*9/20;
		rc.dx = pMe->DeviceInfo.cxScreen*3;
		rc.dy = pMe->DeviceInfo.cyScreen/2;
		*(AEERect*) dwParam = rc;

		if (GetUserData(pMe->pIShell, &pUser))
		{
		
			state=pUser->roomingState;
			
		}

		if ( 0==STRCMP("ON", pUser->roomingState))		
			IVALUEMODEL_SetBool(pMe->valueModel1,TRUE);
		if ( 0==STRCMP("OFF", pUser->roomingState))
			IVALUEMODEL_SetBool(pMe->valueModel2,TRUE);

		FREEIF(pUser)
		return TRUE;
   }

     	 
	if(evt==EVT_KEY)
	{
		if(wParam == AVK_DOWN)
		{
			IWIDGET_MoveFocus(pMe->containerWidget, pMe->rdisable);
		}
		if(wParam == AVK_UP)
		{
			IWIDGET_MoveFocus(pMe->containerWidget, pMe->renable);
		}

		if(evt == EVT_KEY && wParam == AVK_SOFT1)
		{
			IROOTFORM_PopForm(pMe->rootForm);

			IROOTFORM_PopForm(pMe->rootForm);
			deleteDialog(pMe);
			if(pMe->dialog)
			{
				IDIALOG_Release(pMe->dialog);
				pMe->dialog=NULL;
			}		
			if(IVALUEMODEL_GetBool(pMe->valueModel1))
			{
				pMe->dialogEvent=1;
				sstr = (AECHAR*)MALLOC(200);
				ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROOMINGON,sstr , 200);
				title=(AECHAR*)MALLOC(30);
				ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROOMING,title , 30);
				CSettings_Dialog(pMe,sstr,title);
				FREEIF(sstr);
				FREEIF(title);
			}
			if(IVALUEMODEL_GetBool(pMe->valueModel2))
			{
				pMe->dialogEvent=2;
				sstr = (AECHAR*)MALLOC(300);
				ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROOMINGOFF,sstr , 300);
				title=(AECHAR*)MALLOC(30);
				ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_ROOMING,title , 30);
				CSettings_Dialog(pMe,sstr,title);
				FREEIF(sstr);
				FREEIF(title);			
			}
			deleteRoomingState(pMe);
			return TRUE;
		}

		if(wParam == AVK_CLR || wParam == AVK_SOFT2)
		{
			IROOTFORM_PopForm(pMe->rootForm);
			deleteRoomingState(pMe);
			IROOTFORM_PopForm(pMe->rootForm);
			deleteDialog(pMe);
			return TRUE;

		}


	}	
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
   return HANDLERDESC_Call(&pMe->radioListHandler, evt, wParam, dwParam);
}


// ====================================================================================
// FUNCTION		: CSettingsRadioList
// DESCRIPTION	: PUPULATE RADIO BUTTONS LIST FOR ENABLING OR DISABALING ROOMING UPLOAD
// ====================================================================================

 
int CSettingsRadioList(CSettings* pMe) 
{
	int result=0;
	IWidget* backdropWidget=NULL;
	AECHAR* sstr=NULL;
	WidgetPos wpos;
	WExtent we;
	AEEDeviceInfo  deviceInfo;
	IWidget* rlist=NULL;
	IModel *iModal = NULL;
	int nWdtHeight=0;
	int nWdtWidth=0;
	AECHAR format[]={'%','s',' ','\0'};


	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->radioList);
	result +=  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->radioListContainer);
    result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->renable);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->rdisable);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_LISTWIDGET, (void**)&rlist);
	//create Enable Text Widget
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->enable);
	//create Disable text widget
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->disable);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MODEL, (void**)&iModal);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModel1);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModel2);
	
	IFORM_SetResText((IForm*)pMe->radioList, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
	IFORM_SetSoftkeys((IForm*)pMe->radioList, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
	
	IWIDGET_SetProperty(pMe->renable, PROP_VIEWMODEL, (uint32)iModal);
	IWIDGET_SetProperty(pMe->rdisable, PROP_VIEWMODEL, (uint32)iModal);

	IWIDGET_SetModel(pMe->renable, IVALUEMODEL_TO_IMODEL(pMe->valueModel1));
	IWIDGET_SetModel(pMe->rdisable, IVALUEMODEL_TO_IMODEL(pMe->valueModel2));

	pMe->enableText = (AECHAR*) MALLOC(120);
	pMe->disableText = (AECHAR*) MALLOC(120);

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
    IFORM_SetText((IForm*)pMe->radioList,FID_TITLE, pMe->str3);

	FREEIF(pMe->str3);
	FREEIF(sstr);
	
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

	IFORM_SetWidget((IForm*)pMe->radioList, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->radioList, IDF_ALIGN_FILL);

	//Adding the text field to the container
	wpos.x = deviceInfo.cxScreen/2;
    wpos.y = deviceInfo.cyScreen/5;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->radioListContainer, pMe->renable, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen/2+deviceInfo.cxScreen/8;
    wpos.y = deviceInfo.cyScreen/5;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->radioListContainer, pMe->enable, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen/2;
    wpos.y = deviceInfo.cyScreen/3;     
	IXYCONTAINER_Insert(pMe->radioListContainer, pMe->rdisable, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen/2+deviceInfo.cxScreen/8;
    wpos.y = deviceInfo.cyScreen/3;
    wpos.bVisible = TRUE;
	IXYCONTAINER_Insert(pMe->radioListContainer, pMe->disable, WIDGET_ZNORMAL, &wpos);

	// Attach the contaner to the dialog
	result = IXYCONTAINER_QueryInterface(pMe->radioListContainer, AEEIID_WIDGET, (void**)&pMe->containerWidget);
	if(result ==0) 
	{	
		IFORM_SetWidget((IForm*)pMe->radioList, WID_FORM, pMe->containerWidget);
	}


	HANDLERDESC_Init(&pMe->radioListHandler, radioList_HandleEvent, pMe, deleteRoomingState);
	IFORM_SetHandler((IForm*)pMe->radioList, &pMe->radioListHandler);
	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->radioList);
	IWIDGET_Release(rlist);
	IMODEL_Release(iModal);
	FREEIF(sstr);
	
	return TRUE;
}



// ================================================================================
// FUNCTION		: MessageForm_Delete
// DESCRIPTION	: DELETE ALL RESOURECES USED IN SHEDULER FORM(RADIO BUTTIONS POPULATED)
// ================================================================================

void deleteShdulerForm(CSettings* pMe)
{
	if(pMe->shedulerList)
	{
		IFORM_Release(pMe->shedulerList);
		pMe->shedulerList=NULL;
	}
	if(pMe->minutes)
	{
		IWIDGET_Release(pMe->minutes);
		pMe->minutes=NULL;
	}
	if(pMe->hour)
	{
		IWIDGET_Release(pMe->hour);
		pMe->hour=NULL;
	}
	if(pMe->seconds)
	{
		IWIDGET_Release(pMe->seconds);
		pMe->seconds=NULL;
	}
	if(pMe->containerWidget)
	{
		IWIDGET_Release(pMe->containerWidget);
		pMe->containerWidget=NULL;
	}
	
	if(pMe->valueModel3)
	{
		IVALUEMODEL_Release(pMe->valueModel3);
		pMe->valueModel3=NULL;
	}
	if(pMe->valueModel4)
	{
		IVALUEMODEL_Release(pMe->valueModel4);
		pMe->valueModel4=NULL;
	}
	if(pMe->valueModel5)
	{
		IVALUEMODEL_Release(pMe->valueModel5);
		pMe->valueModel5=NULL;
	}
	if(pMe->containerWidget)
	{
		IWIDGET_Release(pMe->containerWidget);
		pMe->containerWidget=NULL;
	}
	if(pMe->rangeTitle)
	{
		IWIDGET_Release(pMe->rangeTitle);
		pMe->rangeTitle=NULL;
	}
	if(pMe->shedulerFormTitle)
	{
		IWIDGET_Release(pMe->shedulerFormTitle);
		pMe->shedulerFormTitle=NULL;
	}
	if(pMe->rangeCreateTextWidget)
	{
		IWIDGET_Release(pMe->rangeCreateTextWidget);
		pMe->rangeCreateTextWidget=NULL;
	}
	if(pMe->shedulerContainer)
	{
		IXYCONTAINER_Release(pMe->shedulerContainer);
		pMe->shedulerContainer=NULL;
	}	
	if(pMe->hourtext)
	{
		IWIDGET_Release(pMe->hourtext);
		pMe->hourtext=NULL;
	}
	if(pMe->minutestext)
	{
		IWIDGET_Release(pMe->minutestext);
		pMe->minutestext=NULL;
	}
	if(pMe->secondstext)
	{
		IWIDGET_Release(pMe->secondstext);
		pMe->secondstext=NULL;
	}
	if(pMe->dialog)
	{
		IDIALOG_Release(pMe->dialog);
		pMe->dialog=NULL;
	}
}

// ================================================================================
// FUNCTION		: shedulerRefreshDown
// DESCRIPTION	: REFRESH RADIO BUTTION WHEN DOWN KEY PRESSED
// ================================================================================

void shedulerRefreshDown(CSettings* pMe)
{
	int result =0;
	result = IXYCONTAINER_QueryInterface(pMe->shedulerContainer, AEEIID_WIDGET, (void**)&pMe->containerWidget);
	if(result ==0) 
		{
			switch(pMe->nFocusWdt)
			{
				case (0):
				
					IFORM_SetWidget(pMe->shedulerList, WID_FORM, pMe->containerWidget);
					IWIDGET_MoveFocus(pMe->containerWidget, pMe->minutes);
					IWIDGET_Release(pMe->containerWidget);
					pMe->nFocusWdt++;
					break;
				case (1):
				
					IFORM_SetWidget(pMe->shedulerList, WID_FORM, pMe->containerWidget);
					IWIDGET_MoveFocus(pMe->containerWidget, pMe->seconds);
					IWIDGET_Release(pMe->containerWidget);
					pMe->nFocusWdt++;
					break;
				case (2):
										
					IFORM_SetWidget(pMe->shedulerList, WID_FORM, pMe->containerWidget);
					IWIDGET_MoveFocus(pMe->containerWidget, pMe->rangeCreateTextWidget);
					IWIDGET_Release(pMe->containerWidget);
					pMe->nFocusWdt++;
					break;
				
			}

		}
}

// ================================================================================
// FUNCTION		: shedulerRefreshUp
// DESCRIPTION	: REFRESH RADIO BUTTION WHEN UP KEY PRESSED
// ================================================================================

void shedulerRefreshUp(CSettings* pMe)
{
	int result =0;
	result = IXYCONTAINER_QueryInterface(pMe->shedulerContainer, AEEIID_WIDGET, (void**)&pMe->containerWidget);
	if(result ==0) 
	{
				
		switch(pMe->nFocusWdt)
		{
			case (1):
			
				IFORM_SetWidget(pMe->shedulerList, WID_FORM, pMe->containerWidget);
				IWIDGET_MoveFocus(pMe->containerWidget, pMe->hour);
				IWIDGET_Release(pMe->containerWidget);
				pMe->nFocusWdt--;
				break;
			case (2):
			
				IFORM_SetWidget(pMe->shedulerList, WID_FORM, pMe->containerWidget);
				IWIDGET_MoveFocus(pMe->containerWidget, pMe->minutes);
				IWIDGET_Release(pMe->containerWidget);
				pMe->nFocusWdt--;
				break;
			case (3):
									
				IFORM_SetWidget(pMe->shedulerList, WID_FORM, pMe->containerWidget);
				IWIDGET_MoveFocus(pMe->containerWidget, pMe->seconds);
				IWIDGET_Release(pMe->containerWidget);
				pMe->nFocusWdt--;
				break;
			
		}

				
	}
}

// ================================================================================
// FUNCTION		: sheduler_HandleEvent
// DESCRIPTION	: HANDLE EVENT FROM CSettingsRadioListForSheduler
// ================================================================================

static boolean sheduler_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	CSettings* pMe = (CSettings*) po;
	AECHAR* rangetext=NULL;
	char* rangetextchar=NULL;
	char* state=NULL;
	unsigned long time=0;
	user *pUser=NULL;
	IWidget* containerWidget = NULL; 
	int result=0;
	int size=0;
	int nLen=0;
	int i=0;
	AECHAR* sstr=NULL;
	AECHAR* title=NULL;
	AECHAR format[]={'%','u',' ',' ','\0'};
	if(evt == EVT_WDG_GETPROPERTY && wParam == FID_PREFRECT) 
	{
		//catch FID_PREFRECT and set preferred extent of menu

		AEERect rc;
		rc.x = pMe->DeviceInfo.cyScreen/10;
		rc.y = pMe->DeviceInfo.cyScreen*15/40;
		rc.dx = pMe->DeviceInfo.cxScreen;
		rc.dy = pMe->DeviceInfo.cyScreen-pMe->DeviceInfo.cyScreen/3;
		*(AEERect*) dwParam = rc;

		//setting current selection
		if (GetUserData(pMe->pIShell, &pUser))
		{
			state=pUser->sheduler;
			time=pUser->shedulePeriod;	
		}
		if ( 0==STRCMP("h", state))
		{		
			IVALUEMODEL_SetBool(pMe->valueModel3,TRUE);
			time= time/3600000;
		}
		if ( 0==STRCMP("m", state))
		{
			IVALUEMODEL_SetBool(pMe->valueModel4,TRUE);
			time= time/60000;
		}
		if ( 0==STRCMP("s", state))
		{
			IVALUEMODEL_SetBool(pMe->valueModel5,TRUE);
			time= time/1000;
		}

		rangetext =(AECHAR*) MALLOC(200);
		WSPRINTF(rangetext, 200, format,time);  
		IWIDGET_SetTextWidgetText(pMe->rangeCreateTextWidget, rangetext);

		FREEIF(pUser);
		FREEIF(rangetext);
		return TRUE;
	}
	
	if(evt==EVT_KEY)
	{
		if(wParam == AVK_CLR || wParam == AVK_SOFT2)
		{
			IROOTFORM_PopForm(pMe->rootForm);
			deleteShdulerForm(pMe);
			IROOTFORM_PopForm(pMe->rootForm);
			deleteDialog(pMe);
			return TRUE;
					
		 }

		if(evt == EVT_KEY && wParam == AVK_SOFT1)
		{
			IROOTFORM_PopForm(pMe->rootForm);
			
			IROOTFORM_PopForm(pMe->rootForm);
			deleteDialog(pMe);
			if(pMe->dialog){
			IDIALOG_Release(pMe->dialog);
			pMe->dialog=NULL;
			}
			pMe->dialogEvent=3;
			
			sstr = (AECHAR*)MALLOC(250);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHEDULERSET,sstr , 250);
			title=(AECHAR*)MALLOC(30);
			ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHEDULER,title , 30);

			CSettings_Dialog(pMe,sstr,title);
			FREEIF(sstr);
			FREEIF(title);					
			return TRUE;
				
		}
			
		if(wParam == AVK_DOWN)
		{			
			shedulerRefreshDown(pMe);			
			return TRUE;					
		}						

		if(wParam == AVK_UP)
		{
			shedulerRefreshUp(pMe);	
			return TRUE;
		}	   
	}
	
   //the  default form handler is swapped with the AppForm handler
   // calling this allows the default form handler to handle the event
	return HANDLERDESC_Call(&pMe->radioListHandler, evt, wParam, dwParam);
}

// ================================================================================
// FUNCTION		: CSettingsRadioListForSheduler
// DESCRIPTION	: POPULATE RIDIO BUTTONS FOR SHEDULER SETTING
// ================================================================================
int CSettingsRadioListForSheduler(CSettings* pMe) 
{
	int result=0;
	IWidget* backdropWidget=NULL;
	IWidget* backWidget = NULL;
	AECHAR* sstr=NULL;
	WidgetPos wpos;
	WExtent we;
	AEEDeviceInfo  deviceInfo;
	IModel *iModel = NULL;
    int nWdtHeight=0;
	int nWdtWidth=0;
	AECHAR format[]={'%','s',' ','\0'};


	result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_POPUPFORM, (void**) &pMe->shedulerList);
	result =  ISHELL_CreateInstance(pMe->pIShell, AEECLSID_XYCONTAINER, (void**) &pMe->shedulerContainer);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->hour);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->minutes);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_RADIOWIDGET, (void**)&pMe->seconds);
	//create Enable Text Widget
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->hourtext);
	//create Disable text widget
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->minutestext);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->secondstext);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->rangeTitle);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void**)&pMe->shedulerFormTitle);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_TEXTWIDGET, (void**)&pMe->rangeCreateTextWidget);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_MODEL, (void**)&iModel);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModel3);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModel4);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_VALUEMODEL, (void**)&pMe->valueModel5);
	
	IFORM_SetResText((IForm*)pMe->shedulerList, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB);
	IFORM_SetSoftkeys((IForm*)pMe->shedulerList, RIPPLEVAULT_RES_FILE, IDS_OK, IDS_CANCEL);
	
	IWIDGET_SetProperty(pMe->hour, PROP_VIEWMODEL, (uint32)iModel);
	IWIDGET_SetProperty(pMe->minutes, PROP_VIEWMODEL, (uint32)iModel);
	IWIDGET_SetProperty(pMe->seconds, PROP_VIEWMODEL, (uint32)iModel);

	IWIDGET_SetModel(pMe->hour, IVALUEMODEL_TO_IMODEL(pMe->valueModel3));
	IWIDGET_SetModel(pMe->minutes, IVALUEMODEL_TO_IMODEL(pMe->valueModel4));
	IWIDGET_SetModel(pMe->seconds, IVALUEMODEL_TO_IMODEL(pMe->valueModel5));

	pMe->hourText = (AECHAR*) MALLOC(30);
	pMe->minutesText = (AECHAR*) MALLOC(30);
	pMe->secondsText = (AECHAR*) MALLOC(30);
	pMe->rangeTitleText = (AECHAR*) MALLOC(30);
	pMe->shedulerFormTitleText = (AECHAR*) MALLOC(30);
	
    if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_HOUR, pMe->hourText, 30) ) return -1;
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_MINUTES, pMe->minutesText, 30) ) return -1;
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SECONDS, pMe->secondsText, 30) ) return -1;
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHEDULERRANGE, pMe->rangeTitleText, 30) ) return -1;
	if (0==ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_SHEDULER, pMe->shedulerFormTitleText, 30) ) return -1;

	IWIDGET_SetText(pMe->hourtext, pMe->hourText, TRUE);
	IWIDGET_SetText(pMe->minutestext, pMe->minutesText, TRUE);
	IWIDGET_SetText(pMe->secondstext, pMe->secondsText, TRUE);
	IWIDGET_SetText(pMe->rangeTitle, pMe->rangeTitleText, TRUE);
	IWIDGET_SetText(pMe->shedulerFormTitle, pMe->shedulerFormTitleText, TRUE);

    ISHELL_GetDeviceInfo(pMe->pIShell, &deviceInfo);
	nWdtHeight = (deviceInfo.cyScreen/12)*8;
	nWdtHeight = nWdtHeight/6;
	nWdtHeight = nWdtHeight;
	nWdtWidth = (deviceInfo.cyScreen/10)*8;

	IWIDGET_SetExtent(pMe->hourtext,&we);
	IWIDGET_SetFlags(pMe->hourtext, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->hourtext, MAKE_RGB(252, 252, 252));

	IWIDGET_SetExtent(pMe->minutestext,&we);
	IWIDGET_SetFlags(pMe->minutestext, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->minutestext, MAKE_RGB(252, 252, 252));

	IWIDGET_SetExtent(pMe->secondstext,&we);
	IWIDGET_SetFlags(pMe->secondstext, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->secondstext, MAKE_RGB(252, 252, 252));

	IWIDGET_SetExtent(pMe->rangeTitle,&we);
	IWIDGET_SetFlags(pMe->rangeTitle, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->rangeTitle, MAKE_RGB(252, 252, 252));

	IWIDGET_SetExtent(pMe->shedulerFormTitle,&we);
	IWIDGET_SetFlags(pMe->shedulerFormTitle, IDF_ALIGN_LEFT);
	IWIDGET_SetBorderColor(pMe->shedulerFormTitle, MAKE_RGB(252, 252, 252));

	pMe->str4 = (AECHAR*)MALLOC(120);
	sstr = (AECHAR*)MALLOC(110);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_RIPPLEPUB,sstr , 110);
	WSPRINTF(pMe->str4, 120, format,sstr);//, pMe->itemCount);
    IFORM_SetText((IForm*)pMe->shedulerList,FID_TITLE, pMe->str4);
	FREEIF(pMe->str4);
	
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->hour,&we);
	IWIDGET_SetBorderColor(pMe->hour, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->hour, 1);
    IWIDGET_SetFlags(pMe->hour, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);
	
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->hourtext,&we);
	IWIDGET_SetBorderColor(pMe->hourtext, MAKE_RGB(100, 100, 100));
	
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->minutestext,&we);
	IWIDGET_SetBorderColor(pMe->minutestext, MAKE_RGB(100, 100, 100));
   
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->minutes,&we);
	IWIDGET_SetBorderColor(pMe->minutes, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->minutes, 1);
    IWIDGET_SetFlags(pMe->minutes, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->seconds,&we);
	IWIDGET_SetBorderColor(pMe->seconds, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->seconds, 1);
    IWIDGET_SetFlags(pMe->seconds, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);

	we.height = nWdtHeight;
    we.width = deviceInfo.cxScreen/2;
	IWIDGET_SetExtent(pMe->rangeCreateTextWidget,&we);
	IWIDGET_SetBorderColor(pMe->rangeCreateTextWidget, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->rangeCreateTextWidget, 1);
    IWIDGET_SetFlags(pMe->rangeCreateTextWidget, IDF_ALIGN_RIGHT | IDF_ALIGN_MIDDLE);
	
	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->secondstext,&we);
	IWIDGET_SetBorderColor(pMe->secondstext, MAKE_RGB(100, 100, 100));
	
	we.height = nWdtHeight;
    we.width = deviceInfo.cxScreen;
	IWIDGET_SetExtent(pMe->rangeTitle,&we);
	IWIDGET_SetBorderColor(pMe->rangeTitle, MAKE_RGB(100, 100, 100));

	we.height = nWdtHeight;
    we.width = nWdtWidth;
	IWIDGET_SetExtent(pMe->shedulerFormTitle,&we);
	IWIDGET_SetBorderColor(pMe->shedulerFormTitle, MAKE_RGB(100, 100, 100));
    IWIDGET_SetBorderWidth(pMe->shedulerFormTitle, 1);
    IWIDGET_SetFlags(pMe->shedulerFormTitle, IDF_ALIGN_LEFT | IDF_ALIGN_MIDDLE);

	IFORM_SetWidget((IForm*)pMe->shedulerList, WID_BACKDROP, backdropWidget);
	IWIDGET_SetFlags((IWidget*)pMe->shedulerList, IDF_ALIGN_FILL);

	//Adding the text field to the container
	wpos.x = deviceInfo.cxScreen/200;
    wpos.y = 0;
    wpos.bVisible = TRUE;
	result +=IXYCONTAINER_Insert(pMe->shedulerContainer, pMe->hour, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen/200+deviceInfo.cxScreen*2/10;
    wpos.y =0;
    wpos.bVisible = TRUE;
	result +=IXYCONTAINER_Insert(pMe->shedulerContainer, pMe->hourtext, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen/200;
    wpos.y = deviceInfo.cyScreen*2/20;
    wpos.bVisible = TRUE;	     
	result +=IXYCONTAINER_Insert(pMe->shedulerContainer, pMe->minutes, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen*2/10+deviceInfo.cxScreen/200;
    wpos.y = deviceInfo.cyScreen*2/20;
    wpos.bVisible = TRUE;
	result +=IXYCONTAINER_Insert(pMe->shedulerContainer, pMe->minutestext, WIDGET_ZNORMAL, &wpos);
	
	wpos.x = deviceInfo.cxScreen/200;
    wpos.y = deviceInfo.cyScreen*2/20+deviceInfo.cyScreen*1/10;
    wpos.bVisible = TRUE;	     
	result +=IXYCONTAINER_Insert(pMe->shedulerContainer, pMe->seconds, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen*2/10+deviceInfo.cxScreen/200;
    wpos.y = deviceInfo.cyScreen*2/20+deviceInfo.cyScreen*1/10;
    wpos.bVisible = TRUE;
	result +=IXYCONTAINER_Insert(pMe->shedulerContainer, pMe->secondstext, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen/200;
    wpos.y = deviceInfo.cyScreen/5+deviceInfo.cyScreen*1/10 ;
    wpos.bVisible = TRUE;
	result +=IXYCONTAINER_Insert(pMe->shedulerContainer, pMe->rangeTitle, WIDGET_ZNORMAL, &wpos);

	wpos.x = deviceInfo.cxScreen/200;
    wpos.y = deviceInfo.cyScreen*2/5;
    wpos.bVisible = TRUE;
	result +=IXYCONTAINER_Insert(pMe->shedulerContainer, pMe->rangeCreateTextWidget, WIDGET_ZNORMAL, &wpos);
	// Attach the contaner to the dialog
	result += IXYCONTAINER_QueryInterface(pMe->shedulerContainer, AEEIID_WIDGET, (void**)&pMe->containerWidget);
	
	if(result ==0) 
	{
		IFORM_SetWidget((IForm*)pMe->shedulerList, WID_FORM, pMe->containerWidget);
		IWIDGET_MoveFocus(pMe->containerWidget, pMe->hour);
	}

	pMe->count=0;
	HANDLERDESC_Init(&pMe->radioListHandler, sheduler_HandleEvent, pMe, deleteShdulerForm);
	IFORM_SetHandler((IForm*)pMe->shedulerList, &pMe->radioListHandler);

	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->shedulerList);
	shedulerRefreshDown(pMe);
	shedulerRefreshUp(pMe);
	IMODEL_Release(iModel);
	FREEIF(sstr);
	
	return TRUE;
}


