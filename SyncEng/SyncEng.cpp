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
 Module Name				: SyncEng.cpp
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : SRI
 Date Created               : 02-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#include "synceng.h"
#include "synceng.bid"

#include "AEEStdlib.h"

#include "SMCommon.h"
#include "ContactHandler.h"
#include "Util.h"
#include "DbHandler.h"

/*===============================================================================
CLASS DEFINITIONS
=============================================================================== */
typedef struct _SyncEngExt
{
	// Declare VTable
	DECLARE_VTBL(ISyncEngExt)//DECLARE_VTBL()INotifier

	// Class member variables
	uint32		m_nRefs;		// References to us
	IShell *	m_pIShell;		// copy of Shell pointer
	IModule *	m_pIModule;		// IModule interface pointer

	CContactHandler *	m_pContHandler;
	
}SyncEngExt;

// Exported AEEClsCreateInstance
extern int			AEEClsCreateInstance(AEECLSID, IShell*, IModule*, void**);

/*-------------------------------------------------------------------
Function Prototypes
-------------------------------------------------------------------*/
static uint32		SyncEngExt_AddRef(ISyncEngExt * po);
static uint32		SyncEngExt_Release(ISyncEngExt * po);

static int			SyncEngExt_SyncContacts(ISyncEngExt * po);
static int			SyncEngExt_RegisterNewUser(ISyncEngExt * po, char *uname, char *phone, char *upass);
static boolean		SyncEngExt_HandleEvent(ISyncEngExt * po, uint16 eCode,uint16 wParam, uint32 dwParam);
static boolean		SyncEngExt_IsRegistered(ISyncEngExt * po);
static uint32		SyncEngExt_GetSheduleTime(ISyncEngExt * po);
static boolean		SyncEngExt_UploadPhoto(ISyncEngExt * po, const char *pszPhoto, const char *pszAlbum, const char *pszDes);
static boolean		SyncEngExt_SyncAlbum(ISyncEngExt * po);
static int			SyncEngExt_GetContacts(ISyncEngExt * po, AECHAR **pszContactsList);
static boolean		SyncEngExt_ShareAlbum(ISyncEngExt * po, AECHAR *pwzAlbum, AECHAR *pwzContactsList, AECHAR *pwzMessage);
static boolean		SyncEngExt_SaveSms(ISyncEngExt * po, AECHAR *pszFromNo, AECHAR *pszMsg, AECHAR *pszTime);
static int			SyncEngExt_GetSms(ISyncEngExt * po, sms **pSmsAr, int nTotal, char **pFilterAr, int nNo);
static int			SyncEngExt_UploadSms(ISyncEngExt * po, sms **pSmsAr, int nNo);

