/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Private implementation of the theme changer.
 *
*/

#include "cpthemechanger.h"
#include "cpthemechanger_p.h"
#include <QStringList>
#include <QSettings>
#include <QFileSystemWatcher>
#include <QSizeF>
#include <hbicon.h>
#include <hbinstance.h>
#include "cpthemeclient_p.h"
#include "cpthemecommon_p.h"
#ifdef Q_OS_SYMBIAN
#include "cpthemeclientsymbian_p.h"
#else
#include "cpthemeclientqt_p.h"
#endif

namespace {
#if !defined(Q_OS_SYMBIAN)
    #include <stdio.h>
    static const char* KThemePathKey = "HB_THEMES_DIR";
#endif

    static const QString KDefaultTheme = "hbdefault";
    static const char* KSettingsCategory = "currenttheme";
    static const bool KDefaultPreviewAvailable = true;
}

CpThemeChangerPrivate::CpThemeChangerPrivate(CpThemeChanger* qq):
    q_ptr(qq),
    themeClient(CpThemeClient::global()),
    fileWatcher(new QFileSystemWatcher(qq)),
    model(this, qq)

{
    Q_Q(CpThemeChanger);

    // Figure out where our themes are. This is platform-dependent,
    // but not worth breaking out into platform-private implementations
    // at the moment. Ideally, this would be given to us by the theme server,
    #ifdef Q_OS_WIN
    static char* _path=NULL;
    static size_t _size=0;
    _dupenv_s(&_path, &_size, KThemePathKey);
    themeRootPath = QString(_path);
    themeRootPathPostfix = QString();
    free(_path);
    #elif defined(Q_OS_SYMBIAN)
    themeRootPath = "c:\\resource\\hb";
    themeRootPathPostfix  = "resource\\hb";
    #elif defined(Q_OS_UNIX)
    themeRootPath = QString(getenv(KThemePathKey));
    themeRootPathPostfix = QString();
    #elif defined(Q_OS_MACX)
    themeRootPath = QDir::homePath() + '/' + "Library" + QString("hb");
    themeRootPathPostfix = QString();
    #else
    themeRootPath = "c:\\resource\\hb";
    themeRootPathPostfix = QString();
    #endif

    // Get our current state
    QSettings settings(QLatin1String(ORGANIZATION), QLatin1String(THEME_COMPONENT));
    mCurrentTheme.name = settings.value(KSettingsCategory).toString();
    updateThemeList(mCurrentTheme.name);

    // Watch for changes to the theme directory in flash.
    // This may change once we start offering a model.
    fileWatcher->addPath(themeRootPath+"/themes/");
    q->connect(fileWatcher, SIGNAL(directoryChanged(const QString&)),
               q, SLOT(_q_themeDirectoryChanged(const QString&)));

    // Connect to the theme server
    connectToServer();
}

CpThemeChangerPrivate::~CpThemeChangerPrivate()
{
    themeClient->releaseInstance();
}

const CpThemeChanger::ThemeInfo& CpThemeChangerPrivate::currentTheme() const
{
    return mCurrentTheme;
}

const QString& CpThemeChangerPrivate::currentThemeName() const
{
    return mCurrentTheme.name.isEmpty() ? KDefaultTheme : mCurrentTheme.name;
}

int CpThemeChangerPrivate::indexOf(const CpThemeChanger::ThemeInfo& theme) const
{
    return themeList.indexOf(theme);
}

