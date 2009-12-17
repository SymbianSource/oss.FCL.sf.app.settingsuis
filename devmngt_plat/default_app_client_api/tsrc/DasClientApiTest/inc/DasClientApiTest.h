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
* Description:  Implementation of class CDasClientApiTest
*
*/


#ifndef C_DASCLIENTAPITEST_H
#define C_DASCLIENTAPITEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <aknserverapp.h> 
#include <das_client.h>
//
#include <CClientLauncher.h>
// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0
// User MACROS
#define KErrGeneral     -2
#define KFirstOption    1
#define KSecondtOption  2

//define client flags
#define KFlagNoObserver     1
#define KFlagReserved1      2
#define KFlagReserved2      4
// Logging path
_LIT( KDasClientApiTestLogPath, "\\logs\\testframework\\DasClientApiTest\\" ); 
// Log file
_LIT( KDasClientApiTestLogFile, "DasClientApiTest.txt" ); 
_LIT( KDasClientApiTestLogFileWithTitle, "DasClientApiTest_[%S].txt" );

//
/**
*  CDasClientApiTest test class for STIF Test Framework TestScripter.
*  @since S60 5.0
*/

NONSHARABLE_CLASS(CDasClientApiTest) : public CScriptBase/*,public MAknServerAppExitObserver*/
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CDasClientApiTest* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CDasClientApiTest();
    
/*private:
    //function from MAknServerAppExitObserver. is used to handle the exit of the server.
    
    void HandleServerAppExit(TInt aReason);*/

public: // Functions from base classes

    /**
    * From CScriptBase Runs a script line.
    * @since S60 5.0
    * @param aItem Script line containing method name and parameters
    * @return Symbian OS error code
    */
    virtual TInt RunMethodL( CStifItemParser& aItem );
    
private:

    /**
    * C++ default constructor.
    */
    CDasClientApiTest( CTestModuleIf& aTestModuleIf );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

    /**
    * Frees all resources allocated from test methods.
    * @since S60 5.0
    */
    void Delete();

    /**
    * Test method.
    * @since S60 5.0
    * @param aItem Script line containing parameters.
    * @return Symbian OS error code.
    */
    virtual TInt InitialClientL(const TInt aOption );
    
    virtual TInt CallClientNewLL( CStifItemParser& aItem );
    
    virtual TInt CallClientNewLCL( CStifItemParser& aItem );
    
    virtual TInt CallServiceAvailableL( CStifItemParser& aItem );
    
//    virtual TInt CallPairsToDisplayLL( CStifItemParser& aItem );
    
    virtual TInt CallChangeDefaultsLL( CStifItemParser& aItem );
    /**
     * Method used to log version of test class
     */
    void SendTestClassVersion();
    
        /**
     * Turn off ScreenSaver
     * @since S60 5.0
     * @return Symbian OS error code.
     */
    void TurnOffScreenSaver();

    /**
     * Restore ScreenSaver
     * @since S60 5.0
     * @return Symbian OS error code.
     */
    void RestoreScreenSaver();


    /**
    * In this method NewLC is called.
    * @since ?Series60_version
    * @param aResReader Resource reader to get information from Resource file
    * @return Symbian OS error code.
    */
    void CallNewlcL(  MAknServerAppExitObserver* aObserver );


private:    // Data

    /**
     * ScreenSaver Property
     */
    TInt iOldScreenSaverProperty;
    
    /**
     * ?description_of_pointer_member
     * Not own.
     */
    //?type* ?member_name;
    CDefaultAppClient* iClient;
    
    //
    CCClientLauncher* iLauncher;
    
    };

#endif      // C_DASCLIENTAPITEST_H

// End of File
