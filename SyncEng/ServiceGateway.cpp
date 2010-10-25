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
 Module Name				: ServiceGateway.cpp
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name   : RVBrewClient.mdl                         :
 Use Case Document Name     : 
 Use Case Realization Document Name   :
 Test Case Document Name      : 
 Abbreviations                :

 Description                  : Handle web service calls to server and xml processing (source). 
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include "ServiceGateway.h"
#include "AEEStdLib.h"
#include "synceng.bid"
#include "AEEFile.h"
#include "Util.h"

#define RESULT_BUFFER_LEN	4*1024
#define DATA_BUFFER_LEN		16*1024
#define BUFFER_LENGTH_SMALL		200
#define READ_BUFFER_LENGTH		200

#define MAX_TIMEOUTS		2

#define ALBUM_FILE		"albums.txt"

// ================================================================================
// FUNCTION		: CServiceGateway 
// DESCRIPTION	: CServiceGateway construction
// ================================================================================
CServiceGateway::CServiceGateway(IShell* pIShell)
{
	m_pIShell = pIShell;
	m_pIWeb = NULL;
	m_piWResp = NULL;
	m_pISource = NULL;

	bIsServiceInit = false;
	m_Status = SG_SYNC_SYNC_PACKET;
	m_nContentLength =0;
	
	m_serverIp = NULL;
	m_userInfo = NULL;
	

	m_nTimeoutCount=0;
}

// ================================================================================
// FUNCTION		: CServiceGateway 
// DESCRIPTION	: CServiceGateway destruction. free all resources.
// ================================================================================
CServiceGateway::~CServiceGateway()
{
	ISHELL_CancelTimer(m_pIShell, TimeOutWebResponseTimerCB, this);//SetTimeout
	ISHELL_CancelTimer(m_pIShell, PollTimerCB, this);

	FreeResources();

	FREEIF(m_pResultbuffer);
	FREEIF(m_pTempBuf);
	FREEIF(m_serverIp);
	FREEIF(m_userId);
	FREEIF(m_deviceId);
	

	delete m_userInfo;

}

// ================================================================================
// FUNCTION		: InitService 
// DESCRIPTION	: Initialize all the resources for CServiceGateway class
// ================================================================================
bool CServiceGateway::InitService()
{
	FREEIF(m_pResultbuffer);
	FREEIF(m_pTempBuf);
	FREEIF(m_serverIp);
	FREEIF(m_userId);
	FREEIF(m_deviceId);
	delete m_userInfo;

	m_pResultbuffer = (char*)MALLOC(RESULT_BUFFER_LEN);
	if ( NULL==m_pResultbuffer ) return false;
	
	m_pTempBuf = (char*)MALLOC(DATA_BUFFER_LEN);
	if ( NULL==m_pTempBuf )
	{
		FREEIF(m_pResultbuffer);
		return false;
	}

	m_serverIp = (char*)MALLOC(BUFFER_LENGTH_SMALL);
	if ( NULL==m_serverIp )
	{
		FREEIF(m_pResultbuffer);
		FREEIF(m_pTempBuf);
		return false;
	}

	m_userId = (char*)MALLOC(BUFFER_LENGTH_SMALL);
	if ( NULL==m_userId )
	{
		FREEIF(m_serverIp);
		FREEIF(m_pResultbuffer);
		FREEIF(m_pTempBuf);
		return false;
	}

	m_deviceId = (char*)MALLOC(BUFFER_LENGTH_SMALL);
	if ( NULL==m_deviceId )
	{
		FREEIF(m_userId);
		FREEIF(m_serverIp);
		FREEIF(m_pResultbuffer);
		FREEIF(m_pTempBuf);
		return false;
	}

	m_userInfo = NULL;
	if ( !CUtil::GetUserInfo(&m_userInfo))
	{
		FREEIF(m_userId);
		FREEIF(m_serverIp);
		FREEIF(m_pResultbuffer);
		FREEIF(m_pTempBuf);
		FREEIF(m_deviceId);
		return false;
	}
	
	MEMSET(&m_Callback, 0, sizeof(m_Callback));
	MEMSET(&m_DataReadCb, 0, sizeof(m_DataReadCb));

	return true;
}

// ================================================================================
// FUNCTION		: FreeResources
// DESCRIPTION	: Used to free resource when a web service call is completed.
// ================================================================================
void CServiceGateway::FreeResources()
{

	CALLBACK_Cancel(&m_Callback);

	if ( NULL!=m_piWResp ) 
	{
		IBASE_Release((IBase*)m_piWResp);
		m_piWResp = NULL;
	}

	if ( NULL!=m_pIWeb )
	{
		IBASE_Release((IBase*)m_pIWeb);
		m_pIWeb = NULL;
	}
	
	MEMSET(m_pResultbuffer, 0, RESULT_BUFFER_LEN);
	
}

// ================================================================================
// FUNCTION		: CreateContactsXml
// DESCRIPTION	: Create xml(contacts xml) to synchronize address book with server.
// ================================================================================
bool CServiceGateway::CreateContactsXml(CContactInfo *pContactAr, int nSize, int nNoOfPackets, int nPacketNo)
{
	CreateContactsXmlHeader(nNoOfPackets, nPacketNo);

	//create body add
	STRCAT(m_pTempBuf, "<add>");
	AppendToContactsXml(pContactAr, nSize, "add");
	STRCAT(m_pTempBuf, "</add>");

	//create body update
	STRCAT(m_pTempBuf, "<update>");
	AppendToContactsXml(pContactAr, nSize, "update");
	STRCAT(m_pTempBuf, "</update>");

	//create body delete
	STRCAT(m_pTempBuf, "<delete>");
	AppendToContactsXml(pContactAr, nSize, "delete");
	STRCAT(m_pTempBuf, "</delete>");

	//contacts xml footer
	STRCAT(m_pTempBuf, "</b></root>");
	m_nContentLength = STRLEN(m_pTempBuf);

	CUtil::LogToFile(m_pTempBuf);
	
	return true;
}

