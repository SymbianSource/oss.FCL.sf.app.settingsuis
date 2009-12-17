/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declaration of CTSCStartupExtension class.
*
*/

#include "tscstartupextension.h"
#include "tscstartupextensiondef.h"
#include "TSCPubSubObserver.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CDiskSpaceReserve::NewL
//
// ---------------------------------------------------------------------------
//
CTSCStartupExtension * CTSCStartupExtension ::NewL()
    {
    TRACES("CTSCStartupExtension :: NewL ()");
    CTSCStartupExtension * self = new ( ELeave ) CTSCStartupExtension ;

    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
//
// ---------------------------------------------------------------------------
//
CTSCStartupExtension::~CTSCStartupExtension ()
    {
    TRACES("CTSCStartupExtension ::~CTSCStartupExtension ()");

    TRACES("CTSCStartupExtension ::~CTSCStartupExtension () End");
    }



// ---------------------------------------------------------------------------
// First phase constructor
//
// ---------------------------------------------------------------------------
//
CTSCStartupExtension::CTSCStartupExtension()
    :iRequestSent(EFalse)
    {
    }

// ---------------------------------------------------------------------------
// CTSCStartupExtension::Initialize
//
// ---------------------------------------------------------------------------
//
TInt CTSCStartupExtension::Initialize( CSsmCustomCommandEnv* /*aCmdEnv*/ )
    {
    TRACES("CTSCStartupExtension::Initialize()");
    iTSCPubSubObserver = CTSCPubSubObserver::NewL( this );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSCStartupExtension::Execute
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtension::Execute(
    const TDesC8& /*aParams*/,
    TRequestStatus& aStatus )
    {
    TRACES("CTSCStartupExtension::ExecuteL()");    
    aStatus = KRequestPending;
    iStatus = &aStatus;
    TRACES("CTSCStartupExtension::ExecuteL(): End");
    }


// ---------------------------------------------------------------------------
// CTSCStartupExtension::ExecuteCancel
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtension::ExecuteCancel()
    {
    // Nothing to do.
    }


// ---------------------------------------------------------------------------
// CTSCStartupExtension::Release
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtension::Release()
    {
    delete this;
    }


// ---------------------------------------------------------------------------
// CTSCStartupExtension::Close
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtension::Close()
    {
    TRACES("CTSCStartupExtension :: Close()");
    if (iTSCPubSubObserver)
        {
        delete iTSCPubSubObserver;
        iTSCPubSubObserver = NULL;
        }
    TRACES("CTSCStartupExtension :: Close() End");
    }

// ---------------------------------------------------------------------------
// CTSCStartupExtension::CalibrationDone
//
// ---------------------------------------------------------------------------
//
void CTSCStartupExtension::CalibrationDone()
    {
    TRACES("CTSCStartupExtension::CalibrationDone()");
    if (!iRequestSent)
        {
        TRACES("CTSCStartupExtension::CalibrationDone(): Send complete request");
        TInt errorCode = KErrNone;    
        User::RequestComplete( iStatus, errorCode );
        iRequestSent = ETrue;
        }
    TRACES("CTSCStartupExtension::CalibrationDone(): End");
    }    


