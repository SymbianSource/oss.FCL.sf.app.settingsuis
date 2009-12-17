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
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <e32cmn.h>
#include <touchscprivatepskeys.h>
#else //RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <StartupAppInternalPSKeys.h>
#endif //RD_STARTUP_ANIMATION_CUSTOMIZATION

// USER INCLUDES
#include "TSCPlugInPubSubObserver.h"
#include "tscpluginsubscriber.h"
#include "tscstartupextensionplugindef.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// TSCPlugInPubSubObserver::TSCPlugInPubSubObserver( CTSCStartupExtensionPlugIn* aTSCStartupExtensionPlugIn )
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------
CTSCPlugInPubSubObserver::CTSCPlugInPubSubObserver( CTSCStartupExtensionPlugIn* aTSCStartupExtensionPlugIn ) :
    iTSCStartupExtensionPlugIn( aTSCStartupExtensionPlugIn )
    {
    }

// ----------------------------------------------------
// CTSCPlugInPubSubObserver::ConstructL()
// ----------------------------------------------------
void CTSCPlugInPubSubObserver::ConstructL()
    {
    TRACES("CTSCPlugInPubSubObserver::ConstructL()");

    iTSCSyncSubscriber = CTSCPlugInSubscriber::NewL( *this, 
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
        iTSCStartupExtensionPlugIn->CalibrationDone();
        }

    TRACES("CTSCPlugInPubSubObserver::ConstructL(): End");
    }

// ----------------------------------------------------------------------------
// CTSCPlugInPubSubObserver::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void CTSCPlugInPubSubObserver::HandlePropertyChangedL( const TUid& /*aCategory*/, TUint aKey )
    {
    TRACES("CTSCPlugInPubSubObserver::HandlePropertyChangedL()");
    TRACES1("CTSCPlugInPubSubObserver::HandlePropertyChangedL(): aKey: %d",aKey );

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
            iTSCStartupExtensionPlugIn->CalibrationDone();
            }
        }

    TRACES("CTSCPlugInPubSubObserver::HandlePropertyChangedL(): End");
    }

// ----------------------------------------------------
// CTSCPlugInPubSubObserver::NewL( CTSCStartupExtensionPlugIn* aTSCStartupExtensionPlugIn )
// ----------------------------------------------------
CTSCPlugInPubSubObserver* CTSCPlugInPubSubObserver::NewL( CTSCStartupExtensionPlugIn* aTSCStartupExtensionPlugIn )
    {
    TRACES("CTSCPlugInPubSubObserver::NewL()");
    CTSCPlugInPubSubObserver* self = new (ELeave) CTSCPlugInPubSubObserver( aTSCStartupExtensionPlugIn );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    TRACES("CTSCPlugInPubSubObserver::NewL(): End");
    return self;
    }

// ----------------------------------------------------
// CTSCPlugInPubSubObserver::~CTSCPlugInPubSubObserver()
// ----------------------------------------------------
CTSCPlugInPubSubObserver::~CTSCPlugInPubSubObserver()
    {
    TRACES("CTSCPlugInPubSubObserver::~CTSCPlugInPubSubObserver()");

    delete iTSCSyncSubscriber;
    iProperty.Close();

    TRACES("CTSCPlugInPubSubObserver::~CTSCPlugInPubSubObserver(): End");
    }

//  End of File
