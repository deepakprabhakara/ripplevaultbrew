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
 Module Name				: PhotoList.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF,LKA.SRI
 Date Created               : 20-08-2005
 Rational Rose Model Name   :                          
 Use Case Document Name     :                          
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By           :
 Code Reviewed Date			:
 
 Recommendations            :
 

***************************************************************/

#ifndef __PHOTOLIST_H__
#define __PHOTOLIST_H__

#include "AEE.h"
#include "AEEForm.h"
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
#include "AEETextWidget.h"
#include "AEE.h"
#include "AEEFile.h"
#include "UserData.h"
#include "RippleVault.bid"
#include "RippleVault.brh"
#include "..\SyncEng\SyncEng.h"
#include "..\SyncEng\SMEvents.h"
#define BMP_EXTENSION ".bmp"
#define JPG_EXTENSION ".jpg"
#define CONFIG_FILE			"config.txt"


typedef struct _PhotoList {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	IShell        *pIShell;    // give a standard way to access the Shell interface
	IPropContainer * mainContainer;
    IXYContainer* albumCreateContainer;
	IRootForm* rootForm;
	IForm* PhotoList;
	IPopupMenu* optionsMenu;
	IPopupMenu* albumMenu;
	IDialog* confirmDialog;
	IDialog* unSelectDialog;
	IForm* newAlbumCreateDialog;
	IDialog* createNewAlbumDialog;
	AEEDeviceInfo  DeviceInfo;
	IWidget* newAlbumWidget;
	IWidget* AlbumCreateStaticText;
	IWidget* AlbumCreateTextWidget;
	HandlerDesc mainFormHandler;
	HandlerDesc optionsMenuHandler;
	HandlerDesc albumMenuHandler;
	HandlerDesc confirmDlgHandler;
	HandlerDesc unSelectDlgHandler;
	HandlerDesc newAlbumDlgHandler;
	HandlerDesc newAlbumTextHandler;
	HandlerDesc createNewAlbumDlgHandler;
	AECHAR* str1;
	AECHAR* str2;
    AECHAR* str3;
	AECHAR* adstr;
	char* newAlbumName;
	char *pszPhotoUploadMsg;
	AECHAR** strAr;
	AECHAR** album;
	IImage* m_pIImage1;
	IImage* m_pIImage2;
	ImageStaticInfo *imageInfoAr;
	ImageStaticInfo imageInfo1;// = NULL;
	ImageStaticInfo imageInfo2;// = NULL;
	ModelListener menuListener; //listner for the menu model
	ModelListener popListener; //listner for the pop up menu
	int selectedItem;	//selected menu option in the popup menu
	int itemCount;	// selected item count in the list
	int totalItems;	//total items in the list
    int nAlbum;
	IDialog* progressDialog;
	HandlerDesc progressHandler;
	int m_nCurrentUploadPhotoNo;
	char m_pszAlbumName[50];
	ISyncEngExt* m_pSyncExt;
	//user settings
	user *userSettings;
	//photo upload dialog message
	IForm *photoUploadMsgDlg;
	IXYContainer *msgDlgContainer;
	HandlerDesc msgDlgHandler;
	IWidget *uploadMsgWgt;
} PhotoList;

int PhotoList_PhotoUploadMsgDlg(PhotoList* pMe);
static void PhotoList_UploadPhoto(PhotoList* pMe);
int PhotoList_New(IForm **ppo, IShell *piShell, IRootForm *pRootForm);
int PhotoList_PopulateMainContainer(PhotoList* pMe) ;
int PhotoListOptionsMenu(PhotoList* pMe);
int ShowAllPhotos(PhotoList* pMe);
int SelectListItems(PhotoList* pMe,int iIndex);
int PhotoListAlbumMenu(PhotoList* pMe);
int PhotoList_CreateConfirmDialog(PhotoList* pMe);
int PhotoList_CreateNewAlumDialog(PhotoList* pMe);
int NewAlbumCreate(PhotoList* pMe);
void AlbumName(PhotoList* pMe);
void deleteAlbumList(PhotoList* pMe);
int PhotoList_UnSelectInformDialog(PhotoList* pMe);
int PhotoList_CreateNewAlbumDialog(PhotoList* pMe);
static void PhotoList_AlbumSelectionHandler(PhotoList* pMe, int idOption);
static boolean newAlbumCreateDialog_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam);
int AlbumList_Setect(PhotoList* pMe,int option);
static boolean newAlbumCreateDialog_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam);
uint32 ReadAlbumFileStream(IShell * pIShell, const char *pszPath, char **pszBuf);
static boolean PLProgressDlg_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam);
int PhotoList_CreateProgressDialog(PhotoList* pMe, uint16 nResId);

#endif