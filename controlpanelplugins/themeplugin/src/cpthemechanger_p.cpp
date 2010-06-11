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

#include "cpthemechanger.h"
#include "cpthemechanger_p.h"
#include "cpthemelistmodel.h"
#include "cpthemeclient_p.h"
#include "cpthemecommon_p.h"

#include <QStringList>
#include <QSettings>
#include <QFileSystemWatcher>
#include <QPixmap>
#include <QFileInfoList>
#include <QDir>

#include <hbicon.h>
#include <hbinstance.h>


#if !defined(Q_OS_SYMBIAN)
    #include <stdio.h>
    static const char* KThemePathKey = "HB_THEMES_DIR";
#endif
    static const QString KDefaultTheme = "sfblacktheme";
    
    static const QString KPreviewThumbnailSVG = "/scalable/qtg_graf_theme_preview_thumbnail.svg";
    
    static const QString KPreviewPrtSVG =       "/scalable/qtg_graf_theme_preview_prt.svg";
    static const QString KPreviewLscSVG =       "/scalable/qtg_graf_theme_preview_lsc.svg";
    
    static const QString KBackgroundPrtSVG =    "/scalable/qtg_graf_screen_bg_prt.svg";
    static const QString KBackgroundLscSVG =    "/scalable/qtg_graf_screen_bg_lsc.svg";
    
    static const QString KBackgroundPrtPNG =    "/pixmap/qtg_graf_screen_bg_prt.png";                     
    static const QString KBackgroundLscPNG =    "/pixmap/qtg_graf_screen_bg_lsc.png";
                     
   

CpThemeChangerPrivate::CpThemeChangerPrivate(CpThemeChanger* qq):
    q_ptr(qq),
    mThemeClient(CpThemeClient::global()),
    mFileWatcher(new QFileSystemWatcher(qq)),
    mModel(this, qq)

{
    Q_Q(CpThemeChanger);

    // Figure out where our themes are. This is platform-dependent,
    // but not worth breaking out into platform-private implementations
    // at the moment. Ideally, this would be given to us by the theme server,
#ifdef Q_OS_WIN
    static char* _path = NULL;
    static size_t _size = 0;
    _dupenv_s(&_path, &_size, KThemePathKey);
    mThemeRootPath = QString(_path);
    mThemeRootPathPostfix = QString();
    free(_path);
#elif defined(Q_OS_SYMBIAN)
    mThemeRootPath = "c:\\resource\\hb";
    mThemeRootPathPostfix = "resource\\hb";
#elif defined(Q_OS_MACX)
    mThemeRootPath = QDir::homePath() + '/' + "Library" + QString("hb");
    mThemeRootPathPostfix = QString();
#elif defined(Q_OS_UNIX)
    mThemeRootPath = QString(getenv(KThemePathKey));
    mThemeRootPathPostfix = QString();
#else
    mThemeRootPath = "c:\\resource\\hb";
    mThemeRootPathPostfix = QString();
#endif

    // Get our current state
    if (HbInstance::instance()) {
        HbTheme *hbTheme = HbInstance::instance()->theme();
        if (hbTheme) {
            mCurrentTheme.name = hbTheme->name();
        }
    }
  
    // Watch for changes to the theme directory in flash.
    // This may change once we start offering a model.
#if defined(Q_OS_SYMBIAN)
    QFileInfoList driveInfoList = QDir::drives();
    foreach (const QFileInfo &driveInfo, driveInfoList) {
        const QString drive = driveInfo.absolutePath();
        mThemesPathList << drive + mThemeRootPathPostfix;
    }
#else
    mThemesPathList << mThemeRootPath;
#endif
    foreach (const QString &path, mThemesPathList) {
        QDir themeDir;
        themeDir.setPath( path ) ;
        QStringList list = themeDir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot,QDir::Name);
        if(list.contains("themes", Qt::CaseSensitive )) {
            mFileWatcher->addPath(themeDir.path() + "/themes/");
        }
    }
    
    q->connect(mFileWatcher, SIGNAL(directoryChanged(const QString&)),
               q, SLOT(_q_themeDirectoryChanged(const QString&)));

    updateThemeList(mCurrentTheme.name);

    // Connect to the theme server
    connectToServer();
}

