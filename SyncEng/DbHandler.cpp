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
 Module Name				: DbHandler.cpp
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
#include "DbHandler.h"
#include "AEEStdLib.h"
#include "Util.h"

#define SM_CHANGEDB "ChangeDb.bdb"
#define SM_BACKUPDB "BackupDb.bdb"
#define SM_SMSDB	"Sms.bdb"
#define	SMS_RECORD_FIELDS	5
#define SM_MAX_RECORDS_PREPACKET	2
#define MAX_NUM_LEN				20

// ================================================================================
// FUNCTION		: CDbHandler
// DESCRIPTION	: Construction of CDbHandler class.
// ================================================================================
CDbHandler::CDbHandler(IShell* pIShell)
{
	m_pIShell = pIShell;
}

// ================================================================================
// FUNCTION		: ~CDbHandler
// DESCRIPTION	: Destructor of CDbHandler class.
// ================================================================================
CDbHandler::~CDbHandler()
{

}

// ================================================================================
// FUNCTION		: CreateChangeDb
// DESCRIPTION	: Create Changes database. This is the database synchronize with server.
// ================================================================================
int CDbHandler::CreateChangeDb()
{	
	IDatabase * pChangeDb = NULL;
	IDatabase * pBackupDb = NULL;

	//if ChangeDb is alreday there. we return SUCCESS.
	//means that last backup is not success and we continue with it.
	if ( OpenChangeDb(&pChangeDb) ) return SM_SUCCESS;

	//Open Backup database
	if ( !OpenBackupDb(&pBackupDb)) return SM_ERROR;
	
	//enumerate Address book to create ChaneDb
	//create address book
	IAddrBook * pIAddrBook=NULL;
	if ( SUCCESS != ISHELL_CreateInstance(m_pIShell, AEECLSID_ADDRBOOK, (void **)&pIAddrBook))
	{
		//release changeDb
		IDATABASE_Release(pChangeDb);
		//release Backup Database
		IDATABASE_Release(pBackupDb);
		return SM_ERROR;
	}

	//Process-1
	//Enumerate through addressbook, copmare it with last time backedup database
	//create ChangeDb. In this process we only do additions and updates to ChangeDb.
	if( AEE_SUCCESS == IADDRBOOK_EnumRecInit(pIAddrBook,AEE_ADDR_CAT_NONE, AEE_ADDRFIELD_NONE,NULL,0) )
	{
		IAddrRec* rec=NULL;
		//enumerate records
		CContactInfo * pAddrBookContact = new CContactInfo();
		CContactInfo * pBackedupContact = new CContactInfo();

		if ( NULL==pAddrBookContact || NULL==pBackedupContact )
		{
			//if only one memory alloc failed.
			if ( NULL!=pAddrBookContact ) delete pAddrBookContact;
			if ( NULL!=pBackedupContact ) delete pBackedupContact;

			//release changeDb
			IDATABASE_Release(pChangeDb);
			//release Backup Database
			IDATABASE_Release(pBackupDb);
			//release address book
			IADDRBOOK_Release(pIAddrBook);
			return SM_NOMEMORY;
		}

		uint16 nRecId = 0;
		DbSearchResult searchStatus;
		while( NULL != (rec=IADDRBOOK_EnumNextRec(pIAddrBook)) )
		{
			//Fetch single contact info from Address book
			//Get current contact from address book
			nRecId = IADDRREC_GetRecID(rec);
			if ( SM_SUCCESS != pAddrBookContact->SetContactInfo(rec) )
			{
				//cleanup and exit
				IADDRREC_Release(rec);
				delete pAddrBookContact;
				delete pBackedupContact;
				//release changeDb
				IDATABASE_Release(pChangeDb);
				//release Backup Database
				IDATABASE_Release(pBackupDb);
				//release address book
				IADDRBOOK_Release(pIAddrBook);

				return SM_ERROR;
			}
	
			//search the corresponding record in backup database.
			searchStatus = SearchContactInBackupDb(pBackupDb, nRecId, pAddrBookContact, pBackedupContact);

			//depending on search status add, update and delete the record to ChangedDb
			if ( !AddToChangesDb(pChangeDb, searchStatus, pAddrBookContact))
			{
				//cleanup and exit
				IADDRREC_Release(rec);
				delete pAddrBookContact;
				delete pBackedupContact;
				//release changeDb
				IDATABASE_Release(pChangeDb);
				//release Backup Database
				IDATABASE_Release(pBackupDb);
				//release address book
				IADDRBOOK_Release(pIAddrBook);

				return SM_ERROR;
			}

			IADDRREC_Release(rec);

		}

		delete pAddrBookContact;
		delete pBackedupContact;
	}
	//End of Process-1

	//Process-2
	//Add deleted records(from last backup) to ChangeDb
	//After this process ChangeDb is completed.
	if ( !SetDeletedRecordsInAddrBook(pBackupDb, pChangeDb))
	{
		//cleanup and exit

	}
	//end process-2

	//release Backup Database
	IDATABASE_Release(pBackupDb);
	//release changeDb
	IDATABASE_Release(pChangeDb);

	//release address book
	IADDRBOOK_Release(pIAddrBook);

	return SM_SUCCESS;
}