// ================================================================================
// FUNCTION		: AppendToContactsXml
// DESCRIPTION	: Create specific parts of contacts xml (add. update, delete)
// ================================================================================
bool CServiceGateway::AppendToContactsXml(CContactInfo *pContactAr, int nSize, const char *pszBodyTag)
{
	char *pszData=NULL;
	char *pszField = NULL;
	AECHAR *pwzTmp = NULL;
	int nBufLen =0;
	int nState=0;
	int nRecId =0;
	int nFieldLen =0;
	char *pszSearchBuf = NULL;
	int nTagLen=0;

	char *mainTag = (char*)MALLOC(10);
	if ( NULL==mainTag ) return false;
	
		
	AECHAR pwzNewField[] = NEW_FIELD;
	nTagLen = WSTRLEN(pwzNewField);
	char *pszNewField = (char*)MALLOC(nTagLen+1);
	if ( NULL==pszNewField )
	{
		FREEIF(mainTag);
		FREEIF(pszNewField);
		return false;
	}
	WSTRTOSTR( pwzNewField, pszNewField, nTagLen+1);

	//Get current time 
	uint32 dwTimeSpan= CUtil::GetCurrentTimeSecs();
	
	for (int i=0; i<nSize; i++)
	{
		//Add contact information to the xml file.
		nBufLen = pContactAr[i].GetSize();
		pszData = (char*)MALLOC(nBufLen+1024); //1024 length for tags
		if ( NULL==pszData )
		{
			FREEIF(mainTag);
			FREEIF(pszNewField);
			return false;
		}

		nRecId = pContactAr[i].GetContactId();
		//check wheather the contact should ADD, UPDATE or DELETE, 
		nState = pContactAr[i].GetState();
		switch ( nState )
		{
		case SM_RECORD_ADD:
				STRCPY(mainTag, "add");
			break;

		case SM_RECORD_UPDATE:
				STRCPY(mainTag, "update");
			break;

		case SM_RECORD_DELETE:
				STRCPY(mainTag, "delete");
			break;

		default:
			FREEIF(mainTag);
			FREEIF(pszData);
			FREEIF(pszNewField);
			return false;
		}

		if ( 0==STRCMP(pszBodyTag,mainTag))
		{

			SPRINTF(pszData, "<ct t=\"%u\" id=\"%d\">",
								dwTimeSpan,
								nRecId);

			//add tags to the contact data field and append it to buffer
			//first name
			pwzTmp = pContactAr[i].GetFName();
			AppendSingleField(pszData, pwzTmp, "fn");
		
			//last name
			pwzTmp = pContactAr[i].GetLName();
			AppendSingleField(pszData, pwzTmp, "ln");
			
			//company
			pwzTmp = pContactAr[i].GetCompany();
			AppendSingleField(pszData, pwzTmp, "cn");
			
			//job titles
			pwzTmp = pContactAr[i].GetJobTitles();
			AppendMultiFiled(pszData, pwzTmp, "ttl", "ttls");

			//Fax
			pwzTmp = pContactAr[i].GetFax();
			AppendMultiFiled(pszData, pwzTmp, "fx", "fxs");

			//Home fax
			pwzTmp = pContactAr[i].GetFaxHome();
			AppendMultiFiled(pszData, pwzTmp, "fxh", "fxsh");

			//work fax
			pwzTmp = pContactAr[i].GetFaxWork();
			AppendMultiFiled(pszData, pwzTmp, "fxb", "fxsb");

			//mobile
			pwzTmp = pContactAr[i].GetMobile();
			AppendMultiFiled(pszData, pwzTmp, "mbl", "mbls");

			//mobile home
			pwzTmp = pContactAr[i].GetMobileHome();
			AppendMultiFiled(pszData, pwzTmp, "mblh", "mblsh");

			//mobile work
			pwzTmp = pContactAr[i].GetMobileWork();
			AppendMultiFiled(pszData, pwzTmp, "mblb", "mblsb");

			//telephone
			pwzTmp = pContactAr[i].GetTel();
			AppendMultiFiled(pszData, pwzTmp, "tel", "tels");

			//telephone home
			pwzTmp = pContactAr[i].GetTelHome();
			AppendMultiFiled(pszData, pwzTmp, "telh", "telsh");

			//telephone work
			pwzTmp = pContactAr[i].GetTelWork();
			AppendMultiFiled(pszData, pwzTmp, "telb", "telsb");

			//email
			pwzTmp = pContactAr[i].GetEmail();
			AppendMultiFiled(pszData, pwzTmp, "eml", "emls");

			//email home
			pwzTmp = pContactAr[i].GetEmailHome();
			AppendMultiFiled(pszData, pwzTmp, "emlh", "emlsh");

			//email work
			pwzTmp = pContactAr[i].GetEmailWork();
			AppendMultiFiled(pszData, pwzTmp, "emlb", "emlsb");
			
			//end of single contact information
			STRCAT(pszData, "</ct>");

			//finally copy the data to main data buffer
			STRCAT(m_pTempBuf, pszData);
				
		}

		FREEIF(pszData);
	}
	

	FREEIF(mainTag);
	FREEIF(pszNewField);

	return true;
}

