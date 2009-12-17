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
* Description:  The Default App Service implementation (server-side)
*
*/



#ifndef C_DEFAULTAPPSERVICE_H
#define C_DEFAULTAPPSERVICE_H

#include <AknServerApp.h>

class CDefaultAppServer;

/**
 *  This class implements the Default Application server-side service.
 *
 *  @since Series 60 v5.0
 */
class CDefaultAppService : public CAknAppServiceBase
    {
public:
    enum TIpcMessageIds
        {
        ESetDefaultAll = RApaAppServiceBase::KServiceCmdBase
        };
        
    /**
     * Symbian OS two-phased constructor
     * @return
     */    
    static CDefaultAppService* NewL(const CDefaultAppServer* aServer);
    
    /**
     * Destructor.
     */
    virtual ~CDefaultAppService();
    
public: // from base class CAknAppServiceBase  

    /**
     * From CAknAppServiceBase.
     * function called when a new message is received
     *
     * @since S60 v5.0
     * @param aMessage the received service message
     */
    void ServiceL(const RMessage2& aMessage);        
    
    /**
     * From CAknAppServiceBase.
     * function called to check client credentials. Used to retrieve client app UID
     *
     * @since S60 v5.0
     * @param aMsg the message being received
     */
    CPolicyServer::TCustomResult SecurityCheckL(const RMessage2& aMsg, TInt& aAction, TSecurityInfo& aMissing);

private: //construction

    /**
     * C++ constructor.
     */
    CDefaultAppService(const CDefaultAppServer* aServer);
    
    /**
     * Sets default for all the Service & MIME pairs supported by the client
     *
     * @since S60 v5.0
     * @param aMessage the received service message
     */
    void HandleSetDefaultAllL(const RMessage2& aMessage);
    
    /**
     * Function where most of the processing happens.
     * We use it so that the service request from the client returns immediately
     *
     * @since S60 v5.0
     * @param aInstance a pointer to the calling CDefaultAppService instance  
     */
    static TInt IdleWorker(TAny *aInstance);
    
public:
    /**
     * The Uid of the client application
     */
    TUid iClientUid;
    
    /**
     * Service flags requested by the client
     */
    TInt iServiceFlags;
    
    /**
     * pointer to the server
     * Not owned.
     */
    const CDefaultAppServer* iDefaultAppServer;  //not owned
    
    /**
     * Active Object where most of the processing happens. We use it so that the service
     * call returns immediately.
     * Owned.
     */
    CIdle *iIdle;
    };

#endif      // C_DEFAULTAPPSERVICE_H

// End of File
