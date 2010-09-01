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
* Description:  This cpp file has the functions to test Services DB API.
*
*/



// [INCLUDE FILES]
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "ServicesDbApiTest.h"
#include <servicesdbapitest.rsg>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CServicesDbApiTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CServicesDbApiTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CServicesDbApiTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        ENTRY( "InitializeSeviceDB",         CServicesDbApiTest::InitializeSeviceDB ),
        ENTRY( "InitializeonStackSeviceDB",  CServicesDbApiTest::InitializeonStackSeviceDB ),
        ENTRY( "CountSeviceDB",              CServicesDbApiTest::CountSeviceDB),
        ENTRY( "ServiceUidLSeviceDB",        CServicesDbApiTest::ServiceUidLSeviceDB),
        ENTRY( "ServiceStringLCSeviceDB",    CServicesDbApiTest::ServiceStringLCSeviceDB),
        ENTRY( "ServiceStringLCUIDSeviceDB", CServicesDbApiTest::ServiceStringLCuidSeviceDB),
        ENTRY( "ServiceNameLCSeviceDB",      CServicesDbApiTest::ServiceNameLCSeviceDB),
        ENTRY( "ServiceNameLCuidSeviceDB",   CServicesDbApiTest::ServiceNameLCuidSeviceDB),
          };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


// -----------------------------------------------------------------------------
// CServicesDbApiTest::CreateSeviceDBL
// This function is used to create CServicesDB object. This function is used
// to call CServicesDB::NewL and CServicesDB::NewLC functions. 
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::CreateSeviceDBL( TInt aOption )
{
      
     // Get CCoeEnv instance
    CEikonEnv* eikEnv = CEikonEnv::Static();
    // Initialize loader
    //open our resource file
    RConeResourceLoader resources(*eikEnv);
    
    TParse parse;
    parse.Set(_L("c:ServicesDbApiTest.rsc"), &KDC_RESOURCE_FILES_DIR, NULL ); 
     
    TFileName fileName( parse.FullName() );
    iLog->Log(_L("Opened Resource file named ::  "));   iLog->Log(fileName);
         
    TRAPD( error, resources.OpenL(fileName) ); //Open resource file
    if ( KErrNone != error )
    {
        iLog->Log(_L("Error in opening resource file. ERROR = %d "),error); //return( error );
        //Check on Z drive if not found on C drive
        parse.Set(_L("z:ServicesDbApiTest.rsc"), &KDC_RESOURCE_FILES_DIR, NULL ); 
        TFileName fileName1( parse.FullName() );
        iLog->Log(_L("Opening Resource file named ::  "));   iLog->Log(fileName1);
        
        TRAPD( error1 , resources.OpenL(fileName1) );
        if ( KErrNone != error1  )
        {
            iLog->Log(_L("Error in opening resource file. ERROR = %d "),error1); 
            return( error1 ); 
        }
    }  
  
    TResourceReader reader;
    eikEnv->CreateResourceReaderLC( reader, R_DA_SERVICE_MIME );
    
    if ( KFirstOption == aOption )
    {
        TRAPD( errorinNewL, iServicesDB=CServicesDB::NewL(&reader) );
        if ( KErrNone != errorinNewL )
        {
            iLog->Log(_L("=>CServicesDB::NewL leaves"));
        }
        CleanupStack::PopAndDestroy(); //reader's resource
        resources.Close();        
        return ( errorinNewL );
    }  
    else 
    {
        TRAPD( errorinNewLC, CallNewlcL(reader) );
        if( KErrNone != errorinNewLC )
        { 
            iLog->Log(_L("=>CServicesDB::NewLC leaves"));
        }
        CleanupStack::PopAndDestroy();
        resources.Close();
        return ( errorinNewLC );
    }
}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallNewlcL
// This function is used because we can not TRAP CServicesDB::NewLC
// -----------------------------------------------------------------------------
//
void CServicesDbApiTest::CallNewlcL(  TResourceReader aResReader )
{ 
   iServicesDB = CServicesDB::NewLC( &aResReader );
   CleanupStack::Pop();
}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::InitializeSeviceDB
// Is used to test CServicesDB::NewL 
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::InitializeSeviceDB( )
{
    TInt result;
    TRAPD( error , result = CreateSeviceDBL( KFirstOption ) );
    if ( KErrNone != error )
    {  iLog->Log(_L("Services DB object not created with NewL.")); return ( error );  }
    else
    {  
       if ( KErrNone != result )
       {
          iLog->Log(_L("Services DB object not created with NewL.")); return ( result );
       }
       else
       {
           iLog->Log(_L("Services DB object created with NewL."));       
       }
    }
    return ( result );
}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::InitializeonStackSeviceDB
// Is used to test CServicesDB::NewLC
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::InitializeonStackSeviceDB( )
{
    TInt result;
    TRAPD( error, result = CreateSeviceDBL( KSecondOption ) );
    if ( KErrNone != error )
    {  iLog->Log(_L("Services DB object not created with NewLC.")); return ( error );  }
    else
    {  
        if ( KErrNone != result )
        {
           iLog->Log(_L("Services DB object not created with NewLC.")); return ( result );
        }
        else
        {
            iLog->Log(_L("Services DB object created with NewLC."));       
        } 
    }
    return ( result );
}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::CountSeviceDB
// Testing - IMPORT_C TInt Count() const;
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::CountSeviceDB()
{
    TInt result = InitializeSeviceDB();
    if ( KErrNone != result )
        {
            return ( result );
        }
    TInt nrServices=iServicesDB->Count();
    iLog->Log(_L("Number of Services listed in resource file = %d"),nrServices);
    if ( KNoOfServices != nrServices )
    { 
        iLog->Log(_L("The number of services returned by CServicesDB::COUNT is incorrect"));
        return ( KErrGeneral );
    }
    return ( KErrNone );
}