// ================================================================================
// FUNCTION		: AppendMultiFiled
// DESCRIPTION	: Construct multi field tag sections in contacts xml.
// ================================================================================
bool CServiceGateway::AppendMultiFiled(char *pszDesBuf, AECHAR *pwzField, char *pszSingleTag, char *pszCompTag)
{
	if ( NULL==pwzField ) return true;

	int nFieldLen=0;
	char *pszSearchBuf=NULL;
	char *pszFieldXml=NULL;
	char *pszTag;
	char *pszNewTag;
	int nTagLen=0;
	
	AECHAR pwzNew[]=NEW_FIELD;
	nTagLen = WSTRLEN(pwzNew);
	pszNewTag = (char*)MALLOC(nTagLen+1);
	if ( NULL==pszNewTag )
	{
		return false;
	}
	WSTRTOSTR( pwzNew, pszNewTag, nTagLen+1);

	pszTag = (char*)MALLOC(50);
	if ( NULL==pszTag )
	{
		FREEIF(pszNewTag);
		return false;
	}

	nFieldLen = WSTRLEN( pwzField );
	pszSearchBuf = (char*)MALLOC(nFieldLen+1);
	if ( NULL==pszSearchBuf )
	{
		FREEIF( pszTag );
		FREEIF( pszNewTag );
		return false;
	}
	//copy the content to search buf
	WSTRTOSTR(pwzField, pszSearchBuf, nFieldLen+1);

	pszFieldXml = (char*)MALLOC(nFieldLen+512);//there can be multiple entirs
											//we dont know exact number of tags
	if ( NULL==pszFieldXml )
	{
		FREEIF(pszTag);
		FREE(pszNewTag);
		FREEIF(pszSearchBuf);
		return false;
	}

	SPRINTF(pszTag, "<%s>", pszCompTag);
	STRCPY(pszFieldXml, pszTag);
	
	//search for NEW_FIELD.
	char *pszNewPos=NULL;
	pszNewPos= STRSTR(pszSearchBuf, pszNewTag);

	if ( NULL != pszNewPos )
	{
		int id=0;
		int nLen=0;
		char *pszStart = pszSearchBuf;
		
		while (NULL!=pszNewPos)
		{
			SPRINTF(pszTag, "<%s id=\"%d\">", pszSingleTag, id);
			STRCAT(pszFieldXml, pszTag);

			nLen = STRLEN(pszStart)- STRLEN(pszNewPos);
			STRNCPY( pszFieldXml+STRLEN(pszFieldXml), pszStart, nLen);
			SPRINTF(pszTag, "</%s>", pszSingleTag);
			STRCAT(pszFieldXml, pszTag);

			pszStart = pszNewPos;
			pszStart = pszStart+ nTagLen;
			pszNewPos = STRSTR(pszStart, pszNewTag);
			

			id++;
		}

		//append the last field
		SPRINTF(pszTag, "<%s id=\"%d\">", pszSingleTag, id);
		STRCAT(pszFieldXml, pszTag);
		STRCAT(pszFieldXml, pszStart);
		SPRINTF(pszTag, "</%s>", pszSingleTag);
		STRCAT(pszFieldXml, pszTag);

	}
	else
	{
		SPRINTF(pszFieldXml+STRLEN(pszFieldXml), "<%s id=\"0\">%S</%s>", pszSingleTag, pwzField, pszSingleTag);
	}

	SPRINTF(pszTag, "</%s>", pszCompTag);
	STRCAT(pszFieldXml, pszTag);
	DBGPRINTF( pszFieldXml );

	STRCAT(pszDesBuf, pszFieldXml);

	FREEIF(pszTag);
	FREEIF(pszNewTag);
	FREEIF(pszSearchBuf);
	FREEIF(pszFieldXml);

	return true;
}

// ================================================================================
// FUNCTION		: AppendSingleField
// DESCRIPTION	: Copy a single tag fields to contacts xml.
// ================================================================================
bool CServiceGateway::AppendSingleField(char *pszDesBuf, AECHAR *pwzField, char *pszFieldName)
{
	int nFieldLen=0;
	char *pszFieldXml=NULL;
	int nTagLen = STRLEN(pszFieldName);
	
	if ( NULL!=pwzField )
	{
		nFieldLen = WSTRLEN(pwzField);
		pszFieldXml = (char*)MALLOC(nFieldLen+(nTagLen*2)+5+1);// len = len"<%s>%S</%s>"
		if ( NULL==pszFieldXml )
		{
			return false;
		}
		SPRINTF(pszFieldXml, "<%s>%S</%s>", pszFieldName, pwzField, pszFieldName);
		STRCAT(pszDesBuf, pszFieldXml);
		FREEIF(pszFieldXml);
	}

	return true;
}

// ================================================================================
// FUNCTION		: CreateContactsXmlHeader
// DESCRIPTION	: Create contacts xml header.
// ================================================================================
bool CServiceGateway::CreateContactsXmlHeader(int nNoOfPackets, int nPacketNo)
{
	uint32 dwTimeSpan=0;
	dwTimeSpan = CUtil::GetCurrentTimeSecs();

	MEMSET(m_pTempBuf, 0, DATA_BUFFER_LEN);
	SPRINTF( m_pTempBuf,	"<root>"
        					"<h>"
        					"<msgt>%s</msgt>" 
        					"<msg/>" 
        					"<uid>%s</uid>" 
        					"<did>%s</did>" 
        					"<currec>%d</currec>" 
        					"<totrec>%d</totrec>" 
        					"<loc>en_us</loc>" 
        					"<ctime>%u</ctime>" 
        					"<lstime>%u</lstime>" 
        					"<synctype>" 
        					"<mod>cs</mod>" 
        					"<gran>contactlevel</gran>" 
        					"<adde>true</adde>" 
        					"<upde>true</upde>" 
        					"<dele>true</dele>" 
        					"</synctype>" 
        					"</h>" 
        					"<b>",
							m_userInfo->status, m_userInfo->username, m_userInfo->deviceid, 
								nPacketNo, nNoOfPackets, dwTimeSpan, m_userInfo->lsyncTime);

	return true;
}

// ================================================================================
// FUNCTION		: CreateISourceFromXml
// DESCRIPTION	: Create ISource from xml.
// ================================================================================
bool CServiceGateway::CreateISourceFromXml()
{	
	if ( NULL==m_pTempBuf ) return false;

	ISourceUtil* pISourceUtil=NULL;
	if ( SUCCESS != ISHELL_CreateInstance(m_pIShell, AEECLSID_SOURCEUTIL, (void **)&pISourceUtil) )
	{
		return false;
	}

	int ret = ISOURCEUTIL_SourceFromMemory(pISourceUtil,
                                      m_pTempBuf,//m_pEncodedData
                                      m_nContentLength,
                                      NULL,
                                      NULL,
                                      &m_pISource);
   if (ret != SUCCESS) {
		
		ISOURCEUTIL_Release(pISourceUtil);

		return false;
   }

	
	ISOURCEUTIL_Release(pISourceUtil);

	return true;
}

