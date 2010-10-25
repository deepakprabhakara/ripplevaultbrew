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
 Module Name				: ContactIfo.cpp
 Language used				: C and Brew APIs
 Coding Standard			: EC Coding Standards
 Author                     : PDF
 Date Created               : 30-07-2005
 Rational Rose Model Name   : RVBrewClient.mdl                         :
 Use Case Document Name     : 
 Use Case Realization Document Name   :
 Test Case Document Name      : 
 Abbreviations                :

 Description                  : Handle database and addressbook (source)
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
#include "ContactInfo.h"
#include "AEEStdLib.h"
#include "Util.h"

#define	NULL_TERMINATE	{'\0'}

// ================================================================================
// FUNCTION		: CContactInfo
// DESCRIPTION	: Contructor for the CContactInfo class.
// ================================================================================
CContactInfo::CContactInfo()
{
	ResetContact();

}

// ================================================================================
// FUNCTION		: ~CContactInfo
// DESCRIPTION	: Destructor for the CContactInfo class.
// ================================================================================
CContactInfo::~CContactInfo()
{
	ReleaseMem();
}

// ================================================================================
// FUNCTION		: ResetContact
// DESCRIPTION	: Reset the resource associate with contact.
// ================================================================================
void CContactInfo::ResetContact()
{
	ReleaseMem();

	m_nContactId = 0;
	m_psFName = NULL;
	m_psMName = NULL;
	m_psLName = NULL;  
	m_psCompany = NULL;	         
	m_psJTitle = NULL;	         
	m_psFaxH = NULL;
	m_psFaxW = NULL;
	m_psFax = NULL;
	m_psMobileH = NULL;	         
	m_psMobileW = NULL;
	m_psMobile = NULL;
	m_psTelephoneH = NULL;
	m_psTelephoneW = NULL;
	m_psTelephone = NULL;
	m_psEmailW = NULL;
	m_psEmailH = NULL;
	m_psEmail = NULL;

	m_nState = SM_DBERROR;
	m_psGlobalId = NULL;//invalid global id
}

// ================================================================================
// FUNCTION		: ReleaseMem
// DESCRIPTION	: Release the memory for contact
// ================================================================================
void CContactInfo::ReleaseMem()
{

	FREEIF(m_psFName);	         	         
	FREEIF(m_psLName);
	FREEIF(m_psMName);
	FREEIF(m_psCompany);	         
	FREEIF(m_psJTitle);	         
	FREEIF(m_psFaxH);
	FREEIF(m_psFaxW);
	FREEIF(m_psFax);
	FREEIF(m_psMobileH);	         
	FREEIF(m_psMobileW);
	FREEIF(m_psMobile);
	FREEIF(m_psTelephoneH);
	FREEIF(m_psTelephoneW);
	FREEIF(m_psTelephone);
	FREEIF(m_psEmailW);
	FREEIF(m_psEmailH);
	FREEIF(m_psEmail);
	FREEIF(m_psGlobalId);

}

// ================================================================================
// FUNCTION		: SetContactInfo
// DESCRIPTION	: Set contact information from a addressbook record.
// ================================================================================
int CContactInfo::SetContactInfo(IAddrRec *pAddrRec)
{
	//clear memory before setting new values
	ReleaseMem();

	//get fields of a record
	AEEAddrField* pField;

	int noOfFilelds = IADDRREC_GetFieldCount(pAddrRec);
	if ( noOfFilelds<1 ) return SM_ADDRBOOK_NoContactInfo;

	m_nContactId = IADDRREC_GetRecID(pAddrRec);

	for ( int i=0; i<noOfFilelds; i++)
	{
		pField = IADDRREC_GetField(pAddrRec, i);

		if (!SetField(pField)) return SM_MallocFailed;
	}

	return SM_SUCCESS;
}

