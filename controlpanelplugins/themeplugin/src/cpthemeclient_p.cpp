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

#include "cpthemeclient_p.h"
#include "cpthemeclient_p_p.h"

static CpThemeClient *clientInst=0;

/**
 * Constructor
 */
CpThemeClient::CpThemeClient():d_ptr(new CpThemeClientPrivate)
{
    
}

/**
 * CpThemeClient::connectToServer()
 */
bool CpThemeClient::connectToServer()
{
    Q_D(CpThemeClient);
    
    return d->connectToServer();
}

bool CpThemeClient::isConnected()
{
    Q_D(const CpThemeClient);
    
    return d->isConnected();
}

/**
 * Change a theme
 */
bool CpThemeClient::changeTheme(const QString& newtheme)
{
    Q_D(CpThemeClient);
    
    return d->changeTheme(newtheme);
}

/**
 * CpThemeClient::~CpThemeClient()
 */
CpThemeClient::~CpThemeClient()
{
    Q_D(CpThemeClient);
    
    delete d;
}

/**
 * CpThemeClient::global()
 */
CpThemeClient *CpThemeClient::global()
{
    if ( !clientInst ) {
        clientInst = new CpThemeClient;
    }
    return clientInst;
}

/**
 * CpThemeClient::releaseInstance()
 */
void CpThemeClient::releaseInstance()
{
    delete clientInst;
    clientInst = 0;
}
