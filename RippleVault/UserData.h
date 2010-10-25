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


#include "AEE.h"
#include "../SyncEng/SMCommon.h"

#ifndef __USERDATA_H__
#define __USERDATA_H__
#define ALBUM_FILE			"albums.txt"

uint32 ReadFileStream(IShell * pIShell, const char *pszPath, char **pszBuf);
uint32 WriteFileStream(IShell * pIShell, const char *pszPath, const char *pszBuf, int nLen, boolean bAppend);
boolean	GetUserData(IShell * pIShell, user **pUserInfo);
boolean	SetUserData(IShell * pIShell, user *pUserInfo);

#endif /* __USERDATA_H__ */

