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

#include "cpthemeclient_p_p.h"
#include "cpthemesymbiancommon_p.h"
#include <e32property.h>


/**
* constructor
*/
CpThemeClientPrivate::CpThemeClientPrivate()
{
}

/**
* CpThemeClientPrivate::connectToServer()
*
* Connects to the server
*/
bool CpThemeClientPrivate::connectToServer()
{
    return true;
}

/**
Indicates a connection to the server
*/
bool CpThemeClientPrivate::isConnected() const
{
    return true;
}

/**
 * Destructor
 */
CpThemeClientPrivate::~CpThemeClientPrivate()
{
}

/**
 * changeTheme
 */
bool CpThemeClientPrivate::changeTheme(const QString& aString )
{
    TInt err = KErrGeneral;
    RProperty themeRequestProp;
    
    User::LeaveIfError( themeRequestProp.Attach( KServerUid3, KNewThemeForThemeChanger ) );
    
    TBuf<256> newThemenameChangeRequest;
    _LIT(KThemeRequestFormatter, "%d:%S");
    TBuf<256> newThemename(aString.utf16());
    newThemenameChangeRequest.Format( KThemeRequestFormatter, EThemeSelection, &newThemename);
    err = themeRequestProp.Set(newThemenameChangeRequest);
    themeRequestProp.Close();
    return (err == KErrNone);
}
