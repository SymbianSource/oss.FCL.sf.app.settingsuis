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
* Description:  
*
*/

#include "cpapplication.h"

/*!
    CpApplication
    Inherit HbApplication, mainly used for emit applicationReady Signal.
 */

/*!
    \fn void CpApplication::applicationReady()

    This signal is emitted when mainwindow has constructed, used for Matti test.
*/

/*!
    constructor.
 */
CpApplication::CpApplication(int &argc, char *argv[]):
 HbApplication(argc, argv)
{
}

/*!
    Destructor.
 */
CpApplication::~CpApplication()
{
}

/*!
    Emit applicationReady
*/
void CpApplication::handleAppReady()
{
    emit applicationReady();
}
