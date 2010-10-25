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
 Module Name				: ContactsHandler.cpp
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Handle CServiceGateway and CDbHander classes. (source)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/////////////

#include "ContactHandler.h"
#include "SMCommon.h"
#include "synceng.bid"
#include "AEEStdLib.h"
#include "Util.h"
#include "SMEvents.h"

#define SM_EVT_TEST			EVT_USER+1

// ================================================================================
// FUNCTION		: new, delete new[] delete[]
// DESCRIPTION	: Global override of new/delete.
// ================================================================================
void* operator new(size_t sz)
{
	return MALLOC(sz);
}

void operator delete(void *p)
{
	FREE(p);
}

void* operator new[](size_t sz)
{
	return MALLOC(sz);
}

void operator delete[](void *p)
{
	FREE(p);
}

// ================================================================================
// FUNCTION		: CContactHandler
// DESCRIPTION	: Constructor for CContactHandler.
// ================================================================================
CContactHandler::CContactHandler(IShell* pIShell)
{
	m_pIShell = pIShell;
	m_pServiceGateway = NULL;
	m_pDbHandler = NULL;
	
}

// ================================================================================
// FUNCTION		: Init
// DESCRIPTION	: Initialize CContactHandler resources
// ================================================================================
bool CContactHandler::Init()
{
	if ( NULL!=m_pIShell )
	{
		m_pDbHandler = new CDbHandler(m_pIShell);
	
		if ( NULL==m_pDbHandler ) return false;
	}

	return true;
}

// ================================================================================
// FUNCTION		: ~CContactHandler
// DESCRIPTION	: CContactHandler destructor.
// ================================================================================
CContactHandler::~CContactHandler()
{
	if ( NULL!=m_pServiceGateway ) delete m_pServiceGateway;
	if ( NULL!=m_pDbHandler ) delete m_pDbHandler;
	
}

// ================================================================================
// FUNCTION		: SyncContacts
// DESCRIPTION	: Synchronise addressbook contacts.
// ================================================================================
int CContactHandler::SyncContacts()
{

	if ( NULL==m_pDbHandler ) return SM_NOMEMORY;

	if ( SM_SUCCESS != m_pDbHandler->CreateChangeDb() ) return SM_CHANGEDB_FAILED;

	StartContactsUpload();
	
	return SM_SUCCESS;

}

// ================================================================================
// FUNCTION		: StartContactsUpload
// DESCRIPTION	: This is the first function to call for contacts upload.
// ================================================================================
void CContactHandler::StartContactsUpload()
{	
	if ( NULL==m_pServiceGateway )
	{
		m_pServiceGateway = new CServiceGateway(m_pIShell);

		if ( NULL==m_pServiceGateway )
		{
			//todo: post a message to ui
			return;
		}

		if ( !m_pServiceGateway->InitService() )
		{
			//couldnt allocate memmory
			delete m_pServiceGateway;
			m_pServiceGateway = NULL;
			return;
		}
	}

	UploadPacket(1);
	
}

// ================================================================================
// FUNCTION		: LoadContactInfoToAr
// DESCRIPTION	: Get records from changes database and fill it to contact array
// ================================================================================
bool CContactHandler::LoadContactInfoToAr(CContactInfo **pContactsAr, int &nSize, int &nNoOfPackets, int nPacketNo)
{
	return m_pDbHandler->GetRecordsInChangeDb(pContactsAr, nSize, nNoOfPackets, nPacketNo);
}

// ================================================================================
// FUNCTION		: UploadPacket
// DESCRIPTION	: Upload a given number of packet to the server
// ================================================================================
bool CContactHandler::UploadPacket(int nPacketNo)
{
	if ( NULL==m_pServiceGateway )
	{
		//todo: post a message to ui
		return false;
	}

	CContactInfo *pContactInfoAr=NULL;
	int nArSize=0;

	int nNoOfPackets =1;
	if ( LoadContactInfoToAr(&pContactInfoAr, nArSize, nNoOfPackets, nPacketNo))
	{
		//create xml from contact aray
		m_pServiceGateway->CreateContactsXml(pContactInfoAr, nArSize, nNoOfPackets, nPacketNo);
		
		//upload to server.
		if ( !m_pServiceGateway->CallWebService(CONTACTS_SYNC))
		{
			//todo: post a error message
			ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYN_FAILED, OUTOFMEMORY, 0);
		}

	}
	else
	{
		delete m_pServiceGateway;
		m_pServiceGateway = NULL;
		ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYN_FAILED, OUTOFMEMORY, 0);
		return false;
	}

	delete [] pContactInfoAr;

	return true;
}

