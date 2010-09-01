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
* Description:  Implements a (minimal) GS Plugin
*
*/


#include "gs_das_emptycontainer.h"

CCEmptyContainer::CCEmptyContainer()
    {
    // No implementation required
    }

CCEmptyContainer::~CCEmptyContainer()
    {
    }

CCEmptyContainer* CCEmptyContainer::NewLC ( const TRect& aRect, 
        const CCoeControl* aParent )
    {
    CCEmptyContainer* self = new (ELeave) CCEmptyContainer();
    CleanupStack::PushL ( self );
    self->ConstructL( aRect, aParent );
    return self;
    }

CCEmptyContainer* CCEmptyContainer::NewL ( const TRect& aRect, 
        const CCoeControl* aParent )
    {
    CCEmptyContainer* self = CCEmptyContainer::NewLC ( aRect, aParent );
    CleanupStack::Pop(); // self;
    return self;
    }

void CCEmptyContainer::ConstructL( const TRect& aRect,const CCoeControl* aParent )
    {
    if ( aParent == NULL )
        {
        CreateWindowL();
        }
    else
        {
        SetContainerWindowL( *aParent );
        }
    SetRect( aRect );
    ActivateL();
    }

TInt CCEmptyContainer::CountComponentControls() const
    {
    return 0;
    }

CCoeControl* CCEmptyContainer::ComponentControl ( TInt aIndex ) const
    {
    switch( aIndex )
        {
        
        }
    return NULL;
    }
