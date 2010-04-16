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
#include <cpitemdatahelper.h>
#include <hbmessagebox.h>
#include "cpprofilesettingform.h"
#include "cpmastervolumevaluecontroller.h"
#include "cppersonalizationcustomviewitem.h"


CpProfilesGroupItemData::CpProfilesGroupItemData(CpItemDataHelper &itemDataHelper)
: CpSettingFormItemData(HbDataFormModelItem::GroupItem,tr("Profiles")),mProfileModel(0)
{
    mProfileModel = new CpProfileModel();
    initItems(itemDataHelper);
}

CpProfilesGroupItemData::~CpProfilesGroupItemData()
{
    delete mProfileModel;
    if (mMasterVolumeValueController != 0) {
        delete mMasterVolumeValueController;
    }
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
}

void CpProfilesGroupItemData::initItems(CpItemDataHelper &itemDataHelper)
{
    itemDataHelper.addItemPrototype(new CpPersonalizationCustomViewItem);
    HbDataFormModelItem *activeProfileItem = new HbDataFormModelItem(
        HbDataFormModelItem::RadioButtonListItem,tr("Profiles"));
    //enable the profile activating
    // why come into slots twice when select a item?
    itemDataHelper.addConnection(activeProfileItem,SIGNAL(itemSelected(int)),this,SLOT(activateProfile(int)));
    
    mCurrentProfileId = static_cast<ProfileWrapperProfileId>(mProfileModel->activeProfileId());
   // QString warnningNote = "Profile Id = "+ mCurrentProfileId;
    //HbMessageBox::information(warnningNote);
    QStringList items;
    items << tr("General") << tr("Meeting") << tr("Silent"); 
    activeProfileItem->setContentWidgetData("items",items);
    switch (mCurrentProfileId) {
    case EProfileWrapperGeneralId:
        activeProfileItem->setContentWidgetData("selected",0);
        break;
    case EProfileWrapperSilentId:
        activeProfileItem->setContentWidgetData("selected",2);
        break;
    case EProfileWrapperMeetingId:
        activeProfileItem->setContentWidgetData("selected",1);
        break;
    default:
      //  HbMessageBox::information("unknown id of profile");
        break;
    };
    
    appendChild(activeProfileItem);
    
    mMasterVolume = new HbDataFormModelItem(static_cast<HbDataFormModelItem::DataItemType>(MasterVolumeSliderItem));

    mMasterVolumeValueController = new CpMasterVolumeValueController(mProfileModel, mMasterVolume,
                                                                        itemDataHelper, activeProfileItem);
    this->appendChild(mMasterVolume);

    HbDataFormModelItem *editProfileItem = new CpSettingFormEntryItemDataImpl<CpProfileSettingView>(
        itemDataHelper,tr("Edit profiles"));
    appendChild(editProfileItem);
}
void CpProfilesGroupItemData::activateProfile(int profileIndex)
{
    switch (profileIndex) {
    case 0: // general
    {
        mProfileModel->activateProfile(EProfileWrapperGeneralId);
        int volumeValue = mProfileModel->ringVolume();
        // update the master volume when profile changed
        // should be used profileChangedObserver in the future;
        mMasterVolume->setEnabled(true);
        mMasterVolume->setContentWidgetData("value", volumeValue);
        break;
    }
    case 1: // meeting
    {
        mProfileModel->activateProfile(EProfileWrapperMeetingId);
        int volumeValue = mProfileModel->ringVolume();
        mMasterVolume->setEnabled(true);
        mMasterVolume->setContentWidgetData("value", volumeValue);
        break;
    }
    case 2: // silent
    {
        mProfileModel->activateProfile(EProfileWrapperSilentId);
        mMasterVolume->setContentWidgetData("value", 0);
        mMasterVolume->setEnabled(false);
        break;
    }
    default:
        break;
        
    }
}
