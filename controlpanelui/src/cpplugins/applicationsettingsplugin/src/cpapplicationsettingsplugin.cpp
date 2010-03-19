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

#include "cpapplicationsettingsplugin.h"
#include "cpapplicationsettingsitemdata.h"

CpApplicationSettingsPlugin::CpApplicationSettingsPlugin()
{
}

CpApplicationSettingsPlugin::~CpApplicationSettingsPlugin()
{
}


QList<CpSettingFormItemData*> CpApplicationSettingsPlugin::createSettingFormItemData(CpItemDataHelper &/*itemDataHelper*/) const
{
	return QList<CpSettingFormItemData*>() << new CpApplicationSettingsItemData();
}

Q_EXPORT_PLUGIN2(cpapplicationsettingsplugin, CpApplicationSettingsPlugin);
