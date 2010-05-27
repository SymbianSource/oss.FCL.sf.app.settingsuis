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
#include "cppersonalizationentryitemdata.h"
#include <QStringList>
#include <hbdataformmodel.h>
#include <hbabstractviewitem.h>
#include <cpitemdatahelper.h>
#include <cpprofilemodel.h>
#include <hbslider.h>

#include <QFileInfo>
#include <QFileIconProvider>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QMap>

#include <xqsettingsmanager.h>
#include <ProfileEngineInternalCRKeys.h>

CpProfileSettingForm::CpProfileSettingForm()
    : mModel(0), mItemDataHelper(new CpItemDataHelper( this )),
    mProfileModel(new CpProfileModel()), mFileIconProvider(new QFileIconProvider),
    mGeneralPage(0),mMeetingPage(0),mCurrentPage( 0 ), mSettingManager(0)
{
    this->setHeading(hbTrId("txt_cp_button_advanced_settings"));
    this->setDescription(hbTrId("txt_cp_info_select_tones_that_play_when_you_select"));
    
    initModel();
}

CpProfileSettingForm::~CpProfileSettingForm()
{
    delete mModel;
    delete mProfileModel;
    delete mFileIconProvider;
    delete mSettingManager;
}

void CpProfileSettingForm::initModel()
{
    mModel = new HbDataFormModel();

    QString generalString( mProfileModel->profileName( EProfileWrapperGeneralId ) );
    if( generalString.isEmpty() ) {
        qDebug( "warning: general profile name is empty");
        generalString = hbTrId("txt_cp_list_general"); 
    }
    QString meetingString( mProfileModel->profileName( EProfileWrapperMeetingId ) );
    if( meetingString.isEmpty() ) {
        qDebug( "warning: meeting profile name is empty");
        meetingString = hbTrId("txt_cp_list_meeting"); 
    }
    
    mSettingManager = new XQSettingsManager();   
    
    XQCentralRepositorySettingsKey key(KCRUidProfileEngine.iUid,KProEngSilenceMode);
    
    QVariant silenceMode = mSettingManager->readItemValue(key,XQSettingsManager::TypeInt);

    mSettingManager->startMonitoring(key,XQSettingsManager::TypeInt);
    connect(mSettingManager, SIGNAL(valueChanged (XQSettingsKey, QVariant)),
            this, SLOT(settingValueChanged(XQSettingsKey, QVariant)));
    
    mGeneralPage = mModel->appendDataFormPage( generalString );
    initProfileItems(EProfileWrapperGeneralId,mGeneralPage);
    
    //initGeneralTonesGroup();
    
    //initGeneralVibraGroup();
    
    mMeetingPage = mModel->appendDataFormPage( meetingString );
    initProfileItems(EProfileWrapperMeetingId,mMeetingPage);
    //initMeetingTonesGroup();
    //initMeetingVibraGroup();
    setModel(mModel);
    settingValueChanged(key,silenceMode);
}

