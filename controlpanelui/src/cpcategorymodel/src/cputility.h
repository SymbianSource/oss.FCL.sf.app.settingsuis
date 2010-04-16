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
#ifndef CP_UTILITY_H
#define CP_UTILITY_H

#include <QString>

class HbDataFormModelItem;
class CpItemDataHelper;

class CpUtility
{
public:
	/*
		load all controlpanel plugins from configuration file,
		and create model items form the loaded plugins 
		and append model items to given parent
	*/
	static void buildConfigPluginItems(HbDataFormModelItem *parent,
		const QString &configFile,
		CpItemDataHelper &itemDataHelper);
		
	/*
		get all physical drives of the devices
	*/
	static QStringList drives();

	/*
		get all controlpanel plugin directories of the device
	*/
	static QStringList pluginDirectories();


	/*
		get all config directories of the device
	*/
	static QStringList configFileDirectories();


};

#endif
