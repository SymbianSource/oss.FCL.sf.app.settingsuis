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

#ifndef CPDEVICELOCKPLUGIN_H
#define CPDEVICELOCKPLUGIN_H

#include <QObject>
#include <cpplugininterface.h>

class CpDeviceLockPlugin : public QObject, public CpPluginInterface
{
	Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
public:
    CpDeviceLockPlugin();
    virtual ~CpDeviceLockPlugin();
    virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;
};

#endif //CPDEVICELOCKPLUGIN_H
