
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
 Module Name				: RippleVault.c
 Language used : Java
 Coding Standard			: BREW Coding Standards
 Author                     : SRI
 Date Created               : 02-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

/*===============================================================================
INCLUDES AND VARIABLE DEFINITIONS
=============================================================================== */
#include "AEEModGen.h"          // Module interface definitions
#include "AEEAppGen.h"          // Applet interface definitions
#include "AEEShell.h"           // Shell interface definitions
#include "AEEStdLib.h"

#include "AEERootForm.h"
#include "ListForm.h"

#include "MainForm.h"
#include "StartForm.h"
#include "RippleVault.brh"
#include "RippleVault.bid"
#include "..\SyncEng\SMCommon.h"
#include "..\SyncEng\SyncEng.h"
#include "AEEFile.h"
#include "UserData.h"
#include "AEETAPI.h"


#define MIN_SHEDULETIME		300
/*-------------------------------------------------------------------
Applet structure. All variables in here are reference via "pMe->"
-------------------------------------------------------------------*/
typedef struct _RippleVault {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	AEEDeviceInfo  DeviceInfo; // always have access to the hardware device information
	IDisplay      *pIDisplay;  // give a standard way to access the Display interface
	IShell        *pIShell;    // give a standard way to access the Shell interface

	IRootForm* rootForm;
	IForm* mainForm;
	boolean m_bGoToBg;
	boolean m_bIsFirstTimeStart;
	boolean m_bIsRegistered;
	boolean m_bSynchronizing;
	boolean m_bAppForeground;
	uint32 m_dwSheduleTime;
	uint32 m_dwSmsTime;
	boolean m_bSmsUploading;

	IDialog* progressDialog;
	HandlerDesc progressHandler;
	//sync engine
	ISyncEngExt* m_pSyncExt;

} RippleVault;

/*-------------------------------------------------------------------
Function Prototypes
-------------------------------------------------------------------*/
static  boolean RippleVault_HandleEvent(RippleVault* pMe, AEEEvent eCode, 
                                             uint16 wParam, uint32 dwParam);
boolean RippleVault_InitAppData(RippleVault* pMe);
void    RippleVault_FreeAppData(RippleVault* pMe);
void	SheduleTimerCB(void *cxt);
boolean	IsRegistered(RippleVault* pMe);
int CreateProgressDialog(RippleVault* pMe, uint16 nResId);
static boolean ProgressDlg_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam);
void SmsTimerCB(void *cxt);
boolean RippleVault_UploadSms(RippleVault* pMe);
/*===============================================================================
FUNCTION DEFINITIONS
=============================================================================== */

// ================================================================================
// FUNCTION		: AEEClsCreateInstance
// DESCRIPTION	: Create a instance of RippleValt
// ================================================================================
int AEEClsCreateInstance(AEECLSID ClsId, IShell *pIShell, IModule *po, void **ppObj)
{
	*ppObj = NULL;

	if( ClsId == AEECLSID_RIPPLEVAULT )
	{
		// Create the applet and make room for the applet structure
		if( AEEApplet_New(sizeof(RippleVault),
                          ClsId,
                          pIShell,
                          po,
                          (IApplet**)ppObj,
                          (AEEHANDLER)RippleVault_HandleEvent,
                          (PFNFREEAPPDATA)RippleVault_FreeAppData) ) // the FreeAppData function is called after sending EVT_APP_STOP to the HandleEvent function
                          
		{
			//Initialize applet data, this is called before sending EVT_APP_START
            // to the HandleEvent function
			if(RippleVault_InitAppData((RippleVault*)*ppObj))
			{
				//Data initialized successfully
				return(AEE_SUCCESS);
			}
			else
			{
				//Release the applet. This will free the memory allocated for the applet when
				// AEEApplet_New was called.
				IAPPLET_Release((IApplet*)*ppObj);
				return EFAILED;
			}

        } // end AEEApplet_New

    }

	return(EFAILED);
}


