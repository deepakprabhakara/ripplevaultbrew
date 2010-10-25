
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
 Module Name				: UserData.c
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF
 Date Created               : 02-08-2005
 Rational Rose Model Name   :                          
 Use Case Document Name     : Use for all use cases.                          
 Use Case Realization Document Name   :
 Test Case Document Name    : 
 Abbreviations              :
 Description                : 
 
 
 Code Reviews
 Code Reviewed By           :
 Code Reviewed Date			:
 
 Recommendations            :
 

***************************************************************/

#include "UserData.h"
#include "AEEFile.h"
#include "AEEShell.h"
#include "AEEStdLib.h"

// ================================================================================
// FUNCTION		: ReadFileStream
// DESCRIPTION	: READ DATA FROM THE FILE
// ================================================================================

uint32 ReadFileStream(IShell * pIShell, const char *pszPath, char **pszBuf)
{
	IFileMgr * pIFileMgr; 
	uint32 dwResult=0;
	FileInfo info;
	IFile *pFile=NULL;
	
	if (ISHELL_CreateInstance(pIShell, AEECLSID_FILEMGR, (void **)&pIFileMgr) == SUCCESS) 
	{
		//get file information
		if ( SUCCESS==IFILEMGR_GetInfo(pIFileMgr, pszPath, &info))
		{
			dwResult = info.dwSize;
			*pszBuf = (char*)MALLOC(dwResult+1);
			if ( NULL==*pszBuf )
			{
				IFILEMGR_Release(pIFileMgr);
				return dwResult;
			}
		
			pFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_READ); 

			if (pFile != NULL) 
			{ 
				dwResult = IFILE_Read(pFile, (void*)(*pszBuf), dwResult);

				IFILE_Release(pFile);
			}
			
			
		}
		IFILEMGR_Release(pIFileMgr); 
	}
	
	return dwResult;
	
}

// ================================================================================
// FUNCTION		: WriteFileStream
// DESCRIPTION	: WRITE DATA TO THE FILE
// ================================================================================

uint32 WriteFileStream(IShell * pIShell, const char *pszPath, const char *pszBuf, int nLen, boolean bAppend)
{
	IFileMgr * pIFileMgr; 
	uint32 dwRes=0;
	IFile * pFile;

	if (ISHELL_CreateInstance(pIShell, AEECLSID_FILEMGR, (void **)&pIFileMgr) == SUCCESS) 
	{ 		
		if ( IFILEMGR_Test(pIFileMgr, pszPath) != SUCCESS) 
			pFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_CREATE);
		else 
		{
			if ( bAppend )
				pFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_APPEND); 
			else
			{
				//remove first
				IFILEMGR_Remove(pIFileMgr, pszPath);
				pFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_CREATE);
			}
		}

		 
		if (pFile != NULL) 
		{ 
			dwRes = IFILE_Write(pFile, pszBuf, nLen);  
			IFILE_Release(pFile); 
		}

		IFILEMGR_Release(pIFileMgr); 
	};

	return dwRes;
}

// ================================================================================
// FUNCTION		: GetUserData
// DESCRIPTION	: GET DATA FROM CONFIG FILE
// ================================================================================

