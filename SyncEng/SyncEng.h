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
 Module Name				: SyncEng.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name   : RVBrewClient.mdl                         :
 Use Case Document Name     : 
 Use Case Realization Document Name   :
 Test Case Document Name      : 
 Abbreviations                :

 Description                  : RippleValut synchronization model- exported functionality header. 
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#ifndef _synceng_h_
#define _synceng_h_

#include "AEE.h"			// Standard AEE definitions
#include "AEEShell.h"		// AEE Shell Services
#include "AEEAppgen.h"		// Madatory include
#include "AEEModgen.h"		// Mandatory include
#include "AEEHeap.h"		// Memory interface
#include "AEEStdlib.h"		// BREW's standard library

#include "SMEvents.h"

// Extension Class typedef
typedef struct _ISyncEngExt ISyncEngExt;

//********************************************************************************************************************************
//
// ISyncEngExt Interface Definition
//
//********************************************************************************************************************************
QINTERFACE(ISyncEngExt)
{
	DECLARE_IBASE(ISyncEngExt)
	int			(*SyncContacts)(ISyncEngExt * po);
	int			(*RegisterNewUser)(ISyncEngExt * po, char *uname, char *phone, char *upass);
	boolean		(*HandleEvent)(ISyncEngExt * po, uint16 eCode,uint16 wParam, uint32 dwParam);
	boolean		(*IsRegistered)(ISyncEngExt * po);
	uint32		(*GetSheduleTime)(ISyncEngExt * po);
	boolean		(*UploadPhoto)(ISyncEngExt * po, const char *pszPhoto, const char *pszAlbum, const char *pszDes);
	boolean		(*SyncAlbum)(ISyncEngExt * po);
	int			(*GetContacts)(ISyncEngExt * po, AECHAR **pszContactsList);
	boolean		(*ShareAlbum)(ISyncEngExt * po, AECHAR *pwzAlbum, AECHAR *pwzContactsList, AECHAR *pwzMessage);
	boolean		(*SaveSms)(ISyncEngExt * po, AECHAR *pszFromNo, AECHAR *pszMsg, AECHAR *pszTime);
	int			(*GetSms)(ISyncEngExt * po, sms **pSmsAr, int nTotal, char **pFilterAr, int nNo);
	int			(*UploadSms)(ISyncEngExt * po, sms **pSmsAr, int nNo);
};

#define ISYNCENG_AddRef(p)				      		GET_PVTBL(p,ISyncEngExt)->AddRef(p);
#define ISYNCENG_Release(p)							GET_PVTBL(p,ISyncEngExt)->Release(p);
#define ISYNCENG_SyncContacts(p)					GET_PVTBL(p,ISyncEngExt)->SyncContacts(p);
#define ISYNCENG_RegisterNewUser(p,un,pn,up)		GET_PVTBL(p,ISyncEngExt)->RegisterNewUser(p,un,pn,up);
#define ISYNCENG_HandleEvent(p,cde,wpm,dpm)			GET_PVTBL(p,ISyncEngExt)->HandleEvent(p,cde,wpm,dpm);
#define ISYNCENG_IsRegistered(p)					GET_PVTBL(p,ISyncEngExt)->IsRegistered(p);
#define ISYNCENG_GetSheduleTime(p)					GET_PVTBL(p,ISyncEngExt)->GetSheduleTime(p);
#define ISYNCENG_UploadPhoto(p,pho,pal,pde)			GET_PVTBL(p,ISyncEngExt)->UploadPhoto(p,pho,pal,pde);
#define ISYNCENG_SyncAlbum(p)						GET_PVTBL(p,ISyncEngExt)->SyncAlbum(p);
#define ISYNCENG_GetContacts(p,pcl)					GET_PVTBL(p,ISyncEngExt)->GetContacts(p,pcl);
#define	ISYNCENG_ShareAlbum(p,abm,cnl,msg)			GET_PVTBL(p,ISyncEngExt)->ShareAlbum(p,abm,cnl,msg);
#define ISYNCENG_SaveSms(p,psn,msg,ptm)				GET_PVTBL(p,ISyncEngExt)->SaveSms(p,psn,msg,ptm);
#define ISYNCENG_GetSms(p,sar,tot,far,no)			GET_PVTBL(p,ISyncEngExt)->GetSms(p,sar,tot,far,no);
#define ISYNCENG_UploadSms(p,sar,no)				GET_PVTBL(p,ISyncEngExt)->UploadSms(p,sar,no);


#endif