// ================================================================================
// FUNCTION		: RippleVault_HandleEvent
// DESCRIPTION	: Handle events of Shell
// ================================================================================
static boolean RippleVault_HandleEvent(RippleVault* pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{  
	AEENotify *n;
	user *pUser=NULL;
	AEESMSMsg *smsInfo;
	JulianType currTime;//only time part
	uint32 currSecs=0;

	// Allow rootform to handle event first
	if(IROOTFORM_HandleEvent(pMe->rootForm, eCode, wParam, dwParam)) 
		return TRUE;
	
    switch (eCode) 
	{
        // App is told it is starting up
        case EVT_APP_START:                        
		    // Add your code here...
		
			//if not the first, pop the main form after application start.
			if ( pMe->m_bIsFirstTimeStart )
			{
				ISHELL_CloseApplet(pMe->pIShell, FALSE); // send applet to background
			}
				

			pMe->m_bIsFirstTimeStart=FALSE;
			
			
			IROOTFORM_Activate(pMe->rootForm);
									
			pMe->m_bAppForeground = TRUE;
			return TRUE;


        // App is told it is exiting
        case EVT_APP_STOP:
            // Add your code here...

			//this need to change
			if ( 1243024==dwParam )//emulator close
				pMe->m_bGoToBg=FALSE;

			if ( pMe->m_bGoToBg )
			{
				*((boolean*) dwParam) = FALSE; // set dwParam to run in bg

				//when user registered and send app to background, need to change the status
				IsRegistered(pMe);

			}
					
			pMe->m_bAppForeground = FALSE;
      		return(TRUE);

        // App is being suspended 
        case EVT_APP_SUSPEND:
		    // Add your code here...

      		return(TRUE);

        // App is being resumed
        case EVT_APP_RESUME:
		    // Add your code here...

      		return(TRUE);


        // An SMS message has arrived for this app. Message is in the dwParam above as (char *)
        // sender simply uses this format "//BREW:ClassId:Message", example //BREW:0x00000001:Hello World
        case EVT_APP_MESSAGE:
		    // Add your code here...

      		return(TRUE);

        // A key was pressed. Look at the wParam above to see which key was pressed. The key
        // codes are in AEEVCodes.h. Example "AVK_1" means that the "1" key was pressed.
        case EVT_KEY:
			{

			}
      		return(TRUE);

		case EVT_NOTIFY:
			{
				
				//start shedule timer when phone is powered on.
				n = (AEENotify*)dwParam;
				if (AEECLSID_SHELL == n->cls && NMASK_SHELL_INIT == n->dwMask) 
				{
					//check autostart is on/off
					if ( GetUserData(pMe->pIShell, &pUser))
					{			
						if(0==STRCMP("OFF", pUser->autostart))
							return TRUE;
						FREEIF(pUser);
					}

					//start applet
					ISHELL_StartApplet(pMe->pIShell, AEECLSID_RIPPLEVAULT);
					//send it background
					pMe->m_bGoToBg = TRUE;

					//if sheduler timer is too short set it to default minimum valude of ...
					if ( pMe->m_dwSheduleTime<MIN_SHEDULETIME )
						pMe->m_dwSheduleTime=MIN_SHEDULETIME;

					//set time for sheduler for contact synchronization
					if ( SUCCESS != ISHELL_SetTimer(pMe->pIShell, pMe->m_dwSheduleTime, SheduleTimerCB, pMe))
					{
						DBGPRINTF("Shedule timer start failed.");
					}

					//set timer for sms backup
					//calculate sms upload time.
					
					GETJULIANDATE(0, &currTime);
					//we want to compare only time part. so set others to 1980 01 06
					currTime.wYear=1980;
					currTime.wMonth=1;
					currTime.wDay=6;
					currSecs = JULIANTOSECONDS(&currTime);
					if ( pMe->m_dwSmsTime>currSecs )
						pMe->m_dwSmsTime= pMe->m_dwSmsTime-currSecs;
					else
						pMe->m_dwSmsTime= 24*3600-(currSecs-pMe->m_dwSmsTime); 
					if ( SUCCESS != ISHELL_SetTimer(pMe->pIShell, pMe->m_dwSmsTime, SmsTimerCB, pMe))
					{
						DBGPRINTF("SMS timer start failed.");
					}
				}

				//SMS received
				if ( NMASK_TAPI_SMS_TEXT==n->dwMask )
				{
					char *pszFromNo=NULL;
					AECHAR *pszText=NULL;
					char *pszTime=NULL;
					AEESMSTimestamp ts;
					smsInfo = (AEESMSMsg*)n->pData;

					pszFromNo = (char*)MALLOC(smsInfo->nDigits+1);
					pszText = (AECHAR*)MALLOC((smsInfo->nLen+1)*sizeof(AECHAR));
					pszTime = (char*)MALLOC(50);

					if ( NULL==pszFromNo || NULL==pszText || NULL==pszTime )
					{
						FREEIF(pszFromNo);
						FREEIF(pszText);
						FREEIF(pszTime);
						return FALSE;
					}

					STRCPY(smsInfo->szDigits, pszFromNo);
					STRTOWSTR(smsInfo->data, pszText, (smsInfo->nLen+1)*sizeof(AECHAR));
					ts = smsInfo->ts;
					if ( ts.hour>12 )
					{
						SPRINTF(pszTime, "%d/%d%d %d:%d:%d pm", ts.day, ts.month, ts.year,
																ts.hour-12, ts.minute, ts.second);
					}
					else
					{
						SPRINTF(pszTime, "%d/%d%d %d:%d:%d am", ts.day, ts.month, ts.year,
																ts.hour, ts.minute, ts.second);

					}

					FREEIF(pszFromNo);
					FREEIF(pszText);
					FREEIF(pszTime);
				}
				return (TRUE);
			}

	case EVT_USER_SHEDULE:
			DBGPRINTF("Timer call!");
			if ( NULL==pMe->m_pSyncExt && !pMe->m_bAppForeground && pMe->m_bIsRegistered
				&& !pMe->m_bSmsUploading)
			{
				//check roaming enable/disable before start sync
				pMe->m_bSynchronizing = TRUE;
				if (NULL==pMe->m_pSyncExt)
					if (SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pMe->m_pSyncExt))
						return TRUE;
				
				//show progressbar
				IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->progressDialog);
				
				pMe->m_bSynchronizing = TRUE;
				ISYNCENG_SyncContacts(pMe->m_pSyncExt);
				
			}
			return (TRUE);
			break;

	case EVT_USER_SMSUPLOAD:
			//back up SMS if contacts are not synchronising
			RippleVault_UploadSms(pMe);
		break;

	case EVT_USER_SHOWMENU:
			//release previos window
			IROOTFORM_PopForm(pMe->rootForm);
			if(pMe->mainForm) {
				IFORM_Release((IForm*)pMe->mainForm);
				pMe->mainForm = NULL;
			}
			//show main form
			MainForm_New(&pMe->mainForm, pMe->pIShell, pMe->rootForm, NULL);
			IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->mainForm);
			return (TRUE);
		break;
	case EVT_USER_EXIT:
			pMe->m_bGoToBg=FALSE;
			ISHELL_CloseApplet(pMe->pIShell, FALSE);
		break;


	default:
		if (pMe->m_pSyncExt != NULL) 
			ISYNCENG_HandleEvent(pMe->m_pSyncExt, eCode, wParam, dwParam);
		break;

   }

   return FALSE;
}

