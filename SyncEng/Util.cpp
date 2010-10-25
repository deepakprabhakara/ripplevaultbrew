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
 Module Name				: Util.cpp
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Utility class. (source)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#include "Util.h"
#include "AEEStdLib.h"
#include "AEEFile.h"
#include "AEEAppGen.h"

#define LOGFILE_PATH "log.txt"

// ================================================================================
// FUNCTION		: CUtil
// DESCRIPTION	: CUtil constructor.
// ================================================================================
CUtil::CUtil()
{

}

// ================================================================================
// FUNCTION		: ~CUtil
// DESCRIPTION	: CUtil destructor.
// ================================================================================
CUtil::~CUtil()
{

}

// ================================================================================
// FUNCTION		: WDBGPrint
// DESCRIPTION	: Debug print a wide string
// ================================================================================
void CUtil::WDBGPrint(AECHAR *psStr)
{
	DBGPRINTF("%S", psStr);
}

// ================================================================================
// FUNCTION		: LogToFile
// DESCRIPTION	: Write a char* to log file
// ================================================================================
void CUtil::LogToFile(const char * const buf)
{ 
	JulianType jt; 
	char timestamp[30]; 
	GETJULIANDATE(GETTIMESECONDS(), &jt); 
	SPRINTF(timestamp, "[%02d:%02d:%02d] ", jt.wHour, jt.wMinute, jt.wSecond);
	WriteFile("log.txt", timestamp, STRLEN(timestamp), true);
	WriteFile("log.txt", buf, STRLEN(buf), true);
	WriteFile("log.txt", "\r\n", 2, true);
}

// ================================================================================
// FUNCTION		: LogToFile
// DESCRIPTION	: Write a AECHAR* to log file
// ================================================================================
void CUtil::LogToFile(const AECHAR * const wbuf)
{
	if ( NULL==wbuf ) return;

	int nLen = WSTRLEN(wbuf);

	char * str = NULL;
	str = (char*)MALLOC(nLen+1);

	if ( NULL==str ) return;

	WSTRTOSTR(wbuf, str, nLen+1);

	LogToFile(str);

	FREEIF(str);
}

// ================================================================================
// FUNCTION		: GetCurrentTimeSecs
// DESCRIPTION	: get current time in seconds
// ================================================================================
uint32 CUtil::GetCurrentTimeSecs()
{
	uint32 dwSecs=0;
	JulianType Date;
	//current time
	GETJULIANDATE(dwSecs, &Date);

	//seconds since Jan 1, 1970
	if ( Date.wYear<1970 ) return 0;

	dwSecs = JULIANTOSECONDS(&Date)+ 315943200; //-3600*24*30no ofseconds from1970 jan 1 to 1980 jan 6=315943200
	DBGPRINTF("CUtil::GetCurrentTimeSecs %u", dwSecs);
	return dwSecs;
}

// ================================================================================
// FUNCTION		: ReadFile
// DESCRIPTION	: Read a file from disk
// ================================================================================
uint32 CUtil::ReadFile(const char *pszPath, char **pszBuf)
{
	AEEApplet * app = (AEEApplet *)GET_APP_INSTANCE();
	IShell * pIShell = app->m_pIShell;
	uint32 dwResult=0;

	IFileMgr * pIFileMgr; 
	
	if (ISHELL_CreateInstance(pIShell, AEECLSID_FILEMGR, (void **)&pIFileMgr) == SUCCESS) 
	{
		//get file information
		FileInfo info;
		if ( SUCCESS==IFILEMGR_GetInfo(pIFileMgr, pszPath, &info))
		{
			dwResult = info.dwSize;
			*pszBuf = (char*)MALLOC(dwResult+1);
			if ( NULL==*pszBuf )
			{
				IFILEMGR_Release(pIFileMgr);
				return false;
			}

			IFile * pIFile; 
			pIFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_READ); 

			if (pIFile != NULL) 
			{ 
				dwResult = IFILE_Read(pIFile, (void*)(*pszBuf), dwResult);

				IFILE_Release(pIFile);
			}
		
		}
		IFILEMGR_Release(pIFileMgr); 
	}

	return dwResult;
}

