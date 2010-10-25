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
 Module Name				: PhotoForm.h
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 05-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name      :EC-GEN-USE-2005-07-20-Photo Backup V1.0.doc                          :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Handle photo upload, photo setting ... etc.. (header)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#ifndef __PHOTOFORM_H__
#define __PHOTOFORM_H__

#include "AEE.h"
#include "AEEForm.h"

//typedef struct PhotoForm PhotoForm;
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
#include "AEEConstraintContainer.h"

#include "PhotoList.h"

#include "RippleVault.bid"
#include "RippleVault.brh"

#include "..\SyncEng\SMEvents.h"

typedef struct _ContactShare
{
	ImageStaticInfo imageInfo;
	AECHAR pwzPhoneNo[20];
	boolean status;
}ContactShare;

typedef struct _PhotoForm {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IShell        *pIShell;    // give a standard way to access the Shell interface
	IPropContainer * mainContainer;
	IConstraintContainer  *pIConstraintContainer;
	IPropContainer *pAlbumListContainer;//IXYContainer
	IXYContainer *m_pDialogContainer;

	IForm *pAlbumSelectForm;

	//store the selected sub form
	int nSubForm;

	IRootForm* rootForm;
	IForm* photoViewForm;
	IForm* PhotoList;
	IForm *pSelectAlbumMenu;
	IForm* PhotoSettings;

	IMenuModel *pShareAlbumMenuModel;
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
	ModelListener AlbumSelectMenuListener;

	IPopupMenu* optionsMenu;
	HandlerDesc optionsMenuHandler;

	IPopupMenu* shareAlbumPopupMenu;
	HandlerDesc shareAlbumHandler;
	int m_nShareAlbum;

	HandlerDesc selectAlbumHandler;

	IPopupMenu* selectContactPopupMenu;
	HandlerDesc selectContactHandler;

	//structure to hold the data of album share contacts
	ContactShare **pTmpContAr;
	ImageStaticInfo **pShareAlbumsAr;
	boolean **shareStatusAr;
	int m_noOfAlbumSharedContacts;
	int m_nNoOfAlbums;

	IForm *m_messageDlg;
	IWidget	*m_messageWidget;
	HandlerDesc messageDlgHandler;
	AECHAR *m_pwzDialogTitle;

	//progress bar
	IDialog* m_photoShareProgressDlg;
	HandlerDesc progressDlgHandler;

	//sync module
	ISyncEngExt* m_pSyncExt;

	//user settings
	user *userSettings;

} PhotoForm;

boolean PhotoForm_SendAlbumToServer(PhotoForm* pMe);
void PhotoForm_AlbumShareSelectHandle(PhotoForm* pMe, int idOption);

int PhotoForm_CreateProgressDialog(PhotoForm* pMe);
boolean PhotoForm_ProgressDlgHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam);

boolean PhotoForm_MessageDlgHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam);
int PhotoForm_GetMessageDlg(PhotoForm* pMe);
boolean PhotoForm_ShowSelectContactPopupMenu(PhotoForm* pMe);
boolean PhotoForm_SelectContactHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam);
void PhotForm_AlbumSelectEH(PhotoForm *pMe, ModelEvent *pev);

boolean PhotoForm_SelectAlbumToShare(PhotoForm* pMe);
boolean PhotoForm_SelectAlbumHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam);

boolean PhotoForm_ShareAlbumMenu(PhotoForm* pMe);
boolean PhotoForm_AlbumShareHandleEvent(PhotoForm *pThis, AEEEvent eCode, uint16 wParam, uint32 dwParam);

int PhotoForm_New(IForm **ppo, IShell *piShell, IRootForm *pRootForm);
int PhotoFormOptionsMenu(PhotoForm* pMe);

#endif