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
#include "cpprofileactivatordialog.h"

#include <hbradiobuttonlist.h>
#include <hbaction.h>
#include <hbdataformmodelitem.h>
#include <hbpopup.h>
#include "cpprofileactivatorentryitem.h"
CpProfileActivatorDialog::CpProfileActivatorDialog(CpSettingFormItemData *profileActivator,
                                                    CpProfileModel &profileModel,
                                                    QGraphicsItem *parent):HbDialog(parent),
                                                     mProfileModel(profileModel),
                                                     mProfileList(0),
                                                     mProfileActivator(profileActivator)
{
    mProfileIds << EProfileWrapperGeneralId
                << EProfileWrapperMeetingId;
    mProfileList = new HbRadioButtonList();
    mProfileList->setItems(profileModel.profileNames());
    
    int currentId = mProfileModel.activeProfileId();
    mProfileList->setSelected(mProfileIds.indexOf(static_cast<ProfileWrapperProfileId>(currentId)));
    
    
        
    mConfirmProfile = new HbAction(hbTrId("txt_common_button_ok"));
    mCancelProfile = new HbAction(hbTrId("txt_common_button_cancel"));
    
    connect(mConfirmProfile, SIGNAL(triggered(bool)), this, SLOT(confirmProfileSelection()));
    connect(mCancelProfile, SIGNAL(triggered(bool)), this, SLOT(cancelProfileSelection()));
        
    
    this->addAction(mConfirmProfile);
    this->addAction(mCancelProfile);
    
    this->setContentWidget(mProfileList);
    this->setModal(true);
    this->setDismissPolicy(HbPopup::NoDismiss);
    this->setTimeout(HbPopup::NoTimeout);
}

CpProfileActivatorDialog::~CpProfileActivatorDialog()
{
    
}
void CpProfileActivatorDialog::confirmProfileSelection()
{
    int currentIndex = mProfileList->selected();
    
    // the best choice is no need to convert the index to id
    mProfileModel.activateProfile(mProfileIds.at(currentIndex));
    // ret should be remove from here
    // no invalid profile exsit
    QString profileName = mProfileModel.profileName(mProfileModel.activeProfileId());
    //change the descripton of entry item
    if (CpSettingFormEntryItemData *entryItem = qobject_cast<CpSettingFormEntryItemData*>(mProfileActivator)) {
        entryItem->setDescription(profileName);
    }
}
void CpProfileActivatorDialog::cancelProfileSelection()
{
    
}
