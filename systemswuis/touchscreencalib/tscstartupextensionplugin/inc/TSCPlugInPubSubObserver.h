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


#ifndef TSCPLUGINPUBSUBOBSERVER_H
#define TSCPLUGINPUBSUBOBSERVER_H

//  INCLUDES
#include <e32property.h>
#include "MTSCPlugInPropertyResponder.h"
#include "tscstartupextensionplugin.h"



// CLASS DECLARATION
class CTSCStartupExtensionPlugIn;
class CTSCPlugInSubscriber;

class CTSCPlugInPubSubObserver : public CBase, public MTSCPlugInPropertyResponder
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        */
        CTSCPlugInPubSubObserver( CTSCStartupExtensionPlugIn* aTSCStartupExtensionPlugIn );
        
        /**
        * Two-phased constructor.
        */
        static CTSCPlugInPubSubObserver* NewL( CTSCStartupExtensionPlugIn* aTSCStartupExtensionPlugIn );
        
        /**
        * Destructor.
        */
        ~CTSCPlugInPubSubObserver();

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        CTSCPlugInPubSubObserver();

    protected: // From MTSCPlugInPropertyResponder

        void HandlePropertyChangedL( const TUid& aCategory, TUint aKey );

    private:    // Data
        //reference to application class
        CTSCStartupExtensionPlugIn*        iTSCStartupExtensionPlugIn; //uses

        RProperty iProperty;
        CTSCPlugInSubscriber*   iTSCSyncSubscriber;
    };

#endif      // TSCPLUGINPUBSUBOBSERVER_H   
            
// End of File
