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


#ifndef TSCPLUGINSUBSCRIBER_H
#define TSCPLUGINSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "MTSCPlugInPropertyResponder.h"

// CLASS DECLARATION
/**
*  CTSCPlugInSubscriber
*  
*  @lib   tscstartupextensionplugin
*  @since 3.2
*/

class CTSCPlugInSubscriber : public CActive
{
public:
    /**
    * Two-phased constructor.
    */         
    static CTSCPlugInSubscriber* NewL( MTSCPlugInPropertyResponder& aTSCPlugInPropertyResponder, 
                                     const TUid& aCategory, 
                                     TUint aKey );

    /**
    * Destructor.
    */
     ~CTSCPlugInSubscriber();

     void Subscribe();

private:
     CTSCPlugInSubscriber( MTSCPlugInPropertyResponder& aTSCPlugInPropertyResponder, 
                         const TUid& aCategory, TUint 
                         aKey );
     void ConstructL();

public: // from CActive
    /**
    *	@param none
    *	@return none
    */    
    void RunL();
    
    /**
    *	@param aError the error returned
    *	@return error
    */
    TInt RunError( TInt aError );

    /**
    *	@param none
    *	@return none
    */    
    void DoCancel();
	
private: // Methods not implemented
    CTSCPlugInSubscriber( const CTSCPlugInSubscriber& );           // Copy constructor
    CTSCPlugInSubscriber& operator=( const CTSCPlugInSubscriber& );// Assigment operator

private:

    MTSCPlugInPropertyResponder&    iTSCPlugInPropertyResponder;
    RProperty                       iProperty;
    TUid                            iCategory;
    TUint                           iKey;
};


#endif // TSCPLUGINSUBSCRIBER_H

// End of File
