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
* Description:  Custom command implementation factory.
*
*/

#ifndef TSCCUSTCMDFACTORY_H
#define TSCCUSTCMDFACTORY_H

#include <e32def.h>

class MSsmCustomCommand;

/**
* Creates custom command objects.
*/
class TSCCustCmdFactory
    {
public:

	IMPORT_C static MSsmCustomCommand* CmdTSCStartupExtensionNewL(); 
    };

#endif // TSCCUSTCMDFACTORY_H
