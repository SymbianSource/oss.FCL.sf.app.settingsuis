/*
* Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
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

#ifndef CPTHEMECLIENT_P_H
#define CPTHEMECLIENT_P_H

#include <qglobal.h>
#include <QByteArray>


class CpThemeClientPrivate;

class CpThemeClient
{
    CpThemeClientPrivate* d_ptr;
    Q_DECLARE_PRIVATE_D(d_ptr, CpThemeClient)

public:
    bool connectToServer();

    bool changeTheme(const QString& newtheme);

    bool isConnected();

public:
    static CpThemeClient *global();
    static void releaseInstance();
    ~CpThemeClient();

private:
    CpThemeClient();
};

#endif /* CPTHEMECLIENT_P_H */