// ================================================================================
// FUNCTION		: OpenChangeDb
// DESCRIPTION	: open changes database.
// ================================================================================
bool CDbHandler::OpenChangeDb(IDatabase ** pDatabase)
{

	//If the change Db already exist we want to close it. and stop the process of
	//making it again.
	if ( OpenDatabase(pDatabase, SM_CHANGEDB, FALSE) )
	{
		IDATABASE_Release(*pDatabase);
		return true;
	}
	else
		return ( !OpenDatabase(pDatabase, SM_CHANGEDB, TRUE) );

}

// ================================================================================
// FUNCTION		: OpenBackupDb
// DESCRIPTION	: Open last backup contacts databse.
// ================================================================================
bool CDbHandler::OpenBackupDb(IDatabase **pDatabase)
{
	return OpenDatabase(pDatabase, SM_BACKUPDB, TRUE);
}

// ================================================================================
// FUNCTION		: OpenDatabase
// DESCRIPTION	: Open a given database in BREW.
// ================================================================================
bool CDbHandler::OpenDatabase(IDatabase **pDb, const char *pszFile, boolean bCreate)
{
	IDBMgr * pDBMgr;
	
	// Create the IDBMgr object
	if(ISHELL_CreateInstance(m_pIShell, AEECLSID_DBMGR, (void**)(&pDBMgr)) != SUCCESS)
	{
		DBGPRINTF("CDbHandler::OpenDatabase >> ISHELL_CreateInstance AEECLSID_DBMGR failed.");
		return false;
	}

	//Open database
	if((*pDb = IDBMGR_OpenDatabase(pDBMgr, pszFile, bCreate)) == NULL) 
	{
		DBGPRINTF("CDbHandler::OpenDatabase >> IDBMGR_OpenDatabase %s failed.", pszFile);
		IDBMGR_Release(pDBMgr);
		return false;
	}
	else
	{
		IDBMGR_Release(pDBMgr);
		return true;
	}
}

// ================================================================================
// FUNCTION		: DeleteDatabase
// DESCRIPTION	: Delete a given database in BREW.
// ================================================================================
bool CDbHandler::DeleteDatabase(const char *pszDbName)
{
	IDBMgr * pDBMgr;
	
	// Create the IDBMgr object
	if(ISHELL_CreateInstance(m_pIShell, AEECLSID_DBMGR, (void**)(&pDBMgr)) != SUCCESS)
	{
		DBGPRINTF("CDbHandler::DeleteDatabase >> ISHELL_CreateInstance %s failed.", pszDbName);
		return false;
	}

	if ( SUCCESS==IDBMGR_Remove(pDBMgr, pszDbName) )
	{
		IDBMGR_Release(pDBMgr);
		return true;
	}
	else
	{
		DBGPRINTF("CDbHandler::DeleteDatabase >> IDBMGR_Release failed.");
		IDBMGR_Release(pDBMgr);
		return false;
	}
	
}

// ================================================================================
// FUNCTION		: DeleteChangeDb
// DESCRIPTION	: Delete changes database.
// ================================================================================
bool CDbHandler::DeleteChangeDb()
{
	return DeleteDatabase(SM_CHANGEDB);
}

