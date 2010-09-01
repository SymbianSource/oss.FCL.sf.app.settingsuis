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


#ifndef TOUCHSCREENCALIBPUBSUBOBSERVER_H
#define TOUCHSCREENCALIBPUBSUBOBSERVER_H

//  INCLUDES
#include <e32property.h>
#include "MTouchScreenCalibPropertyResponder.h"
#include "TouchScreenCalibAppUi.h"

// CLASS DECLARATION
class CTouchScreenCalibAppUi;
class CTouchScreenCalibSubscriber;

class CTouchScreenCalibPubSubObserver : public CBase, public MTouchScreenCalibPropertyResponder
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        */
        CTouchScreenCalibPubSubObserver( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi );
        
        /**
        * Two-phased constructor.
        */
        static CTouchScreenCalibPubSubObserver* NewL( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi );
        
        /**
        * Destructor.
        */
        ~CTouchScreenCalibPubSubObserver();

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        CTouchScreenCalibPubSubObserver();

    protected: // From MTouchScreenCalibPropertyResponder

        void HandlePropertyChangedL( const TUid& aCategory, TUint aKey );

    private:    // Data
        //reference to application class
        CTouchScreenCalibAppUi*         iTouchScreenCalibAppUi; //uses
        RProperty                       iProperty;
        CTouchScreenCalibSubscriber*    iGlobalSWStateSubscriber;
        CTouchScreenCalibSubscriber*    iTsyCallState;
    };

#endif      // TOUCHSCREENCALIBPUBSUBOBSERVER_H   
            
// End of File
