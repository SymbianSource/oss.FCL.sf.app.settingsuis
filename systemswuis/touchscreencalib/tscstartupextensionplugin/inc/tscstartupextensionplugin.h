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
* Description:  Declaration of CTSCStartupExtensionPlugIn class.
*
*/


#ifndef TSCSTARTUPEXTENSIONPLUGIN_H
#define TSCSTARTUPEXTENSIONPLUGIN_H

#include <systemstartupextension.h>

// FORWARD DECLARATIONS
class CTSCPlugInPubSubObserver;

/**
 * System Startup Extension plug-in for executing Touch Screen Calibration
 * during system startup.
 *
 *  @lib tscstartupextensionplugin.lib
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( CTSCStartupExtensionPlugIn )
  : public CSystemStartupExtension
    {

public:

    static CTSCStartupExtensionPlugIn* NewL( TAny* aConstructionParameters );

    virtual ~CTSCStartupExtensionPlugIn();

    /**
     * Execute TSC operations depending on parameters.
     *
     * @since S60 3.2
     * @param aStatus The request status to complete after the task has been
     * finished.
     */
    virtual void ExecuteL( TRequestStatus& aStatus, const TDesC& aParams );

    /**
     * Cancel pending request.
     *
     * @since S60 3.2
     */
    virtual void Cancel();

    /**
     *
     * @since S60 3.2
     * @param none
     */
    void CalibrationDone();

private:

    CTSCStartupExtensionPlugIn( TAny* aConstructionParameters );

    /**
     * Second phase constructor.
     *
     */
    void ConstructL();

private: //data

    CTSCPlugInPubSubObserver* iTSCPlugInPubSubObserver; //owns
    
    TRequestStatus* iStatus;
    TBool iRequestSent;
    };

#endif // TSCSTARTUPEXTENSIONPLUGIN_H
