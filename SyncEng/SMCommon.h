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
 Module Name				: SMCommon.h
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Common data typed and constants. (header)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#ifndef SMCOMMON_H
#define SMCOMMON_H

#include "AEEAddrBook.h"
#include "AEEStdLib.h"
#include "SMEvents.h"

#define SM_ADDRFIELD_NONE		AEE_ADDRFIELD_NONE                
#define SM_ADDRFIELD_FIRSTNAME		AEE_ADDRFIELD_FIRSTNAME
#define SM_ADDRFIELD_LASTNAME		AEE_ADDRFIELD_LASTNAME
#define SM_ADDRFIELD_EMAIL		AEE_ADDRFIELD_EMAIL
#define SM_ADDRFIELD_FAX		AEE_ADDRFIELD_PHONE_FAX
#define SM_ADDRFIELD_MOBILE		AEE_ADDRFIELD_PHONE_CELL
#define SM_ADDRFIELD_PHONE_HOME		AEE_ADDRFIELD_PHONE_HOME
#define SM_ADDRFIELD_PHONE		AEE_ADDRFIELD_PHONE_OTHER
#define SM_ADDRFIELD_JOBTITLE		AEE_ADDRFIELD_JOBTITLE
#define SM_ADDRFIELD_COMPANY		AEE_ADDRFIELD_ORG

//fileld not define in address book header
#define SM_ADDRFIELD_EMAIL_HOME			AEE_ADDRFIELD_PHONE_OTHER+1
#define SM_ADDRFIELD_EMAIL_WORK			AEE_ADDRFIELD_PHONE_OTHER+2
#define SM_ADDRFIELD_FAX_HOME			AEE_ADDRFIELD_PHONE_OTHER+3
#define SM_ADDRFIELD_FAX_WORK			AEE_ADDRFIELD_PHONE_OTHER+4
#define SM_ADDRFIELD_MOBILE_HOME		AEE_ADDRFIELD_PHONE_OTHER+5
#define SM_ADDRFIELD_MOBILE_WORK		AEE_ADDRFIELD_PHONE_OTHER+6
#define SM_ADDRFIELD_PHONE_WORK			AEE_ADDRFIELD_PHONE_OTHER+7

//Not supported in RV xml file
#define SM_ADDRFIELD_MIDDLENAME		AEE_ADDRFIELD_MIDDLENAME
#define SM_ADDRFIELD_NICKNAME		AEE_ADDRFIELD_NICKNAME
#define SM_ADDRFIELD_NAME		AEE_ADDRFIELD_NAME
#define SM_ADDRFIELD_ADDRESS		AEE_ADDRFIELD_ADDRESS
#define SM_ADDRFIELD_CITY		AEE_ADDRFIELD_CITY
#define SM_ADDRFIELD_STATE		AEE_ADDRFIELD_STATE
#define SM_ADDRFIELD_COUNTRY		AEE_ADDRFIELD_COUNTRY
#define SM_ADDRFIELD_ZIPCODE		AEE_ADDRFIELD_ZIPCODXE
#define SM_ADDRFIELD_URL		AEE_ADDRFIELD_URL
#define SM_ADDRFIELD_DEPARTMENT		AEE_ADDRFIELD_DEPARTMENT
#define SM_ADDRFIELD_GENDER		AEE_ADDRFIELD_GENDER
#define SM_ADDRFIELD_WORK		AEE_ADDRFIELD_PHONE_WORK
#define SM_ADDRFIELD_PHONE_VOICE	AEE_ADDRFIELD_PHONE_VOICE
#define SM_ADDRFIELD_PHONE_PREF		AEE_ADDRFIELD_PHONE_PREF
#define SM_ADDRFIELD_PHONE_PAGER	AEE_ADDRFIELD_PHONE_PAGER
#define SM_ADDRFIELD_PHONE_GENERIC	AEE_ADDRFIELD_PHONE_GENERIC
#define SM_ADDRFIELD_PHONE_OTHER	AEE_ADDRFIELD_PHONE_OTHER
#define SM_ADDRFIELD_BDAY		AEE_ADDRFIELD_BDAY

