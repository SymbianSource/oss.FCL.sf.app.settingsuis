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
* Description:  The Default Application server class
*
*/



#ifndef C_DEFAULTAPPSERVER_H
#define C_DEFAULTAPPSERVER_H

#include <AknServerApp.h>

class CDefaultAppApplication;

/**
 *  This is the main implemetation of DefaultApp server.
 *
 *  @since Series 60 v5.0
 */
class CDefaultAppServer: public CAknAppServer
    {
public:  // Constructors and destructor

    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CDefaultAppServer* NewL(CDefaultAppApplication *aApp);
    
// from base class CAknAppServer 

    /**
     * From CAknAppServer
     * Creates a service for the specified service Uid
     *
     * @since S60 v5.0
     * @param aServiceType the service type Uid
     * @return the created service 
     */   
    CApaAppServiceBase* CreateServiceL(TUid aServiceType) const;

private:

    /**
     * C++ default constructor.
     */
    CDefaultAppServer(CDefaultAppApplication *aApp);

public:
    /**
     * pointer to the application. After the client connects, the server side needs to announce the AppUi class, to
     * continue the construction
     * Not owned.
     */
    CDefaultAppApplication* iApp; //Not owned        
    };

#endif      // C_DEFAULTAPPSERVER_H

// End of File