void CpProfileSettingForm::initProfileItems(int profileId,HbDataFormModelItem *parent)
{
    CpProfileSettings profileSettings;
    mProfileModel->profileSettings( profileId, profileSettings );
    
    QHash<int,HbDataFormModelItem*> modelItems;
    
    //ring tone item
    QFileInfo ringToneFileInfo( profileSettings.mRingTone );
    HbDataFormModelItem *modelItem = new CpPersonalizationEntryItemData( *mItemDataHelper,
            hbTrId("txt_cp_dblist_ringtone"), ringToneFileInfo.fileName(), mFileIconProvider->icon( ringToneFileInfo ),
            CpPersonalizationEntryItemData::TONE_Ring, profileId );
    mModel->appendDataFormItem(modelItem, parent);
    modelItems.insert(ProfileItemRingTone,modelItem);
    
    //message tone item
    QFileInfo messageToneFileInfo( profileSettings.mMessageTone );
    modelItem = new CpPersonalizationEntryItemData( *mItemDataHelper,
            hbTrId("txt_cp_dblist_message_tone"), messageToneFileInfo.fileName(), mFileIconProvider->icon( messageToneFileInfo ),
            CpPersonalizationEntryItemData::TONE_Message,
            profileId );
    mModel->appendDataFormItem(modelItem , parent);
    modelItems.insert(ProfileItemMessageTone,modelItem);
    
    //email tone item
    QFileInfo emailToneFileInfo( profileSettings.mEmailTone );
    modelItem = new CpPersonalizationEntryItemData( *mItemDataHelper,
            hbTrId("txt_cp_dblist_email_tone"),  emailToneFileInfo.fileName(), mFileIconProvider->icon( emailToneFileInfo ),
            CpPersonalizationEntryItemData::TONE_Email,
            profileId );
    mModel->appendDataFormItem(modelItem , parent);
    modelItems.insert(ProfileItemEmailTone,modelItem);
    
    //reminder tone item
    QFileInfo reminderToneFileInfo( profileSettings.mReminderTone );
    modelItem = new CpPersonalizationEntryItemData( *mItemDataHelper,
            hbTrId("txt_cp_dblist_reminder_tone"), reminderToneFileInfo.fileName(), mFileIconProvider->icon( reminderToneFileInfo ),
            CpPersonalizationEntryItemData::TONE_Reminder,
            profileId );
    mModel->appendDataFormItem(modelItem , parent);
    modelItems.insert(ProfileItemReminderTone,modelItem);
    
    //notification tones item
    modelItem= mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),parent);
    modelItem->setContentWidgetData("text", hbTrId("txt_cp_list_notification_tones"));
    modelItem->setContentWidgetData( "checkState", profileSettings.mNotificationTone ? 2 : 0 );
    if (profileId == EProfileWrapperGeneralId) {
        addConnection( modelItem, SIGNAL( stateChanged( int )), this, SLOT( on_general_notificationTones_stateChanged( int )));
    }
    else if (profileId == EProfileWrapperMeetingId) {
        addConnection( modelItem, SIGNAL( stateChanged( int )), this, SLOT( on_meeting_notificationTones_stateChanged( int )));
    }
    modelItems.insert(ProfileItemNotificationTones,modelItem);
    
    
    //Key and Touch Screen Tones item
    modelItem = 
            mModel->appendDataFormItem(HbDataFormModelItem::SliderItem ,QString(hbTrId("txt_cp_setlabel_key_and_touchscreen_tones")),parent);
    
    QList<QVariant> sliderElements;
    sliderElements << QVariant(HbSlider::IncreaseElement) << QVariant(HbSlider::TrackElement)
            << QVariant(HbSlider::DecreaseElement) << QVariant(HbSlider::IconElement)
            << QVariant(HbSlider::TextElement);
    modelItem->setContentWidgetData("sliderElements",sliderElements);
    
    
    
    //TODO: profileModel need provide Max and Min value( 0-5 ), current max value from profileModel is 3
    modelItem->setContentWidgetData( QString( "minimum" ), 0 );
    modelItem->setContentWidgetData( QString( "maximum" ), 5 );
    modelItem->setContentWidgetData( QString("value"), profileSettings.mKeyTouchScreenTone );
    QMap< QString, QVariant > elements;
    elements.insert(QString("IncreaseElement") , QVariant(":/icon/hb_vol_slider_increment.svg"));
    elements.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg") );
    if (profileSettings.mKeyTouchScreenTone != 0) {
        elements.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_unmuted.svg") );
    }
    else {
        elements.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_muted.svg") );
    }
    
    modelItem->setContentWidgetData( QString( "elementIcons" ), elements );
    
    if (profileId == EProfileWrapperGeneralId) {
        addConnection( modelItem, SIGNAL( valueChanged( int )), this, SLOT( on_general_keysAndScreenToneSlider_ValueChanged( int )));
    }
    else if (profileId == EProfileWrapperMeetingId) {
        addConnection( modelItem, SIGNAL( valueChanged( int )), this, SLOT( on_meeting_keysAndScreenToneSlider_ValueChanged( int )));
    }
  
    modelItems.insert(ProfileItemKeyandTouchScreenTones,modelItem);
    
    
    //Touch Screen Vibra item
    modelItem = mModel->appendDataFormItem( HbDataFormModelItem::SliderItem, QString( hbTrId( "txt_cp_setlabel_touch_screen_vibra" ) ), parent );
     //TODO: profileModel need provide Max and Min value( 0-5 ), current max value from profileModel is 3
    modelItem->setContentWidgetData( QString( "minimum" ), 0 );
    modelItem->setContentWidgetData( QString( "maximum" ), 5 );
    modelItem->setContentWidgetData( QString("value"), profileSettings.mKeyTouchScreenVibra );
    
    if (profileId == EProfileWrapperGeneralId) {
        addConnection( modelItem, SIGNAL( valueChanged( int )), this, SLOT( on_general_screenVibra_ValueChanged( int )));
    }
    else if (profileId == EProfileWrapperMeetingId) {
        addConnection( modelItem, SIGNAL( valueChanged( int )), this, SLOT( on_meeting_screenVibra_ValueChanged( int )));
    }
    
    modelItems.insert(ProfileItemTouchScreenVibra,modelItem);
    
    mProfileModelItems.insert(profileId,modelItems);
    
}