// ================================================================================
// FUNCTION		: CallWebService
// DESCRIPTION	: send xml to server.
// ================================================================================
bool CServiceGateway::CallWebService(const char *pszService, bool bContentBinary)
{
	if ( !CreateISourceFromXml() ) return false;

	if ( NULL==m_pISource ) return false;

	//clear any memory of previous web service calls
	FreeResources();

	//append the service to host url
	STRCPY(m_serverIp, m_userInfo->host);
	STRCAT(m_serverIp, pszService);

	// Create IWeb instance
	if ( SUCCESS != ISHELL_CreateInstance(m_pIShell, AEECLSID_WEB, (void **)&m_pIWeb) )
		return false;
	
	// Initialize the callback to WebReadCB
	CALLBACK_Init(&m_Callback, WebReadCB, this);

	WebOpt zWebOpts[6];

	zWebOpts[0].nId = WEBOPT_BODY;
	zWebOpts[0].pVal = m_pISource;
	zWebOpts[1].nId = WEBOPT_CONTENTLENGTH;
	zWebOpts[1].pVal = (void*)m_nContentLength;
	zWebOpts[2].nId = WEBOPT_METHOD;
	zWebOpts[2].pVal = "POST";
	zWebOpts[3].nId = WEBOPT_CONNECTTIMEOUT;
	zWebOpts[3].pVal = (void*)5000;//5 seconds
	zWebOpts[4].nId = WEBOPT_HEADER;
	if ( bContentBinary )
		zWebOpts[4].pVal = "Content-Type: application/x-www-form-urlencoded";
	else
		zWebOpts[4].pVal = "Content-Type: application/x-www-form-urlencoded";
	zWebOpts[5].nId = WEBOPT_END;

	//add options
	IWEB_AddOpt(m_pIWeb, zWebOpts);

	// Request IWEB to fetch the information
	IWEB_GetResponse(m_pIWeb,
						(m_pIWeb,
						&m_piWResp, 
						&m_Callback,
						m_serverIp));//m_serverIp

	//need to free source after web service call
	ISOURCE_Release(m_pISource);
	
	//set timout for web response
	SetTimeout();

	return true;

}

// ================================================================================
// FUNCTION		: GetDataCB
// DESCRIPTION	: Read response from server to web service calls.
// ================================================================================
void CServiceGateway::GetDataCB(void *cxt)
{
	CServiceGateway * pMe = (CServiceGateway *)cxt;
	
	char *buf = (char*)MALLOC(READ_BUFFER_LENGTH+1);
	if (NULL==buf)
	{
		//todo:need to post a message
		return;
	}

	//read data
	int32 status = ISOURCE_Read(pMe->m_pISource, buf, READ_BUFFER_LENGTH);

	switch (status)
	{
		case ISOURCE_WAIT:
		{
			CALLBACK_Init(&pMe->m_DataReadCb, GetDataCB, pMe);
			ISOURCE_Readable(pMe->m_pISource, &pMe->m_DataReadCb);
			
		}break;

		case ISOURCE_END:
		{	
			pMe->ProcessServerResponse();
			CUtil::LogToFile(pMe->m_pResultbuffer);
			FREEIF(buf);
			return;

		}break;

		case ISOURCE_ERROR:
		{
			FREEIF(buf);
			return;
		}break;

		default:
			break;

	}


	CALLBACK_Init(&pMe->m_DataReadCb, GetDataCB, pMe);
	ISOURCE_Readable(pMe->m_pISource, &pMe->m_DataReadCb);

	//process data
	int nLen = STRLEN(pMe->m_pResultbuffer);
	buf[READ_BUFFER_LENGTH] = '\0';

	nLen +=STRLEN(buf);
	if ( (nLen+READ_BUFFER_LENGTH+1)<RESULT_BUFFER_LEN )
		STRCAT(pMe->m_pResultbuffer, buf);//, nLen+1);

	DBGPRINTF("Received from web service : %s", buf);

	//we got a response from web server. so cancel the response timeout function
	ISHELL_CancelTimer(pMe->m_pIShell, pMe->TimeOutWebResponseTimerCB, pMe);//SetTimeout

	FREEIF(buf);
	
}

// ================================================================================
// FUNCTION		: WebReadCB
// DESCRIPTION	: Callback function when server response came.
// ================================================================================
void CServiceGateway::WebReadCB(void* cxt)
{
	CServiceGateway * pMe = (CServiceGateway *)cxt;

	// Get information about the response
	WebRespInfo* pWebRespInfo = IWEBRESP_GetInfo(pMe->m_piWResp);
	
	// the body of the response is contained in the ISOURCE within
	pMe->m_pISource = pWebRespInfo->pisMessage;

	if ( NULL != pMe->m_pISource )
	{
		GetDataCB(pMe);
	}
	else
	{
		//no internet connection detected.
		//This is not working on practical!!!
	}
}

// ================================================================================
// FUNCTION		: CreateRegistrationXml
// DESCRIPTION	: Create user registration xml.
// ================================================================================
bool CServiceGateway::CreateRegistrationXml(const char *pszUsername, const char *pszPhoneNo, const char *pszPassword, const char *pszMsgType)
{
	char pDevId[256];
	int size=256;
	ISHELL_GetDeviceInfoEx(m_pIShell, AEE_DEVICEITEM_CHIP_ID, (void*)pDevId, &size);

	STRCPY(m_userId, pszUsername);
	STRCPY(m_deviceId, pDevId);
	STRLCPY(m_pszPhoneNo, pszPhoneNo, PHONENO_SIZE);

	//get current time
	uint32 secs=0;
	JulianType stDate;
	GETJULIANDATE(secs, &stDate);
	char date[30];
	SPRINTF(date, "%d/%d/%d %d:%d", stDate.wYear,
									stDate.wMonth,
									stDate.wDay,
									stDate.wHour,
									stDate.wMinute);
	//new user
	if ( NULL==pszPassword )
	{
		SPRINTF( m_pTempBuf,"<root>"
							"<h>"
							"<msgt>%s</msgt>"
							"<msg></msg>"
							"<uid>%s</uid>"
							"<hp>%s</hp>"
							"<did>%s</did>"
							"<currec>1</currec>"
							"<totrec>1</totrec>"
							"<loc>en_us</loc>"
							"<ctime>%s</ctime>"
							"<lstime>0</lstime>"
							"<synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"
							"</h>"
							"<b>"
							"<hp>%s</hp>"
							"<username>%s</username>"
							"</b>"
							"</root>", 
							pszMsgType, pszUsername, pszPhoneNo, pDevId, date, pszPhoneNo, pszUsername);

		m_Status = SG_INIT_REG;
	}
	else
	{
		SPRINTF( m_pTempBuf,"<root>"
							"<h>"
							"<msgt>eu</msgt>"
							"<msg></msg>"
							"<uid>%s</uid>"
							"<hp>%s</hp>"
							"<did>%s</did>"
							"<currec>1</currec>"
							"<totrec>1</totrec>"
							"<loc>en_us</loc>"
							"<ctime>%s</ctime>"
							"<lstime>0</lstime>"
							"<synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"
							"</h>"
							"<b>"
							"<hp>%s</hp>"
							"<username>%s</username>"
							"<password>%s</password>"
							"</b>"
							"</root>", 
							pszUsername, pszPhoneNo, pDevId, date, 
							pszPhoneNo, pszUsername, pszPassword);
		
		m_Status = SG_EXISTING_USER;
	}

	m_nContentLength = STRLEN(m_pTempBuf);
	CUtil::LogToFile(m_pTempBuf);
	
	return true;
}

