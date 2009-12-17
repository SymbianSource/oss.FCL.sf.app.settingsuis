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
* Description:  Implementation of class CServicesDbApiTest
*
*/



#ifndef SERVICESDBAPITEST_H
#define SERVICESDBAPITEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <services_db.h>
#include <ConeResLoader.h>
#include <data_caging_path_literals.hrh>
#include <StringLoader.h>
#include <barsread.h>
#include <eikenv.h>



// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KServicesDbApiTestLogPath, "\\logs\\testframework\\ServicesDbApiTest\\" ); 
// Log file
_LIT( KServicesDbApiTestLogFile, "ServicesDbApiTest.txt" ); 
_LIT( KServicesDbApiTestLogFileWithTitle, "ServicesDbApiTest_[%S].txt" );

_LIT(KEmptyString,"");

// FORWARD DECLARATIONS
class CServicesDbApiTest;


//Constants
const TInt KNoOfServices = 3; // This is the number of services listed in resource file. 
const TInt KFirstOption = 0;
const TInt KSecondOption = 1;
const TInt KFirstServiceIndex = 0;
const TInt KSecondServiceIndex = 1;
const TInt KThirdServiceIndex=2;
const TInt KFirstServiceUid =   257;
const TInt KSecondServiceUid = 258;
const TInt KThirdServiceUid  =  259;
const TInt KBufSize = 128;

// CLASS DECLARATION

/**
*  CServicesDbApiTest test class for STIF Test Framework TestScripter.
*  This class is written to test Services DB API. 
*
*  @lib ServicesDbApiTest.lib
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CServicesDbApiTest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CServicesDbApiTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CServicesDbApiTest();

 
    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );


    private:

        /**
        * C++ default constructor.
        */
        CServicesDbApiTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //[TestMethods]
        
        /**
        * Creates CServicesDB object
        * @since ?Series60_version
        * @param aOption Option to create CServicesDB object using NewL or NewLC
        * @return Symbian OS error code.
        */
        TInt CreateSeviceDBL(TInt aOption);  
        
        /**
        * In this method NewLC is called.
        * @since ?Series60_version
        * @param aResReader Resource reader to get information from Resource file
        * @return Symbian OS error code.
        */
        void CallNewlcL(  TResourceReader aResReader );
        
        /**
        * This method is used to test CServicesDB::NewL
        * @since ?Series60_version
        * @param No parameters required
        * @return Symbian OS error code.
        */
        TInt InitializeSeviceDB( );
        
        /**
        * This method is used to test CServicesDB::NewLC
        * @since ?Series60_version
        * @param No parameters required
        * @return Symbian OS error code.
        */
        TInt InitializeonStackSeviceDB( );
        
        /**
        * This method is used to test CServicesDB::Count
        * @since ?Series60_version
        * @param No parameters required
        * @return Symbian OS error code.
        */
        TInt CountSeviceDB();
        
        /**
        * This method is used to test CServicesDB::ServiceUidL
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt ServiceUidLSeviceDB( CStifItemParser& aItem );
        
        /**
        * This method is used to test CServicesDB::ServiceStringLC
        * with Index as parameter.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt ServiceStringLCSeviceDB( CStifItemParser& aItem );
        
        /**
        * This method is used to test CServicesDB::ServiceStringLC
        * with UID as parameter
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt ServiceStringLCuidSeviceDB( CStifItemParser& aItem );
        
        /**
        * This method is used to test CServicesDB::ServiceNameLC
        * with Index as parameter
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt ServiceNameLCSeviceDB( CStifItemParser& aItem );
        
        /**
        * This method is used to test CServicesDB::ServiceNameLC
        * with UID as parameter
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt ServiceNameLCuidSeviceDB( CStifItemParser& aItem );
        
        /**
        * This is method encapsulates ServiceStringLC. 
        * This method can be TRAPD and used to help in testing ServiceStringLC
        * @since ?Series60_version
        * @param aIndex the index of the service
        * @param aParam parameter for the localized string
        * @return Symbian OS error code.
        */
        TInt CallServiceStringLCSeviceDBL(TInt aIndex,const TDes& aParam );
        
        /**
        * This is method encapsulates ServiceStringLC. 
        * This method can be TRAPD and used to help in testing ServiceStringLC
        * @since ?Series60_version
        * @param aServiceUid the Uid of the service
        * @param aParam parameter for the localized string
        * @return Symbian OS error code.
        */
        TInt CallServiceStringLCuidSeviceDBL( TUid aServiceUid, const TDes& aParam);
        
        /**
        * This is method encapsulates ServiceNameLC. 
        * This method can be TRAPD and used to help in testing ServiceNameLC
        * @since ?Series60_version
        * @param aIndex the index of the service
        * @return Symbian OS error code.
        */
        TInt CallServiceNameLCSeviceDBL(TInt aIndex);
        
        /**
        * This is method encapsulates ServiceNameLC. 
        * This method can be TRAPD and used to help in testing ServiceNameLC
        * @since ?Series60_version
        * @param aServiceUid the Uid of the service
        * @return Symbian OS error code.
        */
        TInt CallServiceNameLCuidSeviceDBL( TUid aServiceUid );
        
        
    private:    // Data
        
        // This is the pointer to object of the class being tested
        // Through this pointer all the Exported methods are tested. 
        CServicesDB *iServicesDB;
    
        
    };

#endif      // SERVICESDBAPITEST_H

// End of File
