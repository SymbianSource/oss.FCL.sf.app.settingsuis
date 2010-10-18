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

#include "secondpluginforcpcategorymodel.h"
#include "secondpluginviewforcpcategorymodel.h"
#include <cpsettingformentryitemdataimpl.h>

SecondPluginForCpCategoryModel::SecondPluginForCpCategoryModel()
{
}

SecondPluginForCpCategoryModel::~SecondPluginForCpCategoryModel()
{
}
QList<CpSettingFormItemData*> SecondPluginForCpCategoryModel::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    return QList<CpSettingFormItemData*>() 
            << new CpSettingFormEntryItemDataImpl<SecondPluginViewForCategoryModel>(
               itemDataHelper,
               tr("The plugin for test"), 
               tr("TestPlugin."));
}

CpBaseSettingView *SecondPluginForCpCategoryModel::createSettingView(const QVariant &hint) const
{
    return 0;
}

Q_EXPORT_PLUGIN2(SecondPluginForCpCategoryModel, SecondPluginForCpCategoryModel);
