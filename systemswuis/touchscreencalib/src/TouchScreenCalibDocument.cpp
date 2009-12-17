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
* Description:  Document class of the application.
*
*/


// SYSTEM INCLUDES
#include <apgwgnam.h>

// USER INCLUDES
#include "TouchScreenCalibDocument.h"
#include "TouchScreenCalibAppUi.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CTouchScreenCalibDocument::~CTouchScreenCalibDocument()
// ---------------------------------------------------------
CTouchScreenCalibDocument::~CTouchScreenCalibDocument()
    {
    }

// ---------------------------------------------------------
// CTouchScreenCalibDocument::ConstructL()
// ---------------------------------------------------------
void CTouchScreenCalibDocument::ConstructL()
    {
    }

// ---------------------------------------------------------
// CTouchScreenCalibDocument::NewL()
// ---------------------------------------------------------
CTouchScreenCalibDocument* CTouchScreenCalibDocument::NewL(
        CEikApplication& aApp)     // CTouchScreenCalibApp reference
    {
    CTouchScreenCalibDocument* self = new (ELeave) CTouchScreenCalibDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// ----------------------------------------------------
// CTouchScreenCalibDocument::CreateAppUiL()
// ----------------------------------------------------
CEikAppUi* CTouchScreenCalibDocument::CreateAppUiL()
    {
    return new(ELeave) CTouchScreenCalibAppUi;
    }

// ----------------------------------------------------
// CTouchScreenCalibDocument::UpdateTaskNameL()
// Makes TouchScreenCalib-application hidden in menu shell and fastswap window
// ----------------------------------------------------
void CTouchScreenCalibDocument::UpdateTaskNameL( CApaWindowGroupName* aWgName )
    {
    CEikDocument::UpdateTaskNameL( aWgName );
    aWgName->SetHidden( ETrue );
    }

// End of file
