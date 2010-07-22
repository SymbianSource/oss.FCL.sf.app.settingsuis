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

#ifndef CPTHEMEUTIL_H_
#define CPTHEMEUTIL_H_

#include <QList>

class QStringList;
class CpThemeInfo;

class CpThemeUtil {

public:
    static QStringList themePathList();
    static QList<CpThemeInfo> buildThemeList();
    static CpThemeInfo* buildThemeInfo(const QString& themePath, const QString& themeName);
    static QString themePath(const QString& themeName);
    static CpThemeInfo* defaultTheme();
};

#endif /* CPTHEMEUTIL_H_ */