// ================================================================================
// FUNCTION		: SetField
// DESCRIPTION	: Set addressbook field from a contact
// ================================================================================
bool CContactInfo::SetField(AEEAddrField *pField)
{
	int nLen=0;

	switch (pField->fID){

	case SM_ADDRFIELD_FIRSTNAME:
		
		if ( !CopyFieldTo(&m_psFName, (AECHAR*)pField->pBuffer) ) 
			return false;
		
		break;

	case SM_ADDRFIELD_MIDDLENAME://append the middle name to first name. bcos RV xml hasnt got a tag for middle name
		
		nLen = WSTRLEN( (AECHAR*)pField->pBuffer );
		if ( NULL==m_psFName ){
			
			if ( !CopyFieldTo(&m_psFName, (AECHAR*)pField->pBuffer) )
				return false;
		}
		else
		{
			AECHAR* pName=NULL;
			int nFNameLen = WSTRLEN( m_psFName );//for space and endline char
			pName= (AECHAR*)REALLOC( (void*)m_psFName, (nFNameLen+ nLen+ 2)*sizeof(AECHAR));
			if ( NULL==pName ) return false;
			AECHAR	szSpace[] = {' ', '\0'};
			WSTRCAT( pName, szSpace);
			WSTRCAT(pName+ nFNameLen+ 1, (AECHAR*)pField->pBuffer);
			m_psFName = pName;
			
		}
		
		break;

	case SM_ADDRFIELD_LASTNAME:
		
		if ( !CopyFieldTo(&m_psLName, (AECHAR*)pField->pBuffer) )
			return false;
		
		break;

	case SM_ADDRFIELD_EMAIL:

		if ( !AppendFieldTo(&m_psEmail, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_EMAIL_HOME:

		if ( !AppendFieldTo(&m_psEmailH, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_EMAIL_WORK:

		if ( !AppendFieldTo(&m_psEmailW, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_FAX:

		if ( !AppendFieldTo(&m_psFax, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_FAX_HOME:

		if ( !AppendFieldTo(&m_psFaxH, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_FAX_WORK:

		if ( !AppendFieldTo(&m_psFaxW, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_MOBILE:

		if ( !AppendFieldTo(&m_psMobile, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_MOBILE_HOME:

		if ( !AppendFieldTo(&m_psMobileH, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_MOBILE_WORK:

		if ( !AppendFieldTo(&m_psMobileW, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_PHONE_HOME:

		if ( !AppendFieldTo(&m_psTelephoneH, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_PHONE:

		if ( !AppendFieldTo(&m_psTelephone, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	case SM_ADDRFIELD_PHONE_WORK:

		if ( !AppendFieldTo(&m_psTelephoneW, (AECHAR*)pField->pBuffer) )
			return false;
		break;

	
	default:
		break;
	}

	return true;
}

// ================================================================================
// FUNCTION		: CopyFieldTo
// DESCRIPTION	: Create and copy a AECHAR from source to destination
// ================================================================================
bool CContactInfo::CopyFieldTo(AECHAR **psDes, AECHAR *psScr)
{
	int nLen = 0;

	if ( NULL==psScr ) return false;

	if ( NULL==(*psDes) ){
		nLen = WSTRLEN( psScr );
		*psDes = (AECHAR*)MALLOC( (nLen+1)*sizeof(AECHAR) );
		if ( NULL==(*psDes) ) return false;
		WSTRLCPY(*psDes, psScr, nLen+1);
	}
	else
		return false;

	return true;
}

// ================================================================================
// FUNCTION		: AppendFieldTo
// DESCRIPTION	: Append AECHAR from source to destination
// ================================================================================
bool CContactInfo::AppendFieldTo(AECHAR **psDes, AECHAR *psScr)
{
	
	int nLen = WSTRLEN( psScr );
	if ( NULL==*psDes ){
		
		if ( !CopyFieldTo(psDes, psScr ) )
			return false;
		
	}
	else
	{

		AECHAR* pBuf=NULL;
		AECHAR	szNew[] = NEW_FIELD;
		int nDesLen = WSTRLEN( *psDes );
		int newBufLen = nDesLen+ nLen+ WSTRLEN(szNew);
		pBuf = (AECHAR*)REALLOC( (void*)*psDes, (newBufLen+1)*sizeof(AECHAR) );
		
		if ( NULL==pBuf ) return false;
		
		WSTRCAT( pBuf, szNew);
		WSTRLCAT(pBuf+ nDesLen+ WSTRLEN(szNew), psScr, nLen+1);
		*psDes = pBuf;
		
	}

	return true;
}

// ================================================================================
// FUNCTION		: SetDeletedContact
// DESCRIPTION	: Delete a contact resource.
// ================================================================================
bool CContactInfo::SetDeletedContact(int nRecId)
{
	ReleaseMem();

	m_nContactId = nRecId;

	return true;
}

// ================================================================================
// FUNCTION		: GetContactInfo
// DESCRIPTION	: Get contact information to addressbook field from changes database.
// ================================================================================
bool CContactInfo::GetContactInfo(AEEDBField *pFieldArray, AECHAR * pEmptyData)
{
	if ( NULL==pFieldArray ) return false;

	//for empty fields we need to set some data. otherwise creating record fails
	//and the data size of this empty fields are set to 0
	int nSize = 0;

	//set fields in record
	pFieldArray[ DB_CHANGES_RECORD_ID ].fType = SMDB_FT_WORD;
	pFieldArray[ DB_CHANGES_RECORD_ID ].fName = SMDB_FIELD_NUM;
	if ( NULL!=m_nContactId )
	{
		pFieldArray[ DB_CHANGES_RECORD_ID ].wDataLen = sizeof( m_nContactId );		
	}
	else
		pFieldArray[ DB_CHANGES_RECORD_ID ].wDataLen = 0;
	pFieldArray[ DB_CHANGES_RECORD_ID ].pBuffer = &m_nContactId;

	//Firstname
	pFieldArray[ DB_CHANGES_RECORD_FNAME ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_FNAME ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psFName )
	{
		pFieldArray[ DB_CHANGES_RECORD_FNAME ].wDataLen = WSTRSIZE( m_psFName );
		pFieldArray[ DB_CHANGES_RECORD_FNAME ].pBuffer = (void*)m_psFName;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_FNAME ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_FNAME ].pBuffer = (void*)pEmptyData;
	}

	//Lastname
	pFieldArray[ DB_CHANGES_RECORD_LNAME ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_LNAME ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psLName )
	{
		pFieldArray[ DB_CHANGES_RECORD_LNAME ].wDataLen = WSTRSIZE( m_psLName );
		pFieldArray[ DB_CHANGES_RECORD_LNAME ].pBuffer = (void*)m_psLName;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_LNAME ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_LNAME ].pBuffer = (void*)pEmptyData;
	}

	//Company name
	pFieldArray[ DB_CHANGES_RECORD_COMPANY ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_COMPANY ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psCompany )
	{
		pFieldArray[ DB_CHANGES_RECORD_COMPANY ].wDataLen = WSTRSIZE( m_psCompany );
		pFieldArray[ DB_CHANGES_RECORD_COMPANY ].pBuffer = (void*)m_psCompany;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_COMPANY ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_COMPANY ].pBuffer = (void*)pEmptyData;
	}

	//Jon title
	pFieldArray[ DB_CHANGES_RECORD_JTITLE ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_JTITLE ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psJTitle )
	{
		pFieldArray[ DB_CHANGES_RECORD_JTITLE ].wDataLen = WSTRSIZE( m_psJTitle );
		pFieldArray[ DB_CHANGES_RECORD_JTITLE ].pBuffer = (void*)m_psJTitle;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_JTITLE ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_JTITLE ].pBuffer = (void*)pEmptyData;
	}

	//Fax
	pFieldArray[ DB_CHANGES_RECORD_FAX ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_FAX ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psFax )
	{
		pFieldArray[ DB_CHANGES_RECORD_FAX ].wDataLen = WSTRSIZE( m_psFax );
		pFieldArray[ DB_CHANGES_RECORD_FAX ].pBuffer = (void*)m_psFax;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_FAX ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_FAX ].pBuffer = (void*)pEmptyData;
	}

	//Fax home
	pFieldArray[ DB_CHANGES_RECORD_FAXH ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_FAXH ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psFaxH )
	{
		pFieldArray[ DB_CHANGES_RECORD_FAXH ].wDataLen = WSTRSIZE( m_psFaxH );
		pFieldArray[ DB_CHANGES_RECORD_FAXH ].pBuffer = (void*)m_psFaxH;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_FAXH ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_FAXH ].pBuffer = (void*)pEmptyData;
	}

	//Fax work
	pFieldArray[ DB_CHANGES_RECORD_FAXW ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_FAXW ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psFaxW )
	{
		pFieldArray[ DB_CHANGES_RECORD_FAXW ].wDataLen = WSTRSIZE( m_psFaxW );
		pFieldArray[ DB_CHANGES_RECORD_FAXW ].pBuffer = (void*)m_psFaxW;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_FAXW ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_FAXW ].pBuffer = (void*)pEmptyData;
	}
	
	//Mobile
	pFieldArray[ DB_CHANGES_RECORD_MOBILE ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_MOBILE ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psMobile )
	{
		pFieldArray[ DB_CHANGES_RECORD_MOBILE ].wDataLen = WSTRSIZE( m_psMobile );
		pFieldArray[ DB_CHANGES_RECORD_MOBILE ].pBuffer = (void*)m_psMobile;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_MOBILE ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_MOBILE ].pBuffer = (void*)pEmptyData;
	}

	//Mobile home
	pFieldArray[ DB_CHANGES_RECORD_MOBILEH ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_MOBILEH ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psMobileH )
	{
		pFieldArray[ DB_CHANGES_RECORD_MOBILEH ].wDataLen = WSTRSIZE( m_psMobileH );
		pFieldArray[ DB_CHANGES_RECORD_MOBILEH ].pBuffer = (void*)m_psMobileH;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_MOBILEH ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_MOBILEH ].pBuffer = (void*)pEmptyData;
	}

	//Mobile work
	pFieldArray[ DB_CHANGES_RECORD_MOBILEW ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_MOBILEW ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psMobileW )
	{
		pFieldArray[ DB_CHANGES_RECORD_MOBILEW ].wDataLen = WSTRSIZE( m_psMobileW );
		pFieldArray[ DB_CHANGES_RECORD_MOBILEW ].pBuffer = (void*)m_psMobileW;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_MOBILEW ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_MOBILEW ].pBuffer = (void*)pEmptyData;
	}

	//Telephone
	pFieldArray[ DB_CHANGES_RECORD_TELEPHONE ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_TELEPHONE ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psTelephone )
	{
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONE ].wDataLen = WSTRSIZE( m_psTelephone );
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONE ].pBuffer = (void*)m_psTelephone;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONE ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONE ].pBuffer = (void*)pEmptyData;
	}

	//Telephone home
	pFieldArray[ DB_CHANGES_RECORD_TELEPHONEH ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_TELEPHONEH ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psTelephoneH )
	{
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONEH ].wDataLen = WSTRSIZE( m_psTelephoneH );
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONEH ].pBuffer = (void*)m_psTelephoneH;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONEH ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONEH ].pBuffer = (void*)pEmptyData;
	}

	//Telephone work
	pFieldArray[ DB_CHANGES_RECORD_TELEPHONEW ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_TELEPHONEW ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psTelephoneW )
	{
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONEW ].wDataLen = WSTRSIZE( m_psTelephoneW );
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONEW ].pBuffer = (void*)m_psTelephoneW;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONEW ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_TELEPHONEW ].pBuffer = (void*)pEmptyData;
	}

	//Email
	pFieldArray[ DB_CHANGES_RECORD_EMAIL ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_EMAIL ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psEmail )
	{
		pFieldArray[ DB_CHANGES_RECORD_EMAIL ].wDataLen = WSTRSIZE( m_psEmail );
		pFieldArray[ DB_CHANGES_RECORD_EMAIL ].pBuffer = (void*)m_psEmail;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_EMAIL ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_EMAIL ].pBuffer = (void*)pEmptyData;
	}

	//Email home
	pFieldArray[ DB_CHANGES_RECORD_EMAILH ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_EMAILH ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psEmailH )
	{
		pFieldArray[ DB_CHANGES_RECORD_EMAILH ].wDataLen = WSTRSIZE( m_psEmailH );
		pFieldArray[ DB_CHANGES_RECORD_EMAILH ].pBuffer = (void*)m_psEmailH;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_EMAILH ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_EMAILH ].pBuffer = (void*)pEmptyData;
	}

	//email work
	pFieldArray[ DB_CHANGES_RECORD_EMAILW ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_CHANGES_RECORD_EMAILW ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psEmailW )
	{
		pFieldArray[ DB_CHANGES_RECORD_EMAILW ].wDataLen = WSTRSIZE( m_psEmailW );
		pFieldArray[ DB_CHANGES_RECORD_EMAILW ].pBuffer = (void*)m_psEmailW;
	}
	else
	{
		pFieldArray[ DB_CHANGES_RECORD_EMAILW ].wDataLen = nSize;
		pFieldArray[ DB_CHANGES_RECORD_EMAILW ].pBuffer = (void*)pEmptyData;
	}

	return true;
}

// ================================================================================
// FUNCTION		: GetContactInfoToBackup
// DESCRIPTION	: Get contacts information to addressbook field from backup database.
// ================================================================================
bool CContactInfo::GetContactInfoToBackup(AEEDBField *pFieldArray, AECHAR *pEmptyData)
{
	if ( NULL==pFieldArray ) return false;

	//for empty fields we need to set some data. otherwise creating record fails
	//and the data size of this empty fields are set to 0
	int nSize = 0;//WSTRSIZE( pEmptyData );

	//set fields in record
	pFieldArray[ DB_BACKUP_RECORD_ID ].fType = SMDB_FT_WORD;
	pFieldArray[ DB_BACKUP_RECORD_ID ].fName = SMDB_FIELD_NUM;
	if ( NULL!=m_nContactId )
	{
		pFieldArray[ DB_BACKUP_RECORD_ID ].wDataLen = sizeof( m_nContactId );		
	}
	else
		pFieldArray[ DB_BACKUP_RECORD_ID ].wDataLen = 0;
	pFieldArray[ DB_BACKUP_RECORD_ID ].pBuffer = &m_nContactId;

	//Firstname
	pFieldArray[ DB_BACKUP_RECORD_FNAME ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_FNAME ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psFName )
	{
		pFieldArray[ DB_BACKUP_RECORD_FNAME ].wDataLen = WSTRSIZE( m_psFName );
		pFieldArray[ DB_BACKUP_RECORD_FNAME ].pBuffer = (void*)m_psFName;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_FNAME ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_FNAME ].pBuffer = (void*)pEmptyData;
	}

	//Lastname
	pFieldArray[ DB_BACKUP_RECORD_LNAME ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_LNAME ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psLName )
	{
		pFieldArray[ DB_BACKUP_RECORD_LNAME ].wDataLen = WSTRSIZE( m_psLName );
		pFieldArray[ DB_BACKUP_RECORD_LNAME ].pBuffer = (void*)m_psLName;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_LNAME ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_LNAME ].pBuffer = (void*)pEmptyData;
	}

	//Company name
	pFieldArray[ DB_BACKUP_RECORD_COMPANY ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_COMPANY ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psCompany )
	{
		pFieldArray[ DB_BACKUP_RECORD_COMPANY ].wDataLen = WSTRSIZE( m_psCompany );
		pFieldArray[ DB_BACKUP_RECORD_COMPANY ].pBuffer = (void*)m_psCompany;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_COMPANY ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_COMPANY ].pBuffer = (void*)pEmptyData;
	}

	//Jon title
	pFieldArray[ DB_BACKUP_RECORD_JTITLE ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_JTITLE ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psJTitle )
	{
		pFieldArray[ DB_BACKUP_RECORD_JTITLE ].wDataLen = WSTRSIZE( m_psJTitle );
		pFieldArray[ DB_BACKUP_RECORD_JTITLE ].pBuffer = (void*)m_psJTitle;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_JTITLE ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_JTITLE ].pBuffer = (void*)pEmptyData;
	}

	//Fax
	pFieldArray[ DB_BACKUP_RECORD_FAX ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_FAX ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psFax )
	{
		pFieldArray[ DB_BACKUP_RECORD_FAX ].wDataLen = WSTRSIZE( m_psFax );
		pFieldArray[ DB_BACKUP_RECORD_FAX ].pBuffer = (void*)m_psFax;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_FAX ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_FAX ].pBuffer = (void*)pEmptyData;
	}

	//Fax home
	pFieldArray[ DB_BACKUP_RECORD_FAXH ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_FAXH ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psFaxH )
	{
		pFieldArray[ DB_BACKUP_RECORD_FAXH ].wDataLen = WSTRSIZE( m_psFaxH );
		pFieldArray[ DB_BACKUP_RECORD_FAXH ].pBuffer = (void*)m_psFaxH;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_FAXH ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_FAXH ].pBuffer = (void*)pEmptyData;
	}

	//Fax work
	pFieldArray[ DB_BACKUP_RECORD_FAXW ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_FAXW ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psFaxW )
	{
		pFieldArray[ DB_BACKUP_RECORD_FAXW ].wDataLen = WSTRSIZE( m_psFaxW );
		pFieldArray[ DB_BACKUP_RECORD_FAXW ].pBuffer = (void*)m_psFaxW;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_FAXW ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_FAXW ].pBuffer = (void*)pEmptyData;
	}
	
	//Mobile
	pFieldArray[ DB_BACKUP_RECORD_MOBILE ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_MOBILE ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psMobile )
	{
		pFieldArray[ DB_BACKUP_RECORD_MOBILE ].wDataLen = WSTRSIZE( m_psMobile );
		pFieldArray[ DB_BACKUP_RECORD_MOBILE ].pBuffer = (void*)m_psMobile;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_MOBILE ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_MOBILE ].pBuffer = (void*)pEmptyData;
	}

	//Mobile home
	pFieldArray[ DB_BACKUP_RECORD_MOBILEH ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_MOBILEH ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psMobileH )
	{
		pFieldArray[ DB_BACKUP_RECORD_MOBILEH ].wDataLen = WSTRSIZE( m_psMobileH );
		pFieldArray[ DB_BACKUP_RECORD_MOBILEH ].pBuffer = (void*)m_psMobileH;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_MOBILEH ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_MOBILEH ].pBuffer = (void*)pEmptyData;
	}

	//Mobile work
	pFieldArray[ DB_BACKUP_RECORD_MOBILEW ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_MOBILEW ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psMobileW )
	{
		pFieldArray[ DB_BACKUP_RECORD_MOBILEW ].wDataLen = WSTRSIZE( m_psMobileW );
		pFieldArray[ DB_BACKUP_RECORD_MOBILEW ].pBuffer = (void*)m_psMobileW;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_MOBILEW ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_MOBILEW ].pBuffer = (void*)pEmptyData;
	}

	//Telephone
	pFieldArray[ DB_BACKUP_RECORD_TELEPHONE ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_TELEPHONE ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psTelephone )
	{
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONE ].wDataLen = WSTRSIZE( m_psTelephone );
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONE ].pBuffer = (void*)m_psTelephone;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONE ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONE ].pBuffer = (void*)pEmptyData;
	}

	//Telephone home
	pFieldArray[ DB_BACKUP_RECORD_TELEPHONEH ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_TELEPHONEH ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psTelephoneH )
	{
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONEH ].wDataLen = WSTRSIZE( m_psTelephoneH );
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONEH ].pBuffer = (void*)m_psTelephoneH;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONEH ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONEH ].pBuffer = (void*)pEmptyData;
	}

	//Telephone work
	pFieldArray[ DB_BACKUP_RECORD_TELEPHONEW ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_TELEPHONEW ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psTelephoneW )
	{
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONEW ].wDataLen = WSTRSIZE( m_psTelephoneW );
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONEW ].pBuffer = (void*)m_psTelephoneW;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONEW ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_TELEPHONEW ].pBuffer = (void*)pEmptyData;
	}

	//Email
	pFieldArray[ DB_BACKUP_RECORD_EMAIL ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_EMAIL ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psEmail )
	{
		pFieldArray[ DB_BACKUP_RECORD_EMAIL ].wDataLen = WSTRSIZE( m_psEmail );
		pFieldArray[ DB_BACKUP_RECORD_EMAIL ].pBuffer = (void*)m_psEmail;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_EMAIL ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_EMAIL ].pBuffer = (void*)pEmptyData;
	}

	//Email home
	pFieldArray[ DB_BACKUP_RECORD_EMAILH ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_EMAILH ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psEmailH )
	{
		pFieldArray[ DB_BACKUP_RECORD_EMAILH ].wDataLen = WSTRSIZE( m_psEmailH );
		pFieldArray[ DB_BACKUP_RECORD_EMAILH ].pBuffer = (void*)m_psEmailH;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_EMAILH ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_EMAILH ].pBuffer = (void*)pEmptyData;
	}

	//email work
	pFieldArray[ DB_BACKUP_RECORD_EMAILW ].fType = SMDB_FT_STRING;
	pFieldArray[ DB_BACKUP_RECORD_EMAILW ].fName = SMDB_FIELD_TEXT;
	if ( NULL!=m_psEmailW )
	{
		pFieldArray[ DB_BACKUP_RECORD_EMAILW ].wDataLen = WSTRSIZE( m_psEmailW );
		pFieldArray[ DB_BACKUP_RECORD_EMAILW ].pBuffer = (void*)m_psEmailW;
	}
	else
	{
		pFieldArray[ DB_BACKUP_RECORD_EMAILW ].wDataLen = nSize;
		pFieldArray[ DB_BACKUP_RECORD_EMAILW ].pBuffer = (void*)pEmptyData;
	}

	return true;
}