/*
void CpProfileSettingForm::initGeneralTonesGroup()
{
    CpProfileSettings profileSettings;
    mProfileModel->profileSettings( EProfileWrapperGeneralId, profileSettings );
    
    //HbDataFormModelItem *tonesGroup = mModel->appendDataFormGroup(
     //   hbTrId("txt_cp_subhead_tones"),mGeneralPage);
    QFileInfo ringToneFileInfo( profileSettings.mRingTone );
    mModel->appendDataFormItem( new CpPersonalizationEntryItemData( *mItemDataHelper,
        hbTrId("txt_cp_dblist_ringtone"), ringToneFileInfo.fileName(), mFileIconProvider->icon( ringToneFileInfo ) ), mGeneralPage);
    
    QFileInfo messageToneFileInfo( profileSettings.mMessageTone );
    mModel->appendDataFormItem( new CpPersonalizationEntryItemData( *mItemDataHelper,
        hbTrId("txt_cp_dblist_message_tone"), messageToneFileInfo.fileName(), mFileIconProvider->icon( messageToneFileInfo ) ), mGeneralPage);
    
    QFileInfo emailToneFileInfo( profileSettings.mEmailTone );
    mModel->appendDataFormItem( new CpPersonalizationEntryItemData( *mItemDataHelper,
        hbTrId("txt_cp_dblist_email_tone"),  emailToneFileInfo.fileName(), mFileIconProvider->icon( emailToneFileInfo ) ), mGeneralPage);
    
    QFileInfo reminderToneFileInfo( profileSettings.mReminderTone );
    mModel->appendDataFormItem( new CpPersonalizationEntryItemData( *mItemDataHelper,
        hbTrId("txt_cp_dblist_reminder_tone"), reminderToneFileInfo.fileName(), mFileIconProvider->icon( reminderToneFileInfo ) ), mGeneralPage);
    
    HbDataFormModelItem *notificationTones = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),mGeneralPage);
    notificationTones->setContentWidgetData("text", hbTrId("txt_cp_list_notification_tones"));
    notificationTones->setContentWidgetData( "checkState", profileSettings.mNotificationTone?2:0 );
    
    mGeneralKeysAndScreenToneSlider = 
            mModel->appendDataFormItem(HbDataFormModelItem::SliderItem ,QString(hbTrId("txt_cp_setlabel_key_and_touchscreen_tones")),mGeneralPage);
    
    //QMap< QString, QVariant > elements;
    //elements.insert(QString("IncreaseElement") , QVariant(":/icon/hb_vol_slider_increment.svg"));
    //elements.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg") );
    //elements.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_muted.svg") );
    //mGeneralKeysAndScreenToneSlider->setContentWidgetData( QString( "elementIcons" ), elements );
     
    //TODO: profileModel need provide Max and Min value( 0-5 ), current max value from profileModel is 3
    mGeneralKeysAndScreenToneSlider->setContentWidgetData( QString( "minimum" ), 0 );
    mGeneralKeysAndScreenToneSlider->setContentWidgetData( QString( "maximum" ), 5 );
    mGeneralKeysAndScreenToneSlider->setContentWidgetData( QString("value"), profileSettings.mKeyTouchScreenTone );
    
    
    mGeneralSreenVibra = mModel->appendDataFormItem( HbDataFormModelItem::SliderItem, QString( hbTrId( "txt_cp_setlabel_touch_screen_vibra" ) ), mGeneralPage );
     //TODO: profileModel need provide Max and Min value( 0-5 ), current max value from profileModel is 3
    mGeneralSreenVibra->setContentWidgetData( QString( "minimum" ), 0 );
    mGeneralSreenVibra->setContentWidgetData( QString( "maximum" ), 5 );
    mGeneralSreenVibra->setContentWidgetData( QString("value"), profileSettings.mKeyTouchScreenVibra );
    
    addConnection( notificationTones, SIGNAL( stateChanged( int )), this, SLOT( on_general_notificationTones_stateChanged( int )));
    addConnection( mGeneralKeysAndScreenToneSlider, SIGNAL( valueChanged( int )), this, SLOT( on_general_keysAndScreenToneSlider_ValueChanged( int )));
    addConnection( mGeneralSreenVibra, SIGNAL( valueChanged( int )), this, SLOT( on_general_screenVibra_ValueChanged( int )));    
}
*/

