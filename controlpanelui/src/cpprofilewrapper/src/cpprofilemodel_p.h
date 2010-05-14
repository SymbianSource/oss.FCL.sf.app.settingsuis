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

#ifndef CPPROFILEMODEL_P_H
#define CPPROFILEMODEL_P_H

#include <qglobal.h>
#include <QMap>
class CpProfileModel;
class CpProfileSettings;
class MProfileEngineExtended2;
class MProfileExtended2;
class MProfileFeedbackSettings;
class MProfileSetFeedbackSettings;
class CRepository;
class QStringList;
struct TProfileToneSettings;

class CpProfileModelPrivate
{
    Q_DECLARE_PUBLIC(CpProfileModel)
public:
    CpProfileModelPrivate();
    ~CpProfileModelPrivate();
    void initialize(CpProfileModel *parent);
public:
    int initiationFlag();
    QString profileName(int profileId)const;
    QStringList profileNames() const;
    int activateProfile(int profileId);
    int activeProfileId() const;
    void profileSettings(int profileId, CpProfileSettings& profileSettings);
    int setProfileSettings(int profileId, CpProfileSettings& profileSettings);

    /*!
     * set ring tone in personalization group in control panel's homeview
     */
    QString ringTone() const;
    void setRingTone(const QString& filePath);

    int masterVolume() const;
    void setMasterVolume(int volume);

    bool masterVibra() const;
    void setMasterVibra(bool isVibra);

    bool silenceMode() const;
    void setSilenceMode(bool isSlience);

    bool offLineMode() const;    
    void setOffLineMode(bool isOffLine);

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
    void setReminderVibra(int profileId, bool isActive) ;
    
    bool notificationVibra(int profileId) const;
    void setNotificationVibra(int profileId, bool isActive);
    
    int keyTouchScreenVibra(int profileId)const;
    void setKeyTouchScreenVibra(int profileId, int level);
    /*    int setEditingProfile(int profileId);
     QString ringTone();
     int setRingTone(const QString& filePath);
     QString messageTone();
     int setMessageTone(const QString& filePath);
     QString emailTone();
     int setEmailTone(const QString& filePath);
     QString calendarTone();
     void setCalendarTone(const QString& filePath);
     QString alarmTone();
     void setAlarmTone(const QString& filePath);
     int ringVolume();
     void setRingVolume(int volume);
     void activateBeep();
     bool isBeep();
     void activateSilent();
     bool isSilent();
     bool vibraStatus();
     void setVibraStatus(bool status);
     int keyVolume();
     void setKeyVolume(int volume);
     int screenVolume();
     void setScreenVolume(int volume);
     int screenVibra();
     void setScreenVibra(int volume);
     */
#ifdef Q_OS_SYMBIAN
private:
    //void UpdateProfileSettingsL(int profileId);
    //int commitChange();

private:
    MProfileEngineExtended2 *mEngine;
    QMap<int, MProfileExtended2*> mProfileList;
    //MProfileExtended2 *mProfileExt;
    //TProfileToneSettings *mToneSettings;
    //const MProfileFeedbackSettings *mFeedbackSettings;
    //MProfileSetFeedbackSettings *mSetFeedbackSettings;
    //CRepository *mVibraCenRep;
    int mInitErrFlag;
    CRepository *mOffLineCenRep;
    CpProfileModel *q_ptr;
#endif // Q_OS_SYMBIAN
};

#endif
