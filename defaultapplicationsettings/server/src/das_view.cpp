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
* Description:  Implementation of the view class
*
*/



// System includes

#include "das_view.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDefaultAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDefaultAppView* CDefaultAppView::NewL( const TRect& aRect )
    {
    CDefaultAppView* self = CDefaultAppView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CDefaultAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDefaultAppView* CDefaultAppView::NewLC( const TRect& aRect )
    {
    CDefaultAppView* self = new ( ELeave ) CDefaultAppView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CDefaultAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDefaultAppView::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CDefaultAppView::CDefaultAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CDefaultAppView::CDefaultAppView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CDefaultAppView::~CDefaultAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CDefaultAppView::~CDefaultAppView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CDefaultAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CDefaultAppView::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect drawRect( Rect());

    
  	}

// -----------------------------------------------------------------------------
// CDefaultAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CDefaultAppView::SizeChanged()
    {  
    DrawNow();
    }