/*void CpProfileSettingForm::initGeneralVibraGroup()
{
    CpProfileSettings profileSettings;
    mProfileModel->profileSettings( EProfileWrapperGeneralId, profileSettings );
        
    HbDataFormModelItem *vibraGroup = mModel->appendDataFormGroup(
        hbTrId( "txt_cp_subhead_vibra" ), mGeneralPage );
    HbDataFormModelItem *ringVibar = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    ringVibar->setContentWidgetData( "text", hbTrId( "txt_cp_list_ring_alet" ) );
    ringVibar->setContentWidgetData( "checkState", profileSettings.mRingAlertVibra?2:0 );    

    HbDataFormModelItem *messageVibra = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    messageVibra->setContentWidgetData( "text", hbTrId( "txt_cp_list_received_message" ) );
    messageVibra->setContentWidgetData( "checkState", profileSettings.mMessageVibra?2:0 );

    HbDataFormModelItem *emailVibra = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    emailVibra->setContentWidgetData( "text", hbTrId( "txt_cp_list_received_email" ) );
    emailVibra->setContentWidgetData( "checkState", profileSettings.mEmailVibra?2:0 );

    HbDataFormModelItem *reminderVibra = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    reminderVibra->setContentWidgetData( "text", hbTrId( "txt_cp_list_reminder_alert" ));
    reminderVibra->setContentWidgetData( "checkState", profileSettings.mReminderAlertVibra?2:0 );
    
    HbDataFormModelItem *notificationVibra = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    notificationVibra->setContentWidgetData( "text", hbTrId( "txt_cp_list_notification" ) );
    notificationVibra->setContentWidgetData( "checkState", profileSettings.mNotificationVibra?2:0 );
    
    mGeneralSreenVibra = mModel->appendDataFormItem( HbDataFormModelItem::SliderItem, QString( hbTrId( "txt_cp_setlabel_touch_screen_vibra" ) ), vibraGroup );
    //TODO: profileModel need provide Max and Min value( 0-5 ), current max value from profileModel is 3
    mGeneralSreenVibra->setContentWidgetData( QString( "minimum" ), 0 );
    mGeneralSreenVibra->setContentWidgetData( QString( "maximum" ), 5 );
    mGeneralSreenVibra->setContentWidgetData( QString("value"), profileSettings.mKeyTouchScreenVibra );
    
    addConnection( ringVibar, SIGNAL( stateChanged( int )), this, SLOT( on_general_ringVibar_stateChanged( int )));
    addConnection( messageVibra, SIGNAL( stateChanged( int )), this, SLOT( on_general_messageVibra_stateChanged( int )));
    addConnection( emailVibra, SIGNAL( stateChanged( int )), this, SLOT( on_general_emailVibra_stateChanged( int )));
    addConnection( reminderVibra, SIGNAL( stateChanged( int )), this, SLOT( on_general_reminderVibra_stateChanged( int )));
    addConnection( notificationVibra, SIGNAL( stateChanged( int )), this, SLOT( on_general_notificationVibra_stateChanged( int )));
    
    addConnection( mGeneralSreenVibra, SIGNAL( valueChanged( int )), this, SLOT( on_general_screenVibra_ValueChanged( int )));
}*/