// ================================================================================
// FUNCTION		: RippleVault_InitAppData
// DESCRIPTION	: this function is called when your application is starting up
// ================================================================================
boolean RippleVault_InitAppData(RippleVault* pMe)
{
   int result = 0;

    // Get the device information for this handset.
    // Reference all the data by looking at the pMe->DeviceInfo structure
    // Check the API reference guide for all the handy device info you can get
    pMe->DeviceInfo.wStructSize = sizeof(pMe->DeviceInfo);
    ISHELL_GetDeviceInfo(pMe->a.m_pIShell,&pMe->DeviceInfo);

    // The display and shell interfaces are always created by
    // default, so we'll asign them so that you can access
    // them via the standard "pMe->" without the "a."
    pMe->pIDisplay = pMe->a.m_pIDisplay;
    pMe->pIShell   = pMe->a.m_pIShell;

    // Insert your code here for initializing or allocating resources...

   pMe->mainForm = NULL;

   //create rootForm
   if(ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_ROOTFORM, (void**) &pMe->rootForm) == 0)
   {
		ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_DISPLAY, (void**) &pMe->pIDisplay);
		result = 1;
   }

	
	if ( IsRegistered(pMe) )
	{
		//show main form
		MainForm_New(&pMe->mainForm, pMe->pIShell, pMe->rootForm, NULL);
	}
	else
	{
		//show registration window
		if(StartForm_New(&pMe->mainForm, pMe->a.m_pIShell, pMe->rootForm,pMe->DeviceInfo) != 0)
			return 0;
	}
	
	IROOTFORM_PushForm(pMe->rootForm, (IForm*)pMe->mainForm);

	//create progressbar
	CreateProgressDialog(pMe, IDS_SYNC);

	//sync engine
	pMe->m_pSyncExt = NULL;

	pMe->m_bIsFirstTimeStart = TRUE;
	
	pMe->m_bSynchronizing = FALSE;
	pMe->m_bAppForeground = FALSE;
	pMe->m_bSmsUploading = FALSE;

	return result;
}

