/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "extendedlocaleutil.h"

#if defined(Q_OS_SYMBIAN)
#include <QFile>
#include <QTextStream>
#include <QStringList>

#define LANGUAGE_LIST_FILE "/resource/hbi18n/translations/language_list.txt"
#define LANGUAGE_MAPPINGS_FILE "/resource/hbi18n/translations/locale_mappings.txt"
#endif // Q_OS_SYMBIAN

#include "cplanguagepluginlog.h"

/*!
    @beta
    @hbcore
    \class ExtendedLocaleUtil
    \brief ExtendedLocaleUtil provides mappings needed for Control Panel plugin which are not available through HbLocaleUtil class.
*/


/*!
    \brief Return list of language, region, collation and writing language mappings.
    
    \attention Symbian specific API

    \return a list containing mapped locale properties
*/ 

//#define __TESTCODE__

QList<LanguageRegionMapping> ExtendedLocaleUtil::localeMappings(bool onlySupported)
{
    CPLANG_LOG_FUNC_ENTRY("ExtendedLocaleUtil::localeMappings")
    
    QList<LanguageRegionMapping> mps = allLocaleMappings();
    
    if (!onlySupported) {
        return mps;
    }
    
    QList<LanguageRegionMapping> supportedmps;
    
    QStringList supportedLanguages = HbLocaleUtil::supportedLanguages();
    QStringList supportedRegions = HbLocaleUtil::supportedRegions();
    
    foreach (const QString &language,supportedLanguages) {
        LanguageRegionMapping *found = 0;
        for (int i = 0; i < mps.count();i++) {
            if (mps.at(i).languageId == language) {
                found = const_cast<LanguageRegionMapping*>(&mps.at(i));
                break;
            }
        }
        if (found) {
            supportedmps.append(*found);
        }
        else {
            LanguageRegionMapping map;
            map.languageId = language;
            map.primaryWritingLan = HbInputLanguage(QLocale(language).language());
        }
        
    }
    
    return supportedmps;
}

QList<LanguageRegionMapping> ExtendedLocaleUtil::allLocaleMappings()
{
    CPLANG_LOG_FUNC_ENTRY("ExtendedLocaleUtil::allLocaleMappings")
    
    QList<LanguageRegionMapping> mps;
    
#if defined(Q_OS_SYMBIAN)
   
    QString path = "c:";
    path += QString(LANGUAGE_MAPPINGS_FILE);
    QFile* file = new QFile(path);
    if (!file->exists() ) {
        path = "z:";
        path += QString(LANGUAGE_MAPPINGS_FILE);
        delete file;
        file = new QFile(path);
    }
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        delete file;
        return mps;
    }

    QTextStream in(file);
    while (!in.atEnd()) {
        QString line = in.readLine(256);
        if (!line.isEmpty()) {
            QStringList list = line.split(',', QString::SkipEmptyParts);
            if (list.count() < 7) {
                continue;
            }

            QString strCode = list[0];
            QString lanName = list[4];
            QString region = list[5];
            QString collation = list[6];
            
            bool ok;
            int code = strCode.toUInt(&ok);
            if (!ok) {
                continue;
            }
                       
            QString lanPart =  (lanName.indexOf('_')>0) ? lanName.left(lanName.indexOf('_')) : lanName;
            QString regPart = (region.indexOf('_')>0) ? region.left(region.indexOf('_')) : region;           
            QLocale loc = QLocale(QString(lanPart+'_'+regPart));             
            HbInputLanguage  primaryWriting = HbInputLanguage(loc.language(), loc.country());
            HbInputLanguage  secondaryWriting = HbInputLanguage(QLocale::English, QLocale::UnitedKingdom);
            
            LanguageRegionMapping map;
            map.languageId = lanName;
            map.regionId = region;
            map.collationId = collation;
            map.primaryWritingLan = primaryWriting;
            map.secondaryWritingLan = secondaryWriting;
            mps.append(map);
            
        }
    }
    delete file;

#endif // Q_OS_SYMBIAN
    

    return mps;
}