// ================================================================================
// FUNCTION		: AddToChangesDb
// DESCRIPTION	: Add contact to a changes database
// ================================================================================
bool CDbHandler::AddToChangesDb(IDatabase * pDatabase, DbSearchResult status, CContactInfo *pContact)
{
	if ( NULL==pContact ) return false;

	int nStatus = SM_DBERROR;

	switch ( status )
	{
	case SM_RECORD_ADD:
			nStatus = SM_RECORD_ADD;
		break;

	case SM_RECORD_UPDATE:
			nStatus = SM_RECORD_UPDATE;
		break;

	case SM_RECORD_DELETE:
			nStatus = SM_RECORD_DELETE;
		break;

	case SM_RECORD_EQUAL:
		return true;

	case SM_DBERROR:
		return false;
		
	default:
		return false;
	}

	//add Contact information to ChangesDb

	IDBRecord* pRecord;
	AEEDBField pFieldArray[ NUM_DB_CHANGES_RECORD_FIELDS ];
	
	pFieldArray[ DB_CHANGES_RECORD_STATE ].fType = SMDB_FT_WORD;
	pFieldArray[ DB_CHANGES_RECORD_STATE ].fName = SMDB_FIELD_NUM;
	pFieldArray[ DB_CHANGES_RECORD_STATE ].wDataLen = sizeof( nStatus );
	pFieldArray[ DB_CHANGES_RECORD_STATE ].pBuffer = &nStatus;

	AECHAR szEmptyData[] = NODATA;
	pContact->GetContactInfo(pFieldArray, szEmptyData);
	
	// Create a new record based on the AEEDBField field array values
	pRecord = IDATABASE_CreateRecord( pDatabase, pFieldArray,  NUM_DB_CHANGES_RECORD_FIELDS);

	// Return false if the record could not be created
	if( !pRecord )
		return false;

	// Release the record
	IDBRECORD_Release( pRecord );

	return true;
}

// ================================================================================
// FUNCTION		: AddToBackupDb
// DESCRIPTION	: Add contact to a backup database
// ================================================================================
bool CDbHandler::AddToBackupDb(IDatabase *pBkpDb, CContactInfo *pContact)
{
	IDBRecord* pRecord;
	AEEDBField pFieldArray[ NUM_DB_BACKUP_RECORD_FIELDS ];

	AECHAR szEmptyData[] = NODATA;
	//we can use the same function to fill both backup and changes info databases
	//enum types of both these databases has similar fields
	pContact->GetContactInfoToBackup(pFieldArray, szEmptyData);

	// Create a new record based on the AEEDBField field array values
	pRecord = IDATABASE_CreateRecord( pBkpDb, pFieldArray,  NUM_DB_BACKUP_RECORD_FIELDS);

	// Return false if the record could not be created
	if( !pRecord )
		return false;

	// Release the record
	IDBRECORD_Release( pRecord );

	return true;

}

