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

#ifndef TSCPUBSUBOBSERVER_H
#define TSCPUBSUBOBSERVER_H

//  INCLUDES
#include <e32property.h>
#include "MTSCPropertyResponder.h"
#include "tscstartupextension.h"



// CLASS DECLARATION
class CTSCStartupExtension;
class CTSCSubscriber;

class CTSCPubSubObserver : public CBase, public MTSCPropertyResponder
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        */
    	CTSCPubSubObserver( CTSCStartupExtension* aTSCStartupExtension );
        
        /**
        * Two-phased constructor.
        */
        static CTSCPubSubObserver* NewL( CTSCStartupExtension* aTSCStartupExtension );
        
        /**
        * Destructor.
        */
        ~CTSCPubSubObserver();

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        CTSCPubSubObserver();

    protected: // From MTSCPropertyResponder

        void HandlePropertyChangedL( const TUid& aCategory, TUint aKey );

    private:    // Data
        //reference to application class
        CTSCStartupExtension *        iTSCStartupExtension; //uses

        RProperty iProperty;
        CTSCSubscriber*   iTSCSyncSubscriber;
    };

#endif      // TSCPUBSUBOBSERVER_H   
            
// End of File
