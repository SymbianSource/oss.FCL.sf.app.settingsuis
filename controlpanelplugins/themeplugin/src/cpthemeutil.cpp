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

#include "cpthemeinfo.h"
#include "cpthemeutil.h"

#include <QStringList>
#include <QSettings>  
#include <QFileInfoList>
#include <QDir>
#include <QList>

#include <hbicon.h>
#include <hbinstance.h>
#include <restricted/hbthemeservices_r.h>

/*!
 * This class provides utility function to get Theme information.  
 */

#if defined(Q_OS_SYMBIAN)
#include <e32std.h>
#include <centralrepository.h>
        static const TUid KServerUid3={0x20022E82};
        static const TUint32 KDefaultThemeNameKey = 0x2;
  
#endif


    //Location of theme preview and background icons.
    static const QString KPreviewThumbnailNVG = "/scalable/qtg_graf_theme_preview_thumbnail.nvg";
    static const QString KPreviewThumbnailSVG = "/scalable/qtg_graf_theme_preview_thumbnail.svg";
    
    static const QString KPreviewPrtNVG       = "/scalable/qtg_graf_theme_preview_prt.nvg";
    static const QString KPreviewLscNVG       = "/scalable/qtg_graf_theme_preview_lsc.nvg";
    static const QString KPreviewPrtSVG       = "/scalable/qtg_graf_theme_preview_prt.svg";
    static const QString KPreviewLscSVG       = "/scalable/qtg_graf_theme_preview_lsc.svg";
   
    static const QString KBackgroundPrtNVG    = "/scalable/qtg_graf_screen_bg_prt.nvg";
    static const QString KBackgroundLscNVG    = "/scalable/qtg_graf_screen_bg_lsc.nvg";
    static const QString KBackgroundPrtSVG    = "/scalable/qtg_graf_screen_bg_prt.svg";
    static const QString KBackgroundLscSVG    = "/scalable/qtg_graf_screen_bg_lsc.svg";
    
    static const QString KBackgroundPrtPNG    = "/pixmap/qtg_graf_screen_bg_prt.png";                     
    static const QString KBackgroundLscPNG    = "/pixmap/qtg_graf_screen_bg_lsc.png";
    



/*
 * Builds a CpThemeInfo object given theme path and theme name.  It creates the name and 
 * preview icons for the object.  Creates a new CpThemeInfo objects. Caller takes ownership.
 * Returns NULL if can't build the object.
 */
CpThemeInfo* CpThemeUtil::buildThemeInfo(const QString& themePath, const QString& themeName)
{
    CpThemeInfo *themeInfo = new CpThemeInfo();
    QString iconPath;
    
    QString previewPathPrt;
    QString previewPathLsc;
    QString name = themeName;
    QString hidden = "";
    
    //first look into the index.theme file to get theme information.
   
    if(QFileInfo(themePath + "/index.theme").exists()) {
        QSettings iniSetting(themePath + "/index.theme", QSettings::IniFormat);
        iniSetting.beginGroup("Icon Theme");
        name = iniSetting.value("Name").toString();
        hidden = iniSetting.value("Hidden").toString();
        iconPath = iniSetting.value("PreviewThumbnailPath").toString();
        previewPathPrt = iniSetting.value("PreviewIconPath_prt").toString();
        previewPathLsc = iniSetting.value("PreviewIconPath_lsc").toString();
        iniSetting.endGroup();
       
    }
                       
    if(name.isEmpty() || (hidden == "true") ||( hidden == "")) {
        return NULL;
    }
   
    themeInfo->setName(name);
    themeInfo->setItemType(CpThemeInfo::ThemeListItemType_default);
    themeInfo->setItemData(themePath);
          
    //Get the icons. Logic is as follow:
    /* 1. If the icon path is in index.theme and the icon exist, use it.
     * 2. Otherwise look for the icon in the theme folder.
     * 2. If preview icon doesn't exist, use background icon.
     * 3. If no icon exist (background or preview),use default theme icon.
     */
    if(iconPath.isEmpty() || !QFileInfo(themePath + iconPath).exists()){
    //Set thumbnail
        if(QFileInfo(themePath + KPreviewThumbnailNVG).exists()){
            themeInfo->setIcon(HbIcon(themePath + KPreviewThumbnailNVG));
        }else if(QFileInfo(themePath + KPreviewThumbnailSVG).exists()){
            themeInfo->setIcon(HbIcon(themePath + KPreviewThumbnailSVG));
        }else if(QFileInfo(themePath + KBackgroundPrtNVG).exists()){
            themeInfo->setIcon(HbIcon(themePath + KBackgroundPrtNVG));
        } else if(QFileInfo(themePath + KBackgroundPrtSVG).exists()){
            themeInfo->setIcon(HbIcon(themePath + KBackgroundPrtSVG));
        } else if(QFileInfo(themePath + KBackgroundPrtPNG).exists()){
            themeInfo->setIcon(HbIcon(themePath + KBackgroundPrtPNG)); 
        }else{
            themeInfo->setIcon(HbIcon(defaultTheme()->icon()));
        }

    } else {
        themeInfo->setIcon(HbIcon(themePath + iconPath));
    }
          
    //Portrait preview
          
    if(previewPathPrt.isEmpty() || !QFileInfo(themePath + previewPathPrt).exists()) {
          
        if(QFileInfo(themePath + KPreviewPrtNVG).exists()){
            themeInfo->setPortraitPreviewIcon(HbIcon(themePath + KPreviewPrtNVG));
        }else if(QFileInfo(themePath + KPreviewPrtSVG).exists()){
            themeInfo->setPortraitPreviewIcon(HbIcon(themePath + KPreviewPrtSVG));
        }else if(QFileInfo(themePath + KBackgroundPrtNVG).exists()){
            themeInfo->setPortraitPreviewIcon(HbIcon(themePath + KBackgroundPrtNVG));
        } else if(QFileInfo(themePath + KBackgroundPrtSVG).exists()){
            themeInfo->setPortraitPreviewIcon(HbIcon(themePath + KBackgroundPrtSVG));
        } else if(QFileInfo(themePath + KBackgroundPrtPNG).exists()){
            themeInfo->setPortraitPreviewIcon(HbIcon(themePath + KBackgroundPrtPNG));
        } else{
            themeInfo->setPortraitPreviewIcon(HbIcon(defaultTheme()->icon()));
        }
    }
    else {
        themeInfo->setPortraitPreviewIcon(HbIcon(themePath + previewPathPrt));
    }
          
    //Landscape preview
          
    if(previewPathLsc.isEmpty() || !QFileInfo(themePath + previewPathLsc).exists()) {
        if(QFileInfo(themePath + KPreviewLscNVG).exists()){
            themeInfo->setLandscapePreviewIcon(HbIcon(themePath + KPreviewLscNVG));
        }else if(QFileInfo(themePath + KPreviewLscSVG).exists()){
            themeInfo->setLandscapePreviewIcon(HbIcon(themePath + KPreviewLscSVG));
        }else if(QFileInfo(themePath + KBackgroundLscNVG).exists()){
            themeInfo->setLandscapePreviewIcon(HbIcon(themePath + KBackgroundLscNVG));
        } else if(QFileInfo(themePath + KBackgroundLscSVG).exists()){
            themeInfo->setLandscapePreviewIcon(HbIcon(themePath + KBackgroundLscSVG));
        } else if(QFileInfo(themePath + KBackgroundLscPNG).exists()){
            themeInfo->setLandscapePreviewIcon(HbIcon(themePath + KBackgroundLscPNG));
        } else{
            themeInfo->setLandscapePreviewIcon(HbIcon(defaultTheme()->icon()));
        }
    }
    else {
        themeInfo->setLandscapePreviewIcon(HbIcon(themePath + previewPathLsc));
    }
    return themeInfo;

}

