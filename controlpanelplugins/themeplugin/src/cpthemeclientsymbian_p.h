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

#ifndef CPTHEMECLIENTSYMBIAN_P_H
#define CPTHEMECLIENTSYMBIAN_P_H


#include <QIcon>
// TODO Do we need this?
//#include "hbthemecommon_p.h"
//ToDo: replace this with forward declaration
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

class CThemeListenerPrivate;
class QSizeF;
class HbEffectFxmlData;
struct MeshItem;

class CpThemeClientPrivate : public RSessionBase
{
public:
    CpThemeClientPrivate();

    bool connectToServer();
    bool isConnected();

    bool changeTheme(const QString& newtheme);
    ~CpThemeClientPrivate();
    
public:
    bool clientConnected;

private:
    TVersion Version() const;
    TInt StartServer();
    TInt CreateServerProcess();
    
private:
    CThemeListenerPrivate *themelistener;
    QString themeName;
    bool connected;
};

#endif /* CPTHEMECLIENTSYMBIAN_P_H */