// ================================================================================
// FUNCTION		: HandleEvent
// DESCRIPTION	: Handle events coming from different models.
// ================================================================================
void CContactHandler::HandleEvent(uint16 eCode, uint16 wParam, uint32 dwParam)
{
	int nPacketNo;

	switch (eCode)
	{
	case EVT_USER_SYNC_SUCCESS:
			nPacketNo = wParam+1;
			UploadPacket(nPacketNo);
		break;

	case EVT_USER_SYNC_ERROR:
		break;

	case EVT_USER_SYNC_POLL:
			SendContactsPoll();
		break;

	case EVT_USER_REG_SUCCESS:
			StartContactsUpload();
		break;

	case EVT_USER_REG_EXIST:
		break;

	case EVT_USER_REG_ERROR:
		break;

	case EVT_USER_POLL_END:
			ProcessPollResult();
		break;
	case EVT_USER_DONE:
			FinishSyncProcess();
		break;
	default:
		break;
	}
}

// ================================================================================
// FUNCTION		: SendContactsPoll
// DESCRIPTION	: Send poll message to check the contacts upload.
// ================================================================================
void CContactHandler::SendContactsPoll()
{
	if ( NULL==m_pServiceGateway )
	{
		//todo: post a message to ui
		return;
	}

	if ( m_pServiceGateway->CreateContactsPollXml() )
	{
		if ( !m_pServiceGateway->CallWebService(CONTACTS_SYNC) )
		{
		}
	}
}

// ================================================================================
// FUNCTION		: RegisterNewUser
// DESCRIPTION	: Register new user.
// ================================================================================
void CContactHandler::RegisterNewUser(const char *pszUsername, const char *pszPhoneNo, const char *pszPassword)
{
	
	if ( NULL==pszUsername || NULL==pszPhoneNo ) return;

	if ( NULL==m_pServiceGateway )
	{
		m_pServiceGateway = new CServiceGateway(m_pIShell);

		if ( NULL==m_pServiceGateway )
		{
			//todo: post a message to ui
			return;
		}

		if ( !m_pServiceGateway->InitService() )
		{
			//couldnt allocate memmory
			delete m_pServiceGateway;
			m_pServiceGateway = NULL;
			return;
		}
	}

	//register new user
	m_pServiceGateway->CreateRegistrationXml(pszUsername, pszPhoneNo, pszPassword);
	m_pServiceGateway->CallWebService(CONTACTS_SYNC);

}

// ================================================================================
// FUNCTION		: ParseContactsXml
// DESCRIPTION	: Read contacts xml got from server and fill it to contacts array.
// ================================================================================
bool CContactHandler::ParseContactsXml(char *pszXml, CContactInfo **contactAr, int &nArSize)
{
	if ( NULL==pszXml ) return false;

	nArSize =0;
	char *pszFound=NULL;
	char *pszNext;
	pszNext = pszXml;
	char pszTag[]="<ct>";
	//count the number of contacts in xml
	while ( NULL != (pszFound=STRSTR(pszNext, pszTag)))
	{
		nArSize++;
		pszNext = pszFound+1;
	}

	if ( 0==nArSize ) return false;

	//create contact array to hold the info
	(*contactAr) = new CContactInfo[nArSize];
	if ( NULL ==(*contactAr) ) return false;
	int nIncre=0;

	//contacts to add
	AddContactsInBody(pszXml, *contactAr, nIncre, "add");

	//contacts to update
	AddContactsInBody(pszXml, *contactAr, nIncre, "update");
	
	//contacts to delete
	AddContactsInBody(pszXml, *contactAr, nIncre, "delete");

	return true;
}

// ================================================================================
// FUNCTION		: AddContactsInBody
// DESCRIPTION	: Add body of the contacts xml
// ================================================================================
bool CContactHandler::AddContactsInBody(char *pszXml, CContactInfo *contactAr, int &nPos, char *pszBody)
{
	char pszTag[10];
	SPRINTF(pszTag, "<%s>", pszBody);
	char *pszFound=NULL;
	char *pszNext;
	pszNext = pszXml;
	//add body
	if ( NULL != (pszFound=STRSTR(pszNext, pszTag)))
	{
		char *pEnd=NULL;
		SPRINTF(pszTag, "</%s>", pszBody);
		pEnd = STRSTR(pszNext, pszTag);
		if (NULL==pEnd ) return false;
		SPRINTF(pszTag, "<%s>", pszBody);
		int nLen = STRLEN(pszFound) - STRLEN(pEnd)- STRLEN(pszTag);
		if ( 0==nLen ) return false;
		char *pszTmpBuf = (char*)MALLOC(nLen+1);
		if ( NULL==pszTmpBuf ) return false;
		STRLCPY(pszTmpBuf, pszFound+STRLEN(pszTag), nLen+1);
		DBGPRINTF(pszTmpBuf);

		char *pszFoundCt=NULL;
		char *pszNextCt=pszTmpBuf;
		while ( NULL != (pszFoundCt=STRSTR(pszNextCt,"<ct")))
		{

			contactAr[nPos].SetContactFromXml(pszFoundCt, pszBody);
			nPos++;
			pszNextCt = pszFoundCt+1;
		}
		pszNext = pszFound+1;

		FREEIF(pszTmpBuf);
	}

	return true;
}

