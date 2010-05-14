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

#ifndef HBTHEMECHANGER_P_H
#define HBTHEMECHANGER_P_H

#include <hbglobal.h>

#include <QObject>

#include <QAbstractListModel>
#include <QDir>
#include <QModelIndex>
#include <QString>
#include <QStringList>

#include "cpthemechanger.h"

QT_BEGIN_NAMESPACE
class QFileSystemWatcher;
QT_END_NAMESPACE

class CpThemeClient;

class HbThemeListModel : public QAbstractListModel
{
    Q_OBJECT

public:
   
    HbThemeListModel(CpThemeChangerPrivate* dd, QObject *parent = 0);
    virtual ~HbThemeListModel();

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:
    void themeListChanged();

private:
    CpThemeChangerPrivate *mThemeChangerPrivate;
};

class CpThemeChangerPrivate
{
    CpThemeChanger* q_ptr;
    Q_DECLARE_PUBLIC(CpThemeChanger)

public:
    CpThemeChangerPrivate(CpThemeChanger* q);
    ~CpThemeChangerPrivate();

    bool connectToServer();
    bool isConnected() const;

    const QList<CpThemeChanger::ThemeInfo>& themes() const;
    void updateThemeList(const QString& currentThemeName = QString());

    const CpThemeChanger::ThemeInfo& currentTheme() const;
    int indexOf(const CpThemeChanger::ThemeInfo& theme) const;

    const QString& currentThemeName() const;
    bool changeTheme(const QString& newtheme);

    void _q_themeDirectoryChanged(const QString&);

    CpThemeChanger::ThemeInfo mCurrentTheme;

    QString themeRootPath;
    QString themeRootPathPostfix;

    CpThemeClient* themeClient;
    QFileSystemWatcher* fileWatcher;
    QList<CpThemeChanger::ThemeInfo> themeList;
	HbThemeListModel model;
	friend class HbThemeChangerModel;
};

#endif /* HBTHEMECHANGER_P_H */