// ================================================================================
// FUNCTION		: SetDeletedRecordsInAddrBook
// DESCRIPTION	: Set deleted records in changes databse according to addressbook.
// ================================================================================
bool CDbHandler::SetDeletedRecordsInAddrBook(IDatabase *pBackupDb, IDatabase *pCghangeDb)
{
	IDBRecord* pRecord=NULL;
	uint16 nFieldLen;
	AEEDBFieldName rFieldName;
	AEEDBFieldType rFieldType;
	uint16 nId;
	uint16 nAddrRecId;
	IAddrBook * pIAddrBook=NULL;
	IAddrRec* rec=NULL;
	bool bStatus = false;


	//create address book
	if ( SUCCESS != ISHELL_CreateInstance(m_pIShell, AEECLSID_ADDRBOOK, (void **)&pIAddrBook))
	{
		return false;
	}

	//enumerate last backed up database and check any deleted records
	IDATABASE_Reset( pBackupDb );
	int n = (int) IDATABASE_GetRecordCount(pBackupDb);


	while (  NULL != (pRecord=IDATABASE_GetNextRecord( pBackupDb )) )
	{
		// Get the first field. DB_BACKUP_RECORD_ID.
		rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
		if ( (nFieldLen>0) && (SMDB_FT_WORD==rFieldType) )
		{
			IDBRECORD_GetFieldWord(pRecord, &nId);
			
			//check weather the record is present in the addressbook
			if( AEE_SUCCESS == IADDRBOOK_EnumRecInit(pIAddrBook,AEE_ADDR_CAT_NONE, AEE_ADDRFIELD_NONE,NULL,0) )
			{
				while( NULL != (rec=IADDRBOOK_EnumNextRec(pIAddrBook)) )
				{

					nAddrRecId = IADDRREC_GetRecID (rec);
					if ( nId==nAddrRecId ) 
					//if we found the record in addrbook we stop searching
					{
						bStatus = true;
						IADDRREC_Release(rec);
						break;
					}
					IADDRREC_Release(rec);
				}
			}
			else
			{
				// Release the db record
				IDBRECORD_Release( pRecord );
				return false;
			}

		}

		
		// Release the record
		IDBRECORD_Release( pRecord );

		//if record didnt find in address book, add it to ChangeDb as a deleted record.
		if ( !bStatus )
		{
			CContactInfo * pContact = new CContactInfo();
			if ( NULL==pContact ) return false;
			pContact->SetDeletedContact(nId);
			if ( !AddToChangesDb(pCghangeDb, SM_RECORD_DELETE, pContact))
			{
				delete pContact;
				//release address book
				IADDRBOOK_Release(pIAddrBook);
				return false;
			}
			delete pContact;
		}

	}

	//release address book
	IADDRBOOK_Release(pIAddrBook);

	return true;
}

// ================================================================================
// FUNCTION		: SearchContactInBackupDb
// DESCRIPTION	: Rearch a contact in backup database.
// ================================================================================
DbSearchResult CDbHandler::SearchContactInBackupDb(IDatabase* pBkpDb, uint16 nId, CContactInfo *pCurrentContact, CContactInfo *pBkpContact)
{
	if ( NULL==pBkpDb ) return SM_DBERROR;

	IDBRecord* pRecord=NULL;
	uint16 nFieldLen;
	AEEDBFieldName rFieldName;
	AEEDBFieldType rFieldType;
	uint16 nBkpId;
	//enumerate last backup database to find matching contact
	IDATABASE_Reset( pBkpDb );
	int n = (int) IDATABASE_GetRecordCount( pBkpDb );


	while (  NULL != (pRecord=IDATABASE_GetNextRecord( pBkpDb )) )
	{
		// Get the first field. DB_BACKUP_RECORD_ID.
		rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
		if ( (nFieldLen>0) && (SMDB_FT_WORD==rFieldType) )
		{
			IDBRECORD_GetFieldWord(pRecord, &nBkpId);

			if ( nId==nBkpId )
			{
				

				//fetch contact info from backup database
				if ( !pBkpContact->SetContactFromBkpDbRecord(pRecord, nId) )
				{		
					//we have a matching id.  check weather its updated
					IDBRECORD_Release( pRecord );
					return SM_DBERROR;
				}

				//we have a matching id.  check weather its updated
				IDBRECORD_Release( pRecord );

				//compare address book contact info and last backup info
				if ( pCurrentContact->CompareContact(pBkpContact) )
					return SM_RECORD_EQUAL;
				else
					return SM_RECORD_UPDATE;

			}
		}


		// Release the record
		IDBRECORD_Release( pRecord );
	}

	return SM_RECORD_ADD;
}

// ================================================================================
// FUNCTION		: CopyAddrBookToBackupDb
// DESCRIPTION	: Copy all contacts in addrressbook to backup database.
// ================================================================================
bool CDbHandler::CopyAddrBookToBackupDb()
{
	IDatabase *pBkpDatabase;
	IAddrBook * pIAddrBook=NULL;
	IAddrRec* rec=NULL;
	CContactInfo *pContact;

	//delete previous backup database.
	//this should change to: create a tempory backup database. after successfully creating this.
	//rename current bakup to old backup and temp backup to backup and delete old backup
	if ( !DeleteDatabase( SM_BACKUPDB )) return false;

	pContact = new CContactInfo();
	if ( NULL==pContact ) return false;

	if ( !OpenBackupDb(&pBkpDatabase))
	{
		delete pContact;
		return false;
	}

	//enumerate addressbook
	if ( SUCCESS != ISHELL_CreateInstance(m_pIShell, AEECLSID_ADDRBOOK, (void **)&pIAddrBook))
	{
		delete pContact;
		IDATABASE_Release(pBkpDatabase);
		return false;
	}

	if( AEE_SUCCESS == IADDRBOOK_EnumRecInit(pIAddrBook,AEE_ADDR_CAT_NONE, AEE_ADDRFIELD_NONE,NULL,0) )
	{
		while( NULL != (rec=IADDRBOOK_EnumNextRec(pIAddrBook)) )
		{

			pContact->SetContactInfo(rec);
			//Add contact to backup database
			AddToBackupDb( pBkpDatabase, pContact);

			IADDRREC_Release(rec);
		
		}
	}

	delete pContact;
	//release address book
	IADDRBOOK_Release(pIAddrBook);
	IDATABASE_Release(pBkpDatabase);

	return true;
}