// ================================================================================
// FUNCTION		: ProcessPollResult
// DESCRIPTION	: Respond to the poll result
// ================================================================================
void CContactHandler::ProcessPollResult()
{
	if ( NULL!=m_pServiceGateway )
	{
		char *pszResult = m_pServiceGateway->GetResult();

		if ( NULL!=pszResult )
		{
			CContactInfo *pContactAr=NULL;
			int nSize=0;
			ParseContactsXml(pszResult, &pContactAr, nSize);
			
			if ( NULL!=pContactAr )
			{
				m_pDbHandler->UpdateAddrBook(pContactAr, nSize);

				//send server side update back to the server
				m_pServiceGateway->CreateServerUpdatedContactsXml(pContactAr, nSize);
				m_pServiceGateway->CallWebService(CONTACTS_SYNC);

				delete [] pContactAr;
			}
			else
			{
				//poll process end
				ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_DONE, Success, 0);

			}
		}
	}
}

// ================================================================================
// FUNCTION		: FinishSyncProcess
// DESCRIPTION	: After finishing the synchronization delete the changed database and 
//				: create backup database
// ================================================================================
void CContactHandler::FinishSyncProcess()
{
	if (m_pDbHandler->CopyAddrBookToBackupDb())
	{
		m_pDbHandler->DeleteChangeDb();
	}

	ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYN_FINISHED, Success, 0);
}

// ================================================================================
// FUNCTION		: UploadPhoto
// DESCRIPTION	: Upload a photo to server.
// ================================================================================
bool CContactHandler::UploadPhoto(const char *pszPhoto, const char *pszAlbum, const char *pszDescription)
{
	if ( NULL==m_pServiceGateway )
	{
		m_pServiceGateway = new CServiceGateway(m_pIShell);

		if ( NULL==m_pServiceGateway )
		{
			//todo: post a message to ui
			return false;
		}

		if ( !m_pServiceGateway->InitService() )
		{
			//couldnt allocate memmory
			delete m_pServiceGateway;
			m_pServiceGateway = NULL;
			return false;
		}
	}

	if ( m_pServiceGateway->CreatePhotosXml(pszPhoto, pszAlbum, pszDescription))
		m_pServiceGateway->CallWebService(PHOTOS_UPLOAD, true);
	else 
		return false;

	return true;
}

// ================================================================================
// FUNCTION		: SyncAlbums
// DESCRIPTION	: Synchronize albums with server..
// ================================================================================
bool CContactHandler::SyncAlbums()
{
	if ( NULL==m_pServiceGateway )
	{
		m_pServiceGateway = new CServiceGateway(m_pIShell);

		if ( NULL==m_pServiceGateway )
		{
			//todo: post a message to ui
			return false;
		}

		if ( !m_pServiceGateway->InitService() )
		{
			//couldnt allocate memmory
			delete m_pServiceGateway;
			m_pServiceGateway = NULL;
			return false;
		}
	}

	if ( m_pServiceGateway->CreateAlbumSyncXml())
		m_pServiceGateway->CallWebService(ALBUM_SYNC);
	else
		return false;

	return true;
}

// ================================================================================
// FUNCTION		: ShareAlbum
// DESCRIPTION	: Share a album with others
// ================================================================================
bool CContactHandler::ShareAlbum(AECHAR *pwzAlbum, AECHAR *pwzContactsList, AECHAR *pwzMessage)
{
	if ( NULL==m_pServiceGateway )
	{
		m_pServiceGateway = new CServiceGateway(m_pIShell);

		if ( NULL==m_pServiceGateway )
		{
			//todo: post a message to ui
			return false;
		}

		if ( !m_pServiceGateway->InitService() )
		{
			//couldnt allocate memmory
			delete m_pServiceGateway;
			m_pServiceGateway = NULL;
			return false;
		}
	}

	if ( 0==m_pServiceGateway->CreateAlbumShareXml(pwzAlbum, pwzContactsList, pwzMessage))
		m_pServiceGateway->CallWebService(ALBUM_SHARE);
	else
		return false;

	return true;
}

// ================================================================================
// FUNCTION		: UploadSms
// DESCRIPTION	: Upload sms to server.
// ================================================================================
bool CContactHandler::UploadSms(sms **smsAr, int nSize)
{
	if ( NULL==m_pServiceGateway )
	{
		m_pServiceGateway = new CServiceGateway(m_pIShell);

		if ( NULL==m_pServiceGateway )
		{
			//todo: post a message to ui
			return false;
		}

		if ( !m_pServiceGateway->InitService() )
		{
			//couldnt allocate memmory
			delete m_pServiceGateway;
			m_pServiceGateway = NULL;
			return false;
		}
	}

	if ( 0==m_pServiceGateway->CreateSmsUploadXml(smsAr, nSize))
		m_pServiceGateway->CallWebService(SMS_UPLOAD);
	else
		return false;

	return true;
}