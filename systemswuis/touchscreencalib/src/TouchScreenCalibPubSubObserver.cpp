/*
* Copyright (c) 2007 - 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*           This class the handles the received indications from the Public and
*           Subscribe. 
*
*/


// SYSTEM INCLUDES
#include <startupdomainpskeys.h>
#include <ctsydomainpskeys.h>

// USER INCLUDES
#include "TouchScreenCalibPubSubObserver.h"
#include "TouchScreenCalibPubSubObserver.h"
#include "TouchScreenCalibSubscriber.h"
#include "TouchScreenCalibDefines.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CTouchScreenCalibPubSubObserver::CTouchScreenCalibPubSubObserver( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi )
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------
CTouchScreenCalibPubSubObserver::CTouchScreenCalibPubSubObserver( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi ) :
    iTouchScreenCalibAppUi( aTouchScreenCalibAppUi )
    {
    }

// ----------------------------------------------------
// CTouchScreenCalibPubSubObserver::ConstructL()
// ----------------------------------------------------
void CTouchScreenCalibPubSubObserver::ConstructL()
    {
    TRACES("CTouchScreenCalibPubSubObserver::ConstructL()");

    iGlobalSWStateSubscriber = CTouchScreenCalibSubscriber::NewL( *this, 
                                                     KPSUidStartup, 
                                                     KPSGlobalSystemState );
    iGlobalSWStateSubscriber->Subscribe();

    iTsyCallState = CTouchScreenCalibSubscriber::NewL( *this, 
                                                     KPSUidCtsyCallInformation, 
                                                     KCTsyCallState );
    iTsyCallState->Subscribe();
    
    TRACES("CTouchScreenCalibPubSubObserver::ConstructL(): End");
    }

// ----------------------------------------------------------------------------
// CTouchScreenCalibPubSubObserver::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void CTouchScreenCalibPubSubObserver::HandlePropertyChangedL( const TUid& aCategory, TUint aKey )
    {
    TRACES("CTouchScreenCalibPubSubObserver::HandlePropertyChangedL()");
    TRACES1("CTouchScreenCalibPubSubObserver::HandlePropertyChangedL(): aKey: %d",aKey );

    if (aCategory == KPSUidStartup && aKey == KPSGlobalSystemState)
        {
        TInt eventState;
        User::LeaveIfError( RProperty::Get ( KPSUidStartup, KPSGlobalSystemState, eventState ) );

        if( eventState == ESwStateCriticalPhaseOK ||
            eventState == ESwStateEmergencyCallsOnly ||
            eventState == ESwStateNormalRfOn ||
            eventState == ESwStateNormalRfOff ||
            eventState == ESwStateNormalBTSap )
            {
            TRACES("CTouchScreenCalibPubSubObserver::HandlePropertyChangedL(): Critical startup phase ready");
            iTouchScreenCalibAppUi->PrepareToExit();
            }
        }
    else if (aCategory == KPSUidCtsyCallInformation && aKey == KCTsyCallState)
        {
        TInt eventState;
        User::LeaveIfError( RProperty::Get ( KPSUidCtsyCallInformation, KCTsyCallState, eventState ) );

        if( eventState == EPSCTsyCallStateRinging ||
            eventState == EPSCTsyCallStateDisconnecting)
            {
            TRACES("CTouchScreenCalibPubSubObserver::HandlePropertyChangedL(): Call detected");
            iTouchScreenCalibAppUi->PrepareToExit();
            }
        }
    TRACES("CTouchScreenCalibPubSubObserver::HandlePropertyChangedL(): End");
    }

// ----------------------------------------------------
// CTouchScreenCalibPubSubObserver* CTouchScreenCalibPubSubObserver::NewL( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi )
// ----------------------------------------------------
CTouchScreenCalibPubSubObserver* CTouchScreenCalibPubSubObserver::NewL( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi )
    {
    TRACES("CTouchScreenCalibPubSubObserver::NewL()");
    CTouchScreenCalibPubSubObserver* self = new (ELeave) CTouchScreenCalibPubSubObserver( aTouchScreenCalibAppUi );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    TRACES("CTouchScreenCalibPubSubObserver::NewL(): End");
    return self;
    }

// ----------------------------------------------------
// CTouchScreenCalibPubSubObserver::~CTouchScreenCalibPubSubObserver()
// ----------------------------------------------------
CTouchScreenCalibPubSubObserver::~CTouchScreenCalibPubSubObserver()
    {
    TRACES("CTouchScreenCalibPubSubObserver::~CTouchScreenCalibPubSubObserver()");

    delete iGlobalSWStateSubscriber;
    delete iTsyCallState;
    iProperty.Close();

    TRACES("CTouchScreenCalibPubSubObserver::~CTouchScreenCalibPubSubObserver(): End");
    }

//  End of File
