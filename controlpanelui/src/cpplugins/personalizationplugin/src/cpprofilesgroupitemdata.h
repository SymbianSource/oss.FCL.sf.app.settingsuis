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

#ifndef CPPROFILESGROUPITEMDATA_H
#define CPPROFILESGROUPITEMDATA_H

#include <cpsettingformitemdata.h>

class CpItemDataHelper;
class CpProfilesGroupItemData : public CpSettingFormItemData
{
    Q_OBJECT
public:
    explicit CpProfilesGroupItemData(CpItemDataHelper &itemDataHelper);
    virtual ~CpProfilesGroupItemData();
private:
    Q_DISABLE_COPY(CpProfilesGroupItemData)
    void initItems(CpItemDataHelper &itemDataHelper);
};

#endif