CpThemeChangerPrivate::~CpThemeChangerPrivate()
{
    mThemeClient->releaseInstance();
    mThemeClient = 0;
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
    return mThemeList.indexOf(theme);
}

void CpThemeChangerPrivate::updateThemeList(const QString& newThemeName)
{
    if(!mThemeList.isEmpty()) {
        mThemeList.clear();
    }

    mCurrentTheme.name = newThemeName.isEmpty() ? KDefaultTheme : newThemeName;

    foreach (const QString &path, mThemesPathList) {
        QDir themeDir;
        themeDir.setPath( path ) ;
        QStringList iconthemeslist;
        QStringList list = themeDir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot,QDir::Name);
        CpThemeChanger::ThemeInfo nameIconPair;

        if(list.contains("themes", Qt::CaseSensitive )) {
            QDir root(themeDir.path());
            themeDir.setPath(root.path() + "/themes/icons/") ;
            iconthemeslist = themeDir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot,QDir::Name);
            foreach(QString themefolder, iconthemeslist) {
                QDir iconThemePath(root.path() + "/themes/icons/" + themefolder);
                if(iconThemePath.exists("index.theme") &&
                   (iconThemePath.exists("scalable") || iconThemePath.exists("pixmap") )) {
                    QSettings iniSetting(iconThemePath.path() + "/index.theme", QSettings::IniFormat);
                    iniSetting.beginGroup("Icon Theme");
                    QString hidden = iniSetting.value("Hidden").toString();
                    QString name = iniSetting.value("Name").toString();
                    QString iconPath = iniSetting.value("PreviewThumbnailPath").toString();
                    QString previewPathPrt = iniSetting.value("PreviewIconPath_prt").toString();
                    QString previewPathLsc = iniSetting.value("PreviewIconPath_lsc").toString();
                    if (name.isEmpty()) {
                        continue;
                    }
                    
                    QString fullPathToIcon(iconThemePath.path() + iconPath);
                    
                    if(iconPath.isEmpty()|| !QFileInfo(fullPathToIcon).exists()){
                       //Set thumbnail
                        if(QFileInfo(fullPathToIcon + KPreviewThumbnailSVG).exists()){
                            nameIconPair.icon = HbIcon(fullPathToIcon + KPreviewThumbnailSVG);
                        }else if(QFileInfo(fullPathToIcon + KBackgroundPrtSVG).exists()){
                            nameIconPair.icon = HbIcon(fullPathToIcon + KBackgroundPrtSVG);
                        } else if(QFileInfo(fullPathToIcon + KBackgroundPrtPNG).exists()){
                            nameIconPair.icon = HbIcon(fullPathToIcon + KBackgroundPrtPNG);
                        } else{
                            nameIconPair.icon = HbIcon(":/image/themePreview.svg");
                        }
                    } else {
                        nameIconPair.icon = HbIcon(fullPathToIcon);
                    }
                    
                    //Portrait preview
                    QString fullPathToPreviewPrt = (iconThemePath.path() + previewPathPrt );
                    
                    if(previewPathPrt.isEmpty() || !QFileInfo(fullPathToPreviewPrt).exists()) {
                    
                        if(QFileInfo(fullPathToPreviewPrt + KPreviewPrtSVG).exists()){
                            nameIconPair.portraitPreviewIcon = HbIcon(fullPathToPreviewPrt + KPreviewPrtSVG);
                        }else if(QFileInfo(fullPathToPreviewPrt + KBackgroundPrtSVG).exists()){
                            nameIconPair.portraitPreviewIcon = HbIcon(fullPathToPreviewPrt + KBackgroundPrtSVG);
                        } else if(QFileInfo(fullPathToPreviewPrt + KBackgroundPrtPNG).exists()){
                            nameIconPair.portraitPreviewIcon = HbIcon(fullPathToPreviewPrt + KBackgroundPrtPNG);
                        } else{
                            nameIconPair.portraitPreviewIcon = HbIcon(":/image/themePreview.svg");
                        }
                    }
                    else {
                        nameIconPair.portraitPreviewIcon = HbIcon(fullPathToPreviewPrt);
                    }
                    
                    //Landscape preview
                    QString fullPathToPreviewLsc = (iconThemePath.path() + previewPathLsc );
                                    
                    if(previewPathLsc.isEmpty() || !QFileInfo(fullPathToPreviewLsc).exists()) {
                                
                        if(QFileInfo(fullPathToPreviewLsc + KPreviewLscSVG).exists()){
                            nameIconPair.landscapePreviewIcon = HbIcon(fullPathToPreviewLsc + KPreviewLscSVG);
                        }else if(QFileInfo(fullPathToPreviewLsc + KBackgroundLscSVG).exists()){
                            nameIconPair.landscapePreviewIcon = HbIcon(fullPathToPreviewLsc + KBackgroundLscSVG);
                        } else if(QFileInfo(fullPathToPreviewLsc + KBackgroundLscPNG).exists()){
                            nameIconPair.landscapePreviewIcon = HbIcon(fullPathToPreviewLsc + KBackgroundLscPNG);
                        } else{
                            nameIconPair.landscapePreviewIcon = HbIcon(":/image/themePreview.svg");
                        }
                    }
                    else {
                        nameIconPair.landscapePreviewIcon = HbIcon(fullPathToPreviewLsc);
                    }
                
                    nameIconPair.name = name;
                                        
                    mThemeList.append(nameIconPair);

                    if (name == mCurrentTheme.name) {
                        mCurrentTheme = nameIconPair;
                    }

                    iniSetting.endGroup();
                    if((hidden == "true") ||( hidden == "")||(name != themefolder) ) {
                        iconthemeslist.removeOne(themefolder);
                        if(!mThemeList.isEmpty()) {
                            mThemeList.removeLast();
                        }
                    }
                } else {
                    iconthemeslist.removeOne(themefolder);
                    if(!mThemeList.isEmpty()) {
                        mThemeList.removeLast();
                    }
                }
            }
        }
    }
    
    if (mCurrentTheme.name == KDefaultTheme)
    {
        // Include default
         CpThemeChanger::ThemeInfo def;
         def.name = KDefaultTheme;
         def.icon = HbIcon(":/image/themePreview.svg");
         mThemeList.append(def);
  
         mCurrentTheme = def;
    }

}

