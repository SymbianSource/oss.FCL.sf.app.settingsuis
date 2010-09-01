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
* Description:  This class defines the service used to change the default app.
*
*/


#include "das_service.h"


// ======== MEMBER FUNCTIONS ========


const TUid KDefaultAppServiceUid = { 0x10281B9D };

// ---------------------------------------------------------------------------
// Sends SetDefault message to the server
// ---------------------------------------------------------------------------
//        
TInt RDefaultAppService::SetDefault( TInt aFlags )
    {
    return SendReceive( ESetDefaultAll, TIpcArgs( aFlags ) );
    }
        
// ---------------------------------------------------------------------------
// From class RApaAppServiceBase.
// Returns the uid of the service
// ---------------------------------------------------------------------------
//
TUid RDefaultAppService::ServiceUid() const
    {
    return KDefaultAppServiceUid;
    }        
