/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the document class
*
*/



#include "das_doc.h"
#include "das_appui.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
//
CDefaultAppDocument* CDefaultAppDocument::NewL(CEikApplication& aApp)
    {
    CDefaultAppDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
//
CDefaultAppDocument* CDefaultAppDocument::NewLC(CEikApplication& aApp)
    {
    CDefaultAppDocument* self = new (ELeave) CDefaultAppDocument(aApp);
    CleanupStack::PushL(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CDefaultAppDocument::CDefaultAppDocument(CEikApplication& aApp) : CAknDocument(aApp), iDefaultAppUi(NULL)
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// Instantiates CDefaultAppAppUi
// ---------------------------------------------------------------------------
//
CEikAppUi* CDefaultAppDocument::CreateAppUiL()
    {
    iAppUi = iDefaultAppUi = new (ELeave) CDefaultAppAppUi;
    return (static_cast<CEikAppUi*>(iAppUi));
    }