////////////////////////////////////////////////////////////
/*void CpProfileSettingForm::initMeetingTonesGroup()
{
    CpProfileSettings profileSettings;
    mProfileModel->profileSettings( EProfileWrapperMeetingId, profileSettings );
    
   //HbDataFormModelItem *tonesGroup = mModel->appendDataFormGroup(
   //     hbTrId("txt_cp_subhead_tones"),mMeetingPage);
    QFileInfo ringToneFileInfo( profileSettings.mRingTone );
    mModel->appendDataFormItem( new CpPersonalizationEntryItemData( *mItemDataHelper,
        hbTrId("txt_cp_dblist_ringtone"), ringToneFileInfo.fileName(), mFileIconProvider->icon( ringToneFileInfo ) ), mMeetingPage);
    
    QFileInfo messageToneFileInfo( profileSettings.mMessageTone );
    mModel->appendDataFormItem( new CpPersonalizationEntryItemData( *mItemDataHelper,
        hbTrId("txt_cp_dblist_message_tone"), messageToneFileInfo.fileName(), mFileIconProvider->icon( messageToneFileInfo ) ), mMeetingPage);
    
    QFileInfo emailToneFileInfo( profileSettings.mEmailTone );
    mModel->appendDataFormItem( new CpPersonalizationEntryItemData( *mItemDataHelper,
        hbTrId("txt_cp_dblist_email_tone"),  emailToneFileInfo.fileName(), mFileIconProvider->icon( emailToneFileInfo ) ), mMeetingPage);
    
    QFileInfo reminderToneFileInfo( profileSettings.mReminderTone );
    mModel->appendDataFormItem( new CpPersonalizationEntryItemData( *mItemDataHelper,
        hbTrId("txt_cp_dblist_reminder_tone"), reminderToneFileInfo.fileName(), mFileIconProvider->icon( reminderToneFileInfo ) ), mMeetingPage);
    
    HbDataFormModelItem *notificationTones = mModel->appendDataFormItem(HbDataFormModelItem::CheckBoxItem,QString(),mMeetingPage);
    notificationTones->setContentWidgetData("text", hbTrId("txt_cp_list_notification_tones"));
    notificationTones->setContentWidgetData( "checkState", profileSettings.mNotificationTone?2:0 );
    
    mMeetingKeysAndScreenToneSlider = 
            mModel->appendDataFormItem(HbDataFormModelItem::SliderItem ,QString(hbTrId("txt_cp_setlabel_key_and_touchscreen_tones")),mMeetingPage);
    
    //QMap< QString, QVariant > elements;
    //elements.insert(QString("IncreaseElement") , QVariant(":/icon/hb_vol_slider_increment.svg"));
    //elements.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg") );
    //elements.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_muted.svg") );
    //mMeetingkeysAndScreenSlider->setContentWidgetData( QString( "elementIcons" ), elements );
    
    //TODO: profileModel need provide Max and Min value( 0-5 ), current max value from profileModel is 3
    mMeetingKeysAndScreenToneSlider->setContentWidgetData( QString( "minimum" ), 0 );
    mMeetingKeysAndScreenToneSlider->setContentWidgetData( QString( "maximum" ), 5 );
    mMeetingKeysAndScreenToneSlider->setContentWidgetData( QString("value"), profileSettings.mKeyTouchScreenTone );
    
    mMeetingSreenVibra = mModel->appendDataFormItem( HbDataFormModelItem::SliderItem, QString( hbTrId( "txt_cp_setlabel_touch_screen_vibra" ) ), mMeetingPage );
    //TODO: profileModel need provide Max and Min value( 0-5 ), current max value from profileModel is 3
    mMeetingSreenVibra->setContentWidgetData( QString( "minimum" ), 0 );
    mMeetingSreenVibra->setContentWidgetData( QString( "maximum" ), 5 );
    mMeetingSreenVibra->setContentWidgetData( QString("value"), profileSettings.mKeyTouchScreenVibra );
    
    addConnection( notificationTones, SIGNAL( stateChanged( int )), this, SLOT( on_meeting_notificationTones_stateChanged( int )));
    addConnection( mMeetingKeysAndScreenToneSlider, SIGNAL( valueChanged( int )), this, SLOT( on_meeting_keysAndScreenSlider_ValueChanged( int )));
    addConnection( mMeetingSreenVibra, SIGNAL( valueChanged( int )), this, SLOT( on_meeting_screenVibra_ValueChanged( int )));
}
*/
/*void CpProfileSettingForm::initMeetingVibraGroup()
{
    CpProfileSettings profileSettings;
    mProfileModel->profileSettings( EProfileWrapperMeetingId, profileSettings );
        
    HbDataFormModelItem *vibraGroup = mModel->appendDataFormGroup(
        hbTrId( "txt_cp_subhead_vibra" ), mMeetingPage );
    HbDataFormModelItem *ringVibar = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    ringVibar->setContentWidgetData( "text", hbTrId( "txt_cp_list_ring_alet" ) );
    ringVibar->setContentWidgetData( "checkState", profileSettings.mRingAlertVibra?2:0 );    

    HbDataFormModelItem *messageVibra = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    messageVibra->setContentWidgetData( "text", hbTrId( "txt_cp_list_received_message" ) );
    messageVibra->setContentWidgetData( "checkState", profileSettings.mMessageVibra?2:0 );

    HbDataFormModelItem *emailVibra = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    emailVibra->setContentWidgetData( "text", hbTrId( "txt_cp_list_received_email" ) );
    emailVibra->setContentWidgetData( "checkState", profileSettings.mEmailVibra?2:0 );

    HbDataFormModelItem *reminderVibra = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    reminderVibra->setContentWidgetData( "text", hbTrId( "txt_cp_list_reminder_alert" ));
    reminderVibra->setContentWidgetData( "checkState", profileSettings.mReminderAlertVibra?2:0 );
    
    HbDataFormModelItem *notificationVibra = mModel->appendDataFormItem( HbDataFormModelItem::CheckBoxItem, QString(), vibraGroup );
    notificationVibra->setContentWidgetData( "text", hbTrId( "txt_cp_list_notification" ) );
    notificationVibra->setContentWidgetData( "checkState", profileSettings.mNotificationVibra?2:0 );
    
    mMeetingSreenVibra = mModel->appendDataFormItem( HbDataFormModelItem::SliderItem, QString( hbTrId( "txt_cp_setlabel_touch_screen_vibra" ) ), vibraGroup );
    //TODO: profileModel need provide Max and Min value( 0-5 ), current max value from profileModel is 3
    mMeetingSreenVibra->setContentWidgetData( QString( "minimum" ), 0 );
    mMeetingSreenVibra->setContentWidgetData( QString( "maximum" ), 5 );
    mMeetingSreenVibra->setContentWidgetData( QString("value"), profileSettings.mKeyTouchScreenVibra );
    
    addConnection( ringVibar, SIGNAL( stateChanged( int )), this, SLOT( on_meeting_ringVibar_stateChanged( int )));
    addConnection( messageVibra, SIGNAL( stateChanged( int )), this, SLOT( on_meeting_messageVibra_stateChanged( int )));
    addConnection( emailVibra, SIGNAL( stateChanged( int )), this, SLOT( on_meeting_emailVibra_stateChanged( int )));
    addConnection( reminderVibra, SIGNAL( stateChanged( int )), this, SLOT( on_meeting_reminderVibra_stateChanged( int )));
    addConnection( notificationVibra, SIGNAL( stateChanged( int )), this, SLOT( on_meeting_notificationVibra_stateChanged( int )));
        
    addConnection( mMeetingSreenVibra, SIGNAL( valueChanged( int )), this, SLOT( on_meeting_screenVibra_ValueChanged( int )));
}*/