// ================================================================================
// FUNCTION		: SetContactFromChangeDbRecord
// DESCRIPTION	: Set contact information from a given changes database record.
// ================================================================================
bool CContactInfo::SetContactFromChangeDbRecord(IDBRecord *pRecord)
{

	AEEDBFieldName rFieldName;
	AEEDBFieldType rFieldType;
	uint16 nFieldLen;
	AECHAR pszEmpty[] = NODATA;
	uint16 nId;
	uint16 nState;

	// Get the first field. DB_CHANGES_RECORD_ID.
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_WORD==rFieldType) )
	{
		IDBRECORD_GetFieldWord(pRecord, &nId);
	}
	else
	{
		return false;
	}

	//Second field. DB_CHANGES_RECORD_STATE.
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_WORD==rFieldType) )
	{
		IDBRECORD_GetFieldWord(pRecord, &nState);
	}

	//other fileds in change database are simillar and in same order as Backup database
	//so we use same function to retrieve the data
	SetContactFromBkpDbRecord(pRecord, nId);

	//we need to set the state of our contact at last. b'cos previous function 
	//reset the memory
	m_nState = nState;

	return true;
}

// ================================================================================
// FUNCTION		: SetContactFromBkpDbRecord
// DESCRIPTION	: Set contact information from a given backup database record.
// ================================================================================
bool CContactInfo::SetContactFromBkpDbRecord(IDBRecord *pRecord, int nId)
{
	AEEDBFieldName rFieldName;
	AEEDBFieldType rFieldType;
	uint16 nFieldLen;
	AECHAR* psStringPtr;
	AECHAR pszEmpty[] = NODATA;

	//release previous memory taken to contact
	ReleaseMem();

	//set Id
	m_nContactId = nId;

	//DB_BACKUP_RECORD_FNAME
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psFName, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_LNAME
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psLName, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_COMPANY
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psCompany, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_JTITLE
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psJTitle, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_FAX
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psFax, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_FAXH
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psFaxH, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_FAXW
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psFaxW, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_MOBILE
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psMobile, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_MOBILEH
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psMobileH, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_MOBILEW
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psMobileW, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_TELEPHONE
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psTelephone, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_TELEPHONEH
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psTelephoneH, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_TELEPHONEW
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psTelephoneW, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_EMAIL
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psEmail, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_EMAILH
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psEmailH, psStringPtr ) )
				return false;
	}

	//DB_BACKUP_RECORD_EMAILW
	rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
	if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
	{
		psStringPtr = IDBRECORD_GetFieldString(pRecord);
		if ( 0!=WSTRCMP(psStringPtr, pszEmpty) )
			if ( !CopyFieldTo(&m_psEmailW, psStringPtr ) )
				return false;
	}

	return true;
}