// ================================================================================
// FUNCTION		: RippleVault_FreeAppData
// DESCRIPTION	: this function is called when your application is exiting
// ================================================================================
void RippleVault_FreeAppData(RippleVault* pMe)
{
	//cancel timer
	ISHELL_CancelTimer(pMe->pIShell, SheduleTimerCB, pMe);
	ISHELL_CancelTimer(pMe->pIShell, SmsTimerCB, pMe);

	if (NULL!=pMe->m_pSyncExt) 
	{
		ISYNCENG_Release(pMe->m_pSyncExt);
		pMe->m_pSyncExt=NULL;
	}

	if(pMe->progressDialog)
		IDIALOG_Release(pMe->progressDialog);

	if(pMe->mainForm) {
      IFORM_Release((IForm*)pMe->mainForm);
      pMe->mainForm = NULL;
   }

	if(pMe->pIDisplay) {
      IDISPLAY_Release(pMe->pIDisplay);
      pMe->pIDisplay = NULL;
   }

   if(pMe->rootForm) {
      IROOTFORM_Release(pMe->rootForm);
      pMe->rootForm = NULL;
   }

   
}

// ================================================================================
// FUNCTION		: SheduleTimerCB
// DESCRIPTION	: this function sets the sheduler time to trigger the synchronization
// ================================================================================
void SheduleTimerCB(void *cxt)
{
	RippleVault* pMe = (RippleVault*)cxt;

	ISHELL_PostEvent(pMe->pIShell, AEECLSID_RIPPLEVAULT, EVT_USER_SHEDULE, 0, 0);
	

	if ( SUCCESS != ISHELL_SetTimer(pMe->pIShell, pMe->m_dwSheduleTime, SheduleTimerCB, pMe))
	{
		DBGPRINTF("Shedule timer re-start failed.");
	}
}

