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
* Description:  This class defines the service used to change the default app.
*
*/



#ifndef R_DEFAULTAPPSERVICE_H
#define R_DEFAULTAPPSERVICE_H

#include <AknServerApp.h>

/**
 *  Definition for the default app service
 *
 *  This class defines the client side of the default app service.
 *
 *  @lib defaultappclient.dll
 *  @since S60 v5.0
 */
class RDefaultAppService : public RAknAppServiceBase
    {

private:

    /**  messages used */
    enum TIpcMessageIds
            {
            ESetDefaultAll = RApaAppServiceBase::KServiceCmdBase
            };
public:

    /**
     * Sends SetDefault message to the server
     *
     * @since S60 v5.0
     * @param aFlags service class (value is transparent for this class/function)
     * @return 0 for success, negative value on failure
     */
    TInt SetDefault( TInt aFlags );

private: // From RApaAppServiceBase

    /**
     * From RApaAppServiceBase.
     * Returns the uid of the service
     *
     * @since S60 v5.0
     * @return the Uid of the provided service (Default App Service)
     */
    TUid ServiceUid() const;
    };


#endif // R_DEFAULTAPPSERVICE_H

