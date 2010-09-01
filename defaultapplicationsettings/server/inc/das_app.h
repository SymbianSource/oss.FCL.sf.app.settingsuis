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
* Description:  This class implements the application.
*
*/



#ifndef C_DEFAULTAPPAPPLICATION_H
#define C_DEFAULTAPPAPPLICATION_H

#include <aknapp.h>

class CDefaultAppDocument;
class CDefaultAppServer;

/** UID for the application; this should correspond to the uid defined in the mmp file */
const TUid KUidDefaultAppServer = { 0x10281B9C };

/**
 *  This class implements the application.
 *
 *  @since Series 60 v5.0
 */
class CDefaultAppApplication : public CAknApplication
    {
private: // Functions from base classes

    /**
     * From CApaApplication.
     * Returns application's UID (KUidDefaultAppServer).
     *
     * @since S60 v5.0
     * @return The value of KUidDefaultAppServer.
     */
    TUid AppDllUid() const;

    /**
     * From CApaApplication.
     * Creates CDefaultAppDocument document object.
     *
     * @since S60 v5.0
     * @return A pointer to the created document object.
     */
    CApaDocument* CreateDocumentL();
       
    /**
     * From CApaApplication.
     * Creates a new server object (CDefaultAppServer).
     *
     * @since S60 v5.0
     * @param aAppServer pointer to the newly created server object
     */
    void NewAppServerL(CApaAppServer*& aAppServer);
    
public:
    /**
     * C++ default constructor.
     */
    CDefaultAppApplication(void);
    
public: //data
    /**
     * Pointer to the documet class
     * Not owned.
     */
    CDefaultAppDocument* iDocument; //not owned
    
    /**
     * Pointer to the server class
     * Not owned.
     */
    CDefaultAppServer* iServer;     //not owned
    };

#endif      // C_DEFAULTAPPAPPLICATION_H

// End of File