// ================================================================================
// FUNCTION		: CreateContactsPollXml
// DESCRIPTION	: Create the xml to poll the results of a web service call.
// ================================================================================
bool CServiceGateway::CreateContactsPollXml(const char*pszPollType)
{
	//get current time
	uint32 secs=0;
	secs = CUtil::GetCurrentTimeSecs();

	SPRINTF( m_pTempBuf,	"<root>"
							"<h>"
							"<msgt>poll</msgt>"
							"<msg>%s</msg>"
							"<uid>%s</uid>"
							"<did>%s</did>"
							//"<hp>123456</hp>"
							"<currec>1</currec>"
							"<totrec>1</totrec>"
							"<loc>en_us</loc>"
							"<ctime>%u</ctime>"
							"<lstime>%u</lstime>"
							"<synctype>"
							"<mod>cs</mod>"
							"<gran>contactlevel</gran>"
							"<adde>true</adde>"
							"<upde>true</upde>"
							"<dele>true</dele>"
							"</synctype>"
							"</h>"
							"<b>"
							"</b>"
							"</root>",
							pszPollType, m_userInfo->username, m_userInfo->deviceid, secs, m_userInfo->lsyncTime);//, m_userInfo->lsyncTimesecs);

	m_nContentLength = STRLEN(m_pTempBuf);
	m_Status= SG_POLL;
	CUtil::LogToFile(m_pTempBuf);
	return true;

}

// ================================================================================
// FUNCTION		: ProcessServerResponse
// DESCRIPTION	: Process the server response depend on the currunt status of application.
// ================================================================================
int CServiceGateway::ProcessServerResponse()
{
	if (NULL==m_pResultbuffer) return SM_ERROR;
	boolean b;
	char *pszSearch = NULL;

	switch ( m_Status )
	{
	case SG_INIT_REG:
	case SG_EXISTING_USER:

			if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "suc_reg")))
			{
				//store the registration info
				STRCPY(m_userInfo->username, m_userId);
				STRCPY(m_userInfo->deviceid, m_deviceId);
				m_userInfo->lsyncTime = CUtil::GetCurrentTimeSecs()-3600*24*2;
				STRCPY(m_userInfo->status, "sync");
				CUtil::SetUserInfo(m_userInfo);

				if ( SG_INIT_REG==m_Status )
				{
					STRCPY(m_userInfo->status, "init_reg");
					m_Status = SG_IR_SYNC_PACKET;
				}
				else
				{
					STRCPY(m_userInfo->status, "init_eu");
					m_Status = SG_EU_SYNC_PACKET;
				}
								
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_REG_FINISHED, 0, 0);
								
			}
			//resend the user registration information
			else if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "<ack>RR</ack>")))
			{
				if ( ( !CreateRegistrationXml(m_userId, m_pszPhoneNo) )
					|| ( !CallWebService(CONTACTS_SYNC) ))
				{
					ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_REG_FAILED, OUTOFMEMORY, 0);
				}
			}
			else if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "fail_reg")))
			{
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_REG_FAILED, USER_EXIST, 0);
			}
			else
			{
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_REG_FAILED, UNKNOWN, 0);
			}

		break;

	case SG_SYNC_SYNC_PACKET:
	
			if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "lpr_sync")) )
			{
				//last packet received and should start polling
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_POLL, 0, 0);
				m_Status = SG_POLL;
			}
			else if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "cpr_sync")) )
			{
				//current packet received
				char *pszPacketNo=NULL;
				pszPacketNo = pszSearch + STRLEN("cpr_sync");
				int nNo = STRTOUL(pszPacketNo, NULL, 10);
				
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_SUCCESS, nNo, 0);
			}
			else
			{
				//post error message
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_ERROR, 0, 0);
			}

		break;

	case SG_EU_SYNC_PACKET:

			if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "lpr_eu")) )
			{
				m_Status = SG_POLL;
				//last packet received and should start polling after 5 seconds
				if ( SUCCESS != ISHELL_SetTimer(m_pIShell, 5000, PollTimerCB, this))
				{
					b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_ERROR, 0, 0);
				}
			}
			else if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "cpr_eu")) )
			{
				//current packet received
				char *pszPacketNo=NULL;
				pszPacketNo = pszSearch + STRLEN("cpr_eu");
				int nNo = STRTOUL(pszPacketNo, NULL, 10);
				
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_SUCCESS, nNo, 0);
			}
			else
			{
				//post error message
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_ERROR, 0, 0);
			}

		break;

	case SG_IR_SYNC_PACKET:

			if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "lpr_reg")) )
			{
				m_Status = SG_POLL;
				//last packet received and should start polling after 5 seconds
				if ( SUCCESS != ISHELL_SetTimer(m_pIShell, 5000, PollTimerCB, this))
				{
					b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_ERROR, 0, 0);
				}
			}
			else if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "cpr_reg")) )
			{
				//current packet received
				char *pszPacketNo=NULL;
				pszPacketNo = pszSearch + STRLEN("cpr_reg");
				int nNo = STRTOUL(pszPacketNo, NULL, 10);
				
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_SUCCESS, nNo, 0);
			}
			else
			{
				//post error message
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_ERROR, 0, 0);
			}

		break;

	case SG_POLL:
			if ( NULL != (pszSearch=STRSTR(m_pResultbuffer, "wait_time")) )
			{
				//current packet received
				char *pszWaitTime=NULL;
				pszWaitTime = pszSearch + STRLEN("wait_time");
				int nTime = STRTOUL(pszWaitTime, NULL, 10);

				if ( SUCCESS != ISHELL_SetTimer(m_pIShell, nTime*1000, PollTimerCB, this))
				{
					b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SYNC_ERROR, 0, 0);
				}

			}else
			{
				
				m_userInfo->lsyncTime = CUtil::GetCurrentTimeSecs()-3600*24*2;
				STRCPY(m_userInfo->status, "sync");
				CUtil::SetUserInfo(m_userInfo);
			
				//we get a response from poll.
				b = ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_POLL_END, 0, 0);
			}

		break;

	case SG_PHOTO_UPLOAD:
			//ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_PHOTOUPLOAD, 0, 0);
		break;

	case SG_ALBUM_SYNC:
			//write response to album file
			SetAlbumFile();
			ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_ALBUM_SYNC, 0, 0);
		break;

	case SG_SHARE_ALBUM:
			ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_ALBUM_SHARE, 0, 0);
		break;

	case SG_SMS_UPLOAD:
			ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_SMSUPLOAD_FINISHED, 0, 0);
		break;

	default:
		break;

	}
	return SM_SUCCESS;
}

