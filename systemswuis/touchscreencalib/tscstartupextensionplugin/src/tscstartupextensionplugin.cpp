/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CTSCStartupExtensionPlugIn class.
*
*/


#include "tscstartupextensionplugin.h"
#include "tscstartupextensionplugindef.h"
#include "TSCPlugInPubSubObserver.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
//
// ---------------------------------------------------------------------------
//
CTSCStartupExtensionPlugIn* CTSCStartupExtensionPlugIn::NewL(
    TAny* aConstructionParameters )
    {
    CTSCStartupExtensionPlugIn* self =
        new( ELeave ) CTSCStartupExtensionPlugIn( aConstructionParameters );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
//
// ---------------------------------------------------------------------------
//
CTSCStartupExtensionPlugIn::~CTSCStartupExtensionPlugIn()
    {
    TRACES("CTSCStartupExtensionPlugIn::~CTSCStartupExtensionPlugIn()");
    if (iTSCPlugInPubSubObserver)
        {
        delete iTSCPlugInPubSubObserver;
        }
    TRACES("CTSCStartupExtensionPlugIn::~CTSCStartupExtensionPlugIn()");
    }


// ---------------------------------------------------------------------------
// First phase constructor
//
// ---------------------------------------------------------------------------
//
CTSCStartupExtensionPlugIn::CTSCStartupExtensionPlugIn(
    TAny* aConstructionParameters )
  : CSystemStartupExtension( aConstructionParameters ),
    iRequestSent(EFalse)
    {
    }


// ---------------------------------------------------------------------------
// CTSCStartupExtensionPlugIn::ConstructL
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtensionPlugIn::ConstructL()
    {
    TRACES("CTSCStartupExtensionPlugIn::ConstructL()");
    iTSCPlugInPubSubObserver = CTSCPlugInPubSubObserver::NewL( this );
    TRACES("CTSCStartupExtensionPlugIn::ConstructL(): End");
    }

// ---------------------------------------------------------------------------
// CTSCStartupExtensionPlugIn::ExecuteL
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtensionPlugIn::ExecuteL(
    TRequestStatus& aStatus,
    const TDesC& /*aParams*/ )
    {
    TRACES("CTSCStartupExtensionPlugIn::ExecuteL()");    
    aStatus = KRequestPending;
    iStatus = &aStatus;
    TRACES("CTSCStartupExtensionPlugIn::ExecuteL(): End");
    }

// ---------------------------------------------------------------------------
// CTSCStartupExtensionPlugIn::Cancel
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtensionPlugIn::Cancel()
    {
    // Nothing to do.
    }

// ---------------------------------------------------------------------------
// CTSCStartupExtensionPlugIn::CalibrationDone
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtensionPlugIn::CalibrationDone()
    {
    TRACES("CTSCStartupExtensionPlugIn::CalibrationDone()");
    if (!iRequestSent)
        {
        TRACES("CTSCStartupExtensionPlugIn::CalibrationDone(): Send complete request");
        TInt errorCode = KErrNone;    
        User::RequestComplete( iStatus, errorCode );
        iRequestSent = ETrue;
        }
    TRACES("CTSCStartupExtensionPlugIn::CalibrationDone(): End");
    }    