// ================================================================================
// FUNCTION		: SmsTimerCB
// DESCRIPTION	: this function sets the timer for sms backup.
// ================================================================================
void SmsTimerCB(void *cxt)
{
	RippleVault* pMe = (RippleVault*)cxt;
	
	ISHELL_PostEvent(pMe->pIShell, AEECLSID_RIPPLEVAULT, EVT_USER_SMSUPLOAD, 0, 0);

	//after first time this timer triggers after 24 hours
	pMe->m_dwSmsTime = 24*60*60;
	if ( SUCCESS != ISHELL_SetTimer(pMe->pIShell, pMe->m_dwSmsTime, SmsTimerCB, pMe))
	{
			
	}
}

// ================================================================================
// FUNCTION		: IsRegistered
// DESCRIPTION	: return true if the user registred with RippleVault.
// ================================================================================
boolean IsRegistered(RippleVault* pMe)
{
	boolean status=TRUE;
	user *pUser=NULL;

	if (GetUserData(pMe->pIShell, &pUser))
	{
		if ( 0==STRCMP("reg", pUser->status))
			status=FALSE;
		pMe->m_dwSheduleTime = pUser->shedulePeriod;

		//get sms upload time
		pMe->m_dwSmsTime = 36000;//12.00
	}

	FREEIF(pUser);
	pMe->m_bIsRegistered=status;

	return status;
}

// ================================================================================
// FUNCTION		: CreateProgressDialog
// DESCRIPTION	: Create a progressbar dialog.
// ================================================================================
int CreateProgressDialog(RippleVault* pMe, uint16 nResId)
{

	int result=0;

	IWidget* backdropWidget;
	IWidget* backWidget = NULL;

	if (NULL==pMe->progressDialog)
	{
		result = ISHELL_CreateInstance(pMe->pIShell, AEECLSID_PROGRESSDIALOG, (void**) &pMe->progressDialog);
		result += ISHELL_CreateInstance(pMe->pIShell, AEECLSID_STATICWIDGET, (void **)&backdropWidget);

		IFORM_SetResText((IForm*)pMe->progressDialog, FID_TEXT, RIPPLEVAULT_RES_FILE, nResId);	


		IFORM_GetWidget((IForm*)pMe->progressDialog, WID_PROGRESS, &backWidget);
		IWIDGET_SetFGColor(backWidget,MAKE_RGB(128, 128, 255));
		IFORM_SetWidget((IForm*)pMe->progressDialog, WID_PROGRESS, backWidget);
		IWIDGET_Release(backWidget);

		IFORM_GetWidget((IForm*)pMe->progressDialog, WID_BACKGROUND, &backWidget);
		IWIDGET_SetBGColor(backWidget,MAKE_RGB(225, 225, 225));
		IWIDGET_SetBorderWidth(backWidget, 1);
		IFORM_SetWidget((IForm*)pMe->progressDialog, WID_BACKGROUND, backWidget);
		
		IFORM_SetWidget((IForm*)pMe->progressDialog, WID_BACKDROP, backdropWidget);
		IWIDGET_SetFlags((IWidget*)pMe->progressDialog, IDF_ALIGN_LEFT);

		HANDLERDESC_Init(&pMe->progressHandler, ProgressDlg_HandleEvent, pMe, NULL);
		IFORM_SetHandler((IForm*)pMe->progressDialog, &pMe->progressHandler);//pathi

		IFORM_SetProgress((IForm*)pMe->progressDialog,-1);

		IWIDGET_Release(backdropWidget);
		IWIDGET_Release(backWidget);
	}
	else
		IFORM_SetResText((IForm*)pMe->progressDialog, FID_TEXT, RIPPLEVAULT_RES_FILE, nResId);

	return TRUE;
}

