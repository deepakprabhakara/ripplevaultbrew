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
 Module Name				: SMEvents.h
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Application and synchronization module event definitions. (header)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#ifndef SMEVENTS_H
#define SMEVENTS_H

#include "..\RippleVault\RippleVault.bid"

//UI id
#define APPLICATION_UID			AEECLSID_RIPPLEVAULT

//SyncEngine events
#define EVT_USER_ERROR			EVT_USER+1

#define EVT_USER_SYNC_SUCCESS	EVT_USER+10
#define EVT_USER_SYNC_ERROR		EVT_USER+11
#define EVT_USER_SYNC_POLL		EVT_USER+12

#define EVT_USER_REG_SUCCESS	EVT_USER+20
#define EVT_USER_REG_EXIST		EVT_USER+21
#define EVT_USER_REG_ERROR		EVT_USER+22

#define EVT_USER_POLL_END		EVT_USER+23

#define EVT_USER_DONE			EVT_USER+50

//Events for UI
#define EVT_USER_STRAT					EVT_USER+100
#define EVT_USER_FINISHED				EVT_USER+101
#define EVT_USER_REG_FINISHED			EVT_USER+102
#define EVT_USER_REG_FAILED				EVT_USER+103
#define EVT_USER_SYN_FINISHED			EVT_USER+104
#define EVT_USER_SYN_FAILED				EVT_USER+105
#define EVT_USER_PHOTOUPLOAD			EVT_USER+106
#define EVT_USER_ALBUM_SYNC				EVT_USER+107
#define EVT_USER_ALBUM_SHARE			EVT_USER+108
#define EVT_USER_SHOWMENU				EVT_USER+109
#define EVT_USER_SMSUPLOAD_FINISHED		EVT_USER+110
#define EVT_USER_SMSUPLOAD_FAILED		EVT_USER+111

#define 	EVT_USER_SHEDULE 	EVT_USER+200
#define		EVT_USER_EXIT		EVT_USER+201
#define		EVT_USER_MINIMIZE	EVT_USER+202
#define		EVT_USER_SMSUPLOAD	EVT_USER+203

enum SMNotify
{
	Success,
	Error,
	NETWORK_TIMEOUT,
	OUTOFMEMORY,
	USER_EXIST,
	UNKNOWN
};

typedef struct _user
{
	char host[100];
	char username[100];
	char phoneno[50];
	char deviceid[100];
	char status[10];
	char url[30];
	char sheduler[10];
	char autostart[10];
	char roomingState[10];
	unsigned long lsyncTime;
	uint32 shedulePeriod;
	char album[50];
	char message[100];
	char roaming[5];
	char smsuploadType[5];
	char roamingUpload[5];
	uint32 uploadTimeSeconds;
}user;

typedef struct _sms{
	uint16 nSmsId;
	uint16 nContactId;
	AECHAR *pszFrom;
	AECHAR *pszMsg;
	AECHAR *pszTime;
	boolean bStatus;
	uint16 nSent;
}sms;


#endif    // SMEVENTS_H