// -----------------------------------------------------------------------------
// CServicesDbApiTest::ServiceUidLSeviceDB
// Testing - IMPORT_C TUid ServiceUidL(TInt aIndex) const;
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::ServiceUidLSeviceDB( CStifItemParser& aItem )
{
    TInt result = InitializeSeviceDB();
    if ( KErrNone != result )
        {
            return ( result );
        }
    TUid serviceUid;
    TInt index;
     
    if( aItem.GetNextInt(index) != KErrNone )
    { 
        iLog->Log(_L("NO Index number provided, index is required parameter for testing ServiceUid "));
        return ( KErrGeneral );
    }

    TRAPD( error, serviceUid = iServicesDB->ServiceUidL(index) );
    if ( KErrNone != error )
    {
       iLog->Log(_L("Incorrect Index. There is no service matching the Index Number."));
       iLog->Log(_L("=> ServiceUidL leaves when index is incorrect."));
       return (error);
    }
        
    iLog->Log(_L("UID of the Service[%d]=%d"),index,serviceUid.iUid);
    
    switch (index)
        {
        case KFirstServiceIndex: if (serviceUid.iUid == 0x101) 
                {  iLog->Log(_L("Service UID is correct"));   }
                else 
                {  iLog->Log(_L("Service UID is incorrect")); return ( KErrGeneral );  }
                break;
        case KSecondServiceIndex: if (serviceUid.iUid == 0x102) 
                {  iLog->Log(_L("Service UID is correct"));   }
                else 
                {  iLog->Log(_L("Service UID is incorrect")); return ( KErrGeneral );  }
                break;
        case KThirdServiceIndex: if (serviceUid.iUid == 0x103)
                {  iLog->Log(_L("Service UID is correct"));   }
                else 
                {  iLog->Log(_L("Service UID is incorrect")); return ( KErrGeneral );  }
                break;    
        default : break;
        };

    return ( KErrNone );
}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::ServiceStringLCSeviceDB
// Testing - IMPORT_C HBufC* ServiceStringLC(TInt aIndex, const TDes& aParam) const;
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::ServiceStringLCSeviceDB( CStifItemParser& aItem )
{    
    TInt result = InitializeSeviceDB();
    if ( KErrNone != result )
    {
        return ( result );
    }
    TInt index;
    TBuf<KBufSize> format(KEmptyString);
    
    if( aItem.GetNextInt(index) != KErrNone )
    { 
        iLog->Log(_L("NO Index number provided, index is required parameter for testing ServiceUid "));
        return ( KErrGeneral );
    }
    TInt res;
    TRAPD( error, res = CallServiceStringLCSeviceDBL(index,format ));
    if ( KErrNone != error )
    {
        iLog->Log(_L("Incorrect Index. There is no service matching the Index Number."));
        iLog->Log(_L("=> ServiceStringLC leaves when index is incorrect."));
        return (error);
    }
     
    return ( res ); 
}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallServiceStringLCSeviceDBL
// This function is used because we can not trap ServiceStringLC
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::CallServiceStringLCSeviceDBL(TInt aIndex,const TDes& aParam )
{
    HBufC *string;
    string=iServicesDB->ServiceStringLC(aIndex,aParam);
    iLog->Log(_L("LOCALIZED name of the Service[%d] = "),aIndex);
    iLog->Log(*string);
    
    switch (aIndex)
        {
        case KFirstServiceIndex: if(!(string->Des().Compare(_L("Open Localized"))))
                {  iLog->Log(_L("LOCALIZED name is correct"));   }
                else 
                {  iLog->Log(_L("LOCALIZED name is incorrect"));
                   CleanupStack::PopAndDestroy(string); return ( KErrGeneral );  }
                break;
        case KSecondServiceIndex: if(!(string->Des().Compare(_L("Upload Localized"))))
                {  iLog->Log(_L("LOCALIZED name is correct"));   }
                else 
                {  iLog->Log(_L("LOCALIZED name is incorrect")); 
                   CleanupStack::PopAndDestroy(string); return ( KErrGeneral );  }
                break;
        case KThirdServiceIndex: if(!(string->Des().Compare(_L("Print Localized %U"))))
                {  iLog->Log(_L("LOCALIZED name is correct"));   }
                else 
                {  iLog->Log(_L("LOCALIZED name is incorrect"));  
                   CleanupStack::PopAndDestroy(string);return ( KErrGeneral );  }
                break;    
        default : break;
        }; 
    
    CleanupStack::PopAndDestroy(string);    
    return ( KErrNone );

}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::ServiceStringLCuidSeviceDB
// Testing - IMPORT_C HBufC* ServiceStringLC(TUid aServiceUid, const TDes& aParam) const;
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::ServiceStringLCuidSeviceDB( CStifItemParser& aItem )
{
    TInt result = InitializeSeviceDB();
    if ( KErrNone != result )
    {
        return ( result );
    }
    TBuf<KBufSize> format(KEmptyString);
    TInt uid;
    
    if( aItem.GetNextInt(uid) != KErrNone )
    { 
        iLog->Log(_L("NO UID number provided, UID is required parameter for testing ServiceStringLC "));
        return ( KErrGeneral );
    }   
    TUid serviceUID= TUid::Uid(uid);
    iLog->Log(_L("serviceUID = %d"),serviceUID.iUid);
    TInt res ;
    TRAPD(error, res = CallServiceStringLCuidSeviceDBL(serviceUID,format));
    if ( KErrNone != error )
    {
        iLog->Log(_L("=> ServiceStringLC leaves. Error code = %d"),error);
        return (error);
    }
    return ( res ); 

}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallServiceStringLCuidSeviceDBL
// This function is used because we can not trap ServiceStringLC
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::CallServiceStringLCuidSeviceDBL( TUid aServiceUid, const TDes& aParam )
{
    HBufC *string;
    string=iServicesDB->ServiceStringLC(aServiceUid,aParam);
    if ( NULL != string )
    {
       iLog->Log(_L("LOCALIZED name of the Service with UID(%d) = "),aServiceUid.iUid);
       iLog->Log(*string);
    }
    else 
    {
        iLog->Log(_L("No matching service found, Service UID provided is incorrect")); 
    } 
       
    switch (aServiceUid.iUid)
        {
        case KFirstServiceUid: if(!(string->Des().Compare(_L("Open Localized"))))
                {  iLog->Log(_L("LOCALIZED name is correct"));   }
                else 
                {  iLog->Log(_L("LOCALIZED name is incorrect")); 
                   CleanupStack::PopAndDestroy(string); return ( KErrGeneral );  }
                break;
        case KSecondServiceUid: if(!(string->Des().Compare(_L("Upload Localized"))))
                {  iLog->Log(_L("LOCALIZED name is correct"));   }
                else 
                {  iLog->Log(_L("LOCALIZED name is incorrect"));
                   CleanupStack::PopAndDestroy(string); return ( KErrGeneral );  }
                break;
        case KThirdServiceUid: if(!(string->Des().Compare(_L("Print Localized %U"))))
                {  iLog->Log(_L("LOCALIZED name is correct"));   }
                else 
                {  iLog->Log(_L("LOCALIZED name is incorrect"));  
                   CleanupStack::PopAndDestroy(string);return ( KErrGeneral );  }
                break;    
        default : break;
        }; 
       
    CleanupStack::PopAndDestroy(string);    
    return ( KErrNone );
}

