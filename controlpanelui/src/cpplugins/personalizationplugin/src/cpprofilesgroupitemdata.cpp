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
#include "cpprofilesgroupitemdata.h"
#include <QStringList>
#include <cpsettingformentryitemdataimpl.h>
#include <cpbasesettingview.h>
#include "cpprofilesettingform.h"

CpProfilesGroupItemData::CpProfilesGroupItemData(CpItemDataHelper &itemDataHelper)
: CpSettingFormItemData(HbDataFormModelItem::GroupItem,tr("Profiles"))
{
    initItems(itemDataHelper);
}

CpProfilesGroupItemData::~CpProfilesGroupItemData()
{
}

namespace 
{
    class CpProfileSettingView : public CpBaseSettingView
    {
    public:
        CpProfileSettingView() : CpBaseSettingView(new CpProfileSettingForm())
        {
        }
    };
};

void CpProfilesGroupItemData::initItems(CpItemDataHelper &itemDataHelper)
{
    HbDataFormModelItem *activeProfileItem = new HbDataFormModelItem(
        HbDataFormModelItem::RadioButtonListItem,tr("Profiles"));

    QStringList items;
    items << tr("Normal") << tr("Meeting") << tr("Silent");
    activeProfileItem->setContentWidgetData("items",items);
    activeProfileItem->setContentWidgetData("selected",0);

    appendChild(activeProfileItem);

    HbDataFormModelItem *editProfileItem = new CpSettingFormEntryItemDataImpl<CpProfileSettingView>(
        itemDataHelper,tr("Edit profiles"));
    appendChild(editProfileItem);
}
