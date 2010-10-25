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
 Module Name				: ContactInfo.cpp
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
#if !defined(AFX_CONTACTINFO_H__20060BE6_6466_4D4A_B3FF_221C245D4102__INCLUDED_)
#define AFX_CONTACTINFO_H__20060BE6_6466_4D4A_B3FF_221C245D4102__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AEEShell.h"

#include "SMCommon.h"

class CContactInfo  
{
public:
	void SetContactId(uint16 wId);
	AECHAR* GetGlobalId();
	bool FillAddrFields(AEEAddrField **pField, int &nSize);
	int CountValidFields();
	int GetSize();
	uint16 GetContactId();
	AECHAR* GetFName();
	AECHAR* GetLName();
	AECHAR* GetCompany();
	AECHAR* GetJobTitles();
	AECHAR* GetFax();
	AECHAR* GetFaxHome();
	AECHAR* GetFaxWork();
	AECHAR* GetMobile();
	AECHAR* GetMobileHome();
	AECHAR* GetMobileWork();
	AECHAR* GetTel();
	AECHAR* GetTelHome();
	AECHAR* GetTelWork();
	AECHAR* GetEmail();
	AECHAR* GetEmailHome();
	AECHAR* GetEmailWork();

	bool SetValue(char **pszValue, char *pszXml, char *pszTag);
	bool SetValue(AECHAR **pwzValue, char *pszXml, char *pszTag);
	bool SetContactFromXml(char *pszXml, char *pszType);
	bool SetContactFromChangeDbRecord(IDBRecord *pRecord);
	int GetState();
	bool CompareContact(CContactInfo *pContact);
	bool SetContactFromBkpDbRecord(IDBRecord* pRecord, int nId);
	bool GetContactInfoToBackup(AEEDBField *pFieldArray, AECHAR * pEmptyData);
	bool GetContactInfo(AEEDBField *pFieldArray, AECHAR * pEmptyData);
	int SetContactInfo(IAddrRec* pAddrRec);
	bool SetDeletedContact(int nRecId);
	CContactInfo();
	virtual ~CContactInfo();

private:
	void FillMultiField(AEEAddrField *pField, int &nPos, AECHAR *pwzName, AEEAddrFieldID type);
	void CountMultiFields(AECHAR *pwzField, int &nCount);
	bool SetMultiValue(AECHAR **pwsValue, char *pszXml, char *pszMultiTag, char *pszSingleTag);
	bool CompareStrings(AECHAR *pszStr1, AECHAR *pszStr2);
	bool AppendFieldTo(AECHAR **psDes, AECHAR *psScr);
	bool CopyFieldTo(AECHAR **psDes, AECHAR *psScr);
	bool SetField(AEEAddrField* pField);
	void ReleaseMem();
	void ResetContact();

	uint16 m_nContactId;
	AECHAR *m_psFName;
	AECHAR *m_psMName; 
	AECHAR *m_psLName;  
	AECHAR *m_psCompany;	         
	AECHAR *m_psJTitle;	            
	AECHAR *m_psFaxH;
	AECHAR *m_psFaxW;
	AECHAR *m_psFax;
	AECHAR *m_psMobileH;	         
	AECHAR *m_psMobileW;
	AECHAR *m_psMobile;
	AECHAR *m_psTelephoneH;
	AECHAR *m_psTelephoneW;
	AECHAR *m_psTelephone;
	AECHAR *m_psEmailW;
	AECHAR *m_psEmailH;
	AECHAR *m_psEmail;

	//keep the state of contact. ADD, UPDATE or DELETE
	int m_nState;
	//Global id
	AECHAR *m_psGlobalId;
};

#endif // !defined(AFX_CONTACTINFO_H__20060BE6_6466_4D4A_B3FF_221C245D4102__INCLUDED_)