// -----------------------------------------------------------------------------
// CServicesDbApiTest::ServiceNameLCSeviceDB
// Testing - IMPORT_C HBufC* ServiceNameLC(TInt aIndex) const;
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::ServiceNameLCSeviceDB( CStifItemParser& aItem )
{
    TInt result = InitializeSeviceDB();
    if ( KErrNone != result )
    {
        return ( result );
    }
    TInt index;
    TBuf<KBufSize> format(KEmptyString);
    
    if( aItem.GetNextInt(index) != KErrNone )
    { 
        iLog->Log(_L("NO Index number provided, index is required parameter for testing ServiceNameLC "));
        return ( KErrGeneral );
    }
    iLog->Log(_L("checked till here"));
    iLog->Log(_L("index = %d"),index);
    
    TRAPD(error,TInt res = CallServiceNameLCSeviceDBL(index));
    if ( KErrNone != error )
    {
        iLog->Log(_L("Incorrect Index. There is no service matching the Index Number."));
        iLog->Log(_L("=> ServiceNameLC leaves when index is incorrect."));
        return (error);
    }
    return ( KErrNone ); 
   
}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::ServiceNameLCSeviceDB
// This function is used because we can not trap ServiceNameLC
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::CallServiceNameLCSeviceDBL(TInt aIndex)
{
    HBufC *string;
    string=iServicesDB->ServiceNameLC(aIndex);
    iLog->Log(_L("GENERIC name of the Service[%d] = "),aIndex);
    iLog->Log(*string);
    
    switch (aIndex)
        {
        case KFirstServiceIndex: if(!(string->Des().Compare(_L("Open"))))
                {  iLog->Log(_L("GENERIC name is correct"));   }
                else 
                {  iLog->Log(_L("GENERIC name is incorrect"));
                   CleanupStack::PopAndDestroy(string); return ( KErrGeneral );  }
                break;
        case KSecondServiceIndex: if(!(string->Des().Compare(_L("Upload"))))
                {  iLog->Log(_L("GENERIC name is correct"));   }
                else 
                {  iLog->Log(_L("GENERIC name is incorrect"));
                   CleanupStack::PopAndDestroy(string); return ( KErrGeneral );  }
                break;
        case KThirdServiceIndex: if(!(string->Des().Compare(_L("Print"))))
                {  iLog->Log(_L("GENERIC name is correct"));   }
                else 
                {  iLog->Log(_L("GENERIC name is incorrect")); 
                   CleanupStack::PopAndDestroy(string);return ( KErrGeneral );  }
                break;    
        default : break;
        }; 
    
    CleanupStack::PopAndDestroy(string);    
    return ( KErrNone );

}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::ServiceNameLCuidSeviceDB
// Testing - IMPORT_C HBufC* ServiceNameLC(TUid aServiceUid) const;
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::ServiceNameLCuidSeviceDB( CStifItemParser& aItem )
{
    
    TInt result = InitializeSeviceDB();
    if ( KErrNone != result )
    {
        return ( result );
    }
    TBuf<KBufSize> format(KEmptyString);
    TInt uid;
    
    if( aItem.GetNextInt(uid) != KErrNone )
    { 
        iLog->Log(_L("NO UID number provided, UID is required parameter for testing ServiceStringLC "));
        return ( KErrGeneral );
    }
    iLog->Log(_L("checked till here"));
    
    TUid serviceUID= TUid::Uid(uid);
    iLog->Log(_L("serviceUID = %d"),serviceUID.iUid);
    
    TRAPD(error,TInt res = CallServiceNameLCuidSeviceDBL(serviceUID));
    if ( KErrNone != error )
    {
        iLog->Log(_L("=> ServiceNameLC leaves."),error);
        return (error);
    }
    return ( KErrNone ); 
}


