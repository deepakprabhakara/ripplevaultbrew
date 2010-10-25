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
 Module Name				: DbHandler.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name   : RVBrewClient.mdl                         :
 Use Case Document Name     : 
 Use Case Realization Document Name   :
 Test Case Document Name      : 
 Abbreviations                :

 Description                  : Handle database and addressbook (header)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#if !defined(AFX_DBHANDLER_H__537BDB89_CF8A_494C_BB7F_D8C112ADF98D__INCLUDED_)
#define AFX_DBHANDLER_H__537BDB89_CF8A_494C_BB7F_D8C112ADF98D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AEEShell.h"
#include "AEEAddrBook.h"
#include "AEEDB.h"
#include "SMCommon.h"
#include "ContactInfo.h"
#include "SMEvents.h"

class CDbHandler  
{
public:
	boolean IsSmsMatched(AECHAR *pszPhoneNo, char **pFilterAr, int nNo);
	int GetSms(sms **pSmsAr, int nTotal, char **pFilterAr, int nNo);
	int AddToSmsDatabase(int nSent, AECHAR *pszFromNo, AECHAR *pszMsg, AECHAR *pszTime);
	int GetContactList(AECHAR **pszContactList);
	bool DeleteChangeDb();
	void Debug_DumpAddressBook();
	bool UpdateAddrBook(CContactInfo *pContactAr, int nSize);
	bool GetRecordsInChangeDb(CContactInfo **pContactsAr, int &nSize,  int &nNoOfPackets, int nPacketNo);
	void Debug_DumpBackupDb();
	bool CopyAddrBookToBackupDb();
	int CreateChangeDb();
	CDbHandler(IShell* pIShell=NULL);
	virtual ~CDbHandler();

private:
	bool DeleteDatabase(const char *pszDbName);
	bool AddToBackupDb(IDatabase *pBkpDb, CContactInfo *pContact);
	bool SetDeletedRecordsInAddrBook(IDatabase *pBackupDb, IDatabase *pCghangeDb);
	bool OpenDatabase(IDatabase **pDb, const char * pszFile, boolean bCreate);
	bool OpenBackupDb(IDatabase ** pDatabase);
	void Debug_DumpRecordFields(AEEDBField* fieldAr, int size);
	void Debug_DumpChangeDb();
	bool AddToChangesDb(IDatabase * pDatabase, DbSearchResult status, CContactInfo * pContact);
	DbSearchResult SearchContactInBackupDb(IDatabase* pBkpDb, uint16 nId, CContactInfo *pCurrentContact, CContactInfo *pBkpContact);
	bool OpenChangeDb(IDatabase ** pDatabase);
	IShell* m_pIShell;
};

#endif // !defined(AFX_DBHANDLER_H__537BDB89_CF8A_494C_BB7F_D8C112ADF98D__INCLUDED_)