// ================================================================================
// FUNCTION		: CompareContact
// DESCRIPTION	: return true if two contacts equels.
// ================================================================================
bool CContactInfo::CompareContact(CContactInfo *pContact)
{
	if ( m_nContactId != pContact->m_nContactId )
		return false;

	if ( !CompareStrings(m_psFName, pContact->m_psFName) ) return false;
	
	if ( !CompareStrings(m_psLName, pContact->m_psLName) ) return false;

	if ( !CompareStrings(m_psCompany, pContact->m_psCompany) ) return false;

	if ( !CompareStrings(m_psJTitle, pContact->m_psJTitle) ) return false;

	if ( !CompareStrings(m_psFax, pContact->m_psFax) ) return false;

	if ( !CompareStrings(m_psFaxH, pContact->m_psFaxH) ) return false;

	if ( !CompareStrings(m_psFaxW, pContact->m_psFaxW) ) return false;

	if ( !CompareStrings(m_psMobile, pContact->m_psMobile) ) return false;

	if ( !CompareStrings(m_psMobileH, pContact->m_psMobileH) ) return false;

	if ( !CompareStrings(m_psMobileW, pContact->m_psMobileW) ) return false;

	if ( !CompareStrings(m_psTelephone, pContact->m_psTelephone) ) return false;

	if ( !CompareStrings(m_psTelephoneH, pContact->m_psTelephoneH) ) return false;

	if ( !CompareStrings(m_psTelephoneW, pContact->m_psTelephoneW) ) return false;

	if ( !CompareStrings(m_psEmail, pContact->m_psEmail) ) return false;

	if ( !CompareStrings(m_psEmailH, pContact->m_psEmailH) ) return false;

	if ( !CompareStrings(m_psEmailW, pContact->m_psEmailW) ) return false;

	return true;
}

