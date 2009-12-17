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
* Description:  
*           This class the handles the received indications from the Public and
*           Subscribe.
*
*/

// SYSTEM INCLUDES
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <e32cmn.h>
#include <touchscprivatepskeys.h>
#else //RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <StartupAppInternalPSKeys.h>
#endif //RD_STARTUP_ANIMATION_CUSTOMIZATION

// USER INCLUDES
#include "TSCPubSubObserver.h"
#include "tscsubscriber.h"
#include "tscstartupextensiondef.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CTSCPubSubObserver::CTSCPubSubObserver( CTSCStartupExtension* aTSCStartupExtension )
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------
CTSCPubSubObserver::CTSCPubSubObserver( CTSCStartupExtension* aTSCStartupExtension ) :
    iTSCStartupExtension( aTSCStartupExtension )
    {
    }

// ----------------------------------------------------
// CTSCPubSubObserver::ConstructL()
// ----------------------------------------------------
void CTSCPubSubObserver::ConstructL()
    {
    TRACES("CTSCPubSubObserver::ConstructL()");

    iTSCSyncSubscriber = CTSCSubscriber::NewL( *this, 
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
                                                     KPSUidTouchScreenCalibration,
#else //RD_STARTUP_ANIMATION_CUSTOMIZATION
                                                     KPSUidStartup,
#endif //RD_STARTUP_ANIMATION_CUSTOMIZATION                                                     
                                                     KPSTouchScreenCalibration );
    iTSCSyncSubscriber->Subscribe();

    TInt touchScreenCalibrationState;
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    RProperty::Get( KPSUidTouchScreenCalibration, KPSTouchScreenCalibration, touchScreenCalibrationState );
#else //RD_STARTUP_ANIMATION_CUSTOMIZATION
    RProperty::Get( KPSUidStartup, KPSTouchScreenCalibration, touchScreenCalibrationState );
#endif //RD_STARTUP_ANIMATION_CUSTOMIZATION
    if ( touchScreenCalibrationState == ETouchScreenCalibrationOk )
        {
        iTSCStartupExtension->CalibrationDone();
        }

    TRACES("CTSCPubSubObserver::ConstructL(): End");
    }

// ----------------------------------------------------------------------------
// CTSCPubSubObserver::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void CTSCPubSubObserver::HandlePropertyChangedL( const TUid& /*aCategory*/, TUint aKey )
    {
    TRACES("CTSCPubSubObserver::HandlePropertyChangedL()");
    TRACES1("CTSCPubSubObserver::HandlePropertyChangedL(): aKey: %d",aKey );

    if (aKey == KPSTouchScreenCalibration)
        {
        TInt state;
    
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        RProperty::Get( KPSUidTouchScreenCalibration, KPSTouchScreenCalibration, state );
#else //RD_STARTUP_ANIMATION_CUSTOMIZATION
        RProperty::Get( KPSUidStartup, KPSTouchScreenCalibration, state );
#endif //RD_STARTUP_ANIMATION_CUSTOMIZATION

        if ( state == ETouchScreenCalibrationOk )
            {
            iTSCStartupExtension->CalibrationDone();
            }
        }

    TRACES("CTSCPubSubObserver::HandlePropertyChangedL(): End");
    }

// ----------------------------------------------------
// CTSCPubSubObserver::NewL( CTSCStartupExtension* aTSCStartupExtension )
// ----------------------------------------------------
CTSCPubSubObserver* CTSCPubSubObserver::NewL( CTSCStartupExtension* aTSCStartupExtension )
    {
    TRACES("CTSCPubSubObserver::NewL()");
    CTSCPubSubObserver* self = new (ELeave) CTSCPubSubObserver( aTSCStartupExtension );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    TRACES("CTSCPubSubObserver::NewL(): End");
    return self;
    }

// ----------------------------------------------------
// CTSCPubSubObserver::~CTSCPubSubObserver()
// ----------------------------------------------------
CTSCPubSubObserver::~CTSCPubSubObserver()
    {
    TRACES("CTSCPubSubObserver::~CTSCPubSubObserver()");

    delete iTSCSyncSubscriber;
    iProperty.Close();

    TRACES("CTSCPubSubObserver::~CTSCPubSubObserver(): End");
    }

//  End of File
