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

#ifndef CPPROFILESETTINGFORM_H
#define CPPROFILESETTINGFORM_H

#include <hbdataform.h>

class HbDataFormModel;
class HbDataFormModelItem;
class CpItemDataHelper;
class CpProfileModel;
class QFileIconProvider;

class CpProfileSettingForm : public HbDataForm
{
    Q_OBJECT
public:
    CpProfileSettingForm();
    virtual ~CpProfileSettingForm();
    
private slots:
    
    //general tones
    void on_general_notificationTones_stateChanged(int state);
    void on_general_keysAndScreenToneSlider_ValueChanged( int value );
    
    //general vibra
    void on_general_ringVibar_stateChanged( int state );
    void on_general_messageVibra_stateChanged( int state );
    void on_general_emailVibra_stateChanged( int state );
    void on_general_reminderVibra_stateChanged( int state );
    void on_general_notificationVibra_stateChanged( int state );
    void on_general_screenVibra_ValueChanged( int value );

    //meeting tones
    void on_meeting_notificationTones_stateChanged(int state);
    void on_meeting_keysAndScreenSlider_ValueChanged( int value );
    
    //meeting vibar
    void on_meeting_ringVibar_stateChanged( int state );
    void on_meeting_messageVibra_stateChanged( int state );
    void on_meeting_emailVibra_stateChanged( int state );
    void on_meeting_reminderVibra_stateChanged( int state );
    void on_meeting_notificationVibra_stateChanged( int state );
    void on_meeting_screenVibra_ValueChanged( int value );
private:
    void initModel();
    void initGeneralTonesGroup();
    //void initGeneralVibraGroup();
    void initMeetingTonesGroup();
    //void initMeetingVibraGroup();
    bool checkBoxStateToBool( int state );
 //   void initRingToneGroup(HbDataFormModelItem *parent);
 //   void initMessageToneGroup(HbDataFormModelItem *parent);
 //   void initAlertToneGroup(HbDataFormModelItem *parent);
 //   void initKeyAndScreenToneGroup(HbDataFormModelItem *parent);
private:
    HbDataFormModel *mModel;
    CpItemDataHelper *mItemDataHelper;
    CpProfileModel  *mProfileModel;
    QFileIconProvider *mFileIconProvider;
    
    HbDataFormModelItem *mGeneralPage;
    HbDataFormModelItem *mMeetingPage;
    
    HbDataFormModelItem *mCurrentPage;
    
    HbDataFormModelItem *mGeneralKeysAndScreenToneSlider;
    HbDataFormModelItem *mGeneralSreenVibra;
    HbDataFormModelItem *mMeetingKeysAndScreenToneSlider;
    HbDataFormModelItem *mMeetingSreenVibra;
};


#endif //CPPROFILESETTINGFORM_H
