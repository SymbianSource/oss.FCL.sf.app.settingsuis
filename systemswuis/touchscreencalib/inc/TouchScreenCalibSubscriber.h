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
* Description:  TouchScreenCalibSubscriber (Publish & Subscribe).
*
*/


#ifndef TOUCHSCREENCALIBSUBSCRIBER_H
#define TOUCHSCREENCALIBSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "MTouchScreenCalibPropertyResponder.h"

// CLASS DECLARATION
/**
*  CTouchScreenCalibSubscriber
*  
*  @lib   
*  @since 3.2
*/

class CTouchScreenCalibSubscriber : public CActive
{
public:
    /**
    * Two-phased constructor.
    */         
    static CTouchScreenCalibSubscriber* NewL( MTouchScreenCalibPropertyResponder& aTouchScreenCalibPropertyResponder, 
                                     const TUid& aCategory, 
                                     TUint aKey );

    /**
    * Destructor.
    */
     ~CTouchScreenCalibSubscriber();

     void Subscribe();

private:
     CTouchScreenCalibSubscriber( MTouchScreenCalibPropertyResponder& aTouchScreenCalibPropertyResponder, 
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
    CTouchScreenCalibSubscriber( const CTouchScreenCalibSubscriber& );           // Copy constructor
    CTouchScreenCalibSubscriber& operator=( const CTouchScreenCalibSubscriber& );// Assigment operator

private:

    MTouchScreenCalibPropertyResponder& iTouchScreenCalibPropertyResponder;
    RProperty                           iProperty;
    TUid                                iCategory;
    TUint                               iKey;
};

#endif // TOUCHSCREENCALIBSUBSCRIBER_H

// End of File
