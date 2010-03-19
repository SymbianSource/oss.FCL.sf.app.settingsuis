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
#include "cpdisplayplugin.h"
#include "cpdisplayview.h"
#include <cpsettingformentryitemdataimpl.h>

CpDisplayPlugin::CpDisplayPlugin()
{
}

CpDisplayPlugin::~CpDisplayPlugin()
{
}

QList<CpSettingFormItemData*> CpDisplayPlugin::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    CpSettingFormItemData* itemData = 0;
    CpDisplayView* view = new CpDisplayView();
    if ( view->isHidden() ){    // If there is no any items in the view, the view should be hidden.
        itemData = 0;
    } else {
        itemData = new CpSettingFormEntryItemDataImpl<CpDisplayView>(
			itemDataHelper,
            tr("Display "), 
			"");
    }
    delete view;
    return QList<CpSettingFormItemData*>() << itemData;
}

Q_EXPORT_PLUGIN2(cpdisplayplugin, CpDisplayPlugin);
