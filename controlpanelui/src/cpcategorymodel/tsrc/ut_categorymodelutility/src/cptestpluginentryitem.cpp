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

#include "CpTestPluginEntryItem.h"

#include <cpitemdatahelper.h>
#include <hbaction.h>
#include <QFileInfo>
CpTestPluginEntryItemData::CpTestPluginEntryItemData(const HbDataFormModelItem *parent /* = 0*/)
{
    
}
CpTestPluginEntryItemData::CpTestPluginEntryItemData(CpItemDataHelper &itemDataHelper,
                                                       const QString &text /*= QString()*/,
                                                       const QString &description /*= QString()*/,
                                                       const HbIcon &icon /*= HbIcon()*/,
                                                       const HbDataFormModelItem *parent /*= 0*/)
                                                       : CpSettingFormEntryItemData(itemDataHelper,
                                                       text,
                                                       description,
                                                       icon,
                                                       parent)
{
}

CpTestPluginEntryItemData::~CpTestPluginEntryItemData()
{
}

void CpTestPluginEntryItemData::testOnLaunchView()
{
    onLaunchView();
}
CpBaseSettingView *CpTestPluginEntryItemData::createSettingView() const
{
    return 0;
}
