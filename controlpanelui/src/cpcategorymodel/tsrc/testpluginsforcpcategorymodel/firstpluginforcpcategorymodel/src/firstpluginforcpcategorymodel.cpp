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

#include "firstpluginforcpcategorymodel.h"
#include "firstpluginviewforcpcategorymodel.h"
#include <cpsettingformentryitemdataimpl.h>

FirstPluginForCpCategoryModel::FirstPluginForCpCategoryModel()
{
}

FirstPluginForCpCategoryModel::~FirstPluginForCpCategoryModel()
{
}
QList<CpSettingFormItemData*> FirstPluginForCpCategoryModel::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    return QList<CpSettingFormItemData*>() 
            << new CpSettingFormEntryItemDataImpl<FirstPluginViewForCategoryModel>(
               itemDataHelper,
               tr("The plugin for test"), 
               tr("TestPlugin."));
}

CpBaseSettingView *FirstPluginForCpCategoryModel::createSettingView(const QVariant &hint) const
{
    return 0;
}

Q_EXPORT_PLUGIN2(FirstPluginForCpCategoryModel, FirstPluginForCpCategoryModel);
