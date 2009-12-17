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
* Description:  Implementation of the server class
*
*/



#include <eikstart.h>  //for RDebug

#include "das_server.h"
#include "das_service.h"
#include "das_app.h"

const TUid KDefaultAppServiceUid = { 0x10281B9D };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
//
CDefaultAppServer* CDefaultAppServer::NewL(CDefaultAppApplication* aApp)
    {
    CDefaultAppServer* self = new (ELeave) CDefaultAppServer(aApp);
    return self;
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CDefaultAppServer::CDefaultAppServer(CDefaultAppApplication* aApp) : iApp(aApp)
    {
    }

// ---------------------------------------------------------------------------
// From class CAknAppServer.
// Creates a service for the specified service Uid
// ---------------------------------------------------------------------------
//
CApaAppServiceBase* CDefaultAppServer::CreateServiceL(TUid aServiceType) const
    {
    RDebug::Print(_L("CDefaultAppServer::CreateServiceL"));
    if (aServiceType == KDefaultAppServiceUid)
        return CDefaultAppService::NewL(this);
    else
        return CAknAppServer::CreateServiceL(aServiceType);
    }
        

//  End of File
