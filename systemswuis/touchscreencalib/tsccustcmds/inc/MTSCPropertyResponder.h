/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface of Touch screen calibration property change obsever
*
*/

#ifndef MTSCPROPERTYRESPONDER_H
#define MTSCPROPERTYRESPONDER_H

// INCLUDES
#include <e32base.h>

class MTSCPropertyResponder
    {
public:
    virtual void HandlePropertyChangedL( const TUid& aCategory, TUint aKey ) = 0;
    };
#endif //MTSCPROPERTYRESPONDER_H

// End of File
