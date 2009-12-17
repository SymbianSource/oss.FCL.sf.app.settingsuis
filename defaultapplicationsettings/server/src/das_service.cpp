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
* Description:  Implementation of the DefaultApp service
*
*/


//#include <eikstart.h> //for RDebug
#include "das_server.h"
#include "das_service.h"
#include "das_app.h"
#include "das_doc.h"
#include "das_appui.h"
#include "das_servmimeapps.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CDefaultAppService::CDefaultAppService(const CDefaultAppServer* aServer) : iDefaultAppServer(aServer)
    {
    iClientUid=TUid::Uid(0);
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CDefaultAppService::~CDefaultAppService()
    {
    delete iIdle;
    }
// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
//    
CDefaultAppService* CDefaultAppService::NewL(const CDefaultAppServer* aServer)
    {
    CDefaultAppService* self = new (ELeave) CDefaultAppService(aServer);
    return self;
    }

// ---------------------------------------------------------------------------
// From class CAknAppServiceBase.
// function called when a new message is received
// ---------------------------------------------------------------------------
//            
void CDefaultAppService::ServiceL(const RMessage2& aMessage)
    {
    switch (aMessage.Function())
        {
        case ESetDefaultAll : 
            HandleSetDefaultAllL(aMessage);
            break;        
        default:
            CAknAppServiceBase::ServiceL(aMessage);
        }
    }

// ---------------------------------------------------------------------------
// From class CAknAppServiceBase.
// function called to check client credentials. Used to retrieve client app UID
// ---------------------------------------------------------------------------
//
CPolicyServer::TCustomResult CDefaultAppService::SecurityCheckL(const RMessage2& aMsg, TInt& /*aAction*/, TSecurityInfo& /*aMissing*/)
    {
    iClientUid=aMsg.SecureId();
    return CPolicyServer::EPass;
    }        

// ---------------------------------------------------------------------------
// Sets default for all the Service & MIME pairs supported by the client
// ---------------------------------------------------------------------------
//
void CDefaultAppService::HandleSetDefaultAllL(const RMessage2& aMessage)
    {
    //Get flags
    //TInt serviceFlags=aMessage.Int0();
    iServiceFlags=aMessage.Int0();
    aMessage.Complete(KErrNone);
    
	if(!iIdle)
	    {
		iIdle=CIdle::NewL(CActive::EPriorityStandard);
	    TCallBack cb(&IdleWorker,this);
	    iIdle->Start(cb);
	    };
    }

// ---------------------------------------------------------------------------
// Function where most of the processing happens.
// We use it so that the service request from the client returns immediately
// ---------------------------------------------------------------------------
//
TInt CDefaultAppService::IdleWorker(TAny *aInstance)
	{
	CDefaultAppService *service=(CDefaultAppService*)aInstance;
	//construct the view
	if ( service->iDefaultAppServer && 
		 service->iDefaultAppServer->iApp &&
		 service->iDefaultAppServer->iApp->iDocument &&
		 service->iDefaultAppServer->iApp->iDocument->iDefaultAppUi)
	    {
	    service->iDefaultAppServer->iApp->iDocument->iDefaultAppUi->ConstructSrvMimeDlgL(service->iClientUid,service->iServiceFlags);
	    };
	return 0; //no more work to do
	}


   

