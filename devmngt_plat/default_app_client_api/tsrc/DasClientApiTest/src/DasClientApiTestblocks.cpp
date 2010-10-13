/*
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
* Description:  This cpp file has the functions to test Das Client API.
*
*/


// [INCLUDE FILES]
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "DasClientApiTest.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDasClientApiTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CDasClientApiTest::Delete() 
    { 
    if( iLauncher )
    	{
        delete iLauncher;
        iLauncher = NULL;
    	}

    if( iClient )
    	{
        delete iClient;
        iClient = NULL;
    	}
    }

// -----------------------------------------------------------------------------
// CDasClientApiTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CDasClientApiTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CallClientNewLL", CDasClientApiTest::CallClientNewLL ),
        ENTRY( "CallClientNewLCL", CDasClientApiTest::CallClientNewLCL ),
        ENTRY( "CallServiceAvailableL", CDasClientApiTest::CallServiceAvailableL ),
        ENTRY( "CallChangeDefaultsLL", CDasClientApiTest::CallChangeDefaultsLL ),
        // [test cases entries]

        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CDasClientApiTest::CallNewlcL
// This function is used because we can not TRAP CServicesDB::NewLC
// -----------------------------------------------------------------------------
//
void CDasClientApiTest::CallNewlcL(  MAknServerAppExitObserver* aObserver )
{ 
   iClient = CDefaultAppClient::NewLC( aObserver );
   CleanupStack::Pop( iClient );
}

// -----------------------------------------------------------------------------
// CServicesDbApiTest::InitialClientL
// This function is used to create CDefaultAppClient object. This function is used
// to call CDefaultAppClient::NewL and CDefaultAppClient::NewLC functions. 
// -----------------------------------------------------------------------------
//
TInt CDasClientApiTest::InitialClientL( TInt aOption )
    {
    //launcher of the client;
    if( aOption == KFirstOption )
        {
        TRAPD( error, iClient = CDefaultAppClient::NewL( NULL ) );
        if( KErrNone != error )
            {
            iLog->Log( _L( "=>CDefaultAppClient::NewL leaves" ) );
            return error;
            }
        }
    else
        {
        TRAPD( error1, CallNewlcL( NULL ) );
        if( KErrNone != error1 )
            {
            iLog->Log( _L( "=>CDefaultAppClient::NewLC leaves" ) );
            return error1;
            }
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallClientNewLL
// Is used to test CDefaultAppClient::NewL
// -----------------------------------------------------------------------------
TInt CDasClientApiTest::CallClientNewLL( CStifItemParser& /*aItem*/ )
    {
    TInt result;
    
    TRAPD( error, result = InitialClientL( KFirstOption ) );   
    if( KErrNone != error )
        {
        iLog->Log( _L( "CDefaultAppClient object not created with NewL." ) ); 
        return error;
        }
    else
        {
        if( KErrNone != result )
            {
            iLog->Log( _L( "CDefaultAppClient object not created with NewL." ) ); 
            return result;
            }
        else
        	{
        	iLog->Log( _L( "CDefaultAppClient object created with NewL." ) ); 
        	}
        };
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallClientNewLCL
// Is used to test CDefaultAppClient::NewLC
// -----------------------------------------------------------------------------
TInt CDasClientApiTest::CallClientNewLCL( CStifItemParser& /*aItem*/ )
    {
    TInt result;
    TRAPD( error, result = InitialClientL( KSecondtOption ) );
    if( KErrNone != error )
        {
        return error;
        }
    else
        {
        if( KErrNone != result )
            {
            return result;
            }
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallServiceAvailableL
// Is used to test CDefaultAppClient::ServiceAvailableL
// -----------------------------------------------------------------------------
TInt CDasClientApiTest::CallServiceAvailableL( CStifItemParser& /*aItem*/ )
    {
    //Initial the iClient
    TInt result;
    TRAPD( error, result = InitialClientL( KFirstOption ) );
    if( KErrNone != error )
        {
        return error;
        }
    else
        {
        if( KErrNone != result )
            {
            return result;
            }
        };
    //call the function 

    TInt errcode;
    TBool returnvalue;
    TRAPD( callerror, returnvalue = CDefaultAppClient::ServiceAvailable( errcode ) );
    if( KErrNone != callerror )
        {
        iLog->Log(_L( "Call function ServiceAvailable failed.The function leaves,the leace code is %d"), callerror );
        return callerror;
        }
    else
        {
        if( returnvalue != KErrNone )
            {
            iLog->Log( _L( "The function returned sucessfully,But the service is not available. The error code is %d" ), errcode );
            return errcode;
            }
        else
            {
            iLog->Log( _L( "The function ServiceAvailable called success, and the service is available! " ) );
            }
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallChangeDefaultsLL
// Is used to test CDefaultAppClient::ChangeDefaultsL
// -----------------------------------------------------------------------------
TInt CDasClientApiTest::CallChangeDefaultsLL( CStifItemParser& /*aItem*/ )
    {
    //
    TInt result;
    TRAPD( error, result = InitialClientL( KFirstOption ) );
    if(KErrNone != error)
        {
        iLog->Log( _L( "Initial the iClient failed,the failed code is %d" ), error );
        return error;
        }
    else
        {
        if( KErrNone != result )
            {            
            return result;
            }
        };
    TRAPD( errorCall, iClient->ChangeDefaultsL() );
    if( errorCall != KErrNone )
        {
        iLog->Log( _L( "Function ChangeDefaultsL is called failed. The failed code is %d" ), errorCall );
        return errorCall;
        }
    else
        {
        iLog->Log( _L( "The Function is called successfully." ) );
        }
    return KErrNone;
    }

//
//  [End of File]