////////////////////////////////////////////////////
//general tones
void CpProfileSettingForm::on_general_notificationTones_stateChanged(int state)
{
    mProfileModel->setNotificationTone( EProfileWrapperGeneralId, checkBoxStateToBool( state ) );
}

void CpProfileSettingForm::on_general_keysAndScreenToneSlider_ValueChanged( int value )
{
    mProfileModel->setKeyTouchScreenTone( EProfileWrapperGeneralId, value );
    HbDataFormModelItem *modelItem = profileItem(EProfileWrapperGeneralId,ProfileItemKeyandTouchScreenTones);
    if (modelItem) {
        modelItem->setContentWidgetData( QString("value"), value );
        QMap< QString, QVariant > elements;
        if (value != 0) {      
            elements.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg"));
            elements.insert(QString("IncreaseElement"), QVariant(":/icon/hb_vol_slider_increment.svg"));
            elements.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_unmuted.svg") );            
        }
        else {
            elements.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg"));
            elements.insert(QString("IncreaseElement"), QVariant(":/icon/hb_vol_slider_increment.svg"));
            elements.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_muted.svg") );  
        }
        modelItem->setContentWidgetData( QString( "elementIcons" ), elements );            
    }
}


//general vibra
void CpProfileSettingForm::on_general_ringVibar_stateChanged( int state )
{
    mProfileModel->setRingAlertVibra( EProfileWrapperGeneralId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_general_messageVibra_stateChanged( int state )
{
    mProfileModel->setMessageVibra( EProfileWrapperGeneralId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_general_emailVibra_stateChanged( int state )
{
    mProfileModel->setEmailVibra( EProfileWrapperGeneralId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_general_reminderVibra_stateChanged( int state )
{
    mProfileModel->setReminderVibra( EProfileWrapperGeneralId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_general_notificationVibra_stateChanged( int state )
{
    mProfileModel->setNotificationVibra( EProfileWrapperGeneralId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_general_screenVibra_ValueChanged( int value )
{
    mProfileModel->setKeyTouchScreenVibra( EProfileWrapperGeneralId, value );
    
    HbDataFormModelItem *modelItem = profileItem(EProfileWrapperGeneralId,ProfileItemTouchScreenVibra);
    if (modelItem) {
        modelItem->setContentWidgetData( QString("value"), value );
    }

}

////////////////////////////////////////////////////
//meeting Tones
void CpProfileSettingForm::on_meeting_notificationTones_stateChanged(int state)
{
    mProfileModel->setNotificationTone( EProfileWrapperMeetingId, checkBoxStateToBool( state ) );
}

void CpProfileSettingForm::on_meeting_keysAndScreenToneSlider_ValueChanged( int value )
{
    mProfileModel->setKeyTouchScreenTone( EProfileWrapperMeetingId, value );
    
    HbDataFormModelItem *modelItem = profileItem(EProfileWrapperMeetingId,ProfileItemKeyandTouchScreenTones);
    if (modelItem) {
        modelItem->setContentWidgetData( QString("value"), value );
        QMap< QString, QVariant > elements;
        if (value != 0) {
            elements.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg"));
            elements.insert(QString("IncreaseElement"), QVariant(":/icon/hb_vol_slider_increment.svg"));
            elements.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_unmuted.svg") );            
        }
        else {
            elements.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg"));
            elements.insert(QString("IncreaseElement"), QVariant(":/icon/hb_vol_slider_increment.svg"));
            elements.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_muted.svg") );  
        }
        modelItem->setContentWidgetData( QString( "elementIcons" ), elements );            
    }
}


//meeting vibra
void CpProfileSettingForm::on_meeting_ringVibar_stateChanged( int state )
{
    mProfileModel->setRingAlertVibra( EProfileWrapperMeetingId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_meeting_messageVibra_stateChanged( int state )
{
    mProfileModel->setMessageVibra( EProfileWrapperMeetingId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_meeting_emailVibra_stateChanged( int state )
{
    mProfileModel->setEmailVibra( EProfileWrapperMeetingId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_meeting_reminderVibra_stateChanged( int state )
{
    mProfileModel->setReminderVibra( EProfileWrapperMeetingId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_meeting_notificationVibra_stateChanged( int state )
{
    mProfileModel->setNotificationVibra( EProfileWrapperMeetingId, checkBoxStateToBool( state ) );
}
void CpProfileSettingForm::on_meeting_screenVibra_ValueChanged( int value )
{
    mProfileModel->setKeyTouchScreenVibra( EProfileWrapperMeetingId, value );
    
    HbDataFormModelItem *modelItem = profileItem(EProfileWrapperMeetingId,ProfileItemTouchScreenVibra);
    if (modelItem) {
        modelItem->setContentWidgetData( QString("value"), value );
    }
}

bool CpProfileSettingForm::checkBoxStateToBool( int state )
{
    if( state == Qt::Checked ) {
        return true;
    } else {
        return false;
    }
}


void CpProfileSettingForm::settingValueChanged(const XQSettingsKey &key, const QVariant &value)
{
    if (key.uid() == KCRUidProfileEngine.iUid && key.key() == KProEngSilenceMode && value.isValid()) {
    
		static const int silenceSensitiveModelItemIds[] = 
		{       
			CpProfileSettingForm::ProfileItemRingTone,
			CpProfileSettingForm::ProfileItemMessageTone,
			CpProfileSettingForm::ProfileItemEmailTone,
			CpProfileSettingForm::ProfileItemReminderTone,
			CpProfileSettingForm::ProfileItemNotificationTones,
			CpProfileSettingForm::ProfileItemKeyandTouchScreenTones
		};
    
        QHash< int,QHash<int,HbDataFormModelItem*>  >::const_iterator it (mProfileModelItems.begin());
        for(; it != mProfileModelItems.end(); ++it ) {          
            for (int i = 0; i < sizeof(silenceSensitiveModelItemIds)/sizeof(silenceSensitiveModelItemIds[0]);++i) {           
                QHash<int,HbDataFormModelItem*>::const_iterator found = it.value().find(silenceSensitiveModelItemIds[i]);
                if (found != it.value().end()) {
                    found.value()->setEnabled(!value.toBool());
                }
            }   
        }
    }
}

HbDataFormModelItem *CpProfileSettingForm::profileItem(int profileId,int profileItemId)
{
    return mProfileModelItems.value(profileId).value(profileItemId);
}

//End of File
