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
* Description:  CTouchScreenCalibSubscriber implementation.
 *
*/


// INCLUDES
#include <e32svr.h>
#include "TouchScreenCalibAppUi.h"
#include "TouchScreenCalibSubscriber.h"
#include "TouchScreenCalibDefines.h"

// CONSTANTS

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CTouchScreenCalibSubscriber::NewL()
// ----------------------------------------------------------------------------
CTouchScreenCalibSubscriber* CTouchScreenCalibSubscriber::NewL( 
                            MTouchScreenCalibPropertyResponder& aTouchScreenCalibPropertyResponder,
                            const TUid& aCategory, 
                            TUint aKey )
    {
    CTouchScreenCalibSubscriber* self = new (ELeave) CTouchScreenCalibSubscriber( aTouchScreenCalibPropertyResponder,
                                                                aCategory, 
                                                                aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CTouchScreenCalibSubscriber::ConstructL()
// ----------------------------------------------------------------------------
void CTouchScreenCalibSubscriber::ConstructL()
    {
    CActiveScheduler::Add( this );
    iProperty.Attach( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CTouchScreenCalibSubscriber::Subscribe()
// ----------------------------------------------------------------------------
void CTouchScreenCalibSubscriber::Subscribe()
    {
    TRACES("CTouchScreenCalibSubscriber::Subscribe()");
    iProperty.Subscribe( iStatus );
    SetActive();
    TRACES("CTouchScreenCalibSubscriber::Subscribe(): End");
    }

// ----------------------------------------------------------------------------
// CTouchScreenCalibSubscriber::CTouchScreenCalibSubscriber()
// ----------------------------------------------------------------------------
CTouchScreenCalibSubscriber::CTouchScreenCalibSubscriber( MTouchScreenCalibPropertyResponder& aTouchScreenCalibPropertyResponder, 
                                        const TUid& aCategory, 
                                        TUint aKey ) :
    CActive( EPriorityStandard ),
    iTouchScreenCalibPropertyResponder( aTouchScreenCalibPropertyResponder ),
    iCategory( aCategory),
    iKey( aKey )
    {
    }

// ----------------------------------------------------------------------------
// CTouchScreenCalibSubscriber::RunL()
// ----------------------------------------------------------------------------
void CTouchScreenCalibSubscriber::RunL()
    {
    TRACES("CTouchScreenCalibSubscriber::RunL()");
    Subscribe();
    iTouchScreenCalibPropertyResponder.HandlePropertyChangedL( iCategory, iKey );
    TRACES("CTouchScreenCalibSubscriber::RunL(): End");
    }

// ----------------------------------------------------------------------------
// CTouchScreenCalibSubscriber::DoCancel()
// ----------------------------------------------------------------------------
void CTouchScreenCalibSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }

// ----------------------------------------------------------------------------
// CTouchScreenCalibSubscriber::RunError()
// ----------------------------------------------------------------------------
TInt CTouchScreenCalibSubscriber::RunError( TInt aError )
    {    
    return aError;
    }

// ----------------------------------------------------------------------------
// CTouchScreenCalibSubscriber::~CTouchScreenCalibSubscriber()
// ----------------------------------------------------------------------------
CTouchScreenCalibSubscriber::~CTouchScreenCalibSubscriber()
    {
    TRACES("CTouchScreenCalibSubscriber::~CTouchScreenCalibSubscriber()");
    Cancel();
    iProperty.Close();
    TRACES("CTouchScreenCalibSubscriber::~CTouchScreenCalibSubscriber(): End");
    }

// End of File