// ================================================================================
// FUNCTION		: SyncEngExt_New
// DESCRIPTION	: Construction of the class and initialization of it's members
// ================================================================================
int SyncEngExt_New(int16 nSize, IShell *pIShell, IModule* pIModule, IModule ** ppMod)
{
	SyncEngExt *			   pMe = NULL;
	
	VTBL(ISyncEngExt) *	modFuncs;

	if( !ppMod || !pIShell || !pIModule )
		return EFAILED;

	*ppMod = NULL;
	

	// Allocate memory for the ExtensionCls object
	if( nSize < sizeof(SyncEngExt) )
		nSize += sizeof(SyncEngExt);

	if( (pMe = (SyncEngExt *)MALLOC(nSize + sizeof(ISyncEngExtVtbl))) == NULL )
		return ENOMEMORY;

	// Allocate the vtbl and initialize it. Note that the modules and apps must not
	// have any static data. Hence, we need to allocate the vtbl as well.
	modFuncs = (ISyncEngExtVtbl *)((byte *)pMe + nSize);
	
	//Initialize individual entries in the VTBL
	modFuncs->AddRef			= SyncEngExt_AddRef;
	modFuncs->Release			= SyncEngExt_Release;
	
	modFuncs->SyncContacts		= SyncEngExt_SyncContacts;
	modFuncs->RegisterNewUser	= SyncEngExt_RegisterNewUser;
	modFuncs->HandleEvent		= SyncEngExt_HandleEvent;
	modFuncs->IsRegistered		= SyncEngExt_IsRegistered;
	modFuncs->GetSheduleTime	= SyncEngExt_GetSheduleTime;
	modFuncs->UploadPhoto		= SyncEngExt_UploadPhoto;
	modFuncs->SyncAlbum			= SyncEngExt_SyncAlbum;
	modFuncs->GetContacts		= SyncEngExt_GetContacts;
	modFuncs->ShareAlbum		= SyncEngExt_ShareAlbum;
	modFuncs->SaveSms			= SyncEngExt_SaveSms;
	modFuncs->GetSms			= SyncEngExt_GetSms;
	modFuncs->UploadSms			= SyncEngExt_UploadSms;

	// initialize the vtable
	INIT_VTBL(pMe, IModule, *modFuncs);

	// initialize the data members
	pMe->m_nRefs      = 1;
	pMe->m_pIShell    = pIShell;
	pMe->m_pIModule   = pIModule;
	
	pMe->m_pContHandler = NULL;
	
	ISHELL_AddRef(pIShell);
	IMODULE_AddRef(pIModule);
	
		
	// Set the pointer in the parameter
	*ppMod = (IModule*)pMe;

	return AEE_SUCCESS;
}

// ================================================================================
// FUNCTION		: SyncEngExt_AddRef
// DESCRIPTION	: This function increases the reference count for the SyncEngExt object
// ================================================================================
static uint32 SyncEngExt_AddRef(ISyncEngExt * po)
{
   return (++((SyncEngExt *)po)->m_nRefs);
}

// ================================================================================
// FUNCTION		: SyncEngExt_Release
// DESCRIPTION	: This function decreases the reference count for the SyncEngExt object. If
//				: the reference count reaches zero, it does the required cleanup
// ================================================================================
static uint32 SyncEngExt_Release(ISyncEngExt * po)
{
	SyncEngExt *		pMe = (SyncEngExt *)po;

	//release member variable data
	if ( NULL!=pMe->m_pContHandler ) delete pMe->m_pContHandler;

	if( --pMe->m_nRefs != 0 )
		return pMe->m_nRefs;

	// Ref count is zero. So, release memory associated with this object.

	// Release interfaces
	ISHELL_Release(pMe->m_pIShell);
	IMODULE_Release(pMe->m_pIModule);
	//Free the object itself
	FREE_VTBL(pMe, IModule);
	FREE( pMe );

	return 0;
}


// ================================================================================
// FUNCTION		: AEEClsCreateInstance
// DESCRIPTION	: Create a instance of SyncEngExt class.
// ================================================================================
int AEEClsCreateInstance(AEECLSID ClsId,IShell * pIShell,IModule * po,void ** ppObj)
{
	*ppObj = NULL;
		
	if( ClsId == AEECLSID_SYNCENG )
	{
		if( SyncEngExt_New(sizeof(SyncEngExt), pIShell, po, (IModule **)ppObj) == SUCCESS )
			return AEE_SUCCESS;
	}
	return EFAILED;

}

// ================================================================================
// FUNCTION		: SyncEngExt_SyncContacts
// DESCRIPTION	: Synchronize addressbook with the server.
// ================================================================================
static int SyncEngExt_SyncContacts(ISyncEngExt * po)
{
	SyncEngExt *		pMe = (SyncEngExt *)po;

	if ( NULL==pMe->m_pContHandler )
	{

		pMe->m_pContHandler =  new CContactHandler(pMe->m_pIShell);

		if ( NULL==pMe->m_pContHandler ) return SM_NOMEMORY;
		if ( !pMe->m_pContHandler->Init() ) return SM_NOMEMORY;

	}

	return pMe->m_pContHandler->SyncContacts();

}

