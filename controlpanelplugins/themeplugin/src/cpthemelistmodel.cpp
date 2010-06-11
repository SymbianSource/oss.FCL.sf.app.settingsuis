/*
 * ============================================================================
 *  Name        : cpthemelistmodel_p.cpp
 *  Part of     : LibHb / theme
 *  Description : Private implementation of the theme listmodel.
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

#include "cpthemelistmodel.h"
#include "cpthemechanger_p.h"
#include <QFileSystemWatcher>

/*
    CpThemeChangerModel provides an interface to the data contained in the
    CpThemeChanger using QAbstractListModel.
*/

/*
    Constructor
*/
CpThemeListModel::CpThemeListModel(CpThemeChangerPrivate *dd, QObject* parent)
    : QAbstractListModel(parent)
    , mThemeChangerPrivate(dd)
{
    connect(dd->mFileWatcher, SIGNAL(directoryChanged(const QString&)),
        this, SLOT(themeListChanged()));
}

/*
    Destructor
*/
CpThemeListModel::~CpThemeListModel()
{

}

/*
    Reimplemented from QAbstractListModel.
*/
int CpThemeListModel::rowCount(const QModelIndex&) const
{
    return mThemeChangerPrivate->mThemeList.size();
}

/*
    Reimplemented from QAbstractListModel.  Provides the data for Qt::DisplayRole and
    Qt::DecorationRole.
*/
QVariant CpThemeListModel::data(const QModelIndex& index, int role) const
{
    QVariant retVal = QVariant();

    if (index.isValid()) {
        switch (role) {
            case Qt::DisplayRole:
                retVal = mThemeChangerPrivate->mThemeList.at(index.row()).name;
                break;

            case Qt::DecorationRole:
                retVal = mThemeChangerPrivate->mThemeList.at(index.row()).icon;
                break;

            case Qt::SizeHintRole:
                retVal = mThemeChangerPrivate->mThemeList.at(index.row()).icon.size();
                break;
                
        	  case CpThemeChanger::PortraitPreviewRole:
          	    retVal = mThemeChangerPrivate->mThemeList.at(index.row()).portraitPreviewIcon;
                break;
                
            case CpThemeChanger::LandscapePreviewRole:
                retVal = mThemeChangerPrivate->mThemeList.at(index.row()).landscapePreviewIcon;
                break;

            default:
                // do nothing
                qt_noop();
        }
    }

    return retVal;
}

/*
    Responds appropriately when the underlying data in the theme changer is modified.

    Unfortunately the directory watcher from QFileWatcher only says when something changed
    not what changed.  Therefore the model is considered reset rather than having rows
    with dataChanged.
*/
void CpThemeListModel::themeListChanged()
{
    beginResetModel();

    mThemeChangerPrivate->themes();

    endResetModel();
}