// ================================================================================
// FUNCTION		: GetRecordsInChangeDb
// DESCRIPTION	: Get records of changes database in to a contact array.
// ================================================================================
bool CDbHandler::GetRecordsInChangeDb(CContactInfo **pContactsAr, int &nSize,  int &nNoOfPackets, int nPacketNo)
{
	IDatabase *pDatabase=NULL;	
	IDBRecord* pRecord=NULL;

	if ( !OpenDatabase(&pDatabase, SM_CHANGEDB, FALSE)) return false;

	IDATABASE_Reset(pDatabase);
	nSize = (int) IDATABASE_GetRecordCount(pDatabase);
	
	if ( 0==nSize )
	{
		//empty database
		nNoOfPackets=1;
		IDATABASE_Release(pDatabase);
		return true;
	}

	//calculate no of packets to send to server
	if ( 0== nSize % SM_MAX_RECORDS_PREPACKET )
	{
		nNoOfPackets = nSize/SM_MAX_RECORDS_PREPACKET;
	}else
		nNoOfPackets = (nSize/SM_MAX_RECORDS_PREPACKET)+1;

	int nNoOfRecsInPacket =0;
	if ( nNoOfPackets==nPacketNo )
		nNoOfRecsInPacket = nSize - (nPacketNo-1)*SM_MAX_RECORDS_PREPACKET;
	else
		nNoOfRecsInPacket = SM_MAX_RECORDS_PREPACKET;
	
	(*pContactsAr) = new CContactInfo[nNoOfRecsInPacket];//nNoOfRecsInPacket(CContactInfo*)MALLOC(sizeof(CContactInfo)*nSize);//

	if ( NULL==(*pContactsAr) )
	{
		IDATABASE_Release(pDatabase);
		return false;
	}

	//jump to the current packet
	int nPass=(nPacketNo-1)*SM_MAX_RECORDS_PREPACKET;
	if (nPass>nSize)
	{
		delete [] (*pContactsAr);
		IDATABASE_Release(pDatabase);
		return false;
	}

	for (int i=0; i<nPass; i++)
	{
		pRecord=IDATABASE_GetNextRecord(pDatabase);
		if (NULL!=pRecord) IDBRECORD_Release( pRecord );
	}
	/////////////////////////////

	int nCount=0;
	while (  NULL != (pRecord=IDATABASE_GetNextRecord(pDatabase)) )
	{	
	
		//get contact information
		(*pContactsAr)[nCount].SetContactFromChangeDbRecord(pRecord);

		// Release the record
		IDBRECORD_Release( pRecord );

		nCount++;

		//check packet size
		if ( SM_MAX_RECORDS_PREPACKET==nCount ) break;
	}

	nSize = nCount;

	IDATABASE_Release(pDatabase);

	return true;
}