// ================================================================================
// FUNCTION		: PollTimerCB
// DESCRIPTION	: Timer to wait for the poll request.
// ================================================================================
void CServiceGateway::PollTimerCB(void *cxt)
{
	CServiceGateway * pMe = (CServiceGateway *)cxt;
	//poll again
	ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_SYNC_POLL, 0, 0);
}

// ================================================================================
// FUNCTION		: TimeOutWebResponseTimerCB
// DESCRIPTION	: Callback function for web response timer.
// ================================================================================
void CServiceGateway::TimeOutWebResponseTimerCB(void *cxt)
{
	//web response timeout
	CServiceGateway * pMe = (CServiceGateway *)cxt;

	DBGPRINTF("CServiceGateway::TimeOutWebResponseTimerCB = %d", pMe->m_Status);

	//timeout calls the maxmium no of times. no service detected
	if (MAX_TIMEOUTS==pMe->m_nTimeoutCount) 
	{
		//post a service hangout message
		ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_SYN_FAILED, NETWORK_TIMEOUT, 0);
	}

	switch ( pMe->m_Status )
	{
	case SG_INIT_REG:
			//ACK does not reach for Registration Request.
			if ( ( !pMe->CreateRegistrationXml(pMe->m_userId, pMe->m_pszPhoneNo, NULL, "StatusReg") )
					|| ( !pMe->CallWebService(CONTACTS_SYNC) ))
			{
				ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_SYN_FAILED, OUTOFMEMORY, 0);
			}
		break;

	case SG_EXISTING_USER:		
		break;

	case SG_IR_SYNC_PACKET:
	case SG_EU_SYNC_PACKET:
	case SG_SYNC_SYNC_PACKET:
			//Polling signal (P1) to get the ACK to check status of current request at the server side 
			pMe->CreateContactsPollXml();
			if ( !pMe->CallWebService(CONTACTS_SYNC))
			{
				ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_SYN_FAILED, OUTOFMEMORY, 0);
			}
		break;

	case SG_POLL:
			ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_SYN_FAILED, NETWORK_TIMEOUT, 0);
		break;

	case SG_PHOTO_UPLOAD:
			ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_PHOTOUPLOAD, 0, 0);
		break;
	
	case SG_ALBUM_SYNC:
			ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_ALBUM_SYNC, 0, 0);
		break;

	case SG_SHARE_ALBUM:
			ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_ALBUM_SHARE, 0, 0);
		break;

	case SG_SMS_UPLOAD:
			ISHELL_PostEvent(pMe->m_pIShell, APPLICATION_UID, EVT_USER_SMSUPLOAD_FINISHED, 0, 0);
		break;

	default:
		break;
	}

	pMe->m_nTimeoutCount++;
}

// ================================================================================
// FUNCTION		: SetTimeout
// DESCRIPTION	: Set timeout for web response.
// ================================================================================
void CServiceGateway::SetTimeout()
{
	if ( SUCCESS != ISHELL_SetTimer(m_pIShell, 5000, TimeOutWebResponseTimerCB, this))
	{
		ISHELL_PostEvent(m_pIShell, APPLICATION_UID, EVT_USER_ERROR, 0, 0);
	}
}

// ================================================================================
// FUNCTION		: SetStatus
// DESCRIPTION	: Set the status of module.
// ================================================================================
void CServiceGateway::SetStatus(ServiceStatus status)
{
	m_Status = status;
}

// ================================================================================
// FUNCTION		: GetResult
// DESCRIPTION	: return the data buffer contains server response.
// ================================================================================
char* CServiceGateway::GetResult()
{
	return m_pResultbuffer;
}

// ================================================================================
// FUNCTION		: CreateServerUpdatedContactsXml
// DESCRIPTION	: Create the xml to get the server side contacts updates
// ================================================================================
bool CServiceGateway::CreateServerUpdatedContactsXml(CContactInfo *pContactAr, int nSize)
{
	char *pszTemp= (char*)MALLOC(100);
	if ( NULL==pszTemp ) return false;

	CreateContactsXmlHeader(1,1);

	//server side updates
	int nState=0;
	STRCAT(m_pTempBuf, "<mapsa>");
	for (int i=0; i<nSize; i++)
	{
		//Add contact information to the xml file.
		nState=pContactAr[i].GetState();
		if (SM_RECORD_ADD==nState)
		{
			SPRINTF(pszTemp, "<ct id=\"%u\"><gcid>%S</gcid></ct>", 
					pContactAr[i].GetContactId(), pContactAr[i].GetGlobalId());		

			STRCAT(m_pTempBuf, pszTemp); 
		}
	}
	STRCAT(m_pTempBuf, "</mapsa>");

	STRCAT(m_pTempBuf, "<mapsd>");
	for (int j=0; j<nSize; j++)
	{
		//Add contact information to the xml file.
		nState=pContactAr[j].GetState();
		if (SM_RECORD_DELETE==nState)
		{
			
			SPRINTF(pszTemp, "<ct id=\"%u\"></ct>", 
					pContactAr[j].GetContactId());		

			STRCAT(m_pTempBuf, pszTemp); 
		}
	}
	STRCAT(m_pTempBuf, "</mapsd>");

	//contacts xml footer
	STRCAT(m_pTempBuf, "</b></root>");
	m_nContentLength = STRLEN(m_pTempBuf);

	CUtil::LogToFile(m_pTempBuf);

	FREEIF(pszTemp);

	return false;
}

