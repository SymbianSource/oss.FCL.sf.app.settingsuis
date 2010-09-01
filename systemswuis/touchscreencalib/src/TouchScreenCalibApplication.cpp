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
* Description:  
*     This module implements the application core i.e. application
*     class and standard exported functions
*
*/


// INCLUDE FILES
#include "TouchScreenCalibApplication.h"
#include "TouchScreenCalibDocument.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CTouchScreenCalibApplication::AppDllUid()
// Returns application UID
// ---------------------------------------------------------------------------
TUid CTouchScreenCalibApplication::AppDllUid() const
    {
    return KUidTouchScreenCalib;
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibApplication::CreateDocumentL()
// Creates CTouchScreenCalibDocument object
//
// ---------------------------------------------------------------------------
CApaDocument* CTouchScreenCalibApplication::CreateDocumentL()
    {
    return CTouchScreenCalibDocument::NewL(*this);
    }

// ===================== OTHER EXPORTED FUNCTIONS ============================
#include <eikstart.h>

LOCAL_C CApaApplication* NewApplication()
    {
    return new CTouchScreenCalibApplication;
    }

GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

// End of file