// ================================================================================
// FUNCTION		: UpdateAddrBook
// DESCRIPTION	: Update phone addressbook.
// ================================================================================
bool CDbHandler::UpdateAddrBook(CContactInfo *pContactAr, int nSize)
{

	int nState=0;
	IAddrBook *pAddrBook=NULL;
	IAddrRec *pRec=NULL;
	AEEAddrField *field=NULL;
	int nCount=0;
	int nRet=0;
	uint16 wId=0;
	AECHAR* pwsGlobalId=NULL;


	if(SUCCESS!=ISHELL_CreateInstance(m_pIShell, AEECLSID_ADDRBOOK, (void **)&pAddrBook))
		return false;

	for (int i=0; i<nSize; i++)
	{
		nState = pContactAr[i].GetState();

		if ( pContactAr[i].FillAddrFields(&field, nCount))
		{
		
			switch (nState)
			{
			case SM_RECORD_ADD:

										
				pRec = IADDRBOOK_CreateRec(pAddrBook,AEE_ADDR_CAT_NONE, (AEEAddrField*)field, nCount);

				if (NULL==pRec)
				{
					IADDRBOOK_Release(pAddrBook);
					delete [] field;
					return false;
				}
				
				//set contact id of record.
				pContactAr[i].SetContactId( IADDRREC_GetRecID(pRec));

				IADDRREC_Release(pRec);
				pRec=NULL;
							
				break;

			case SM_RECORD_UPDATE:
	
				wId = pContactAr[i].GetContactId();

				pRec = IADDRBOOK_GetRecByID(pAddrBook, wId);

				if ( NULL!=pRec )
				{
					if ( AEE_SUCCESS != IADDRREC_UpdateAllFields(pRec,field,nCount))
					{
						
						IADDRREC_Release(pRec);
						IADDRBOOK_Release(pAddrBook);
						delete [] field;
						return false;
					}

					//set contact id of record.
					//pContactAr[i].SetContactId( IADDRREC_GetRecID(pRec));

					IADDRREC_Release(pRec);
					pRec=NULL;
				}
				
				break;

			case SM_RECORD_DELETE:

				wId = pContactAr[i].GetContactId();
				DBGPRINTF("Deleted rec id= %u", wId);
				pRec = IADDRBOOK_GetRecByID(pAddrBook, wId);
				if ( NULL!=pRec )
				{
					//set contact id of record.
					pContactAr[i].SetContactId( IADDRREC_GetRecID(pRec));

					if(AEE_SUCCESS !=IADDRREC_RemoveRec(pRec))
					{
						IADDRBOOK_Release(pAddrBook);
						delete [] field;
						return false;
					}

					DBGPRINTF("deleted record");
					//no need to relase the address record after deleting it.
				}

				break;

			default:
				break;
			}

			if ( NULL!=field )
			{
				delete [] field;
				field=NULL;
			}
		}
	}

	IADDRBOOK_Release(pAddrBook);

	return true;
}

// ================================================================================
// FUNCTION		: GetContactList
// DESCRIPTION	: Get the addressbook contacts 
// ================================================================================
int CDbHandler::GetContactList(AECHAR **pszContactList)
{
	
	//create address book
	IAddrBook * pIAddrBook=NULL;
	if ( SUCCESS != ISHELL_CreateInstance(m_pIShell, AEECLSID_ADDRBOOK, (void **)&pIAddrBook))
	{
		return 0;
	}

	//get the number of records in addressbook and create a buffer to hold the names
	int nSize = IADDRBOOK_GetNumRecs(pIAddrBook);
	*pszContactList = (AECHAR*)MALLOC(nSize*100*sizeof(AECHAR));//assume approximate name length + mobile number is 100
	if ( NULL==*pszContactList ) goto error;

	if( AEE_SUCCESS == IADDRBOOK_EnumRecInit(pIAddrBook,AEE_ADDR_CAT_NONE, AEE_ADDRFIELD_NONE,NULL,0) )
	{
		IAddrRec* rec=NULL;

		while( NULL != (rec=IADDRBOOK_EnumNextRec(pIAddrBook)) )
		{
			AEEAddrField* pField;

			int noOfFilelds = IADDRREC_GetFieldCount(rec);

			for ( int i=0; i<noOfFilelds; i++)
			{
				pField = IADDRREC_GetField(rec, i);
				//append first name and last name to the buffer
				if ( SM_ADDRFIELD_FIRSTNAME==pField->fID )
				{
					WSTRCAT(*pszContactList, (AECHAR*)pField->pBuffer);
					WSTRCAT(*pszContactList, L" ");
				}

				//append the last name to the buffer
				if ( SM_ADDRFIELD_LASTNAME==pField->fID )
				{
					WSTRCAT(*pszContactList, (AECHAR*)pField->pBuffer);
					
				}

				//append the mobile number
				if ( SM_ADDRFIELD_MOBILE==pField->fID )
				{
					WSTRCAT(*pszContactList, L":");
					WSTRCAT(*pszContactList, (AECHAR*)pField->pBuffer);
					
				}
			}

			WSTRCAT(*pszContactList, L";");

			IADDRREC_Release(rec);
		
		}

	}

error:
	IADDRBOOK_Release(pIAddrBook);

	return nSize;
	
}