// ================================================================================
// FUNCTION		: CreatePhotosXml
// DESCRIPTION	: Create xml to upload photos.
// ================================================================================
bool CServiceGateway::CreatePhotosXml(const char *pszPhoto, const char *pszAlbum, const char *pszDescription)
{

	char *pData=NULL;
	uint32 dwSize = GetPhotoData(pszPhoto, &pData);
	if ( 0==dwSize ) return false;
	FREEIF(m_pTempBuf);
	m_pTempBuf = (char*) MALLOC( dwSize+512);
	if ( NULL==m_pTempBuf ) return false;

	SPRINTF( m_pTempBuf,"%u-", dwSize);

	//append the image data
	uint32 dwLen = STRLEN(m_pTempBuf);
	MEMCPY((void*)(m_pTempBuf+dwLen), pData, dwSize);
	
	SPRINTF( m_pTempBuf+ dwLen+ dwSize,	"<root>"
										"<h>"
										"<msgt>photosbackup</msgt>"
										"<msg></msg>"
										"<uid>%s</uid>"
										"<hp>123</hp>"
										"<did>%s</did>"
										"</h>"
										"<b>"
										"<folder>"
										"<name>%s</name>"
										"</folder>"
										"<description>%s</description>"
										"</b>"
										"</root>",
										m_userInfo->username, m_userInfo->deviceid, pszAlbum, pszDescription
										);

	

	m_nContentLength = dwLen+ dwSize +STRLEN(m_pTempBuf+ dwLen+ dwSize);
	//CUtil::WriteFile("imgLog.jpg", (char*)pData, dwSize);
	
	m_Status = SG_PHOTO_UPLOAD;
	FREEIF(pData);
	return true;

}

// ================================================================================
// FUNCTION		: GetPhotoData
// DESCRIPTION	: Get the raw data of a photo.
// ================================================================================
uint32 CServiceGateway::GetPhotoData(const char *pszPhoto, char **pDataBuf)
{
	uint32 res=0;
	char *pszPath=(char*)MALLOC(256);
	if (NULL==pszPath) return res;
	SPRINTF(pszPath, "%s", pszPhoto);
	if ( NULL==pszPath ) return 0;

	//read image data
	IFileMgr * pIFileMgr=NULL;
	if (ISHELL_CreateInstance(m_pIShell, AEECLSID_FILEMGR, (void **)&pIFileMgr) == SUCCESS) 
	{
		//get file information
		FileInfo info;
		if ( SUCCESS==IFILEMGR_GetInfo(pIFileMgr, pszPath, &info))
		{
			res = info.dwSize;
			*pDataBuf = (char*)MALLOC(res+1);
			if ( NULL==*pDataBuf )
			{
				IFILEMGR_Release(pIFileMgr);
				return 0;
			}

			IFile * pIFile; 
			pIFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_READ); 

			if (pIFile != NULL) 
			{ 
				//dwResult = IFILE_Read(pIFile, (void*)(*pszBuf), dwResult);
				IAStream * pIStream = (IAStream *) pIFile;
				res = IASTREAM_Read(pIStream, (void*)(*pDataBuf), res);

				IFILE_Release(pIFile);
			}
			
			
		}
		IFILEMGR_Release(pIFileMgr); 
	}

	FREEIF(pszPath);

	return res;
}

// ================================================================================
// FUNCTION		: CreateAlbumSyncXml
// DESCRIPTION	: Create the xml to synchronize albums with server..
// ================================================================================
bool CServiceGateway::CreateAlbumSyncXml()
{
	if (NULL==m_pTempBuf)
	{
		m_pTempBuf = (char*)MALLOC(DATA_BUFFER_LEN);
		if ( NULL==m_pTempBuf ) return false;
	}

	SPRINTF( m_pTempBuf,	"<root>"
							"<h>"
							"<msgt>albumsynch</msgt>"
							"<msg></msg>"
							"<uid>%s</uid>"
							"<hp>%s</hp>"
							"<did>%s</did>"
							"<currec></currec>"
							"<totrec></totrec>"
							"<loc>en_us</loc>"
							"<ctime></ctime>"
							"<lstime>0</lstime>"
							"<synctype><mod>cs</mod>"
							"<gran>contactlevel</gran>"
							"<adde>true</adde><upde>"
							"true</upde><dele>true</dele></synctype>"
							"</h><b></b></root>", m_userInfo->username, m_userInfo->phoneno, m_userInfo->deviceid);

							

	m_nContentLength = STRLEN(m_pTempBuf);
	m_Status = SG_ALBUM_SYNC;

	CUtil::LogToFile(m_pTempBuf);
	return true;

}

// ================================================================================
// FUNCTION		: SetAlbumFile
// DESCRIPTION	: Create album file in phone to match with server
// ================================================================================
void CServiceGateway::SetAlbumFile()
{
	if ( NULL==m_pResultbuffer ) return;

	char *pszAlbumList=NULL;
	pszAlbumList = (char*)MALLOC(STRLEN(m_pResultbuffer)+1);
	if ( NULL==pszAlbumList ) return;

	char *pStart=NULL;
	char *pFound=NULL;
	char *pEnd=NULL;
	char pszSTag[20];
	char pszETag[20];
	int nLen=0;
	STRCPY(pszSTag, "<existing>");
	STRCPY(pszETag, "</existing>");
	int nTagLen = STRLEN(pszSTag);
	char *pszAlbum=NULL;

	pStart = m_pResultbuffer;
	while ( NULL!=(pFound=STRSTR(pStart, pszSTag)))
	{
		pStart = pFound+ nTagLen;
		pEnd = STRSTR(pStart, pszETag);
		nLen = STRLEN(pStart)-STRLEN(pEnd);
		pszAlbum = (char*)MALLOC(nLen+1);
		if ( NULL!=pszAlbum )
		{
			STRLCPY(pszAlbum, pStart, nLen+1);

			//append it to album list
			STRCAT(pszAlbumList, pszAlbum);
			STRCAT(pszAlbumList, ";");
		}
		FREEIF(pszAlbum);
	}


	//write it to album list file
	nLen = STRLEN(pszAlbumList);
	CUtil::WriteFile( ALBUM_FILE, pszAlbumList, nLen);

	FREEIF(pszAlbumList);
}