// ================================================================================
// FUNCTION		: CompareStrings
// DESCRIPTION	: return true if two strings matched.
// ================================================================================
bool CContactInfo::CompareStrings(AECHAR *pszStr1, AECHAR *pszStr2)
{
	if ( (NULL==pszStr1) && (NULL==pszStr2) )
		return true;

	if (NULL==pszStr1) return false;

	if (NULL==pszStr2) return false;

	if ( 0!=WSTRCMP(pszStr1, pszStr2) )
		return false;

	return true;

}

// ================================================================================
// FUNCTION		: GetState
// DESCRIPTION	: Get the state
// ================================================================================
int CContactInfo::GetState()
{
	return m_nState;
}

uint16 CContactInfo::GetContactId()
{
	return m_nContactId;
}

AECHAR* CContactInfo::GetFName()
{
	return m_psFName;
}

AECHAR* CContactInfo::GetLName()
{
	return m_psLName;
}

AECHAR* CContactInfo::GetCompany()
{
	return m_psCompany;
}

AECHAR* CContactInfo::GetJobTitles()
{
	return m_psJTitle;
}

AECHAR* CContactInfo::GetFax()
{
	return m_psFax;
}

AECHAR* CContactInfo::GetFaxHome()
{
	return m_psFaxH;
}

AECHAR* CContactInfo::GetFaxWork()
{
	return m_psFaxW;
}

AECHAR* CContactInfo::GetMobile()
{
	return m_psMobile;
}

AECHAR* CContactInfo::GetMobileHome()
{
	return m_psMobileH;
}

AECHAR* CContactInfo::GetMobileWork()
{
	return m_psMobileW;
}

AECHAR* CContactInfo::GetTel()
{
	return m_psTelephone;
}

