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
#ifndef CPPROFILEACTIVATORENTRYITEM_H
#define CPPROFILEACTIVATORENTRYITEM_H

#include <cpsettingformentryitemdata.h>

class CpProfileModel;
class CpProfileMonitor;

class CpProfileActivatorEntryItem : public CpSettingFormEntryItemData
{
Q_OBJECT
public:
    explicit CpProfileActivatorEntryItem(CpItemDataHelper &itemDataHelper,
            const QString &text = QString(), const QString &description =
                    QString(), const HbIcon &icon = HbIcon(),
            const HbDataFormModelItem *parent = 0);
    virtual ~CpProfileActivatorEntryItem();
private slots:
    void onLaunchView();
    void onProfileChanged(int activeProfileId);
    //void handleOk(const QVariant &result);
    //void handleError(int errorCode, const QString& errorMessage);
private:
    virtual CpBaseSettingView *createSettingView() const;
private:
    CpProfileModel *mProfileModel;
    CpProfileMonitor *mProfileMonitor;
};

#endif // CPPROFILEACTIVATORENTRYITEM_H
