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

#ifndef CPPROFILEMODEL_H
#define CPPROFILEMODEL_H

#include "cpprofilewrappermacro.h"
#include <QtCore/qglobal.h>
#include <QString>
#include <QtCore/QObject>
class CpProfileModelPrivate;

/*!
     Profile id is defined in profile engine
 */
enum ProfileWrapperProfileId
{
    EProfileWapperStart = -1,
    EProfileWrapperGeneralId = 0, 
    EProfileWrapperMeetingId = 2,
    EPRofileWrapperEnd    
};
class CpProfileSettings
{
public:
    QString mRingTone;
    QString mMessageTone;
    QString mEmailTone;
    QString mReminderTone;
    bool mNotificationTone;
    int mKeyTouchScreenTone; // 0-5
    
    bool mRingAlertVibra;
    bool mMessageVibra;
    bool mEmailVibra;
    bool mReminderAlertVibra;
    bool mNotificationVibra;
    int mKeyTouchScreenVibra; //0-5    
};
class PROFILE_WRAPPER_EXPORT CpProfileModel : QObject
{
    Q_OBJECT
public:
    CpProfileModel(QObject *parent = 0);
    ~CpProfileModel();

public:
    int initiationFlag();
    QString profileName(int profileId)const;
    QStringList profileNames()const;
    int activateProfile(int profileId);
    int activeProfileId() const;
    void profileSettings(int profileId, CpProfileSettings& profileSettings);
    int setProfileSettings(int profileId, CpProfileSettings& profileSettings );
    
    /*!
     * set ring tone in personalization group in control panel's homeview
     */
    QString ringTone() const;
    void setRingTone(const QString& filePath);
    
   /* QString alarmTone();
    void setAlarmTone(const QString& filePath);*/
    
    int masterVolume() const;
    void setMasterVolume(int volume);
    
    
    bool masterVibra() const;
    void setMasterVibra(bool isVibra);
        
    bool silenceMode() const;
    void setSilenceMode(bool isSlience);
    
    bool offLineMode() const;    
    void setOffLineMode(bool isOffLine);
    
    /*!
     *  For profile settings 
     */
    
    QString ringTone(int profileId)const;
    void setRingTone(int profileId, const QString& filePath);
    QString messageTone(int profileId) const;
    void setMessageTone(int profileId, const QString& filePath);
    QString emailTone(int profileId) const;
    void setEmailTone(int profileId, const QString& filePath);
    QString reminderTone(int profileId) const;
    void setReminderTone(int profileId, const QString& filePath);
    
    bool notificationTone(int profileId) const;
    void setNotificationTone(int profileId, bool isActive);
    
    int keyTouchScreenTone(int profileId) const;
    void setKeyTouchScreenTone(int profileId, int level);
    
    bool ringAlertVibra(int profileId) const;
    void setRingAlertVibra(int profileId, bool isActive);
    
    bool messageVibra(int profileId) const;
    void setMessageVibra(int profileId, bool isActive);
    
    bool emailVibra(int profileId) const;
    void setEmailVibra(int profileId, bool isActive);
    
    bool reminderVibra(int profileId) const;
    void setReminderVibra(int profileId, bool isActive);
    
    bool notificationVibra(int profileId) const;
    void setNotificationVibra(int profileId, bool isActive);
    
    int keyTouchScreenVibra(int profileId)const;
    void setKeyTouchScreenVibra(int profileId, int level);

private:
    CpProfileModelPrivate *const d_ptr;

private:    
    Q_DISABLE_COPY(CpProfileModel)
    Q_DECLARE_PRIVATE_D(d_ptr,CpProfileModel)
};

#endif