// ================================================================================
// FUNCTION		: SyncEngExt_RegisterNewUser
// DESCRIPTION	: Register a user.
// ================================================================================
static int SyncEngExt_RegisterNewUser(ISyncEngExt * po, char* uname, char *phone, char* upass)
{
	SyncEngExt *		pMe = (SyncEngExt *)po;

	if ( NULL!=pMe->m_pContHandler )
	{
		delete pMe->m_pContHandler;
		pMe->m_pContHandler = NULL;
	}

	pMe->m_pContHandler =  new CContactHandler(pMe->m_pIShell);

	if ( NULL==pMe->m_pContHandler ) return SM_NOMEMORY;
	if ( !pMe->m_pContHandler->Init() ) return SM_NOMEMORY;;
	
	pMe->m_pContHandler->RegisterNewUser(uname, phone, upass);
	return SM_SUCCESS;
}

// ================================================================================
// FUNCTION		: SyncEngExt_HandleEvent
// DESCRIPTION	: This function dispath the events to CContactHandler class
// ================================================================================
static boolean SyncEngExt_HandleEvent(ISyncEngExt * po, uint16 eCode,uint16 wParam, uint32 dwParam)
{
	SyncEngExt *		pMe = (SyncEngExt *)po;

	if ( NULL==pMe->m_pContHandler ) return FALSE;

	pMe->m_pContHandler->HandleEvent(eCode, wParam, dwParam);

	return TRUE;
}

// ================================================================================
// FUNCTION		: SyncEngExt_IsRegistered
// DESCRIPTION	: This function returns true if the current user is registered.
// ================================================================================
static boolean SyncEngExt_IsRegistered(ISyncEngExt * po)
{
	boolean st=TRUE;
	user *pUserInfo=NULL;
	if ( CUtil::GetUserInfo(&pUserInfo))
	{
		if ( 0==STRCMP("reg", pUserInfo->status))
			st=FALSE;
	}
	return st;
}

// ================================================================================
// FUNCTION		: SyncEngExt_GetSheduleTime
// DESCRIPTION	: Returns the number of milli seconds seconds to start the synchronization.
// ================================================================================
static uint32 SyncEngExt_GetSheduleTime(ISyncEngExt * po)
{
	uint32 time=0;

	time = CUtil::GetSheduleTime();
	
	return time;
}

// ================================================================================
// FUNCTION		: SyncEngExt_UploadPhoto
// DESCRIPTION	: Upload photos to the server.
// ================================================================================
static boolean SyncEngExt_UploadPhoto(ISyncEngExt * po, const char *pszPhoto, const char *pszAlbum, const char *pszDes)
{
	SyncEngExt *		pMe = (SyncEngExt *)po;

	if ( NULL==pMe->m_pContHandler )
	{

		pMe->m_pContHandler =  new CContactHandler(pMe->m_pIShell);

		if ( NULL==pMe->m_pContHandler ) return SM_NOMEMORY;
		if ( !pMe->m_pContHandler->Init() ) return SM_NOMEMORY;

	}

	if ( NULL!=pMe->m_pContHandler )
	{
		return pMe->m_pContHandler->UploadPhoto(pszPhoto, pszAlbum, pszDes);
	}

	return FALSE;
}

// ================================================================================
// FUNCTION		: SyncEngExt_SyncAlbum
// DESCRIPTION	: Synchronize album names with the RippleVault server.
// ================================================================================
static boolean SyncEngExt_SyncAlbum(ISyncEngExt * po)
{
	SyncEngExt *		pMe = (SyncEngExt *)po;

	if ( NULL==pMe->m_pContHandler )
	{

		pMe->m_pContHandler =  new CContactHandler(pMe->m_pIShell);

		if ( NULL==pMe->m_pContHandler ) return SM_NOMEMORY;
		if ( !pMe->m_pContHandler->Init() ) return SM_NOMEMORY;

	}

	if ( NULL!=pMe->m_pContHandler )
	{
		return pMe->m_pContHandler->SyncAlbums();
	}

	return FALSE;

}