AECHAR* CContactInfo::GetTelHome()
{
	return m_psTelephoneH;
}

AECHAR* CContactInfo::GetTelWork()
{
	return m_psTelephoneW;
}

AECHAR* CContactInfo::GetEmail()
{
	return m_psEmail;
}

AECHAR* CContactInfo::GetEmailHome()
{
	return m_psEmailH;
}

AECHAR* CContactInfo::GetEmailWork()
{
	return m_psEmailW;
}

int CContactInfo::GetSize()
{
	int nSize=0;

	nSize += sizeof(m_nContactId);

	nSize += sizeof(m_nState);

	if ( NULL!=m_psFName)			nSize += WSTRSIZE(m_psFName);
	if ( NULL!=m_psLName)			nSize += WSTRSIZE(m_psLName); 
	if ( NULL!=m_psCompany)			nSize += WSTRSIZE(m_psCompany);	         
	if ( NULL!=m_psJTitle)			nSize += WSTRSIZE(m_psJTitle);	               
	if ( NULL!=m_psFaxH)			nSize += WSTRSIZE(m_psFaxH);
	if ( NULL!=m_psFaxW)			nSize += WSTRSIZE(m_psFaxW);
	if ( NULL!=m_psFax)				nSize += WSTRSIZE(m_psFax);
	if ( NULL!=m_psMobileH)			nSize += WSTRSIZE(m_psMobileH);	         
	if ( NULL!=m_psMobileW)			nSize += WSTRSIZE(m_psMobileW);
	if ( NULL!=m_psMobile)			nSize += WSTRSIZE(m_psMobile);
	if ( NULL!=m_psTelephoneH)		nSize += WSTRSIZE(m_psTelephoneH);
	if ( NULL!=m_psTelephoneW)		nSize += WSTRSIZE(m_psTelephoneW);
	if ( NULL!=m_psTelephone)		nSize += WSTRSIZE(m_psTelephone);
	if ( NULL!=m_psEmailW)			nSize += WSTRSIZE(m_psEmailW);
	if ( NULL!=m_psEmailH)			nSize += WSTRSIZE(m_psEmailH);
	if ( NULL!=m_psEmail)			nSize += WSTRSIZE(m_psEmail);

	return nSize;
}

// ================================================================================
// FUNCTION		: SetContactFromXml
// DESCRIPTION	: Set contact from a xml.
// ================================================================================
bool CContactInfo::SetContactFromXml(char *pszXml, char *pszType)
{
	DBGPRINTF(pszXml);

	ReleaseMem();

	char *pszFound=NULL;

	//set type of contact
	if ( 0==STRCMP("add", pszType))
		m_nState = SM_RECORD_ADD;
	else if ( 0==STRCMP("update", pszType))
		m_nState = SM_RECORD_UPDATE;
	else if ( 0==STRCMP("delete", pszType))
		m_nState = SM_RECORD_DELETE;
	else
		return false;

	int nLen=0;
	char *pszEndTag=NULL;
	char *pszNumber=NULL;
	
	//set contact id
	if ( !SetValue(&pszNumber, pszXml, "id")) return false;
	if ( NULL!=pszNumber ) m_nContactId = (uint16)STRTOUL(pszNumber, NULL, 10);
	DBGPRINTF("Contact id= %d", m_nContactId);
	FREEIF(pszNumber);

	//set global id
	if ( !SetValue(&m_psGlobalId, pszXml, "gcid")) return false;
	
	//set first name
	if ( !SetValue(&m_psFName, pszXml, "fn")) return false;
    if (NULL!=m_psFName) DBGPRINTF("%S", m_psFName);  
	if ( !SetValue(&m_psLName, pszXml, "ln")) return false;  
	if (NULL!=m_psLName) DBGPRINTF("%S", m_psLName);
	if ( !SetValue(&m_psCompany, pszXml, "cn")) return false;
	if (NULL!=m_psCompany) DBGPRINTF("%S", m_psCompany);
	if ( !SetMultiValue(&m_psJTitle, pszXml, "ttls", "ttl")) return false;
	if (NULL!=m_psJTitle) DBGPRINTF("%S", m_psJTitle);
	if ( !SetMultiValue(&m_psFaxH, pszXml, "fxsh", "fxh")) return false;
	if (NULL!=m_psFaxH) DBGPRINTF("%S", m_psFaxH);
	if ( !SetMultiValue(&m_psFaxW, pszXml, "fxsw", "fxw")) return false;
	if (NULL!=m_psFaxW) DBGPRINTF("%S", m_psFaxW);
	if ( !SetMultiValue(&m_psFax, pszXml, "fxs", "fx")) return false;
	if ( !SetMultiValue(&m_psMobileH, pszXml, "mblsh", "mblh")) return false;	         
	if ( !SetMultiValue(&m_psMobileW, pszXml, "mblsb", "mblb")) return false;
	if ( !SetMultiValue(&m_psMobile, pszXml, "mbls", "mbl")) return false;
	if ( !SetMultiValue(&m_psTelephoneH, pszXml, "telsh", "telh")) return false;
	if ( !SetMultiValue(&m_psTelephoneW, pszXml, "telsb", "telb")) return false;
	if ( !SetMultiValue(&m_psTelephone, pszXml, "tels", "tel")) return false;
	if ( !SetMultiValue(&m_psEmailW, pszXml, "emlsb", "emlb")) return false;
	if ( !SetMultiValue(&m_psEmailH, pszXml, "emlsh", "emlh")) return false;
	if ( !SetMultiValue(&m_psEmail, pszXml, "emls", "eml")) return false;
	if (NULL!=m_psEmail) DBGPRINTF("%S", m_psEmail);

	return true;
}

bool CContactInfo::SetValue(AECHAR **pwzValue, char *pszXml, char *pszTag)
{
	FREEIF(*pwzValue);

	char *pszTmp=NULL;
	if ( SetValue(&pszTmp, pszXml, pszTag))
	{
		if ( NULL!=pszTmp )
		{
			int nLen = STRLEN(pszTmp);
		
			*pwzValue = (AECHAR*)MALLOC( (nLen+1)*sizeof(AECHAR));
			
			if ( NULL==*pwzValue ) 
			{
				FREEIF(pszTmp);
				return false;
			}
		
		
			STRTOWSTR(pszTmp, *pwzValue, (nLen+1)*sizeof(AECHAR)); 
			FREEIF(pszTmp);

		}
	}

	return true;
}

