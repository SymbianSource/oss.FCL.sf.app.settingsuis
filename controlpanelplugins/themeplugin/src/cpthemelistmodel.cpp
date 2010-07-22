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

#include <QDir>
#include <QStringList>
#include <QFileSystemWatcher>

#include <HbIcon>

#include "cpthemelistmodel.h"
#include "cpthemeinfo.h"
#include "cpthemeutil.h"

/*
    CpThemeChangerModel provides an interface to the data contained in the
    theme list using QAbstractListModel.
*/

/*
    Constructor
*/
CpThemeListModel::CpThemeListModel(QObject* parent)
    : QAbstractListModel(parent)
    , mTopThemeList()
    , mThemeList()
    , mBottomThemeList()
    , mFileWatcher(new QFileSystemWatcher(this))
{
    //Build theme list
    mThemeList = CpThemeUtil::buildThemeList();
    
    //Look into theme paths and add a file watcher for it
    //to get notified when themes are added.
    QStringList themesPathList = CpThemeUtil::themePathList();
    foreach (const QString &path, themesPathList) {
        QDir themeDir;
        themeDir.setPath( path ) ;
        QStringList list = themeDir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot,QDir::Name);
        if(list.contains("themes", Qt::CaseSensitive )) {
            mFileWatcher->addPath(themeDir.path() + "/themes/");
        }
    }
    connect(mFileWatcher, SIGNAL(directoryChanged(const QString&)),
           this, SLOT(themeListChanged()));
       
    // data for the list which appears after the themes:
    CpThemeInfo fetchFromStore;
    fetchFromStore.setName(hbTrId("txt_cp_list_get_more_tones"));
    fetchFromStore.setItemType(CpThemeInfo::ThemeListItemType_URL);
    fetchFromStore.setItemData(QString("http://lr.ovi.mobi/store/themes"));
    fetchFromStore.setIcon(HbIcon("qtg_large_ovistore"));
    mBottomThemeList.append(fetchFromStore);
}

/*
    Destructor
*/
CpThemeListModel::~CpThemeListModel()
{
    delete mFileWatcher;
    mFileWatcher = 0;

}

/*
    Reimplemented from QAbstractListModel.
*/
int CpThemeListModel::rowCount(const QModelIndex&) const
{
    return mTopThemeList.size() +
        mThemeList.size() +
        mBottomThemeList.size();
}

/*
    Reimplemented from QAbstractListModel. 
*/
QVariant CpThemeListModel::data(const QModelIndex& index, int role) const
{
    QVariant retVal = QVariant();

    if (index.isValid()) {
        // figure out which list we're in and do the appropriate mapping
        int row = index.row();
        const QList<CpThemeInfo> *list = 0;
        if (row < mTopThemeList.size()) {
            list = &mTopThemeList;
        } else {
            row -= mTopThemeList.size();
            if ( row < mThemeList.size() ) {
                list = &mThemeList;
            } else {
                row -= mThemeList.size();
                if ( row < mBottomThemeList.size() ) {
                    list = &mBottomThemeList;
                }
            }
        }

        if (list) {
            switch (role) {
                case Qt::DisplayRole:
                    retVal = list->at(row).name();
                    break;

                case Qt::DecorationRole:
                    retVal = list->at(row).icon();
                    break;

                case Qt::SizeHintRole:
                    retVal = list->at(row).icon().size();
                    break;
                    
                case PortraitPreviewRole:
          	        retVal = list->at(row).portraitPreviewIcon();
                    break;
                    
                case LandscapePreviewRole:
                    retVal = list->at(row).landscapePreviewIcon();
                    break;
                    
                case ItemDataRole:
                    retVal = list->at(row).itemData();
                    break;

                case ItemTypeRole:
                    retVal = QVariant::fromValue<CpThemeInfo::ThemeListItemType>(list->at(row).itemType());
                    break;

                default:
                    // do nothing
                    qt_noop();
            }
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
    if(!mThemeList.empty()) {
        mThemeList.clear();
    }
    mThemeList = CpThemeUtil::buildThemeList();
  
    endResetModel();
}
/*!
 * Returns index of theme infor within the theme list.
 */
int CpThemeListModel::indexOf(const CpThemeInfo& theme) const
{
    return mThemeList.indexOf(theme);
}