// -----------------------------------------------------------------------------
// CServicesDbApiTest::ServiceNameLCuidSeviceDB
// This function is used because we can not trap ServiceNameLC
// -----------------------------------------------------------------------------
//
TInt CServicesDbApiTest::CallServiceNameLCuidSeviceDBL( TUid aServiceUid )
{
    HBufC *string;
    string=iServicesDB->ServiceNameLC(aServiceUid);
    if ( NULL != string )
    {
       iLog->Log(_L("GENERIC name of the Service with UID(%d) = "),aServiceUid.iUid);
       iLog->Log(*string);
    }
    else 
    {
        iLog->Log(_L("No matching service found, Service UID provided is incorrect")); 
    } 
       
    switch (aServiceUid.iUid)
        {
        case KFirstServiceUid: if(!(string->Des().Compare(_L("Open"))))
                {  iLog->Log(_L("GENERIC name is correct"));   }
                else 
                {  iLog->Log(_L("GENERIC name is incorrect")); 
                   CleanupStack::PopAndDestroy(string); return ( KErrGeneral );  }
                break;
        case KSecondServiceUid: if(!(string->Des().Compare(_L("Upload"))))
                {  iLog->Log(_L("GENERIC name is correct"));   }
                else 
                {  iLog->Log(_L("GENERIC name is incorrect"));
                   CleanupStack::PopAndDestroy(string); return ( KErrGeneral );  }
                break;
        case KThirdServiceUid: if(!(string->Des().Compare(_L("Print"))))
                {  iLog->Log(_L("GENERIC name is correct"));   }
                else 
                {  iLog->Log(_L("GENERIC name is incorrect"));  
                   CleanupStack::PopAndDestroy(string);return ( KErrGeneral );  }
                break;    
        default : break;
        }; 
       
    CleanupStack::PopAndDestroy(string);    
    return ( KErrNone );
}


//  [End of File] - Do not remove
