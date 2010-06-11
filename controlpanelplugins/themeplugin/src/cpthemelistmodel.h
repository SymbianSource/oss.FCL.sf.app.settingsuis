/*
* ============================================================================
*  Name        : cpthemelistmodel_p.h
*  Part of     : LibHb / theme
*  Description : CpThemeListModel class definition
*  Version     : %version: 1 % << Don't touch! Updated by Synergy at check-out.
*
*  Copyright (c) 2008-2009 Nokia.  All rights reserved.
*  This material, including documentation and any related computer
*  programs, is protected by copyright controlled by Nokia.  All
*  rights are reserved.  Copying, including reproducing, storing,
*  adapting or translating, any or all of this material requires the
*  prior written consent of Nokia.  This material also contains
*  confidential information which may not be disclosed to others
*  without the prior written consent of Nokia.
* ============================================================================
*/

#ifndef CP_THEME_LIST_MODEL_P_H
#define CP_THEME_LIST_MODEL_P_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QObject>

class CpThemeChangerPrivate;

class CpThemeListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    CpThemeListModel(CpThemeChangerPrivate* dd, QObject *parent = 0);
    virtual ~CpThemeListModel();

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:
    void themeListChanged();

private:
    CpThemeChangerPrivate *mThemeChangerPrivate;
};

#endif //CP_THEME_LIST_MODEL_P_H