bool CContactInfo::SetValue(char **pszValue, char *pszXml, char *pszTag)
{
	FREEIF(*pszValue);

	char pszT[20];
	SPRINTF(pszT, "<%s>", pszTag);
	
	char *pszFound=NULL;
	char *pszNext = pszXml;
	if ( NULL != (pszFound=STRSTR(pszNext, pszT)))
	{
	    //<ct><id>3</id></ct>
		SPRINTF(pszT, "</%s>", pszTag);
		char *pszEndTag=NULL;
		pszEndTag=STRSTR(pszFound, pszT);
		if ( NULL==pszEndTag ) return false;
		int nLen=0;
		SPRINTF(pszT, "<%s>", pszTag);
		int nTagLen = STRLEN(pszT);
		nLen = STRLEN(pszFound)-STRLEN(pszEndTag)-nTagLen; // len of <id>3
		

		if ( nLen>0 )
		{
			*pszValue = (char*)MALLOC((nLen+1)*sizeof(AECHAR));

			if (NULL==(*pszValue)) return false;
			 
			STRLCPY(*pszValue, pszFound+nTagLen, nLen+1);
			DBGPRINTF(*pszValue);
		}
		
	}

	return true;
}

bool CContactInfo::SetMultiValue(AECHAR **pwsValue, char *pszXml, char *pszMultiTag, char *pszSingleTag)
{
	FREEIF(*pwsValue);

	char *pszData=NULL;

	if ( !SetValue(&pszData, pszXml, pszMultiTag)) return false;

	//<emls><eml>dxx@hotmail.com</eml><eml>nil@yahoo.com</eml></emls>

	if ( NULL!=pszData )
	{
		//create a temporary buffer
		char *pszTmp = (char*)MALLOC(STRLEN(pszData));//need to change this 100 add. we r adding null characters at the end of each
		
		if ( NULL==pszTmp ) return false;

		char pszT[20];
		SPRINTF(pszT, "<%s>", pszSingleTag);
		AECHAR pwzNewTag[]=NEW_FIELD;
		char pszNewtag[10];
		SPRINTF(pszNewtag, "%S", pwzNewTag);

		char *pszFound=NULL;
		char *pszNext = pszData;

		bool bCount=false;
		int nBufLen=0;
		while ( NULL != (pszFound=STRSTR(pszNext, pszT)))
		{
			
			if ( bCount )
			{
				//add null charachter at the end
				//STRCAT(pszTmp, "\0");
				//add new tag
				STRCAT(pszTmp, pszNewtag);
				
			}
			bCount = true;

			//find the end position
			SPRINTF(pszT, "</%s>", pszSingleTag);
			char *pszEndTag=NULL;
			pszEndTag=STRSTR(pszFound, pszT);
			if ( NULL==pszEndTag ) return false;
			
			int nLen=0;
			SPRINTF(pszT, "<%s>", pszSingleTag);
			int nTagLen = STRLEN(pszT);
			nLen = STRLEN(pszFound)-STRLEN(pszEndTag)-nTagLen; // len of <id>3
		 
			nBufLen = STRLEN(pszTmp);
			STRLCAT(pszTmp+nBufLen, pszFound+nTagLen, nLen+1);
			nBufLen = STRLEN(pszTmp);

			pszNext = pszFound+1;
		}

		DBGPRINTF(pszTmp);
		nBufLen = STRLEN(pszTmp);
		//convert and copy to return buffer
		*pwsValue = (AECHAR*)MALLOC((nBufLen+1)*sizeof(AECHAR));
		if ( NULL==(*pwsValue) )
		{
			FREEIF(pszData);
			FREEIF(pszTmp);
			return false;
		}
		STRTOWSTR(pszTmp, *pwsValue, (nBufLen+1)*sizeof(AECHAR));

		FREEIF(pszData);
		FREEIF(pszTmp);
	}

	return true;
}

// ================================================================================
// FUNCTION		: CountValidFields
// DESCRIPTION	: This return the number of valid fields in a contact.
// ================================================================================
int CContactInfo::CountValidFields()
{
	int nSize=0;

	if ( NULL!=m_psFName)			nSize++;
	if ( NULL!=m_psLName)			nSize++;
	if ( NULL!=m_psCompany)			nSize++;
	if ( NULL!=m_psJTitle)			nSize++;
	if ( NULL!=m_psFaxH)			CountMultiFields(m_psFaxH, nSize);
	if ( NULL!=m_psFaxW)			CountMultiFields(m_psFaxW, nSize);
	if ( NULL!=m_psFax)				CountMultiFields(m_psFax, nSize);
	if ( NULL!=m_psMobileH)			CountMultiFields(m_psMobileH, nSize);
	if ( NULL!=m_psMobileW)			CountMultiFields(m_psMobileW, nSize);
	if ( NULL!=m_psMobile)			CountMultiFields(m_psMobile, nSize);
	if ( NULL!=m_psTelephoneH)		CountMultiFields(m_psTelephoneH, nSize);
	if ( NULL!=m_psTelephoneW)		CountMultiFields(m_psTelephoneW, nSize);
	if ( NULL!=m_psTelephone)		CountMultiFields(m_psTelephone, nSize);
	if ( NULL!=m_psEmailW)			CountMultiFields(m_psEmailW, nSize);
	if ( NULL!=m_psEmailH)			CountMultiFields(m_psEmailH, nSize);
	if ( NULL!=m_psEmail)			CountMultiFields(m_psEmail, nSize);

	return nSize;
}

void CContactInfo::CountMultiFields(AECHAR *pwzField, int &nCount)
{
	if (NULL==pwzField ) return;

	nCount++;

	AECHAR pwzNewTag[]=NEW_FIELD;
	char pszNewtag[10];
	SPRINTF(pszNewtag, "%S", pwzNewTag);

	char *pszTmp=NULL;
	int nLen=WSTRLEN(pwzField);
	pszTmp = (char*)MALLOC(nLen+1);
	if ( NULL==pszTmp ) return;
	WSTRTOSTR(pwzField, pszTmp, nLen+1);

	char *pNext=pszTmp;
	char *pFound=NULL;
	while ( NULL!=(pFound=STRSTR(pNext, pszNewtag)))
	{
		nCount++;
		pNext = pFound+1;
	}

	FREEIF(pszTmp);
}

