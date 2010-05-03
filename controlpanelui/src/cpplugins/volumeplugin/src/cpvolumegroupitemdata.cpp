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
#include "cpvolumegroupitemdata.h"
#include <cplogger.h>
#include <QStringList>
#include <cpsettingformentryitemdataimpl.h>
#include <cpbasesettingview.h>
#include <cpitemdatahelper.h>
#include <hbmessagebox.h>
#include <cpprofilemodel.h>
//#include "cpmastervolumevaluecontroller.h"
#include "cpvolumecontroller.h"
#include "cppersonalizationcustomviewitem.h"
#include <hbslider.h>

CpVolumeGroupItemData::CpVolumeGroupItemData(CpItemDataHelper &itemDataHelper)
: CpSettingFormItemData(HbDataFormModelItem::GroupItem,tr("txt_cp_subhead_volume")),//mSilenceIndicator(0),
                                                                                   //mMasterVolume(0),
                                                                                   //mMasterVibra(0),
                                                                                   mVolumeController(0),
                                                                                   mProfileModel(0)
{
    initItems(itemDataHelper);
}

CpVolumeGroupItemData::~CpVolumeGroupItemData()
{
    delete mProfileModel;
}

void CpVolumeGroupItemData::initItems(CpItemDataHelper &itemDataHelper)
{
    mProfileModel = new CpProfileModel();
        
    itemDataHelper.addItemPrototype(new CpPersonalizationCustomViewItem);
    CPFW_LOG("CpVolumeGroupItemData::initItems(), get active profile id");
    CPFW_LOG("CpVolumeGroupItemData::initItems(), succeed in getting id");
    
    HbDataFormModelItem *silenceIndicator = new HbDataFormModelItem(static_cast<HbDataFormModelItem::DataItemType>(SilenceIndicatorItem));
    mItemList.insert(CpVolumeGroupItemData::EVolumeSilenceItem, silenceIndicator);
    silenceIndicator->setContentWidgetData("text",hbTrId("txt_cp_button_silence"));
    this->appendChild(silenceIndicator);
    
    HbDataFormModelItem *masterVolume = new HbDataFormModelItem(HbDataFormModelItem::SliderItem,
                                            hbTrId("txt_cp_setlabel_volume"));
    mItemList.insert(CpVolumeGroupItemData::EVolumeMasterVolumeItem, masterVolume);
    QList<QVariant> elements;
    elements << QVariant(HbSlider::IncreaseElement) << QVariant(HbSlider::TrackElement)
            << QVariant(HbSlider::DecreaseElement) << QVariant(HbSlider::IconElement)
            << QVariant(HbSlider::TextElement);
    masterVolume->setContentWidgetData("sliderElements",elements);
    
    QMap<QString, QVariant> iconMaps;
    iconMaps.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg"));
    iconMaps.insert(QString("IncreaseElement"), QVariant(":/icon/hb_vol_slider_increment.svg"));
    //iconMaps.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_muted.svg"));
    
    masterVolume->setContentWidgetData("elementIcons", iconMaps);
    masterVolume->setContentWidgetData("minimum", 1);
    masterVolume->setContentWidgetData("maximum", 3);
    masterVolume->setContentWidgetData("majorTickInterval",1);
    
    QStringList tickLabels;
    tickLabels<<hbTrId("Soft")
              <<hbTrId("Med")
              <<hbTrId("Loud");
    masterVolume->setContentWidgetData("majorTickLabels",tickLabels);
    
    this->appendChild(masterVolume);
    
    HbDataFormModelItem *masterVibra = new HbDataFormModelItem(HbDataFormModelItem::CheckBoxItem);
    mItemList.insert(CpVolumeGroupItemData::EVolumeMasterVibraItem, masterVibra);
    masterVibra->setContentWidgetData("text",hbTrId("txt_cp_list_vibrate"));
    
    this->appendChild(masterVibra);
    
    mVolumeController = new CpVolumeController(mProfileModel, mItemList, itemDataHelper);
}
//void CpVolumeGroupItemData::activateProfile(int profileIndex)
//{
//    switch (profileIndex) {
//    case 0: // general
//    {
//        mProfileModel->activateProfile(EProfileWrapperGeneralId);
//        int volumeValue = mProfileModel->ringVolume();
//        // update the master volume when profile changed
//        // should be used profileChangedObserver in the future;
//        mMasterVolume->setContentWidgetData(/*"visible"*/"enabled",true);
//        mMasterVolume->setContentWidgetData("value", volumeValue);
//        break;
//    }
//    case 1: // meeting
//    {
//        mProfileModel->activateProfile(EProfileWrapperMeetingId);
//        int volumeValue = mProfileModel->ringVolume();
//        mMasterVolume->setContentWidgetData(/*"visible"*/"enabled",true);
//        mMasterVolume->setContentWidgetData("value", volumeValue);
//        break;
//    }
//    case 2: // silent
//    {
//        mProfileModel->activateProfile(EProfileWrapperSilentId);
//        mMasterVolume->setContentWidgetData("value", 0);
//        mMasterVolume->setContentWidgetData(/*"visible"*/"enabled",false);
//        break;
//    }
//    default:
//        break;
//        
//    }
//}
