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
* Description:  CTSCPlugInSubscriber implementation.
 *
*/


// INCLUDES
#include <e32svr.h>
#include "tscstartupextensionplugin.h"
#include "tscpluginsubscriber.h"
#include "tscstartupextensionplugindef.h"

// CONSTANTS

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CTSCPlugInSubscriber::NewL()
// ----------------------------------------------------------------------------
CTSCPlugInSubscriber* CTSCPlugInSubscriber::NewL( 
                            MTSCPlugInPropertyResponder& aTSCPlugInPropertyResponder,
                            const TUid& aCategory, 
                            TUint aKey )
    {
    CTSCPlugInSubscriber* self = new (ELeave) CTSCPlugInSubscriber( aTSCPlugInPropertyResponder,
                                                                aCategory, 
                                                                aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CTSCPlugInSubscriber::ConstructL()
// ----------------------------------------------------------------------------
void CTSCPlugInSubscriber::ConstructL()
    {
    CActiveScheduler::Add( this );
    iProperty.Attach( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CTSCPlugInSubscriber::Subscribe()
// ----------------------------------------------------------------------------
void CTSCPlugInSubscriber::Subscribe()
    {
    TRACES("CTSCPlugInSubscriber::Subscribe()");
    iProperty.Subscribe( iStatus );
    SetActive();
    TRACES("CTSCPlugInSubscriber::Subscribe(): End");
    }

// ----------------------------------------------------------------------------
// CTSCPlugInSubscriber::CTSCPlugInSubscriber()
// ----------------------------------------------------------------------------
CTSCPlugInSubscriber::CTSCPlugInSubscriber( MTSCPlugInPropertyResponder& aTSCPlugInPropertyResponder, 
                                        const TUid& aCategory, 
                                        TUint aKey ) :
    CActive( EPriorityStandard ),
    iTSCPlugInPropertyResponder( aTSCPlugInPropertyResponder ),
    iCategory( aCategory),
    iKey( aKey )
    {
    }

// ----------------------------------------------------------------------------
// CTSCPlugInSubscriber::RunL()
// ----------------------------------------------------------------------------
void CTSCPlugInSubscriber::RunL()
    {
    TRACES("CTSCPlugInSubscriber::RunL()");
    Subscribe();
    iTSCPlugInPropertyResponder.HandlePropertyChangedL( iCategory, iKey );
    TRACES("CTSCPlugInSubscriber::RunL(): End");
    }

// ----------------------------------------------------------------------------
// CTSCPlugInSubscriber::DoCancel()
// ----------------------------------------------------------------------------
void CTSCPlugInSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }

// ----------------------------------------------------------------------------
// CTSCPlugInSubscriber::RunError()
// ----------------------------------------------------------------------------
TInt CTSCPlugInSubscriber::RunError( TInt aError )
    {    
    return aError;
    }

// ----------------------------------------------------------------------------
// CTSCPlugInSubscriber::~CTSCPlugInSubscriber()
// ----------------------------------------------------------------------------
CTSCPlugInSubscriber::~CTSCPlugInSubscriber()
    {
    TRACES("CTSCPlugInSubscriber::~CTSCPlugInSubscriber()");
    Cancel();
    iProperty.Close();
    TRACES("CTSCPlugInSubscriber::~CTSCPlugInSubscriber(): End");
    }

// End of File