// ================================================================================
// FUNCTION		: ProgressDlg_HandleEvent
// DESCRIPTION	: Handle events from progress dialog.
// ================================================================================
static boolean ProgressDlg_HandleEvent(void *po, AEEEvent evt, uint16 wParam, uint32 dwParam)
{
	RippleVault* pMe = (RippleVault*) po;
	switch (evt)
	{
	case EVT_KEY:

			break;

	case EVT_USER_SYN_FINISHED:
			//pop progressbar
			IFORM_SetProgress((IForm*)pMe->progressDialog,1);
			IROOTFORM_PopForm(pMe->rootForm);

			pMe->m_bSynchronizing = FALSE;
			//release sync module
			if (NULL!=pMe->m_pSyncExt) 
			{
				ISYNCENG_Release(pMe->m_pSyncExt);
				pMe->m_pSyncExt=NULL;
			}

		break;
		
	case EVT_USER_SYN_FAILED:
			//pop progressbar
			IFORM_SetProgress((IForm*)pMe->progressDialog,1);
			IROOTFORM_PopForm(pMe->rootForm);

			pMe->m_bSynchronizing = FALSE;
			if (NULL!=pMe->m_pSyncExt) 
			{
				ISYNCENG_Release(pMe->m_pSyncExt);
				pMe->m_pSyncExt=NULL;
			}

		break;

	case EVT_USER_SMSUPLOAD_FINISHED:
			//pop progressbar
			IFORM_SetProgress((IForm*)pMe->progressDialog,1);
			IROOTFORM_PopForm(pMe->rootForm);

			pMe->m_bSmsUploading = FALSE;
			//release sync module
			if (NULL!=pMe->m_pSyncExt) 
			{
				ISYNCENG_Release(pMe->m_pSyncExt);
				pMe->m_pSyncExt=NULL;
			}

		break;
		
	case EVT_USER_SMSUPLOAD_FAILED:
			//pop progressbar
			IFORM_SetProgress((IForm*)pMe->progressDialog,1);
			IROOTFORM_PopForm(pMe->rootForm);

			pMe->m_bSmsUploading = FALSE;
			if (NULL!=pMe->m_pSyncExt) 
			{
				ISYNCENG_Release(pMe->m_pSyncExt);
				pMe->m_pSyncExt=NULL;
			}

		break;

	default:
		if (pMe->m_pSyncExt != NULL) 
			ISYNCENG_HandleEvent(pMe->m_pSyncExt, evt, wParam, dwParam);
		break;
	}

	return HANDLERDESC_Call(&pMe->progressHandler, evt, wParam, dwParam);
}

// FUNCTION		: RippleVault_InitAppData
// DESCRIPTION	: this function is called when your application is starting up
// ================================================================================
boolean RippleVault_UploadSms(RippleVault* pMe)
{
	sms **pSmsAr=NULL;
	int nSize=0;
	int i=0;

	if (NULL==pMe->m_pSyncExt)
		if (SUCCESS!=ISHELL_CreateInstance(pMe->pIShell, AEECLSID_SYNCENG, (void **)&pMe->m_pSyncExt))
			return FALSE;

	//show progressbar
	IROOTFORM_PushForm(pMe->rootForm,(IForm*)pMe->progressDialog);

	//get the sms
	//ISYNCENG_SaveSms(pMe->m_pSyncExt, L"", L"", L"");
	//first get the no of records by passing a NULL to sms array
	nSize = ISYNCENG_GetSms(pMe->m_pSyncExt, 0, NULL, NULL, 0);
	//allocate memoryfor sms array
	pSmsAr = (sms**)MALLOC(nSize*sizeof(sms*));
	ISYNCENG_GetSms(pMe->m_pSyncExt, pSmsAr, nSize, NULL, 0);
	ISYNCENG_UploadSms(pMe->m_pSyncExt, pSmsAr, nSize);//ISYNCENG_SaveSms(pMe->m_pSyncExt, L"", L"", L"");

	//ISYNCENG_Release(pMe->m_pSyncExt);
	
	for (i=0;i<nSize;i++)
	{
		FREEIF(pSmsAr[i]->pszFrom);
		FREEIF(pSmsAr[i]->pszMsg);
		FREEIF(pSmsAr[i]->pszTime);
		FREEIF(pSmsAr[i]);
	}
	FREEIF(pSmsAr);
	
	
	return TRUE;
}