/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  This is the app ui implementation of default application server.
*
*/


#include <eikserverapp.h> 
#include <akncontext.h>
#include <apgicnfl.h> 
#include <AknIconArray.h> 
#include <AknsUtils.h>
#include <aknlists.h> 
#include <defaultappserver.rsg> 
#include "das_appui.h"
#include "das_view.h"
#include "das_srvmime_dlg.h"
#include "das_servmimeapps.h"
#include "das.hrh"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CDefaultAppAppUi::~CDefaultAppAppUi()
    {
    if(iView)
        delete iView; 
    if(iServMimeApps)
        delete iServMimeApps;
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//    
CDefaultAppAppUi::CDefaultAppAppUi() : iView(NULL), iDlg(NULL), iExitBack(EFalse)
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CDefaultAppAppUi::ConstructL()
{
    BaseConstructL(EAknEnableSkin|EAknEnableMSK|EAknSingleClickCompatible);
    // Create view object
    iView = CDefaultAppView::NewL( ClientRect() );
}

// ---------------------------------------------------------------------------
// Continues the construction, after the client connects to the server and some data is known
// (client UID and service flags)
// ---------------------------------------------------------------------------
//
void CDefaultAppAppUi::ConstructSrvMimeDlgL(TUid aClientUid, TInt aServiceFlags)
{
    SetClientIconL(aClientUid);
    iClientUid = aClientUid;
    iSelectedItemSimple=-1;
    iSelectedItemAdvanced=-1;
    iReloadDlgAdvanced=(aClientUid==KUidGS?EFalse:ETrue);
    iReloadDlg=ETrue;
    do
    {        
        if(!iServMimeApps)
            iServMimeApps=CDefaultAppServMimeApps::NewL(aClientUid, aServiceFlags);
        	
        if(iReloadDlgAdvanced)
        {
    	    //some settings for the advanced view
    	    iServMimeApps->iSimplifiedView=EFalse;
        }
        else
        {
            //some settings for the simple view
    	    iServMimeApps->iSimplifiedView=ETrue;
        };
        iServMimeApps->GetServicesAndMimesListL(iServMimeApps->iList);
        
        iDlg=CDefaultAppSrvMimeDlg::NewL(this);
        SetDialogExist( ETrue );
        iDlg->SetMopParent(this);
        iDlg->ExecuteLD(R_DAS_DLG);
        iDlg=NULL;
    }
    while(iReloadDlg);
    
    
    //exit the server
    if(iExitBack)
    {
        //if we exit by "Back" button, notify the client app, so that it does not exit.
        CEikAppServer* server = iEikonEnv->AppServer(); //if we do not do this, the client exits when backkey is pressed
        if ( server )
        {
            server->NotifyServerExit( EAknSoftkeyBack );
        }
    };
    //exit this application
    RunAppShutter();
}

// ---------------------------------------------------------------------------
// From class CAknAppUi.
// handle menu command
// ---------------------------------------------------------------------------
//
void CDefaultAppAppUi::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
    case EAknSoftkeyExit:
        __ASSERT_ALWAYS(0,User::Leave(KErrGeneral));
        break;
    case EEikCmdExit:
    
        Exit();
        break;
    //no need for default. We may also have other commands that we do not handle here.
        }
    }


// ---------------------------------------------------------------------------
// imports and displays the icon of the client application.
// ---------------------------------------------------------------------------
//
void CDefaultAppAppUi::SetClientIconL(TUid aClietAppUid)
    {
    CEikStatusPane *statusPane=StatusPane();
    TUid contextPaneUid=TUid::Uid(EEikStatusPaneUidContext);
    if(statusPane && statusPane->PaneCapabilities(contextPaneUid).IsPresent())
        {
        CAknContextPane *contextPane=(CAknContextPane*)(statusPane->ControlL(contextPaneUid)); //we don't get the ownership
        MAknsSkinInstance* skinInstance = AknsUtils::SkinInstance(); //we don't get the ownership
        
        CFbsBitmap* iconBmp = NULL;
        CFbsBitmap* iconBmpMask = NULL;
    
        AknsUtils::CreateAppIconLC(skinInstance,aClietAppUid,EAknsAppIconTypeContext,
                                       iconBmp,iconBmpMask);
        CleanupStack::Pop(2);//the 2 icons
        
        contextPane->SetPicture(iconBmp,iconBmpMask);// ownership transfer for the 2 pictures
        }
    }

// ---------------------------------------------------------------------------
// Handles layout changes (e.g. from portrait to landscape)
// ---------------------------------------------------------------------------
//
void CDefaultAppAppUi::HandleResourceChangeL(TInt aType)
    {
     CAknAppUi::HandleResourceChangeL( aType );
     
     if(iView)
     iView->HandleResourceChange(aType);
     if( iDlg && iDialogExist )
     iDlg->HandleResourceChange(aType);
        
     if ( aType == KEikDynamicLayoutVariantSwitch )
         {
         if( iView )
         iView->SetRect(ClientRect());
         if( iDlg && iDialogExist )
         iDlg->SetRect(ClientRect());
         }
    }

// ---------------------------------------------------------------------------
// Handles Window Server events (we are interested in catching the end key event 
// and exiting the application
// ---------------------------------------------------------------------------
//
void CDefaultAppAppUi::HandleWsEventL(const TWsEvent & aEvent, CCoeControl * aDestination)
    {
     CAknAppUi::HandleWsEventL(aEvent,aDestination);
    }

// ---------------------------------------------------------------------------
// This functino is used to record the exist of the dialog; 
// and exiting the application
// ---------------------------------------------------------------------------
//
void CDefaultAppAppUi::SetDialogExist( TBool aExist )
    {
    iDialogExist = aExist;
    }