// ================================================================================
// FUNCTION		: AddToSmsDatabase
// DESCRIPTION	: Add a sms to sms database.
// ================================================================================
int CDbHandler::AddToSmsDatabase(int nSent, AECHAR *pszFromNo, AECHAR *pszMsg, AECHAR *pszTime)
{
	IDatabase *pSmsDb=NULL;
	//open sms database. create it if doesnt exist.
	if ( OpenDatabase(&pSmsDb, SM_SMSDB, TRUE))
	{
		IDBRecord* pRecord;
		AEEDBField pFieldArray[ SMS_RECORD_FIELDS ];
		int nContactId=0;
		//fill record fields with sms information.

		//sent/received
		pFieldArray[0].fType = SMDB_FT_WORD;
		pFieldArray[0].fName = SMDB_FIELD_NUM;
		pFieldArray[0].wDataLen = sizeof(nSent);
		pFieldArray[0].pBuffer = &nSent;

		//contact id
		pFieldArray[1].fType = SMDB_FT_WORD;
		pFieldArray[1].fName = SMDB_FIELD_NUM;
		pFieldArray[1].wDataLen = sizeof(nContactId);
		pFieldArray[1].pBuffer = &nContactId;
		//Sms from number
		pFieldArray[2].fType = AEEDB_FT_STRING;
		pFieldArray[2].fName = AEEDBFIELD_TEXT;
		pFieldArray[2].wDataLen = WSTRSIZE(pszFromNo);
		pFieldArray[2].pBuffer = (void*)pszFromNo;
		//Sms message
		pFieldArray[3].fType = AEEDB_FT_STRING;
		pFieldArray[3].fName = AEEDBFIELD_TEXT;
		pFieldArray[3].wDataLen = WSTRSIZE(pszMsg);
		pFieldArray[3].pBuffer = (void*)pszMsg;
		//Sms received time
		pFieldArray[4].fType = AEEDB_FT_STRING;
		pFieldArray[4].fName = AEEDBFIELD_TEXT;
		pFieldArray[4].wDataLen = WSTRSIZE(pszTime);
		pFieldArray[4].pBuffer = (void*)pszTime;

		// Create a new record based on the AEEDBField field array values
		pRecord = IDATABASE_CreateRecord( pSmsDb, pFieldArray,  SMS_RECORD_FIELDS);

		// Return false if the record could not be created
		if( !pRecord )
			return false;

		// Release the record
		IDBRECORD_Release( pRecord );	
		IDATABASE_Release(pSmsDb);
	}
	return SUCCESS;
}

