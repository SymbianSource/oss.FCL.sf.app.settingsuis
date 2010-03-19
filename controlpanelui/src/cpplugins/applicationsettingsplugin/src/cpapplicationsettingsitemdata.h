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

#ifndef CPAPPLICATIONSETTINGSITEMDATA_H
#define CPAPPLICATIONSETTINGSITEMDATA_H

#include <cpcategorysettingformitemdata.h>

class CpApplicationSettingsItemData : public CpCategorySettingFormItemData
{
	Q_OBJECT
public:
	CpApplicationSettingsItemData();
	virtual ~CpApplicationSettingsItemData();
private:
    virtual void afterLoadingConfigPlugins(CpItemDataHelper &itemDataHelper);
};

#endif //CPAPPLICATIONSETTINGSITEMDATA_H

//End of File