const QList<CpThemeChanger::ThemeInfo>& CpThemeChangerPrivate::themes() const
{
   return mThemeList;
}

bool CpThemeChangerPrivate::connectToServer()
{
    return mThemeClient->connectToServer();
}

/**
 * Indicate if the client is connected to the server
 */
bool CpThemeChangerPrivate::isConnected() const
{
    return mThemeClient->isConnected();
}

/**
 * Change a theme
 */
bool CpThemeChangerPrivate::changeTheme(const QString& newTheme)
{
    bool result = false;
    // Skip doing this if the request is for the current theme
    if (newTheme.isEmpty() || newTheme == mCurrentTheme.name) {
        return result;
    }

    // Make sure it's a valid theme name and set the current theme.
    bool exists = false;
    QList<CpThemeChanger::ThemeInfo>::const_iterator i;
    for (i = mThemeList.constBegin(); i != mThemeList.constEnd(); ++i) {
        if ( newTheme == i->name) {
            exists = true;
            break;
        }
    }

    if (exists) {
        result = mThemeClient->changeTheme(newTheme);
        if(result) {
            mCurrentTheme = *i;
        }
    }
    return result;
}

void CpThemeChangerPrivate::_q_themeDirectoryChanged(const QString&)
{
    updateThemeList();
}



// End of file