// ================================================================================
// FUNCTION		: CreateAlbumShareXml
// DESCRIPTION	: Create xml to share a photo album.
// ================================================================================
int CServiceGateway::CreateAlbumShareXml(AECHAR *pwzAlbum, AECHAR *pwzContactsList, AECHAR *pwzMessage)
{
	AECHAR *pStart=NULL;
	AECHAR *pEnd=NULL;
	AECHAR *pPhoneNo=NULL;
	AECHAR *pwsTemp=NULL;
	char *pszName=NULL;
	int nLen=0;
	int nSize=0;
	char *pszPhoneNo=NULL;
	int nPhoneNoLen=0;

	SPRINTF( m_pTempBuf,	"<root>"
							"<h>"
							"<msgt>sharealbum</msgt>"
							"<msg></msg>"
							"<uid>%s</uid>"
							"<hp>%s</hp>"
							"<did>%s</did>"
							"<currec></currec>"
							"<totrec></totrec>"
							"<loc>en_us</loc>"
							"<ctime></ctime>"
							"<lstime>0</lstime>"
							"<synctype>"
							"<mod>cs</mod>"
							"<gran>contactlevel</gran>"
							"<adde>true</adde>"
							"<upde>true</upde>"
							"<dele>true</dele>"
							"</synctype>"
							"</h>"
							"<b>"
							"<share>"
							"<albumid>%S</albumid>"
							"<message>%S</message>"
							"<recipients>",
							m_userInfo->username, m_userInfo->phoneno, m_userInfo->deviceid, 
							pwzAlbum, pwzMessage);
	
	
	pStart=pwzContactsList;
	while( NULL!=(pEnd=WSTRCHR(pStart,';')))
	{
		nLen = WSTRLEN(pStart)-WSTRLEN(pEnd);

		nSize=(nLen+1)*sizeof(AECHAR);

		pwsTemp = (AECHAR*) MALLOC(nSize);
		pszName = (char*) MALLOC(nLen+1);

		if ( NULL==pwsTemp || NULL==pszName)
		{
			FREEIF(pwsTemp);
			FREEIF(pszName);
			return -1;
		}

		WSTRLCPY(pwsTemp, pStart, nLen+1);

		//extract the phone number
		pPhoneNo = WSTRCHR(pwsTemp, ':');
		if (pPhoneNo)
		{	
			//remove phone number from name string
			WSTRCPY(pPhoneNo, L"\0");
			nPhoneNoLen = WSTRLEN(pPhoneNo+1);
			pszPhoneNo = (char*)MALLOC(nPhoneNoLen+1);
			WSTRTOSTR(pPhoneNo+1, pszPhoneNo, nPhoneNoLen+1);
			if (NULL==pszPhoneNo)
			{
				FREEIF(pwsTemp);
				FREEIF(pszName);
				return -1;
			}
			WSTRTOSTR(pPhoneNo+1, pszPhoneNo, nPhoneNoLen+1);
			
		}

		
		WSTRTOSTR(pwsTemp, pszName, nLen+1-nPhoneNoLen);

		//add contacts name to the buffer
		STRCAT( m_pTempBuf, "<object><name>");
		STRCAT( m_pTempBuf, pszName);
		STRCAT( m_pTempBuf, "</name>");
		//add contacts mobile number to the buffer
		STRCAT( m_pTempBuf, "<mobile>");
		if (pszPhoneNo)	
			STRCAT( m_pTempBuf, pszPhoneNo);
		STRCAT( m_pTempBuf, "</mobile>");
		STRCAT( m_pTempBuf, "</object>");

		pStart = pStart+nLen+1;
		

		FREEIF(pwsTemp);
		FREEIF(pszName);
		FREEIF(pszPhoneNo);
	}

	STRCAT( m_pTempBuf, "</recipients></share></b></root>");

	m_nContentLength = STRLEN(m_pTempBuf);
	m_Status = SG_SHARE_ALBUM;
	CUtil::LogToFile(m_pTempBuf);
	return SUCCESS;
}

// ================================================================================
// FUNCTION		: CreateSmsUploadXml
// DESCRIPTION	: Create xml to upload the sms.
// ================================================================================
int CServiceGateway::CreateSmsUploadXml(sms **smsAr, int nSize)
{
	SPRINTF( m_pTempBuf,	"<root>"
							"<h>"
							"<msgt>smsbackup</msgt>"
							"<msg></msg>"
							"<uid>%s</uid>"
							"<hp>%s</hp>"
							"<did>%s</did>"
							"<currec></currec>"
							"<totrec></totrec>"
							"<loc>en_us</loc>"
							"<ctime></ctime>"
							"<lstime>0</lstime>"
							"<synctype>"
							"<mod>cs</mod>"
							"<gran>contactlevel</gran>"
							"<adde>true</adde>"
							"<upde>true</upde>"
							"<dele>true</dele>"
							"</synctype>"
							"</h>"
							"<b>", m_userInfo->username, m_userInfo->phoneno, m_userInfo->deviceid);


	char *pszTemp=NULL;
	pszTemp = (char*)MALLOC(50);
	int nLen=0;
	for (int i=0;i<nSize;i++)
	{
		WSTRTOSTR(smsAr[i]->pszFrom, pszTemp, WSTRLEN(smsAr[i]->pszFrom)+1);

		STRCAT(m_pTempBuf, "<sms><folder>");
		if (0==smsAr[i]->nSent)
		{
			STRCAT(m_pTempBuf, "Inbox</folder><from>");
			STRCAT(m_pTempBuf, pszTemp);
			STRCAT(m_pTempBuf, "</from><to>");
			STRCAT(m_pTempBuf, m_userInfo->phoneno);
			STRCAT(m_pTempBuf, "</to>");
		}
		else
		{
			STRCAT(m_pTempBuf, "Outbox</folder>");
			STRCAT(m_pTempBuf, m_userInfo->phoneno);
			STRCAT(m_pTempBuf, "</from><to>");
			STRCAT(m_pTempBuf, pszTemp);
			STRCAT(m_pTempBuf, "</to>");
		}

		
		FREEIF(pszTemp);
		nLen =WSTRLEN(smsAr[i]->pszTime)+1;
		pszTemp = (char*)MALLOC(nLen);
		WSTRTOSTR(smsAr[i]->pszTime, pszTemp, nLen);
		STRCAT(m_pTempBuf, "<time>");
		STRCAT(m_pTempBuf, pszTemp);
		STRCAT(m_pTempBuf, "</time>");

		FREEIF(pszTemp);
		nLen =WSTRLEN(smsAr[i]->pszMsg)+1;
		pszTemp = (char*)MALLOC(nLen);
		WSTRTOSTR(smsAr[i]->pszMsg, pszTemp, nLen);
		STRCAT(m_pTempBuf, "<text>");
		STRCAT(m_pTempBuf, pszTemp);
		STRCAT(m_pTempBuf, "</text>");
		
		STRCAT(m_pTempBuf, "</sms>");
	}

	FREEIF(pszTemp);
	STRCAT( m_pTempBuf, "</b></root>");
	m_nContentLength = STRLEN(m_pTempBuf);
	m_Status = SG_SMS_UPLOAD;
	CUtil::LogToFile(m_pTempBuf);
	return SUCCESS;
}