void CpThemeChangerPrivate::updateThemeList(const QString& newThemeName)
{
    if(!themeList.isEmpty()) {
        themeList.clear();
    }

    mCurrentTheme.name = newThemeName.isEmpty() ? KDefaultTheme : newThemeName;

    // Get the list of Drives here
    QStringList themesPathList;

#if defined(Q_OS_WIN32)
    themesPathList << themeRootPath;
#elif defined(Q_OS_SYMBIAN)
    QFileInfoList driveInfoList = QDir::drives();
    foreach (const QFileInfo &driveInfo, driveInfoList) {
        const QString drive = driveInfo.absolutePath();
        themesPathList << drive + themeRootPathPostfix;
    }
#elif defined(Q_OS_UNIX)
    themesPathList << themeRootPath;
#elif defined(Q_OS_MACX)
    themesPathList << themeRootPath;
#endif

    foreach (const QString &path, themesPathList) {
        QDir themeDir;
        themeDir.setPath( path ) ;
        QStringList iconthemeslist;
        QStringList list = themeDir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot,QDir::Name);
        CpThemeChanger::ThemeInfo nameIconPair;

        if(list.contains("themes",Qt::CaseSensitive )) {
            QDir root(themeDir.path());
            themeDir.setPath(root.path()+"/themes/icons/") ;
            iconthemeslist=themeDir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot,QDir::Name);
            foreach(QString themefolder, iconthemeslist) {
                QDir iconThemePath(root.path()+"/themes/icons/"+themefolder);
                if(iconThemePath.exists("index.theme") &&
                   (iconThemePath.exists("scalable") || iconThemePath.exists("pixmap") )) {
                    QSettings iniSetting(iconThemePath.path()+"/index.theme",QSettings::IniFormat);
                    iniSetting.beginGroup("Icon Theme");
                    QString hidden = iniSetting.value("Hidden").toString();
                    QString name = iniSetting.value("Name").toString();
                    QString iconPath = iniSetting.value("PreviewIconPath").toString();
                    if (name.isEmpty()) {
                        continue;
                    }
                    if (!KDefaultPreviewAvailable && iconPath.isEmpty())
                    {
                        continue;
                    }
                    QString fullPathToIcon(iconThemePath.path() + iconPath);
                    // Don't treat this as a theme unless it's got a preview.
                    if (!QFileInfo(fullPathToIcon).exists() && !KDefaultPreviewAvailable) {
                        continue;
                    }
                    if(iconPath.isEmpty()){
                        
                        //if no preview graphics path specified,look for the background graphic.
                        //first look in /scalable folder.  if not there, look in pixmap folder.
                    
                        //Set thumbnail
                        if(QFileInfo(fullPathToIcon + "/scalable/qtg_graf_theme_preview_thumbnail.svg").exists()){
                            nameIconPair.icon = HbIcon(fullPathToIcon + "/scalable/qtg_graf_theme_preview_thumbnail.svg");
                        }else if(QFileInfo(fullPathToIcon + "/scalable/qtg_graf_screen_bg_prt.svg").exists()){
                            nameIconPair.icon = HbIcon(fullPathToIcon + "/scalable/qtg_graf_screen_bg_prt.svg");
                            qreal width = nameIconPair.icon.width();
                            nameIconPair.icon.setHeight(width);
                        } else if(QFileInfo(fullPathToIcon + "/pixmap/qtg_graf_screen_bg_prt.png").exists()){
                            nameIconPair.icon = HbIcon(fullPathToIcon + "/pixmap/qtg_graf_screen_bg_prt.png");
                            qreal width = nameIconPair.icon.width();
                            nameIconPair.icon.setHeight(width);
                        } else{
                            nameIconPair.icon = HbIcon(":/image/themePreview.svg");
                        }
                    } else {
                        nameIconPair.icon = HbIcon(fullPathToIcon);
                    }
                    
                    //set portrait preview
                    if(QFileInfo(fullPathToIcon + "/scalable/qtg_graf_theme_preview_prt.svg").exists()){
                        nameIconPair.portraitPreviewIcon = HbIcon(fullPathToIcon + "/scalable/qtg_graf_theme_preview_prt.svg");
                    }else if(QFileInfo(fullPathToIcon + "/scalable/qtg_graf_screen_bg_prt.svg").exists()){
                        nameIconPair.portraitPreviewIcon = HbIcon(fullPathToIcon + "/scalable/qtg_graf_screen_bg_prt.svg");
                    } else if(QFileInfo(fullPathToIcon + "/pixmap/qtg_graf_screen_bg_prt.png").exists()){
                        nameIconPair.portraitPreviewIcon = HbIcon(fullPathToIcon + "/pixmap/qtg_graf_screen_bg_prt.png");
                    } else{
                        nameIconPair.portraitPreviewIcon = HbIcon(":/image/themePreview.svg");
                    }
                    
                    //set landscape preview
                    if(QFileInfo(fullPathToIcon + "/scalable/qtg_graf_theme_preview_lsc.svg").exists()){
                        nameIconPair.landscapePreviewIcon = HbIcon(fullPathToIcon + "/scalable/qtg_graf_theme_preview_lsc.svg");
                    }else if(QFileInfo(fullPathToIcon + "/scalable/qtg_graf_screen_bg_lsc.svg").exists()){
                        nameIconPair.landscapePreviewIcon = HbIcon(fullPathToIcon + "/scalable/qtg_graf_screen_bg_lsc.svg");
                    } else if(QFileInfo(fullPathToIcon + "/pixmap/qtg_graf_screen_bg_lsc.png").exists()){
                        nameIconPair.landscapePreviewIcon = HbIcon(fullPathToIcon + "/pixmap/qtg_graf_screen_bg_lsc.png");
                    } else{
                        nameIconPair.landscapePreviewIcon = HbIcon(":/image/themePreview.svg");
                    }
                
                    nameIconPair.name = name;
                                        
                    themeList.append(nameIconPair);

                    if (name == mCurrentTheme.name) {
                        mCurrentTheme = nameIconPair;
                    }

                    iniSetting.endGroup();
                    if((hidden == "true") ||( hidden == "")||(name!=themefolder) ) {
                        iconthemeslist.removeOne(themefolder);
                        if(!themeList.isEmpty()) {
                            themeList.removeLast();
                        }
                    }
                } else {
                    iconthemeslist.removeOne(themefolder);
                    if(!themeList.isEmpty()) {
                        themeList.removeLast();
                    }
                }
            }
        }
    }
    // Include default
    CpThemeChanger::ThemeInfo def;
    def.name = KDefaultTheme;
    def.icon = HbIcon(":/image/themePreview.svg");
    themeList.append(def);
    if (mCurrentTheme.name == KDefaultTheme)
    {
        mCurrentTheme = def;
    }

}

