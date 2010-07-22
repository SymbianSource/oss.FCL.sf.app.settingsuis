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
#include "cpcommunicationgroupitemdata.h"
#include <QStringList>
#include <QtAlgorithms>
#include <CoreApplicationUIsSDKCRKeys.h>
#include <xqsettingsmanager.h>
#include <cpitemdatahelper.h>

CpCommunicationGroupItemData::CpCommunicationGroupItemData(const QString &configFile,
                                                           const HbDataFormModelItem *parent):
                                                           CpCategorySettingFormItemData(configFile,parent),
                                                           mAirplaneModeItem(0),
                                                           mSettingManager(new XQSettingsManager())
{

}

CpCommunicationGroupItemData::CpCommunicationGroupItemData(HbDataFormModelItem::DataItemType type,
                                                           const QString &label,
                                                           const QString &configFile,
                                                           const HbDataFormModelItem *parent):
                                                           CpCategorySettingFormItemData(type, label, configFile, parent),
                                                           mAirplaneModeItem(0),
                                                           mSettingManager(new XQSettingsManager())
{

}

CpCommunicationGroupItemData::~CpCommunicationGroupItemData()
{
    delete mSettingManager;
}

void CpCommunicationGroupItemData::beforeLoadingConfigPlugins(CpItemDataHelper &itemDataHelper)
{
    mAirplaneModeItem = new HbDataFormModelItem(HbDataFormModelItem::ToggleValueItem);
    mAirplaneModeItem->setDescription(hbTrId("txt_cp_info_in_offline_mode_all_wireless_communica"));
    
    itemDataHelper.addConnection(mAirplaneModeItem,
            SIGNAL(clicked()),
            this,
            SLOT(toggleAirplaneMode()));
    
    XQCentralRepositorySettingsKey key(KCRUidCoreApplicationUIs.iUid,KCoreAppUIsNetworkConnectionAllowed);
    QVariant airplaneMode = mSettingManager->readItemValue(key,XQSettingsManager::TypeInt);
    settingValueChanged(key,airplaneMode);
    
    mSettingManager->startMonitoring(key,XQSettingsManager::TypeInt);
    connect(mSettingManager, SIGNAL(valueChanged (XQSettingsKey, QVariant)),
            this, SLOT(settingValueChanged(XQSettingsKey, QVariant)));
    
    this->appendChild(mAirplaneModeItem);
    
}

void CpCommunicationGroupItemData::toggleAirplaneMode()
{
    XQCentralRepositorySettingsKey key(KCRUidCoreApplicationUIs.iUid,KCoreAppUIsNetworkConnectionAllowed);
    QVariant airplaneMode = mSettingManager->readItemValue(key,XQSettingsManager::TypeInt);
        
    airplaneMode.setValue( static_cast<int> (!airplaneMode.toBool()) );

    mSettingManager->writeItemValue(key,airplaneMode);
}

void CpCommunicationGroupItemData::settingValueChanged(const XQSettingsKey &key, const QVariant &value)
{
    if (mAirplaneModeItem 
        && key.uid() == KCRUidCoreApplicationUIs.iUid 
        && key.key() == KCoreAppUIsNetworkConnectionAllowed 
        && value.isValid()) {
        QString text = hbTrId("txt_cp_setlabel_offline_mode_val_on");
        QString additionalText = hbTrId("txt_cp_setlabel_offline_mode_val_off");
        if (ECoreAppUIsNetworkConnectionAllowed == value.toInt()) {
            ::qSwap (text, additionalText);
        }
        mAirplaneModeItem->setContentWidgetData("text",text);
        mAirplaneModeItem->setContentWidgetData("additionalText",additionalText);
    }
}

//End of File
