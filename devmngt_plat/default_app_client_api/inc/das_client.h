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
* Description:  Client interface to ask for setting the default application
*
*/



#ifndef C_DEFAULTAPPCLIENT_H
#define C_DEFAULTAPPCLIENT_H

#include <e32def.h>
#include <e32base.h>
#include <AknServerApp.h>

class RDefaultAppService; 
 
 
/**
 *  Client API for setting the default application.
 *
 *  This class defines the API that applications should use to trigger the launch of the
 *  Default Application Server, that asks the user to select default applications for those
 *  Service & MIME pairs for which the client application is a candidate.
 *
 *  A client application should first instantiate the service, using eithe NewL() or NewLC()
 *  The client application can check first if the sercive is available in the system (by using 
 *  the ServiceAvailable() function. The client application may also check if there is 
 *  anything to display (if there is any Service & MIME that the client application supports for 
 *  which there is at least another application supporting the pair). If there is no such pair,
 *  then launching the server would display an empty list). This checking is performed using the 
 *  PairsToDisplayL() function. 
 *
 *  After the above checkings are done, the client application can ask the server to change
 *  defaults by calling the ChangeDefaultsL() function. 
 *    
 *
 *  @lib defaultappclient.dll
 *  @since S60 v5.0
 */
class CDefaultAppClient : public CBase 
    {

public:

    /**  flags used to modify the service behaviour */
    enum TFlags
        {
        EFlagNoObserver=1,
        EFlagReserved1=2,
        EFlagReserved2=4
        };

    /**
     * Symbian OS two-phased constructor
     * @return
     */
    IMPORT_C static CDefaultAppClient* NewL(MAknServerAppExitObserver* aObserver);

    /**
     * Symbian OS two-phased constructor
     * @return
     */
    IMPORT_C static CDefaultAppClient* NewLC(MAknServerAppExitObserver* aObserver);

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CDefaultAppClient();

    /**
     * Function to check if a server is present in the system. If the
     * server is present, the subsequent functions should not fail due
     * to server unavailability.
     *
     * @since S60 v5.0
     * @return ETrue or EFalse, depending service/server availability
     */
    IMPORT_C static TBool ServiceAvailable( TInt& aErrorCode );
    
    /**
     * Function to check the number of Service & MIME pairs the server would display if the 
     * ChangeDefaultsL() would be called. The purpose of this function
     * is to allow the client application not to display an entry for starting the server, in the 
     * Options menu, in case the Server's list of Service & MIME pairs is empty.
     * 
     * Please note that the function may return a higher number than the actual number
     * of pairs, because it does not check for MIMEs with System priority (that would not be 
     * displayed).
     *
     * @since S60 v5.0
     * @return the number of Service & MIME pairs the server would display
     */
    IMPORT_C TInt PairsToDisplayL();
    
    /**
     * This function launches the server, as a chained application. The client application will not be
     * available to the user until the server does not exit.
     * When the server exits, the client application gains control again. The observer (if specified 
     * during instantiation (NewL) is notified that the server has exited.
     *
     * @since S60 v5.0
     * @param aFlags service flags 
     */
    IMPORT_C void ChangeDefaultsL( TInt aFlags = 0 );
    

private:

    /**
     * C++ default constructor.
     */
    CDefaultAppClient();

    /**
     * Symbian constructor.
     */
    void ConstructL( MAknServerAppExitObserver* aObserver );
    
    /**
     * This function finds out the uid of the Default Application Server 
     * (the application registered to handle the DefaultApp service)
     *
     * @since S60 v5.0
     * @param aServerAppUid If parameter is non-NULL, it returns the Uid of the server there
     */
    static void GetServiceParamsL( TUid* aServerAppUid); 

private: // data

    /**
     * The UID of the server application (we discover the server)
     */
    TUid iServerAppUid;

    /**
     * Pointer to the service class.
     * Own.
     */
    RDefaultAppService *iService; 
    
    /**
     * Pointer to the Observer to call after the operation initiated by
     * ChangeDefaultsAsyncL() has finished.
     * Not Owned.
     */
    MAknServerAppExitObserver* iObserver;
    
    /**
     * Monitor object that calls the iObserver when the server has exited.
     * Owned.
     */
    CApaServerAppExitMonitor* iMonitor;
    
    };

#endif // C_DEFAULTAPPCLIENT_H
