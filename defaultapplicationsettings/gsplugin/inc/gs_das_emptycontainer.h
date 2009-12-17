/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implements a empty container for gsplugin
*
*/


#ifndef CEMPTYCONTAINER_H
#define CEMPTYCONTAINER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

// CLASS DECLARATION

/**
 *  CCEmptyContainer
 * 
 */
class CCEmptyContainer : public CCoeControl
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CCEmptyContainer();

    /**
     * Two-phased constructor.
     */
    static CCEmptyContainer* NewL ( const TRect& aRect, const CCoeControl* aParent );

    /**
     * Two-phased constructor.
     */
    static CCEmptyContainer* NewLC ( const TRect& aRect, const CCoeControl* aParent );
    
public:
    // from base class CCoeControl
       TInt CountComponentControls() const;
       
       CCoeControl* ComponentControl( TInt aIndex ) const;

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CCEmptyContainer();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL ( const TRect& aRect, const CCoeControl* aParent );

    };

#endif // CEMPTYCONTAINER_H
