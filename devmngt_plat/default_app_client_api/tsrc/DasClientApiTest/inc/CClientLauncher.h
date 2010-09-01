/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CCClientLauncher declaration.
*
*/

#ifndef CCLIENTLAUNCHER_H
#define CCLIENTLAUNCHER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <aknserverapp.h> 
#include <aknappui.h> 

// CLASS DECLARATION

/**
 *  CCClientLauncher
 * 
 */
class CCClientLauncher : public CAknAppUi,public MAknServerAppExitObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CCClientLauncher();

    /**
     * Two-phased constructor.
     */
    static CCClientLauncher* NewL();
    
    /**
     * Two-phased constructor.
     */
    static CCClientLauncher* NewLC();
    
public:
    //function from CAknAppUi
    void  ProcessCommandL (TInt aCommand);
    
private:
    //function from MAknServerAppExitObserver
    //handle the exit of the server.
    void HandleServerAppExit(TInt aReason);
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CCClientLauncher();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    };

#endif // CCLIENTLAUNCHER_H