const QList<CpThemeChanger::ThemeInfo>& CpThemeChangerPrivate::themes() const
{
    return themeList;
}

bool CpThemeChangerPrivate::connectToServer()
{
    return themeClient->connectToServer();
}

/**
 * Indicate if the client is connected to the server
 */
bool CpThemeChangerPrivate::isConnected() const
{
    return themeClient->isConnected();
}

/**
 * Change a theme
 */
bool CpThemeChangerPrivate::changeTheme(const QString& newTheme)
{
    bool result = false;
    // Skip doing this if the request is for the current theme
    if (newTheme.isEmpty() || newTheme == mCurrentTheme.name) return result;

    // Make sure it's a valid theme name
    bool exists = false;
    QList<CpThemeChanger::ThemeInfo> themeList = themes();
    QList<CpThemeChanger::ThemeInfo>::const_iterator i;
    for (i = themeList.constBegin(); i != themeList.constEnd(); ++i) {
        if ( newTheme == i->name) {
            exists = true;
            break;
        }
    }

    if (exists) {
        result = themeClient->changeTheme(newTheme);
		updateThemeList(newTheme);
}
    return result;
}

void CpThemeChangerPrivate::_q_themeDirectoryChanged(const QString&)
{
    updateThemeList();
}

/*
    HbThemeChangerModel provides an interface to the data contained in the
    HbThemeChanger using QAbstractListModel.
*/

/*
    Constructor
*/
HbThemeListModel::HbThemeListModel(CpThemeChangerPrivate *dd, QObject* parent)
    : QAbstractListModel(parent)
    , mThemeChangerPrivate(dd)
{
    connect(dd->fileWatcher, SIGNAL(directoryChanged(const QString&)),
        this, SLOT(themeListChanged()));
}

/*
    Destructor
*/
HbThemeListModel::~HbThemeListModel()
{

}

/*
    Reimplemented from QAbstractListModel.
*/
int HbThemeListModel::rowCount(const QModelIndex&) const
{
    return mThemeChangerPrivate->themeList.size();
}

/*
    Reimplemented from QAbstractListModel.  Provides the data for Qt::DisplayRole and
    Qt::DecorationRole.
*/
QVariant HbThemeListModel::data(const QModelIndex& index, int role) const
{
    QVariant retVal = QVariant();

    if (index.isValid()) {
        switch (role) {
            case Qt::DisplayRole:
                retVal = mThemeChangerPrivate->themeList.at(index.row()).name;
                break;

            case Qt::DecorationRole:
                retVal = mThemeChangerPrivate->themeList.at(index.row()).icon;
                break;

        case Qt::SizeHintRole:
                retVal = mThemeChangerPrivate->themeList.at(index.row()).icon.size();
                break;
        case CpThemeChanger::PortraitPreviewRole:
                retVal = mThemeChangerPrivate->themeList.at(index.row()).portraitPreviewIcon;
                break;
        case CpThemeChanger::LandscapePreviewRole:
                retVal = mThemeChangerPrivate->themeList.at(index.row()).landscapePreviewIcon;

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
void HbThemeListModel::themeListChanged()
{
    beginResetModel();

    mThemeChangerPrivate->themes();

    endResetModel();
}

// End of file
