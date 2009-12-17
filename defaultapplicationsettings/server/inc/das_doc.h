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
* Description:  This class implements the application document.
*
*/



#ifndef C_DEFAULTAPPDOC_H
#define C_DEFAULTAPPDOC_H

#include <AknDoc.h>

class CDefaultAppAppUi;

/**
 *  This class implements the application document.
 *
 *  @since Series 60 v5.0
 */
class CDefaultAppDocument : public CAknDocument
    {
public:

    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CDefaultAppDocument* NewL(CEikApplication& aApp);
    
    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CDefaultAppDocument* NewLC(CEikApplication& aApp);

// from base class CAknDocument
    /**
     * From CAknDocument.
     * Function to create the AppUi.
     *
     * @since S60 v5.0
     * @return the created AppUi instance
     */
    CEikAppUi* CreateAppUiL();

private:

    /**
     * C++ default constructor.
     */
    CDefaultAppDocument(CEikApplication& aApp);
public:
    /**
     * Pointer to the AppUi
     * Not owned
     */
    CDefaultAppAppUi* iDefaultAppUi; //not owned
    };

#endif      // C_DEFAULTAPPDOC_H

// End of File