// ================================================================================
// FUNCTION		: SyncEngExt_GetContacts
// DESCRIPTION	: Get the contact names from phone addressbook.
// ================================================================================
static int SyncEngExt_GetContacts(ISyncEngExt * po, AECHAR **pszContactsList)
{
	
	SyncEngExt *		pMe = (SyncEngExt *)po;
	int result=0;
	CDbHandler *addrbookHandler = new CDbHandler(pMe->m_pIShell);
	if ( NULL==addrbookHandler ) return FALSE;

	result = addrbookHandler->GetContactList(pszContactsList);

	delete addrbookHandler;

	return result;
}

// ================================================================================
// FUNCTION		: SyncEngExt_ShareAlbum
// DESCRIPTION	: Send request to server to share the album in phone.
// ================================================================================
static boolean SyncEngExt_ShareAlbum(ISyncEngExt * po, AECHAR *pwzAlbum, AECHAR *pwzContactsList, AECHAR *pwzMessage)
{
	SyncEngExt *		pMe = (SyncEngExt *)po;

	if ( NULL==pMe->m_pContHandler )
	{

		pMe->m_pContHandler =  new CContactHandler(pMe->m_pIShell);

		if ( NULL==pMe->m_pContHandler ) return SM_NOMEMORY;
		if ( !pMe->m_pContHandler->Init() ) return SM_NOMEMORY;

	}

	if ( NULL!=pMe->m_pContHandler )
	{
		return pMe->m_pContHandler->ShareAlbum(pwzAlbum, pwzContactsList, pwzMessage);
	}

	return FALSE;

}

// ================================================================================
// FUNCTION		: SyncEngExt_SaveSms
// DESCRIPTION	: Save sms to database.
// ================================================================================
static boolean SyncEngExt_SaveSms(ISyncEngExt * po, AECHAR *pszFromNo, AECHAR *pszMsg, AECHAR *pszTime)
{
	
	SyncEngExt *		pMe = (SyncEngExt *)po;
	boolean result=TRUE;
	CDbHandler *addrbookHandler = new CDbHandler(pMe->m_pIShell);
	if ( NULL==addrbookHandler ) return FALSE;

	delete addrbookHandler;

	return result;
}

// ================================================================================
// FUNCTION		: SyncEngExt_GetSms
// DESCRIPTION	: Get sms information into a array(pSmsAr). Return parameter is no of items
//				: in the array.
// ================================================================================
static int SyncEngExt_GetSms(ISyncEngExt * po, sms **pSmsAr, int nTotal, char **pFilterAr, int nNo)
{
	
	SyncEngExt *		pMe = (SyncEngExt *)po;
	int result=TRUE;
	
	CDbHandler *addrbookHandler = new CDbHandler(pMe->m_pIShell);
	if ( NULL==addrbookHandler ) return FALSE;

	result = addrbookHandler->GetSms(pSmsAr, nTotal, pFilterAr, nNo);

	delete addrbookHandler;

	return result;
}

// ================================================================================
// FUNCTION		: SyncEngExt_UploadSms
// DESCRIPTION	: Upload smses to server.
// ================================================================================
static int SyncEngExt_UploadSms(ISyncEngExt * po, sms **pSmsAr, int nNo)
{
	SyncEngExt *		pMe = (SyncEngExt *)po;

	if ( NULL==pMe->m_pContHandler )
	{

		pMe->m_pContHandler =  new CContactHandler(pMe->m_pIShell);

		if ( NULL==pMe->m_pContHandler ) return SM_NOMEMORY;
		if ( !pMe->m_pContHandler->Init() ) return SM_NOMEMORY;

	}

	if ( NULL!=pMe->m_pContHandler )
	{
		return pMe->m_pContHandler->UploadSms(pSmsAr, nNo);
	}

	return FALSE;
	
	return SUCCESS;
}