bool CContactInfo::FillAddrFields(AEEAddrField **pField, int &nSize)
{
	nSize = CountValidFields();
	*pField = new AEEAddrField[nSize];
	if ( NULL==(*pField) ) return true;

	int nCount=0;

	if ( NULL!=m_psFName )
	{
		(*pField)[nCount].fID = SM_ADDRFIELD_FIRSTNAME; // fill in first name field
		(*pField)[nCount].fType = AEEDB_FT_STRING;
		(*pField)[nCount].pBuffer = m_psFName;
		(*pField)[nCount].wDataLen = (WSTRLEN(m_psFName)+1)*sizeof(AECHAR);
		nCount++;
	}
	else
	{
		//sometime we get id=null records with empy fields.
		delete [] (*pField);
		(*pField) = NULL;
		return false;
	}
	
	if ( NULL!=m_psLName )
	{
		(*pField)[nCount].fID = SM_ADDRFIELD_LASTNAME; // fill in last name field
		(*pField)[nCount].fType = AEEDB_FT_STRING;
		(*pField)[nCount].pBuffer = m_psLName;
		(*pField)[nCount].wDataLen = (WSTRLEN(m_psLName)+1)*sizeof(AECHAR);
		nCount++;
	}


	if ( NULL!=m_psCompany )
	{
		(*pField)[nCount].fID = SM_ADDRFIELD_COMPANY; // fill in last name field
		(*pField)[nCount].fType = AEEDB_FT_STRING;
		(*pField)[nCount].pBuffer = m_psCompany;
		(*pField)[nCount].wDataLen = (WSTRLEN(m_psCompany)+1)*sizeof(AECHAR);
		nCount++;
	}

	if ( NULL!=m_psJTitle )
	{
		(*pField)[nCount].fID = SM_ADDRFIELD_JOBTITLE; // fill in last name field
		(*pField)[nCount].fType = AEEDB_FT_STRING;
		(*pField)[nCount].pBuffer = m_psJTitle;
		(*pField)[nCount].wDataLen = (WSTRLEN(m_psJTitle)+1)*sizeof(AECHAR);
		nCount++;
	}

	if ( NULL!=m_psEmail )
	{
		FillMultiField((*pField), nCount, m_psEmail, SM_ADDRFIELD_EMAIL); 
	}

	if ( NULL!=m_psFax )
	{
		(*pField)[nCount].fID = SM_ADDRFIELD_FAX; // fill in last name field
		(*pField)[nCount].fType = AEEDB_FT_STRING;
		(*pField)[nCount].pBuffer = m_psFax;
		(*pField)[nCount].wDataLen = (WSTRLEN(m_psFax)+1)*sizeof(AECHAR);
		nCount++;
	}

	if ( NULL!=m_psTelephone )
	{
		(*pField)[nCount].fID = SM_ADDRFIELD_PHONE; // fill in last name field
		(*pField)[nCount].fType = AEEDB_FT_STRING;
		(*pField)[nCount].pBuffer = m_psTelephone;
		(*pField)[nCount].wDataLen = (WSTRLEN(m_psTelephone)+1)*sizeof(AECHAR);
		nCount++;
	}

	if ( NULL!=m_psTelephoneH )
	{
		(*pField)[nCount].fID = SM_ADDRFIELD_PHONE_HOME; // fill in last name field
		(*pField)[nCount].fType = AEEDB_FT_STRING;
		(*pField)[nCount].pBuffer = m_psTelephoneH;
		(*pField)[nCount].wDataLen = (WSTRLEN(m_psTelephoneH)+1)*sizeof(AECHAR);
		nCount++;
	}

	if ( NULL!=m_psMobile )
	{
		(*pField)[nCount].fID = SM_ADDRFIELD_MOBILE; // fill in last name field
		(*pField)[nCount].fType = AEEDB_FT_STRING;
		(*pField)[nCount].pBuffer = m_psMobile;
		(*pField)[nCount].wDataLen = (WSTRLEN(m_psMobile)+1)*sizeof(AECHAR);
		nCount++;
	}

	return true;
}

void CContactInfo::FillMultiField(AEEAddrField *pField, int &nPos, AECHAR *pwzName, AEEAddrFieldID type)
{
	//	pwzName	=		dxx@hotmail.com<NEW>nil@yahoo.com

	if (NULL==pwzName ) return;

	AECHAR pwzNewTag[]=NEW_FIELD;
	char pszNewtag[10];
	SPRINTF(pszNewtag, "%S", pwzNewTag);
	int nTagLen = STRLEN(pszNewtag);

	char *pszTmp=NULL;
	int nLen=WSTRLEN(pwzName);
	pszTmp = (char*)MALLOC(nLen+1);
	if ( NULL==pszTmp ) return;
	WSTRTOSTR(pwzName, pszTmp, nLen+1);

	char *pNext=pszTmp;
	char *pFound=NULL;
	char *pValue=NULL;
	char *pEnd=NULL;
	int nValueLen =0;
	int nBufferPos=0;

	pValue = pNext;
	DBGPRINTF("%s", pValue);
	while ( NULL!=(pFound=STRSTR(pNext, pszNewtag)))
	{
		pField[nPos].fID = type;
		pField[nPos].fType = AEEDB_FT_STRING;
		
		nBufferPos = STRLEN(pszTmp)-STRLEN(pValue);
		pField[nPos].pBuffer = pwzName+ nBufferPos;
		
		//value lenth
		nValueLen = STRLEN(pValue)-STRLEN(pFound);
		
		DBGPRINTF("%s", pValue);
		pField[nPos].wDataLen = (nValueLen+1)*sizeof(AECHAR);

		nPos++;
		pNext = pFound+1;
		pValue = pFound+ nTagLen;
	}

	DBGPRINTF("%s", pValue);
	pField[nPos].fID = type;
	pField[nPos].fType = AEEDB_FT_STRING;
	nBufferPos = STRLEN(pszTmp)-STRLEN(pValue);
	pField[nPos].pBuffer = pwzName+ nBufferPos;
	nValueLen=STRLEN(pValue);
	pField[nPos].wDataLen = (nValueLen+1)*sizeof(AECHAR);

	nPos++;

	FREEIF(pszTmp);

	AECHAR *pwzSart=pwzName;
	AECHAR *pwzEnd=NULL;
	AECHAR pwzNull = '\0';
	while ( NULL!=(pwzEnd=WSTRCHR(pwzSart, '<')))
	{
		*pwzEnd='\0';
		pwzSart = pwzEnd+4;
	}
	
}

AECHAR* CContactInfo::GetGlobalId()
{
	return m_psGlobalId;
}

void CContactInfo::SetContactId(uint16 wId)
{
	m_nContactId=wId;
}


