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
* Description:  Implementation table
*
*/


// System includes
#include <e32std.h>
#include <implementationproxy.h>

// User includes
#include "gs_das.h"
#include <das_client.h>

// Constants
const TImplementationProxy KDASGSPluginImplementationTable[] = 
    {
    // Uid for plugin implementation:
    IMPLEMENTATION_PROXY_ENTRY( 0x10281BA1,    CDefaultAppGSPlugin::NewL )
    };


// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
    {
    TInt error;
    if( !CDefaultAppClient::ServiceAvailable(error) )
        {
        //there was an error, service not found.
        aTableCount=0;
        }
    else //service was found
        aTableCount = sizeof( KDASGSPluginImplementationTable ) 
            / sizeof( TImplementationProxy );
            
    return KDASGSPluginImplementationTable;
    }

// End of File