boolean GetUserData(IShell * pIShell, user **pUserInfo)
{
	char *pszUserInfo=NULL;
	char *pNewLine=NULL;
	char *pStart=NULL;
	int nCount=0;
	int nLen;
	char pszLSyncTime[20];
	char pszShedule[20];
	char uploadTimeSeconds[25];

	FREEIF(*pUserInfo);
	*pUserInfo = MALLOC(sizeof(user));
	if (NULL==(*pUserInfo) ) return FALSE;

	if ( NULL==pUserInfo ) return FALSE;
	
	if ( ReadFileStream(pIShell, CONFIG_FILE, &pszUserInfo)>0 )
	{
		pStart = pszUserInfo;
		nCount=0;
		nLen=STRLEN(pStart);
		
		while( NULL!=(pNewLine=STRSTR(pStart, ,"\r\n")))
		{
			nLen = STRLEN(pStart)-STRLEN(pNewLine);

			switch (nCount)
			{
			case 0:
				STRLCPY((*pUserInfo)->host, pStart, nLen+1);
				break;
			case 1:
				STRLCPY((*pUserInfo)->status, pStart, nLen+1);
				break;
			case 2:
				STRLCPY((*pUserInfo)->username, pStart, nLen+1);
				break;
			case 3:
				STRLCPY((*pUserInfo)->phoneno, pStart, nLen+1);
				break;
			case 4:
				STRLCPY((*pUserInfo)->deviceid, pStart, nLen+1);
				break;
			case 5:
				STRLCPY(pszLSyncTime, pStart, nLen+1);
				(*pUserInfo)->lsyncTime = STRTOUL(pszLSyncTime, NULL, 10);
				break;
			case 6:
				STRLCPY(pszShedule, pStart, nLen+1);
				(*pUserInfo)->shedulePeriod = STRTOUL(pszShedule, NULL, 10);
				break;
			case 7:
				STRLCPY((*pUserInfo)->roomingState, pStart, nLen+1);
				break;
			case 8:
				STRLCPY((*pUserInfo)->url, pStart, nLen+1);
				break;
			case 9:
				STRLCPY((*pUserInfo)->sheduler, pStart, nLen+1);
				break;
			case 10:
				STRLCPY((*pUserInfo)->autostart, pStart, nLen+1);
				break;
			case 11:
				STRLCPY((*pUserInfo)->album, pStart, nLen+1);
				break;
			case 12:
				STRLCPY((*pUserInfo)->message, pStart, nLen+1);
				break;
			case 13:
				STRLCPY((*pUserInfo)->roaming, pStart, nLen+1);
				break;
			case 14:
				STRLCPY((*pUserInfo)->smsuploadType, pStart, nLen+1);
				break;
			case 15:
				STRLCPY((*pUserInfo)->roamingUpload, pStart, nLen+1);
				break;
			case 16:
				STRLCPY(uploadTimeSeconds, pStart, nLen+1);
				(*pUserInfo)->uploadTimeSeconds = STRTOUL(uploadTimeSeconds, NULL, 10);
				break;
			default:
				break;
			}

			pStart = pNewLine+2;
			nCount++;
		}

		nLen=STRLEN(pStart);
		switch (nCount)
		{
		case 0:
			STRLCPY((*pUserInfo)->host, pStart, nLen+1);
			break;
		case 1:
			STRLCPY((*pUserInfo)->status, pStart, nLen+1);
			break;
		case 2:
			STRLCPY((*pUserInfo)->username, pStart, nLen+1);
			break;
		case 3:
			STRLCPY((*pUserInfo)->phoneno, pStart, nLen+1);
			break;
		case 4:
			STRLCPY((*pUserInfo)->deviceid, pStart, nLen+1);
			break;
		case 5:
			STRLCPY(pszLSyncTime, pStart, nLen+1);
			(*pUserInfo)->lsyncTime = STRTOUL(pszLSyncTime, NULL, 10);
			break;
		case 6:
			STRLCPY(pszShedule, pStart, nLen+1);
			(*pUserInfo)->shedulePeriod = STRTOUL(pszShedule, NULL, 10);
			break;
		case 7:
			STRLCPY((*pUserInfo)->roomingState, pStart, nLen+1);
			break;
		case 8:
			STRLCPY((*pUserInfo)->url, pStart, nLen+1);
			break;
		case 9:
			STRLCPY((*pUserInfo)->sheduler, pStart, nLen+1);
			break;
		case 10:
			STRLCPY((*pUserInfo)->autostart, pStart, nLen+1);
			break;
		case 11:
			STRLCPY((*pUserInfo)->album, pStart, nLen+1);
			break;
		case 12:
			STRLCPY((*pUserInfo)->message, pStart, nLen+1);
			break;
		case 13:
			STRLCPY((*pUserInfo)->roaming, pStart, nLen+1);
			break;
		case 14:
			STRLCPY((*pUserInfo)->smsuploadType, pStart, nLen+1);
			break;
		case 15:
			STRLCPY((*pUserInfo)->roamingUpload, pStart, nLen+1);
			break;
		case 16:
			STRLCPY(uploadTimeSeconds, pStart, nLen+1);
			(*pUserInfo)->uploadTimeSeconds = STRTOUL(uploadTimeSeconds, NULL, 10);
			break;
		default:
			break;
		}

	}

	FREEIF(pszUserInfo);

	return TRUE;
}

// ================================================================================
// FUNCTION		: SetUserData
// DESCRIPTION	: SET DATA TO THE CONFIG FILE
// ================================================================================

boolean SetUserData(IShell * pIShell, user *pUserInfo)
{
	char *pszTmp=NULL;
	char pszTime[20];
	char pszShedule[20];
	char uploadTimeSeconds[25];
	int nLen;

	SPRINTF(pszTime, "%u", pUserInfo->lsyncTime);
	SPRINTF(pszShedule, "%u", pUserInfo->shedulePeriod);
	SPRINTF(uploadTimeSeconds, "%u", pUserInfo->uploadTimeSeconds);

	pszTmp = (char*)MALLOC(STRLEN(pUserInfo->host)+STRLEN(pUserInfo->status)+
						STRLEN(pUserInfo->username)+STRLEN(pUserInfo->deviceid)+
							STRLEN(pszTime)+ STRLEN(pszShedule)+ STRLEN(pUserInfo->roomingState)+
							STRLEN(pUserInfo->url)+STRLEN(pUserInfo->sheduler)+	STRLEN(pUserInfo->autostart)+
							STRLEN(pUserInfo->album) +STRLEN(pUserInfo->message) +
							STRLEN(pUserInfo->roaming) +STRLEN(pUserInfo->smsuploadType)+STRLEN(pUserInfo->smsuploadType)+STRLEN(pUserInfo->smsuploadType)+125);//100=space for "/r/n"
	if (NULL==pszTmp) return FALSE;

	SPRINTF(pszTmp, "%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s",
								pUserInfo->host,
								pUserInfo->status,
								pUserInfo->username,
								pUserInfo->phoneno,
								pUserInfo->deviceid,
								pszTime,
								pszShedule,
								pUserInfo->roomingState,
								pUserInfo->url,
								pUserInfo->sheduler,
								pUserInfo->autostart,
								pUserInfo->album,
								pUserInfo->message,
								pUserInfo->roaming,
								pUserInfo->smsuploadType,
								pUserInfo->roamingUpload,
								uploadTimeSeconds);
	nLen = STRLEN(pszTmp);

	if ( WriteFileStream(pIShell, CONFIG_FILE, pszTmp, nLen, FALSE) >0)
	{
		FREEIF(pszTmp);
		return TRUE;
	}

	FREEIF(pszTmp);

	return FALSE;

}