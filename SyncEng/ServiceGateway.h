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
 Module Name				: ServiceGateway.h
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name   : RVBrewClient.mdl                         :
 Use Case Document Name     : 
 Use Case Realization Document Name   :
 Test Case Document Name      : 
 Abbreviations                :

 Description                  : Handle web service calls to server and xml processing (header information). 
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#if !defined(AFX_SERVICEGATEWAY_H__73083B41_FBB9_4C47_A484_118A021CEF9F__INCLUDED_)
#define AFX_SERVICEGATEWAY_H__73083B41_FBB9_4C47_A484_118A021CEF9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ContactInfo.h"
#include "AEEShell.h"
#include "AEEWeb.h"
#include "SMEvents.h"


#define PHONENO_SIZE		20

class CServiceGateway  
{
public:
	int CreateSmsUploadXml(sms **smsAr, int nSize);
	int CreateAlbumShareXml(AECHAR *pwzAlbum, AECHAR *pwzContactsList, AECHAR *pwzMessage);
	bool CServiceGateway::CreateAlbumSyncXml();
	bool CreatePhotosXml(const char *pszPhoto, const char *pszAlbum, const char *pszDescription);
	bool CreateContactsPollXml(const char*pszPollType="p1");
	bool CallWebService(const char *pszService, bool bContentBinary=false);
	bool CreateRegistrationXml(const char *pszUsername, const char *pszPhoneNo, const char *pszPassword=NULL, const char *pszMsgType="reg");
	bool CreateContactsXml(CContactInfo* pContactAr, int nSize, int nNoOfPackets, int nPacketNo);
	bool UploadContacts(CContactInfo* pContactAr, int nSize);
	bool InitService();
	CServiceGateway(IShell* pIShell=NULL);
	virtual ~CServiceGateway();
	
private:
	void SetAlbumFile();
	int m_nTimeoutCount;
	uint32 GetPhotoData(const char *pszPhoto, char **pDataBuf);
	user *m_userInfo;

	bool AppendToContactsXml(CContactInfo *pContactAr, int nSize, const char *pszBodyTag);
	void SetTimeout();
	static void TimeOutWebResponseTimerCB(void *cxt);
	static void PollTimerCB(void *cxt);
	int ProcessServerResponse();
	static void GetDataCB(void *cxt);
	static void WebReadCB(void* cxt);
	bool CreateISourceFromXml();
	bool AppendSingleField(char *pszDesBuf, AECHAR *pwzField, char *pszFieldName);
	bool AppendMultiFiled(char *pszDesBuf, AECHAR *pwzField, char *pszSingleTag, char *pszCompTag);
	bool GetCurrentPacketInfo(int& nCurrPacket, int& nTotalPackets);
	bool CreateContactsXmlHeader(int nNoOfPackets, int nPacketNo);
	void FreeResources();
	bool bIsServiceInit;
	IShell *m_pIShell;

	IWebResp * m_piWResp;
	IWeb * m_pIWeb;
	AEECallback m_Callback;
	AEECallback m_DataReadCb;
	ISource* m_pISource;
	int m_nContentLength;
	char * m_pResultbuffer;
	char* m_pTempBuf;
	char* m_serverIp;

	enum ServiceStatus{
		SG_ERROR,
		SG_INIT_REG,
		SG_EXISTING_USER,
		SG_IR_SYNC_PACKET,
		SG_EU_SYNC_PACKET,
		SG_SYNC_SYNC_PACKET,
		SG_POLL,
		SG_PHOTO_UPLOAD,
		SG_ALBUM_SYNC,
		SG_SHARE_ALBUM,
		SG_SMS_UPLOAD
	};

	ServiceStatus m_Status;
	char *m_userId;
	char *m_deviceId;

	char m_pSyncType[20];
	char m_pszPhoneNo[PHONENO_SIZE];

public:
	bool CreateServerUpdatedContactsXml(CContactInfo *pContactAr, int nSize);
	char* GetResult();
	void SetStatus(ServiceStatus status);
};

#endif // !defined(AFX_SERVICEGATEWAY_H__73083B41_FBB9_4C47_A484_118A021CEF9F__INCLUDED_)
