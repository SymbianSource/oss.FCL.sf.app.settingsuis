/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declaration of CTSCStartupExtension class.
*
*/

#ifndef TSCSTARTUPEXTENSION_H
#define TSCSTARTUPEXTENSION_H

#include <e32base.h>
#include <ssm/ssmcustomcommand.h>

// FORWARD DECLARATIONS
class CTSCPubSubObserver;

/**
*/
NONSHARABLE_CLASS( CTSCStartupExtension )
  : public CBase,
    public MSsmCustomCommand
    {
public:

    static CTSCStartupExtension * NewL();
    virtual ~CTSCStartupExtension ();

    void CalibrationDone();

private: // From MSsmCustomCommand

    TInt Initialize( CSsmCustomCommandEnv* aCmdEnv );
    void Execute( const TDesC8& aParams, TRequestStatus& aStatus );
    void ExecuteCancel();
    void Release();
    void Close();

private:

    /**
    * First phase constructor.
    */
    CTSCStartupExtension ();

private: //data

    CTSCPubSubObserver* iTSCPubSubObserver; //owns
    
    TRequestStatus* iStatus;
    TBool iRequestSent;

    };

#endif // TSCSTARTUPEXTENSION_H