// ================================================================================
// FUNCTION		: GetSms
// DESCRIPTION	: retrive the sms in to array(pSmsAr). Make sure to free the pSmsAr after use.
//				  even GetSms failed there may be resource allocted to pSmsAr and need to free it.
// ================================================================================
int CDbHandler::GetSms(sms **pSmsAr, int nTotal, char **pFilterAr, int nNo)
{
	IDatabase *pDatabase=NULL;	
	IDBRecord* pRecord=NULL;
	uint16 nFieldLen;
	AEEDBFieldName rFieldName;
	AEEDBFieldType rFieldType;
	AECHAR* psStringPtr=NULL;
	AECHAR* psStringPtr2=NULL;
	uint16 wTemp=0;

	int nCount=0;
	
	if ( !OpenDatabase(&pDatabase, SM_SMSDB, FALSE)) return ENOMEMORY;

	IDATABASE_Reset(pDatabase);
	nTotal = (int) IDATABASE_GetRecordCount(pDatabase);

	//if sms array is NUll we return the no of records in database.
	if (NULL==pSmsAr)
	{
		IDATABASE_Release(pDatabase);
		return nTotal;
	}
	
	
	while (  NULL != (pRecord=IDATABASE_GetNextRecord(pDatabase)) )
	{
		//allocate memory for single sms
		pSmsAr[nCount] = (sms*)MALLOC(sizeof(sms));
		if (NULL==pSmsAr[nCount] )
		{
			IDBRECORD_Release( pRecord );
			IDATABASE_Release(pDatabase);
			return ENOMEMORY;
		}

		//set sms state
		pSmsAr[nCount]->bStatus = FALSE;

		//set sms record id
		pSmsAr[nCount]->nSmsId = IDBRECORD_GetID(pRecord);

		// Get the first field. sms sent/received
		rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
		if ( (nFieldLen>0) && (SMDB_FT_WORD==rFieldType) )
		{
			IDBRECORD_GetFieldWord(pRecord, &pSmsAr[nCount]->nSent);			
		}
						
		// Get the first field. sms contact id.
		rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
		if ( (nFieldLen>0) && (SMDB_FT_WORD==rFieldType) )
		{
			IDBRECORD_GetFieldWord(pRecord, &pSmsAr[nCount]->nContactId);			
		}

		// Get next field. sms from field 
		rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
		if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
		{
			psStringPtr = IDBRECORD_GetFieldString(pRecord);
			pSmsAr[nCount]->pszFrom = (AECHAR*)MALLOC(WSTRSIZE(psStringPtr)+2);
			if (NULL==pSmsAr[nCount]->pszFrom)
			{
				IDBRECORD_Release( pRecord );
				IDATABASE_Release(pDatabase);
				return ENOMEMORY;
			}
			
			WSTRCPY(pSmsAr[nCount]->pszFrom, psStringPtr);
						
		}

		// Get next field. sms message field 
		rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
		if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
		{
			psStringPtr = IDBRECORD_GetFieldString(pRecord);
			pSmsAr[nCount]->pszMsg = (AECHAR*)MALLOC(WSTRSIZE(psStringPtr)+2);
			if (NULL==pSmsAr[nCount]->pszMsg)
			{
				IDBRECORD_Release( pRecord );
				IDATABASE_Release(pDatabase);
				return ENOMEMORY;
			}
			
			WSTRCPY(pSmsAr[nCount]->pszMsg, psStringPtr);
		}

		// Get next field. sms message field 
		rFieldType = IDBRECORD_NextField( pRecord, &rFieldName, &nFieldLen );
		if ( (nFieldLen>0) && (SMDB_FT_STRING==rFieldType) )
		{
			psStringPtr = IDBRECORD_GetFieldString(pRecord);
			pSmsAr[nCount]->pszTime = (AECHAR*)MALLOC(WSTRSIZE(psStringPtr)+2);
			if (NULL==pSmsAr[nCount]->pszTime)
			{
				IDBRECORD_Release( pRecord );
				IDATABASE_Release(pDatabase);
				return ENOMEMORY;
			}
			
			WSTRCPY(pSmsAr[nCount]->pszTime, psStringPtr);
		}

		// Release the record
		IDBRECORD_Release( pRecord );

		//check the sms with filtered phone numbers
		//if ( IsSmsMatched(pSmsAr[nCount]->pszFrom, pFilterAr, nNo))
		{
			nCount++;
		}
		//else
		//	FREEIF(pSmsAr[nCount]);
	}

	nTotal = nCount;
	IDATABASE_Release(pDatabase);
	return SUCCESS;
}

// ================================================================================
// FUNCTION		: IsSmsMatched
// DESCRIPTION	: Check wheather the sms matched with the filtered phone number..
// ================================================================================
boolean CDbHandler::IsSmsMatched(AECHAR *pszPhoneNo, char **pFilterAr, int nNo)
{
	if ( 0==nNo ) return TRUE;

	char *pszTemp=NULL;
	int nLen= WSTRLEN(pszPhoneNo)+1;
	pszTemp = (char*)MALLOC(nLen);
	if (NULL==pszTemp) return FALSE;
	WSTRTOSTR(pszPhoneNo, pszTemp, nLen);
	
	//search the phone no in array
	for (int i=0; i<nNo; i++)
	{
		if ( 0==STRCMP(pFilterAr[i], pszTemp))
		{
			FREEIF(pszTemp);
			return TRUE;
		}
	}

	FREEIF(pszTemp);
	return FALSE;
}
