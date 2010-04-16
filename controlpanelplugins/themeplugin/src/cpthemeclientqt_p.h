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


#ifndef CPTHEMECLIENTQT_P_H
#define CPTHEMECLIENTQT_P_H

#include <QIcon>
#include "cpthemecommon_p.h"

class QString;
class QSizeF;
class QLocalSocket;

class CpThemeClientPrivate : public QObject
{
    Q_OBJECT

public:
    CpThemeClientPrivate();
    bool connectToServer();
    bool isConnected();

    bool changeTheme(const QString& newtheme);

    ~CpThemeClientPrivate();
    
public slots:
    void themeChanged();

public:
    bool clientConnected;

private:
    void handleThemeChangeRequest(QDataStream &dataStream);

private:
    QLocalSocket* localSocket;
    QString themeName;
};

#endif // HBTHEMECLIENTQT_P_H
