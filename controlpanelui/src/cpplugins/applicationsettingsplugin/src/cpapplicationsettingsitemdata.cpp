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
#include "cpapplicationsettingsitemdata.h"
#include <QString>
#include <QtAlgorithms>
//#include <cppluginloader.h>
#include <cpplugininterface.h>
#include <cpsettingformentryitemdata.h>

CpApplicationSettingsItemData::CpApplicationSettingsItemData()
:  CpCategorySettingFormItemData(HbDataFormModelItem::GroupItem,
								 tr("Application Settings"),
								 QString("cpapplicationsettingsplugin.cpcfg"))
{
}

CpApplicationSettingsItemData::~CpApplicationSettingsItemData()
{
}

/*
struct ItemDataNameLessThan
{
	bool operator() (CpSettingFormItemData *left,CpSettingFormItemData *right) const {
		QString leftName;
		if (CpSettingFormEntryItemData *leftEntry = qobject_cast<CpSettingFormEntryItemData*>(left)) {
			leftName = leftEntry->text();
		}
		else {
			leftName = left->label();
		}

		QString rightName;
		if (CpSettingFormEntryItemData *rightEntry = qobject_cast<CpSettingFormEntryItemData*>(right)) {
			rightName = rightEntry->text();
		}
		else {
			rightName = right->label();
		}
	
		return leftName.compare(rightName,Qt::CaseInsensitive) < 0;
	}
};
*/

void CpApplicationSettingsItemData::afterLoadingConfigPlugins(CpItemDataHelper &/*itemDataHelper*/)
{
    /*
	QList<CpSettingFormItemData*> allItemData;

	QList<CpPluginInterface*> appplugins = CpPluginLoader::loadApplicationCpPlugins();
	foreach (CpPluginInterface *plugin, appplugins) {
		allItemData += plugin->createSettingFormItemData(itemDataHelper);
	}

	//sort by name
	qSort(allItemData.begin(),allItemData.end(),ItemDataNameLessThan());

	foreach(CpSettingFormItemData *itemData,allItemData) {
		appendChild(itemData);
	}
    */
}

// End of File