enum SMError{
	SM_SUCCESS,
	SM_ERROR,
	SM_MallocFailed,
	SM_NOMEMORY,
					//Address book errors
	SM_ADDRBOOK_NoContactInfo,

					//Database errors
	SM_CHANGEDB_FAILED
};

enum DbSearchResult{
	SM_DBERROR,
	SM_RECORD_EQUAL,
	SM_RECORD_ADD,
	SM_RECORD_UPDATE,
	SM_RECORD_DELETE
};

typedef enum
{
	DB_CHANGES_RECORD_ID,
	DB_CHANGES_RECORD_STATE,
	DB_CHANGES_RECORD_FNAME,
	DB_CHANGES_RECORD_LNAME,
	DB_CHANGES_RECORD_COMPANY,
	DB_CHANGES_RECORD_JTITLE,
	DB_CHANGES_RECORD_FAX,
	DB_CHANGES_RECORD_FAXH,
	DB_CHANGES_RECORD_FAXW,
	DB_CHANGES_RECORD_MOBILE,
	DB_CHANGES_RECORD_MOBILEH,
	DB_CHANGES_RECORD_MOBILEW,
	DB_CHANGES_RECORD_TELEPHONE,
	DB_CHANGES_RECORD_TELEPHONEH,
	DB_CHANGES_RECORD_TELEPHONEW,
	DB_CHANGES_RECORD_EMAIL,
	DB_CHANGES_RECORD_EMAILH,
	DB_CHANGES_RECORD_EMAILW,
	NUM_DB_CHANGES_RECORD_FIELDS
} EChanesDbType;

//Backup database
typedef enum
{
	DB_BACKUP_RECORD_ID,
	DB_BACKUP_RECORD_FNAME,
	DB_BACKUP_RECORD_LNAME,
	DB_BACKUP_RECORD_COMPANY,
	DB_BACKUP_RECORD_JTITLE,
	DB_BACKUP_RECORD_FAX,
	DB_BACKUP_RECORD_FAXH,
	DB_BACKUP_RECORD_FAXW,
	DB_BACKUP_RECORD_MOBILE,
	DB_BACKUP_RECORD_MOBILEH,
	DB_BACKUP_RECORD_MOBILEW,
	DB_BACKUP_RECORD_TELEPHONE,
	DB_BACKUP_RECORD_TELEPHONEH,
	DB_BACKUP_RECORD_TELEPHONEW,
	DB_BACKUP_RECORD_EMAIL,
	DB_BACKUP_RECORD_EMAILH,
	DB_BACKUP_RECORD_EMAILW,
	NUM_DB_BACKUP_RECORD_FIELDS
} EBackupDbType;

//SyncManager Database field types  and field names.
#define SMDB_FT_WORD	AEEDB_FT_WORD
#define SMDB_FT_STRING	AEEDB_FT_STRING

#define SMDB_FIELD_NUM		AEEDBFIELD_PREF_ID
#define SMDB_FIELD_TEXT		AEEDBFIELD_TEXT

//used to represent empty field in database
#define NODATA			{'N', 'O', 'D', 'A', 'T', 'A', '\0'}

//new field im multiple fields in addressbook
#define NEW_FIELD		{'<', 'N', 'E', 'W', '>', '\0'}

//web services
#define CONTACTS_SYNC		"/ripple/rippleservlet"
#define PHOTOS_UPLOAD		"/ripple/mediauploadservlet"
#define ALBUM_SYNC			"/ripple/albumsyncingservlet"
#define ALBUM_SHARE			"/ripple/sharingservlet"
#define SMS_UPLOAD			"/ripple/smsServlet"

#define CONFIG_FILE			"config.txt"

//character array limits


#endif    // SMCOMMON_H