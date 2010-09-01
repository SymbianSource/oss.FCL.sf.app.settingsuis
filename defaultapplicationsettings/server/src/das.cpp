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
* Description:  Executable module to start the process and load the DefaultAppServer.
*
*/


#include <e32base.h>
#include "das_app.h"
#include <eikstart.h>

// ---------------------------------------------------------------------------
// NewApplication()
// constructs CDefaultAppApplication
// Returns: CApaDocument*: created application object
//
// ---------------------------------------------------------------------------
LOCAL_C CApaApplication* NewApplication()
    {
    return new CDefaultAppApplication;
    }

// ---------------------------------------------------------------------------
// E32Main()
//
// ---------------------------------------------------------------------------
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

//  End of File
