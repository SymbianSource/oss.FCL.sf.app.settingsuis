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
* Description:  This is the main application implementation of default app server.
*
*/


#include "das_app.h"
#include "das_doc.h"
#include "das_server.h"

// ---------------------------------------------------------------------------
// Return the UID for the CSoundApplication application
// ---------------------------------------------------------------------------
//
TUid CDefaultAppApplication::AppDllUid() const
    {
    return KUidDefaultAppServer;
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CDefaultAppApplication::CDefaultAppApplication(void) : iDocument(NULL), iServer(NULL)
    {}
    
// ---------------------------------------------------------------------------
// Create an CDefaultAppDocument document, and return a pointer to it
// ---------------------------------------------------------------------------
//
CApaDocument* CDefaultAppApplication::CreateDocumentL()
    {
    iDocument = CDefaultAppDocument::NewL(*this);
    return (static_cast<CApaDocument*>(iDocument));
    }

// ---------------------------------------------------------------------------
// Simple function to return a server object
// ---------------------------------------------------------------------------
//
void CDefaultAppApplication::NewAppServerL(CApaAppServer*& aAppServer)
    {
    aAppServer = iServer = CDefaultAppServer::NewL(this);
    }

// End of File
