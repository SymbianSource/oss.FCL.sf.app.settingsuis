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
#include "cputility.h"
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <cpbasepath.h>
#include <cppluginloader.h>
#include <cpplugininterface.h>
#include <cppluginplatinterface.h>
#include <cplogger.h>
#include <cpsettingformentryitemdata.h>
#include "cpcategorysettingformitemdata.h"
#include "cppluginconfigreader.h"
#include "cpplaceholderitemdata.h"


static bool setEntryItemContentIfEmpty(CpSettingFormItemData *itemData, 
        const CpPluginConfig &pluginConfig)
{
    CpSettingFormEntryItemData *entryItemData = qobject_cast< CpSettingFormEntryItemData* > (itemData);
    if (!entryItemData) {
        return false;
    }
    
    if (entryItemData->text().isEmpty()) {
        entryItemData->setText(pluginConfig.mDisplayName);
    }
    if (entryItemData->description().isEmpty()) {
        entryItemData->setDescription(pluginConfig.mDescription);
    }
    if (entryItemData->iconName().isEmpty()) {
        entryItemData->setIconName(
                  CP_RESOURCE_PATH
                  + QDir::separator() 
                  + ICON_SUB_PATH 
                  + QDir::separator() 
                  + QLatin1String("qgn_prop_set_default_sub.svg") );
    }
    
    return true;
}


void CpUtility::buildConfigPluginItems(HbDataFormModelItem *parent,
									   const QString &configFile,
									   CpItemDataHelper &itemDataHelper)
{
    if (!parent) {
        return;
    }

	QString configPath(configFile);
	QFileInfo fi(configFile);
    //if it is a relative path, search the config file from device drives.
	if (!fi.isAbsolute()) {
		QStringList dirs = CpUtility::configFileDirectories();
		foreach(const QString &dir,dirs) {
			configPath = dir + fi.fileName();
			if (QFileInfo(configPath).exists()) {
                CPFW_LOG(configPath + " has been found.");
				break;		
			}
		}
	}
    	
    QList<CpPluginConfig> pluginConfigs = CpPluginConfigReader(configPath).readCpPluginConfigs();
		
	foreach(const CpPluginConfig &pluginConfig, pluginConfigs)  {
        CPFW_LOG(QLatin1String("Load plugin: ") + pluginConfig.mPluginFile + " from " + configPath);

		QList<CpSettingFormItemData*> itemDataList;
		
        //firstly, handle CpPluginInterface
		if (CpPluginInterface *plugin = CpPluginLoader().loadCpPluginInterface(pluginConfig.mPluginFile)) {
            CPFW_LOG("Load  root component CpPluginInterface succeed.");
			itemDataList = plugin->createSettingFormItemData(itemDataHelper);
		}
        //handle CpPluginPlatInterface
		else if (CpPluginPlatInterface *plugin = CpPluginLoader().loadPlatCpPlugin(pluginConfig.mPluginFile)) {
            CPFW_LOG("Load  root component CpPluginPlatInterface succeed.");
            CPFW_LOG("WARNING: CpPluginPlatInterface is deprecated. Please return CpPluginInterface from plugin.");
			CpSettingFormItemData *itemData = plugin->createSettingFormItemData(itemDataHelper);
			if (itemData) {
				itemDataList.append(itemData);
			}
		}

		else {
            CPFW_LOG(QLatin1String("Load plugin:") + pluginConfig.mPluginFile + QLatin1String(" failed."));
            #ifdef _DEBUG
              CPFW_LOG(QLatin1String("***Add a placeholder."));
              itemDataList.append(new CpPlaceHolderItemData(itemDataHelper,pluginConfig));
            #endif
		}

		if (itemDataList.count()) {
			foreach(CpSettingFormItemData *itemData,itemDataList) {
				if (itemData) {
					//append the new created setting form item to its parent item.
					parent->appendChild(itemData);

					if (CpCategorySettingFormItemData *categoryItemData 
						= qobject_cast<CpCategorySettingFormItemData*>(itemData)) {
						categoryItemData->initialize(itemDataHelper);
					}

					//set the text and description from config if it is empty.
					setEntryItemContentIfEmpty(itemData,pluginConfig);
				}
			} //end foreach
		}

	} //end foreach
}

QStringList CpUtility::drives()
{
	static QStringList drives;

	if (drives.empty()) {
        CPFW_LOG("device drives:");
#ifdef WIN32
		drives.append("C:");
        CPFW_LOG("C:");
#else
		QFileInfoList fileInfoList = QDir::drives();
		foreach(const QFileInfo &fileInfo,fileInfoList) {
			QString str = fileInfo.filePath();
			if (str.length() > 2) {
				str = str.left(2);
			}
			drives.append(str);
            CPFW_LOG(str);
		}
#endif  
	}

	return drives;
}

static QStringList directoriesFromAllDrives(const QString &baseDir)
{
	QStringList dirs;

	QStringList drives = CpUtility::drives();
	foreach(const QString &drive,drives) {
		QString dir = drive + baseDir + QDir::separator();
		if (QFileInfo(dir).exists()) {
			dirs.append(dir);
            CPFW_LOG(dir);
		}
	}

	return dirs;
}

QStringList CpUtility::pluginDirectories()
{
	static QStringList dirs;
	if (dirs.empty()) {
        CPFW_LOG("ControlPanel plugin derectories:")
		dirs = directoriesFromAllDrives(CP_PLUGIN_PATH);
	}
	return dirs;
}

QStringList CpUtility::configFileDirectories()
{
	static QStringList dirs;
	if (dirs.empty()) {
        CPFW_LOG("ControlPanel configuration file derectories:");
		dirs = directoriesFromAllDrives(CP_PLUGIN_CONFIG_PATH);
	}
	return dirs;
}

