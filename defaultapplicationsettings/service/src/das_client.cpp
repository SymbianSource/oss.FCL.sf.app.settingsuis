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
* Description:  This class implemets the Default App Client API
*
*/


#include <e32std.h>
#include <apgcli.h>


#include <das_client.h>
#include "das_service.h"

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaid.h>
#else
#include <apaid.h>
#include <apaidpartner.h> // new file introduced by xSymbian
#endif

const TUid KDefaultAppServiceUid = { 0x10281B9D };



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CDefaultAppClient::NewL()
// 
//
// ---------------------------------------------------------------------------
//
EXPORT_C CDefaultAppClient* CDefaultAppClient::NewL( MAknServerAppExitObserver* aObserver )
    {
    CDefaultAppClient* self = CDefaultAppClient::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CDefaultAppClient::NewLC()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C CDefaultAppClient* CDefaultAppClient::NewLC( MAknServerAppExitObserver* aObserver )
    {
    CDefaultAppClient* self = new ( ELeave ) CDefaultAppClient;
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    return self;
    }

// ---------------------------------------------------------------------------
// CDefaultAppClient::CDefaultAppClient()
// Default constructor
//
// ---------------------------------------------------------------------------
//
CDefaultAppClient::CDefaultAppClient()
    {
    iServerAppUid.iUid = 0;
    }

// ---------------------------------------------------------------------------
// CDefaultAppClient::ConstructL()
// 
//
// ---------------------------------------------------------------------------
//
void CDefaultAppClient::ConstructL( MAknServerAppExitObserver* aObserver )
    {
    iObserver=aObserver;
    iService = new (ELeave) RDefaultAppService;
    }

// ---------------------------------------------------------------------------
// CDefaultAppClient::~CDefaultAppClient
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C CDefaultAppClient::~CDefaultAppClient()
    {
    if(iMonitor)
        {
        iMonitor->Cancel();
        delete iMonitor;
        };
    if(iService)
        {
        iService->Close();
        delete iService;
        };
    
    
        
    // iObserver is not owned
    }


// ---------------------------------------------------------------------------
// Function to check if a server is present in the system. If the
// server is present, the subsequent functions should not fail due
// to server unavailability.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CDefaultAppClient::ServiceAvailable(TInt& aErrorCode)
    {
    TRAP(aErrorCode,GetServiceParamsL(NULL));
    if(aErrorCode != KErrNone) return EFalse;
    return ETrue;
    }

// ---------------------------------------------------------------------------
// Function to check the number of Service & MIME pairs the server would display if the 
// ChangeDefaultsL() would be called. The purpose of this function
// is to allow the client application not to display an entry for starting the server, in the 
// Options menu, in case the Server's list of Service & MIME pairs is empty.
// 
// Please note that the function may return a higher number than the actual number
// of pairs, because it does not check for MIMEs with System priority (that would not be 
// displayed).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CDefaultAppClient::PairsToDisplayL()
    {
    RApaLsSession ls;
    TInt i,j;
    TInt pairs=0;
    
    //Get own UID
    TUid clientAppUid = User::Identity(); 
    
    //connect
    User::LeaveIfError(ls.Connect());
    
    //we are connected, so some initialization
    CleanupClosePushL( ls );
    User::LeaveIfError(ls.GetAllApps());
    
    //get all services & MIMEs supported by our application
    CApaAppServiceInfoArray* servicesAndDataTypes=ls.GetAppServicesLC(clientAppUid);
    
    for(i=0; i<servicesAndDataTypes->Array().Count(); i++ )
        {
        const TApaAppServiceInfo currentServiceInfo = servicesAndDataTypes->Array()[i];
        for ( j = 0 ; j < currentServiceInfo.DataTypes().Count() ; j++ )
            {
            //check if there are more than one applications supporting this Service & MIME pair
            CApaAppServiceInfoArray* apps=ls.GetServiceImplementationsLC(currentServiceInfo.Uid(),currentServiceInfo.DataTypes()[j].iDataType);
            
            if(apps->Array().Count()>1)
                {
            	//more that one app handles this Service & MIME
            	pairs++;
                };
        
            CleanupStack::PopAndDestroy(apps);
            };
        };
      
    //clean
    CleanupStack::PopAndDestroy(servicesAndDataTypes);
    CleanupStack::PopAndDestroy();  // closes RApaLsSession        
    
    return pairs;
    }
    
// ---------------------------------------------------------------------------
// This function launches the server, as a chained application. The client application will not be
// available to the user until the server does not exit.
// When the server exits, the client application gains control again. The observer (if specified 
// during instantiation (NewL) is notified that the server has exited.
// ---------------------------------------------------------------------------
//
EXPORT_C void CDefaultAppClient::ChangeDefaultsL( TInt aFlags )
    {
    if(iServerAppUid.iUid == 0)
        GetServiceParamsL( &iServerAppUid );
    
    iService->ConnectChainedAppL(iServerAppUid);
    
    if(iObserver)
        iMonitor = CApaServerAppExitMonitor::NewL(*iService, *iObserver, CActive::EPriorityStandard);
    else
        aFlags|=EFlagNoObserver;
    
    iService->SetDefault(aFlags);
    }



// ---------------------------------------------------------------------------
// This function finds out the uid of the Default Application Server 
// (the application registered to handle the DefaultApp service)
// ---------------------------------------------------------------------------
//     
void CDefaultAppClient::GetServiceParamsL( TUid *aServerAppUid)
    {
    CApaAppServiceInfoArray* services = NULL;    
    RApaLsSession ls;
    TInt count = 0; //number of services found so far.
    
    User::LeaveIfError(ls.Connect());
    CleanupClosePushL( ls );
    
    User::LeaveIfError(ls.GetServerApps( KDefaultAppServiceUid ));

    services = ls.GetServiceImplementationsLC( KDefaultAppServiceUid );
        
    if( services )
        {
        count = services->Array().Count();
        // get the server's Uid
        if( aServerAppUid )
            aServerAppUid->iUid = services->Array()[0].Uid().iUid;
        };
    CleanupStack::PopAndDestroy( 2 );  // destroys services + closes RApaLsSession
            
    if ( count == 0) User::Leave(KErrNotFound);
    if ( count > 1) User::Leave(KErrNotSupported);
        
    // if we are here, it means that we have a single server handling the defaults
    }



   
