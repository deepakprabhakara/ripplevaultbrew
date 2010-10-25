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
 Module Name				: VideoForm.cpp
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 05-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name      :EC-GEN-USE-2005-07-20-Update Photo Video Settings V1.0.doc                          :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Handle video upload, photo setting ... etc.. (header)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#include "AEEModGen.h"        
#include "AEEAppGen.h"        
#include "AEEShell.h"         

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

#include "VideoForm.h"

#include "RippleVault.bid"
#include "RippleVault.brh"


typedef struct _VideoForm {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IShell        *pIShell;    // give a standard way to access the Shell interface
	IPropContainer * mainContainer;

	IRootForm* rootForm;
	IForm* VideoForm;

	HandlerDesc mainFormHandler;

	AECHAR* str1;
	AECHAR* str2;
	AECHAR* str3;
	AECHAR* str4;
	AECHAR* str5;
	AECHAR* str6;

	IImage* m_pIImage1;
	IImage* m_pIImage2;
	IImage* m_pIImage3;
	IImage* m_pIImage4;
	IImage* m_pIImage5;
	IImage* m_pIImage6;
	
	ImageStaticInfo imageInfo1;
	ImageStaticInfo imageInfo2;
	ImageStaticInfo imageInfo3;
	ImageStaticInfo imageInfo4;
	ImageStaticInfo imageInfo5;
	ImageStaticInfo imageInfo6;

	ModelListener menuListener; 

} VideoForm;

#define RIPPLEVAULT_RES_FILE "RippleVault.bar"

// ================================================================================
// FUNCTION		: VideoForm_Delete
// DESCRIPTION	: Delete resources associated with video form.
// ================================================================================
void VideoForm_Delete(VideoForm* pMe) 
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

   if(pMe)
      FREE(pMe);
}


static void MenuModal_EventHandler(VideoForm *pMe, ModelEvent *pev) 
{

	if(pev->evCode == EVT_MDL_FOCUS_SELECT) 
		if(pev->dwParam == 0)	//user has selected contacts
		{

		}
}

// ================================================================================
// FUNCTION		: VideoForm_HandleEvent
// DESCRIPTION	: Video form event handler
// ================================================================================
static boolean VideoForm_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
   VideoForm* pMe = (VideoForm*) po;
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
			}
		}
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
// FUNCTION		: VideoForm_PopulateMainContainer
// DESCRIPTION	: create widgets im video form
// ================================================================================
int VideoForm_PopulateMainContainer(VideoForm* pMe) 
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

	//load the strings
	pMe->str1 = (AECHAR*) MALLOC(30);
	pMe->str2 = (AECHAR*) MALLOC(40);
	pMe->str3 = (AECHAR*) MALLOC(30);
	pMe->str4 = (AECHAR*) MALLOC(40);
	pMe->str5 = (AECHAR*) MALLOC(30);
	pMe->str6 = (AECHAR*) MALLOC(20);

	result = ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VVIEWGAL, pMe->str1, 30);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VSENTVAULT, pMe->str2, 40);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VGETVAULT, pMe->str3, 30);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VSHARE, pMe->str4, 40);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VUNSHARE, pMe->str5, 30);
	result += ISHELL_LoadResString(pMe->pIShell, RIPPLEVAULT_RES_FILE, IDS_VSETTINGS, pMe->str6, 20);
	result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FONTSYSBOLD, (void**)&piFont);

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
		IFORM_SetWidget((IForm*)pMe->VideoForm, WID_FORM, containerWidget);
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
// FUNCTION		: VideoForm_New
// DESCRIPTION	: create video form
// ================================================================================
int VideoForm_New(IForm** ppo, IShell *piShell,IRootForm *pRootForm) 
{
	int result=0;

	//allocate space for the form
	VideoForm *pMe = MALLOCREC(VideoForm);
	IImage* m_pIImage;

	if(!pMe)
		return ENOMEMORY;

	pMe->pIShell = piShell;
	pMe->rootForm = pRootForm;

	if(ISHELL_CreateInstance(pMe->pIShell, AEECLSID_FORM, (void**) &pMe->VideoForm) == 0)
	{
		*ppo = pMe->VideoForm;
		m_pIImage = ISHELL_LoadResImage(piShell,RIPPLEVAULT_RES_FILE,IDI_TITLE);
		IFORM_SetTitleImage((IForm*)pMe->VideoForm,m_pIImage);
		result = IFORM_SetResText((IForm*)pMe->VideoForm, FID_TITLE, RIPPLEVAULT_RES_FILE, IDS_VIDEOS);
		result =  IFORM_SetSoftkeys((IForm*)pMe->VideoForm, RIPPLEVAULT_RES_FILE, MAINSK1, PHOTOSK2);
		result = VideoForm_PopulateMainContainer(pMe);	//add items to the form

		HANDLERDESC_Init(&pMe->mainFormHandler, VideoForm_HandleEvent, pMe, VideoForm_Delete);
		IFORM_SetHandler((IForm*)pMe->VideoForm, &pMe->mainFormHandler);
	}
	RELEASEIF(m_pIImage);
	return result;
}