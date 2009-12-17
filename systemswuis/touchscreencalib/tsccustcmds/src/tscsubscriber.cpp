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
* Description:  TSC Subscriber (Publish & Subscribe).
*
*/

// INCLUDES
#include <e32svr.h>
#include "tscstartupextension.h"
#include "tscsubscriber.h"
#include "tscstartupextensiondef.h"

// CONSTANTS

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CTSCSubscriber::NewL()
// ----------------------------------------------------------------------------
CTSCSubscriber* CTSCSubscriber::NewL( 
                            MTSCPropertyResponder& aTSCPropertyResponder,
                            const TUid& aCategory, 
                            TUint aKey )
    {
    CTSCSubscriber* self = new (ELeave) CTSCSubscriber( aTSCPropertyResponder,
                                                        aCategory, 
                                                        aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CTSCSubscriber::ConstructL()
// ----------------------------------------------------------------------------
void CTSCSubscriber::ConstructL()
    {
    CActiveScheduler::Add( this );
    iProperty.Attach( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CTSCSubscriber::Subscribe()
// ----------------------------------------------------------------------------
void CTSCSubscriber::Subscribe()
    {
    TRACES("CTSCSubscriber::Subscribe()");
    iProperty.Subscribe( iStatus );
    SetActive();
    TRACES("CTSCSubscriber::Subscribe(): End");
    }

// ----------------------------------------------------------------------------
// CTSCSubscriber::CTSCSubscriber()
// ----------------------------------------------------------------------------
CTSCSubscriber::CTSCSubscriber( MTSCPropertyResponder& aTSCPropertyResponder, 
                                        const TUid& aCategory, 
                                        TUint aKey ) :
    CActive( EPriorityStandard ),
    iTSCPropertyResponder( aTSCPropertyResponder ),
    iCategory( aCategory),
    iKey( aKey )
    {
    }

// ----------------------------------------------------------------------------
// CTSCSubscriber::RunL()
// ----------------------------------------------------------------------------
void CTSCSubscriber::RunL()
    {
    TRACES("CTSCSubscriber::RunL()");
    Subscribe();
    iTSCPropertyResponder.HandlePropertyChangedL( iCategory, iKey );
    TRACES("CTSCSubscriber::RunL(): End");
    }

// ----------------------------------------------------------------------------
// CTSCSubscriber::DoCancel()
// ----------------------------------------------------------------------------
void CTSCSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }

// ----------------------------------------------------------------------------
// CTSCSubscriber::RunError()
// ----------------------------------------------------------------------------
TInt CTSCSubscriber::RunError( TInt aError )
    {    
    return aError;
    }

// ----------------------------------------------------------------------------
// CTSCSubscriber::~CTSCSubscriber()
// ----------------------------------------------------------------------------
CTSCSubscriber::~CTSCSubscriber()
    {
    TRACES("CTSCSubscriber::~CTSCSubscriber()");
    Cancel();
    iProperty.Close();
    TRACES("CTSCSubscriber::~CTSCSubscriber(): End");
    }

// End of File