// ================================================================================
// FUNCTION		: WriteFile
// DESCRIPTION	: write a  file to disk
// ================================================================================
uint32 CUtil::WriteFile(const char *pszPath, const char *pszBuf, int nLen, bool bAppend)
{
	AEEApplet * app = (AEEApplet *)GET_APP_INSTANCE();
	IShell * pIShell = app->m_pIShell; 
	uint32 dwRes=0;

	IFileMgr * pIFileMgr; 

	if (ISHELL_CreateInstance(pIShell, AEECLSID_FILEMGR, (void **)&pIFileMgr) == SUCCESS) 
	{ 
		IFile * pIFile; 
		if ( IFILEMGR_Test(pIFileMgr, pszPath) != SUCCESS) 
			pIFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_CREATE);
		else 
		{
			if ( bAppend )
				pIFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_APPEND); 
			else
			{
				//remove first
				IFILEMGR_Remove(pIFileMgr, pszPath);
				pIFile = IFILEMGR_OpenFile(pIFileMgr, pszPath, _OFM_CREATE);
			}
		}
		 
		if (pIFile != NULL) 
		{ 
			dwRes = IFILE_Write(pIFile, pszBuf, nLen);  
			IFILE_Release(pIFile); 
		}

		IFILEMGR_Release(pIFileMgr); 
	};

	return dwRes;
}

// ================================================================================
// FUNCTION		: GetUserInfo
// DESCRIPTION	: get user information from config file.
// ================================================================================
bool CUtil::GetUserInfo(user **pUserInfo)
{
	AEEApplet * app = (AEEApplet *)GET_APP_INSTANCE();
	IShell * pIShell = app->m_pIShell; 

	FREEIF(*pUserInfo);
	*pUserInfo = new user;
	if (NULL==(*pUserInfo) ) return false;

	if ( NULL==pUserInfo ) return false;

	//set user info with default information
	STRCPY((*pUserInfo)->host, "http://172.20.1.95:8080");
	STRCPY((*pUserInfo)->status, "sync");
	STRCPY((*pUserInfo)->username, "newuser");
	STRCPY((*pUserInfo)->phoneno, "123");
	STRCPY((*pUserInfo)->deviceid, "0000");
	(*pUserInfo)->lsyncTime=0;

	char *pszUserInfo=NULL;
	if ( ReadFile(CONFIG_FILE, &pszUserInfo)>0 )
	{
		char *pNewLine=NULL;
		char *pStart = pszUserInfo;
		int nCount=0;
		int nLen=STRLEN(pStart);
		char pszLSyncTime[20];
		char pszShedule[20];

		while( NULL!=(pNewLine=STRSTR(pStart, ,"\r\n")))
		{
			nLen = STRLEN(pStart)-STRLEN(pNewLine);

			if (nLen>0)
			{

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
				default:
					break;
				}
			
				nCount++;
			}

			pStart = pNewLine+2;
			
		}

		//add last
		nLen=STRLEN(pStart);
		if (nLen>0)
		{

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
			default:
				break;
			}
		
		}

		
	}

	FREEIF(pszUserInfo);

	return true;

}

// ================================================================================
// FUNCTION		: SetUserInfo
// DESCRIPTION	: Set user information to config file.
// ================================================================================
bool CUtil::SetUserInfo(user *pUserInfo)
{
	char *pszTmp=NULL;
	char pszTime[20];
	char pszShedule[20];
	int nLen;

	SPRINTF(pszTime, "%u", pUserInfo->lsyncTime);
	SPRINTF(pszShedule, "%u", pUserInfo->shedulePeriod);

	pszTmp = (char*)MALLOC(STRLEN(pUserInfo->host)+STRLEN(pUserInfo->status)+
						STRLEN(pUserInfo->username)+STRLEN(pUserInfo->deviceid)+
							STRLEN(pszTime)+ STRLEN(pszShedule)+ STRLEN(pUserInfo->roomingState)+STRLEN(pUserInfo->url)+STRLEN(pUserInfo->sheduler)+STRLEN(pUserInfo->autostart)+50);//20=space for "/r/n"
	if (NULL==pszTmp) return FALSE;

	SPRINTF(pszTmp, "%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s",
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
								pUserInfo->autostart);
	nLen = STRLEN(pszTmp);

	if ( WriteFile(CONFIG_FILE, pszTmp, nLen, FALSE) >0)
	{
		FREEIF(pszTmp);
		return TRUE;
	}

	FREEIF(pszTmp);

	return FALSE;
}

// ================================================================================
// FUNCTION		: GetSheduleTime
// DESCRIPTION	: Get shedule time from config file.
// ================================================================================
uint32 CUtil::GetSheduleTime()
{
	uint32 time=60*60;//default time 1 hour
	
	user *pUserInfo=NULL;
	if ( GetUserInfo(&pUserInfo))
	{
		time=pUserInfo->shedulePeriod;
		delete pUserInfo;
	}
	return time;
}