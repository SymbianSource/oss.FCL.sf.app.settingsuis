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

#ifndef FIRSTPLUGINFORCPCATEGORYMODEL_H
#define FIRSTPLUGINFORCPCATEGORYMODEL_H

#include <qobject.h>
#include <cpplugininterface.h>
#include <cplauncherinterface.h>

class FirstPluginForCpCategoryModel : public QObject, public CpPluginInterface, public CpLauncherInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
    Q_INTERFACES(CpLauncherInterface)
public:
    FirstPluginForCpCategoryModel();
    virtual ~FirstPluginForCpCategoryModel();
    virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;
    virtual CpBaseSettingView *createSettingView(const QVariant &hint) const;
};

#endif