/*! Creates a list of CpThemeInfo objects representing theme information.
 *  
 */
QList<CpThemeInfo> CpThemeUtil::buildThemeList()
{
    QList<CpThemeInfo> themeList; 
  
    QList<QPair<QString, QString> > mThemesPathList = availableThemes();
    QPair<QString, QString>pair;
    foreach (pair, mThemesPathList) {
        QDir themeDir;
        QString name = pair.first;
        QString path = pair.second;
        themeDir.setPath( path ) ;
        CpThemeInfo* themeInfo;
        if(themeDir.exists("index.theme") &&
          (themeDir.exists("scalable") || themeDir.exists("pixmap") )) {
            themeInfo = buildThemeInfo(path, name);
            if(themeInfo && !themeInfo->name().isEmpty()) {
                themeList.append(*themeInfo);
            }
        }
    }
    qSort( themeList );
    return themeList;
}

/*!
 * Returns the default theme information. 
 */
CpThemeInfo* CpThemeUtil::defaultTheme()
{
    //static because default theme doesn't change.
    static CpThemeInfo *defaultTheme = 0;
    if(!defaultTheme) {
        defaultTheme = new CpThemeInfo();
    }
    QString defaultThemePath;
    if(defaultTheme->name().isEmpty()) {
       

#ifdef Q_OS_SYMBIAN
        CRepository *repository = 0;
        TRAP_IGNORE(repository = CRepository::NewL(KServerUid3));
        if (repository) {
            TBuf<256> value;
            if (KErrNone == repository->Get((TUint32)KDefaultThemeNameKey, value)) {
                QString qvalue((QChar*)value.Ptr(), value.Length());
                defaultThemePath = qvalue.trimmed();
            }
            value.Zero();
           delete repository;
        }
          
#endif
        defaultTheme = buildThemeInfo(defaultThemePath);

    }
    return defaultTheme;
}


const QStringList CpThemeUtil::themeDirectories(const QList<CpThemeInfo>& themeInfoList)
{
    QStringList themeDirs;
    
    foreach(const CpThemeInfo& themeInfo, themeInfoList) {
        QDir themePath(themeInfo.itemData());
        QString topDir;
        if(themePath.cdUp()) {
            topDir = themePath.path();
            if(!themeDirs.contains(topDir)) {
                themeDirs.append(topDir);
            }
        }
    }
    return themeDirs;
}

const QList< QPair< QString, QString > > CpThemeUtil::availableThemes( )
{
    
    QList<QPair<QString, QString> > result = HbThemeServices::availableThemes();
    return result;
    
}


