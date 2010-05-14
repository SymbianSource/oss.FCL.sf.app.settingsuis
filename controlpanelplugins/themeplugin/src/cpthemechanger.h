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

#ifndef CPTHEMECHANGER_H
#define CPTHEMECHANGER_H

#include <QStringList>
#include <hbicon.h>
#include <hbglobal.h>

class CpThemeChangerPrivate;

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

class CpThemeChanger : public QObject
{
    Q_OBJECT

public:
     explicit CpThemeChanger(QObject* parent=0);
    ~CpThemeChanger();

    struct ThemeInfo 
    {
        QString name;
        HbIcon  icon;
        HbIcon portraitPreviewIcon;
        HbIcon landscapePreviewIcon;
        bool operator < (const struct ThemeInfo &other) const   {			
            return name.localeAwareCompare(other.name) < 0;
        }	
        bool operator == (const struct ThemeInfo &other) const {
            return name.localeAwareCompare(other.name) == 0;
        }
    };
    
    enum ThemeListUserRole {
        PortraitPreviewRole = Qt::UserRole,
        LandscapePreviewRole
    };
    
    QAbstractItemModel& model();

    const ThemeInfo& currentTheme() const;
    int indexOf(const ThemeInfo& theme) const;

    bool changeTheme(const QString& newtheme);
	
	bool connectToServer();
	bool isConnected() const;

private:
    CpThemeChangerPrivate* d_ptr;
    Q_DECLARE_PRIVATE(CpThemeChanger)
    Q_PRIVATE_SLOT(d_func(), void _q_themeDirectoryChanged(const QString&))
};

#endif /* CPTHEMECHANGER_H */
