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
 Module Name				: ContactsHandler.h
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Handle CServiceGateway and CDbHander classes. (header)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************//////////

#if !defined(AFX_CONTACTHANDLER_H__EC2A756E_43A0_4BD4_A69A_823B3A39F482__INCLUDED_)
#define AFX_CONTACTHANDLER_H__EC2A756E_43A0_4BD4_A69A_823B3A39F482__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AEEShell.h"
#include "ServiceGateway.h"
#include "DbHandler.h"

class CContactHandler  
{
public:
	bool UploadSms(sms **smsAr, int nSize);
	bool ShareAlbum(AECHAR *pwzAlbum, AECHAR *pwzContactsList, AECHAR *pwzMessage);
	bool SyncAlbums();
	bool UploadPhoto(const char *pszPhoto, const char *pszAlbum, const char *pszDescription);
	bool Init();
	void RegisterNewUser(const char *pszUsername, const char *pszPhoneNo, const char *pszPassword);
	void HandleEvent(uint16 eCode,uint16 wParam, uint32 dwParam);
	bool UploadPacket(int nPacketNo);
	int SyncContacts();
	CContactHandler(IShell* pIShell=NULL);
	virtual ~CContactHandler();

private:
	void FinishSyncProcess();	
	void ProcessPollResult();
	bool AddContactsInBody(char *pszXml, CContactInfo *contactAr, int &nPos, char *pszBody);
	bool ParseContactsXml(char *pszXml, CContactInfo **contactAr, int &nArSize);
	void SendContactsPoll();
	bool LoadContactInfoToAr(CContactInfo **pContactsAr, int &nSize,  int &nNoOfPackets, int nPacketNo);
	void StartContactsUpload();

	CDbHandler *m_pDbHandler;
	CServiceGateway* m_pServiceGateway;
	IShell* m_pIShell;

};

#endif // !defined(AFX_CONTACTHANDLER_H__EC2A756E_43A0_4BD4_A69A_823B3A39F482__INCLUDED_)
