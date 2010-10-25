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
 Module Name				: Util.h
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Utility class. (header)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#if !defined(AFX_UTIL_H__52CF8EBC_0E0A_4240_ADBD_51F9D18F9E44__INCLUDED_)
#define AFX_UTIL_H__52CF8EBC_0E0A_4240_ADBD_51F9D18F9E44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AEE.h"
#include "SMCommon.h"


class CUtil  
{
public:
	static unsigned long GetSheduleTime();
	static bool SetUserInfo(user *pUserInfo);
	static bool GetUserInfo(user **pUserInfo);
	static uint32 WriteFile(const char *pFile, const char *pszBuf, int nLen, bool bAppend=false);
	static uint32 ReadFile(const char *pszPath, char **pszBuf);
	static uint32 GetCurrentTimeSecs();
	static void WDBGPrint(AECHAR* psStr);
	CUtil();
	virtual ~CUtil();
	static void LogToFile(const char * const buf);
	static void LogToFile(const AECHAR * const wbuf);
};

#endif // !defined(AFX_UTIL_H__52CF8EBC_0E0A_4240_ADBD_51F9D18F9E44__INCLUDED_)
