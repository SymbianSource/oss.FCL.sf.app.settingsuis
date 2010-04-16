/*
* Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#include "cpthemeclientsymbian_p.h"
#include "cpthemesymbiancommon_p.h"

#include <QFile>
#include <QDebug>
#include <QTime>

const TUint KDefaultMessageSlots = 4;

/**
* constructor
*/
CpThemeClientPrivate::CpThemeClientPrivate():
    clientConnected(false)
{
}

/**
* CpThemeClientPrivate::connectToServer()
*
* Connects to the server
*/
bool CpThemeClientPrivate::connectToServer()
{
   
    TInt error; 
    
    error = CreateSession(KThemeServerName, Version(), KDefaultMessageSlots);
    return( clientConnected = (KErrNone == error ) );
}

/**
Indicates a connection to the server
*/
bool CpThemeClientPrivate::isConnected()
{
    return connected;
}



/**
Returns the version number
*/  
TVersion CpThemeClientPrivate::Version(void) const
{
    return(TVersion(KThemeServerMajorVersionNumber,
                    KThemeServerMinorVersionNumber,
                    KThemeServerBuildVersionNumber));
}

/**
 * Destructor
 */
CpThemeClientPrivate::~CpThemeClientPrivate()
{
    RSessionBase::Close();
}

/**
 * StartServer
 */
TInt CpThemeClientPrivate::StartServer()
{
    TFindServer findThemeServer(KThemeServerName);
    TFullName name;
    if ( KErrNone == findThemeServer.Next( name ) ) {
        // Server already running
        return KErrNone;
    }
    return CreateServerProcess();
}

/**
 * CreateServerProcess
 */
TInt CpThemeClientPrivate::CreateServerProcess()
{
    TInt result;
    const TUid KServerUid2={0x100039CE};
    const TUidType serverUid( KNullUid, KServerUid2, KServerUid3 );
    RProcess server;
    _LIT(KThemeServerExe,"hbthemeserver.exe");
    
    result = server.Create( KThemeServerExe, KNullDesC, EOwnerProcess );
    
    if (KErrNone != result) {
        return  result; 
    }
    else {
        TRequestStatus status;
        server.Rendezvous(status);
        if (status!=KRequestPending) {
            server.Kill(0); // abort startup
        }
        else {
            server.Resume(); // logon OK - start the server
        }
        User::WaitForRequest(status);// wait for start or death
        server.Close();
        connected == status.Int() == KErrNone ? true : false;
        return status.Int(); // return the error
    }
}

/**
 * changeTheme
 */
bool CpThemeClientPrivate::changeTheme(const QString& aString )
{
    TPtrC themeName(reinterpret_cast<const TUint16 *>(aString.constData()));
    TIpcArgs args(&themeName);
    TInt err = SendReceive(EThemeSelection, args);
    return KErrNone == err;
}
