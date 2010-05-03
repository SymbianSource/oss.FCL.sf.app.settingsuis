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
#ifndef CPPROFILEACTIVATORDIALOG_H
#define CPPROFILEACTIVATORDIALOG_H

#include <hbdialog.h>
#include <cpprofilemodel.h>
class HbRadioButtonList;
class HbDataFormModelItem;
class CpProfileActivatorEntryItem;
class CpSettingFormItemData;

class CpProfileActivatorDialog: public HbDialog
{
    Q_OBJECT
public:
    explicit CpProfileActivatorDialog(CpSettingFormItemData *profileActivator,
                                      CpProfileModel &profileModel,
                                      QGraphicsItem *parent = 0);
    ~CpProfileActivatorDialog();
    
private slots:
    void confirmProfileSelection();
    void cancelProfileSelection();
private:
    CpProfileModel &mProfileModel;
    HbRadioButtonList *mProfileList;
    CpSettingFormItemData *mProfileActivator;
    HbAction *mConfirmProfile;
    HbAction *mCancelProfile;
    QList<ProfileWrapperProfileId> mProfileIds;
};
#endif
