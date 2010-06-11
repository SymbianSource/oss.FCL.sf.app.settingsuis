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

#ifndef CPTHEMECLIENT_P_P_H
#define CPTHEMECLIENT_P_P_H

#include <QIcon>
#include "cpthemecommon_p.h"

#ifdef Q_OS_SYMBIAN
#include <e32base.h>
#endif

class QSizeF;
#ifndef Q_OS_SYMBIAN
class QLocalSocket;
#endif

class CpThemeClientPrivate :
#ifdef Q_OS_SYMBIAN
public RSessionBase
#else
public QObject
#endif
{
#ifndef Q_OS_SYMBIAN
    Q_OBJECT
#endif

public:
    CpThemeClientPrivate();
    bool connectToServer();
    bool isConnected() const;
	bool changeTheme(const QString& newTheme);
    ~CpThemeClientPrivate();
    
#ifndef Q_OS_SYMBIAN
public slots:
    void changeTheme();
#endif

public:
    bool clientConnected;

private:
#ifdef Q_OS_SYMBIAN
    TVersion Version() const;
    TInt StartServer();
    TInt CreateServerProcess();
#endif

private:
#ifdef Q_OS_SYMBIAN
	friend class CThemeListenerPrivate;
#else
    QLocalSocket* localSocket;
#endif 
};

#endif // HBTHEMECLIENT_P_P_H
