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

#include "cpprofilesettingform.h"
#include <QStringList>
#include <hbdataformmodel.h>
#include "cppersonalizationcustomviewitem.h"

CpProfileSettingForm::CpProfileSettingForm()
: mModel(0)
{
    this->setHeading(tr("Profiles"));
    this->setDescription(tr("With profiles you can quickly change audio settings of your device"));

    QList<HbAbstractViewItem *> protoTypeList = itemPrototypes();
    protoTypeList.append(new CpPersonalizationCustomViewItem);
    setItemPrototypes(protoTypeList);

    initModel();
}

CpProfileSettingForm::~CpProfileSettingForm()
{
    delete mModel;
}

void CpProfileSettingForm::initModel()
{
    mModel = new HbDataFormModel();

    QStringList profileList;
    profileList << "General" << "Meeting" << "Silent"; //should be got from engine.
    foreach (const QString &profile,profileList) {
        HbDataFormModelItem *profilePage = mModel->appendDataFormPage(profile);
        //hard code: profile name should be got from profile engine
        if (profile != "Silent") {
        initVolumeGroup(profilePage);
        }
        initVibraGroup(profilePage);
        //initRingToneGroup(profilePage);
        //initMessageToneGroup(profilePage);
        //initAlertToneGroup(profilePage);
        //initKeyAndScreenToneGroup(profilePage);
    }
    setModel(mModel);
}

void CpProfileSettingForm::initVolumeGroup(HbDataFormModelItem *parent)
{
    HbDataFormModelItem *volumeGroup = mModel->appendDataFormGroup(
        tr("Volume"),parent);
   // mModel->appendDataFormItem(static_cast<HbDataFormModelItem::DataItemType>(MasterVolumeSliderItem),QString("Master volume"),volumeGroup);
    HbDataFormModelItem *informationTone = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),volumeGroup);
    informationTone->setContentWidgetData("text", tr("information tones"));
    mModel->appendDataFormItem(static_cast<HbDataFormModelItem::DataItemType>(MasterVolumeSliderItem),QString("Key and touch screen tones"),volumeGroup);
}
void CpProfileSettingForm::initVibraGroup(HbDataFormModelItem *parent)
{
    HbDataFormModelItem *vibraGroup = mModel->appendDataFormGroup(
        tr("Vibra"),parent);
    HbDataFormModelItem *ringVibar = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),vibraGroup);
    ringVibar->setContentWidgetData("text", tr("Ring alert vibra"));

    HbDataFormModelItem *messageVibra = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),vibraGroup);
    messageVibra->setContentWidgetData("text", tr("New message alert vibra"));

    HbDataFormModelItem *emailVibra = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),vibraGroup);
    emailVibra->setContentWidgetData("text", tr("New e-mail alert vibra"));

    HbDataFormModelItem *reminderVibra = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),vibraGroup);
    reminderVibra->setContentWidgetData("text", tr("Reminder alarm vibra"));

    HbDataFormModelItem *clockVibra = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),vibraGroup);
    clockVibra->setContentWidgetData("text", tr("Clock alarm vibra"));

    HbDataFormModelItem *informationVibra = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),vibraGroup);
    informationVibra->setContentWidgetData("text", tr("Information vibra"));

}

/*void CpProfileSettingForm::initRingToneGroup(HbDataFormModelItem *parent)
{
    HbDataFormModelItem *ringToneGroup = mModel->appendDataFormGroup(
        tr("Ring tone"),parent);

    QStringList tonesList;
    tonesList << "Off" << "Beep" << "Default ring tone" << "set profile-specific tone";

    foreach (const QString &tone,tonesList) {
        HbDataFormModelItem *toneGroup = mModel->appendDataFormGroupPage(tone,ringToneGroup);
        mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,tr("Say caller name"),toneGroup);
        HbDataFormModelItem *threeDEffect = mModel->appendDataFormItem(
            HbDataFormModelItem::ComboBoxItem,tr("3D effect"),toneGroup);
        
        threeDEffect->setContentWidgetData("items",QStringList() 
            << tr("Off") << tr("Circular") << tr("Fly-by") << tr("Zigzag") 
            << tr("Meander") << tr("Chaos") << tr("Stereo widening"));
        threeDEffect->setContentWidgetData("currentIndex",0);

        HbDataFormModelItem *treeEcho = mModel->appendDataFormItem(
            HbDataFormModelItem::ComboBoxItem,tr("3D echo"),toneGroup);
        treeEcho->setContentWidgetData("items",QStringList()
            << tr("Off") << tr("Living room") << tr("Cave") << tr("Railway station")
            << tr("Forest") << tr("Duct") << tr("underwater"));
        treeEcho->setContentWidgetData("currentIndex",0);
    }
}

void CpProfileSettingForm::initMessageToneGroup(HbDataFormModelItem *parent)
{
    HbDataFormModelItem *messageToneGroup = mModel->appendDataFormGroup(
        tr("Message tones"),parent);

    HbDataFormModelItem *messageTone = mModel->appendDataFormItem(
        HbDataFormModelItem::ComboBoxItem,tr("Message tone"),messageToneGroup);

    messageTone->setContentWidgetData("items",QStringList() 
        << tr("Off") << tr("Beep") << tr("Default mesage tone") << tr("set profile-specific tone"));
    messageTone->setContentWidgetData("currentIndex",2);

    HbDataFormModelItem *emailTone = mModel->appendDataFormItem(
        HbDataFormModelItem::ComboBoxItem,tr("E-mail tone"),messageToneGroup);

    emailTone->setContentWidgetData("items",QStringList() 
        << tr("Off") << tr("Beep") << tr("Default e-mail tone") << tr("set profile-specific tone"));
    emailTone->setContentWidgetData("currentIndex",2);

}

void CpProfileSettingForm::initAlertToneGroup(HbDataFormModelItem *parent)
{
    HbDataFormModelItem *alertToneGroup = mModel->appendDataFormGroup(
        tr("Alert tones"),parent);

    HbDataFormModelItem *reminderTone = mModel->appendDataFormItem(
        HbDataFormModelItem::ComboBoxItem,tr("Reminder tone"),alertToneGroup);

    reminderTone->setContentWidgetData("items",QStringList() 
        << tr("Off") << tr("Beep") << tr("Default reminder tone") << tr("set profile-specific tone"));
    reminderTone->setContentWidgetData("currentIndex",2);

    HbDataFormModelItem *clockAlarmTone = mModel->appendDataFormItem(
        HbDataFormModelItem::ComboBoxItem,tr("Clock alarm tone"),alertToneGroup);

    clockAlarmTone->setContentWidgetData("items",QStringList() 
        << tr("Off") << tr("Beep") << tr("Default clock alarm tone") << tr("set profile-specific tone"));
    clockAlarmTone->setContentWidgetData("currentIndex",2);

}

void CpProfileSettingForm::initKeyAndScreenToneGroup(HbDataFormModelItem *parent)
{
    HbDataFormModelItem *keyAndScreenToneGroup= mModel->appendDataFormGroup(
        tr("Key and screen tones"),parent);
    mModel->appendDataFormItem(static_cast<HbDataFormModelItem::DataItemType>(MasterVolumeSliderItem),
        tr("Key and touch screen tones"),keyAndScreenToneGroup);
    mModel->appendDataFormItem(HbDataFormModelItem::SliderItem,
        tr("Touch screen vibra"),keyAndScreenToneGroup);
}
*/
