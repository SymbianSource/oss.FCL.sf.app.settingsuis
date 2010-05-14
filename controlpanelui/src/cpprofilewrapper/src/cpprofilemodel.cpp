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

#include "cpprofilemodel.h"
#include "cpprofilemodel_p.h"
#include <QtCore/QStringList>

CpProfileModel::CpProfileModel(QObject *parent /*=0*/):QObject(parent),
                                                       d_ptr(new CpProfileModelPrivate())
{
    d_ptr->initialize(this);    
}

CpProfileModel::~CpProfileModel()
{
    delete d_ptr;
}

/*
 * Get the result of the initiation
 */
int CpProfileModel::initiationFlag()
{
    return d_ptr->initiationFlag();
}

/*
 * Get profile name with its id
 */
QString CpProfileModel::profileName(int profileId) const
{
    return d_ptr->profileName(profileId);
}
/*!
 * get profile name list 
 */
QStringList CpProfileModel::profileNames()const
{
    return d_ptr->profileNames();
}
/*
 * Activate a profile with its id, return the operation code.
 */
int CpProfileModel::activateProfile(int profileId)
{
    return d_ptr->activateProfile(profileId);
}

/*
 * Get active profile's id
 */
int CpProfileModel::activeProfileId() const
{
    return d_ptr->activeProfileId();
}

/*!
     Get profile settings, store in center repository keys 
 */
void CpProfileModel::profileSettings(int profileId, CpProfileSettings& profileSettings)
{
    d_ptr->profileSettings(profileId, profileSettings);
}

/*!
     Set profile settings from center repository keys
 */
int CpProfileModel::setProfileSettings(int profileId, CpProfileSettings& profileSettings )
{
    return d_ptr->setProfileSettings(profileId, profileSettings);
}

/*!
     Get ring tone of profiles, if the general profile's ring tone is different with 
     meeting ring tone, it returns the general profile's ring tone
 */
QString CpProfileModel::ringTone() const
{
    return d_ptr->ringTone();
}

/*!
     Set ring tone for all profiles
 */
void CpProfileModel::setRingTone(const QString& filePath)
{
    d_ptr->setRingTone(filePath);
}

/*!
     Return master volume for device
 */
int CpProfileModel::masterVolume() const
{
    return d_ptr->masterVolume();
}

/*!
     Set device's master volume
 */
void CpProfileModel::setMasterVolume(int volume)
{
    return d_ptr->setMasterVolume(volume);
}

/*!
     Return the master vibra's status of device
 */
bool CpProfileModel::masterVibra() const
{
    return d_ptr->masterVibra();
}

/*!
     Set the master vibra's status of device, if the master vibra is on,
     the vibra settings in profiles are valid, if the master is off, the 
     vibra settings in profiles are invalid.
 */
void CpProfileModel::setMasterVibra(bool isVibra)
{
    d_ptr->setMasterVibra(isVibra);
}

/*!
     Return the silence mode of device, silence mode will kill all outgoing 
     audio.
 */
bool CpProfileModel::silenceMode() const
{
    return d_ptr->silenceMode();
}

/*!
     Set silence mode for deivce
 */
void CpProfileModel::setSilenceMode(bool isSilence)
{
    d_ptr->setSilenceMode(isSilence);
}

/*!
     Return the status of off line mode (air plane mode) of device
 */
bool CpProfileModel::offLineMode() const
{
    return d_ptr->offLineMode();
}

/*!
     Set off line mode of device
 */
void CpProfileModel::setOffLineMode(bool isOffLine)
{
    d_ptr->setOffLineMode(isOffLine);
}

QString CpProfileModel::ringTone(int profileId)const
{
    return d_ptr->ringTone(profileId);
}

void CpProfileModel::setRingTone(int profileId, const QString& filePath)
{
    d_ptr->setRingTone(profileId, filePath);
}

QString CpProfileModel::messageTone(int profileId) const
{
    return d_ptr->messageTone(profileId);
}

void CpProfileModel::setMessageTone(int profileId, const QString& filePath)
{
    d_ptr->setMessageTone(profileId, filePath);
}

QString CpProfileModel::emailTone(int profileId) const
{
    return d_ptr->emailTone(profileId);
}

void CpProfileModel::setEmailTone(int profileId, const QString& filePath)
{
    d_ptr->setEmailTone(profileId,filePath);
}

QString CpProfileModel::reminderTone(int profileId) const
{
    return d_ptr->reminderTone(profileId);
}

void CpProfileModel::setReminderTone(int profileId, const QString& filePath)
{
    d_ptr->setReminderTone(profileId,filePath);
}

bool CpProfileModel::notificationTone(int profileId) const
{
    return d_ptr->notificationTone(profileId);
}

void CpProfileModel::setNotificationTone(int profileId, bool isActive)
{
    d_ptr->setNotificationTone(profileId, isActive);
}

int CpProfileModel::keyTouchScreenTone(int profileId) const
{
    return d_ptr->keyTouchScreenTone(profileId);
}

void CpProfileModel::setKeyTouchScreenTone(int profileId, int level)
{
    d_ptr->setKeyTouchScreenTone(profileId,level);
}

bool CpProfileModel::ringAlertVibra(int profileId) const
{
    return d_ptr->ringAlertVibra(profileId);
}

void CpProfileModel::setRingAlertVibra(int profileId, bool isActive)
{
    d_ptr->setRingAlertVibra(profileId,isActive);
}

bool CpProfileModel::messageVibra(int profileId) const
{
    return d_ptr->messageVibra(profileId);
}

void CpProfileModel::setMessageVibra(int profileId, bool isActive)
{
    return d_ptr->setMessageVibra(profileId,isActive);
}

bool CpProfileModel::emailVibra(int profileId) const
{
    return d_ptr->emailVibra(profileId);
}

void CpProfileModel::setEmailVibra(int profileId, bool isActive)
{
    d_ptr->setEmailVibra(profileId,isActive);    
}

bool CpProfileModel::reminderVibra(int profileId) const
{
    return d_ptr->reminderVibra(profileId);
}

void CpProfileModel::setReminderVibra(int profileId, bool isActive)
{
    d_ptr->setReminderVibra(profileId,isActive);    
}

bool CpProfileModel::notificationVibra(int profileId) const
{
    return d_ptr->notificationVibra(profileId);
}

void CpProfileModel::setNotificationVibra(int profileId, bool isActive)
{
    d_ptr->setNotificationVibra(profileId,isActive);
}

int CpProfileModel::keyTouchScreenVibra(int profileId)const
{
    return  d_ptr->keyTouchScreenVibra(profileId);
}

void CpProfileModel::setKeyTouchScreenVibra(int profileId, int level)
{
    d_ptr->setKeyTouchScreenVibra(profileId,level);
}

///*
// * Set a profile as current editing profile
// */
//int CpProfileModel::setEditingProfile(int profileId)
//{
//    return d_ptr->setEditingProfile(profileId);
//}
//
///*
// * Get path and file name of ring tone file
// */
//QString CpProfileModel::ringTone()
//{
//    return d_ptr->ringTone();
//}
//
///*
// * Set path and file to ring tone
// */
//int CpProfileModel::setRingTone(const QString& filePath)
//{
//    int err = d_ptr->setRingTone(filePath);
//	return err;
//}
//
///*
// * Get path and file name of message tone file
// */
//QString CpProfileModel::messageTone()
//{
//    return d_ptr->messageTone();
//}
//
///*
// * Set path and file to message tone
// */
//int CpProfileModel::setMessageTone(const QString& filePath)
//{
//    int err = d_ptr->setMessageTone(filePath);
//	return err;
//}
//
///*
// * Get path and file name of email tone file
// */
//QString CpProfileModel::emailTone()
//{
//    return d_ptr->emailTone();
//}
//
///*
// * Set path and file to email tone
// */
//int CpProfileModel::setEmailTone(const QString& filePath)
//{
//    int err = d_ptr->setEmailTone(filePath);
//	return err;
//}
//
///*
// * Get path and file name of calendar event tone file
// */
//QString CpProfileModel::calendarTone()
//{
//    return d_ptr->calendarTone();
//}
//
///*
// * Set path and file to calendar event tone
// */
//void CpProfileModel::setCalendarTone(const QString& filePath)
//{
//    d_ptr->setCalendarTone(filePath);
//}
//
///*
// * Get path and file name of clock alarm tone file
// */
//QString CpProfileModel::alarmTone()
//{
//    return d_ptr->alarmTone();
//}
//
///*
// * Set path and file to clock alarm tone
// */
//void CpProfileModel::setAlarmTone(const QString& filePath)
//{
//    d_ptr->setAlarmTone(filePath);
//}
//
///*
// * Get the value of master volume
// */
//int CpProfileModel::ringVolume()
//{
//    return d_ptr->ringVolume();
//}
//
///*
// * Set master volume, the value should be between 1-10
// */
//void CpProfileModel::setRingVolume(int volume)
//{
//    d_ptr->setRingVolume(volume);
//}
//
///*
// * Activate master volume to beep
// */
//void CpProfileModel::activateBeep()
//{
//    d_ptr->activateBeep();
//}
//
///*
// * Get beep status in master volume
// */
//bool CpProfileModel::isBeep()
//{
//    return d_ptr->isBeep();
//}
//
///*
// * Activate master volume to silent
// */
//void CpProfileModel::activateSilent()
//{
//    d_ptr->activateSilent();
//}
//
///*
// * Get silent status in master volume
// */
//bool CpProfileModel::isSilent()
//{
//    return d_ptr->isSilent();
//}
//
///*
// * Get master vibra's status
// */
//bool CpProfileModel::vibraStatus()
//{
//    return d_ptr->vibraStatus();
//}
//
///*
// * Set master vibra's status
// */
//void CpProfileModel::setVibraStatus(bool status)
//{
//    d_ptr->setVibraStatus(status);
//}
//
///*
// * Get keypad' volume
// */
//int CpProfileModel::keyVolume()
//{
//    return d_ptr->keyVolume();
//}
//
///*
// * Set keypad's volume, 
// * the value of the volume should be between 0-3
// */
//void CpProfileModel::setKeyVolume(int volume)
//{
//    d_ptr->setKeyVolume(volume);
//}
//
///*
// * Get screen tone's volume
// */
//int CpProfileModel::screenVolume()
//{
//    return d_ptr->screenVolume();
//}
//
///*
// * Set screen tone's volume, 
// * the value of the volume should be between 0-3
// */
//void CpProfileModel::setScreenVolume(int volume)
//{
//    d_ptr->setScreenVolume(volume);
//}
//
///*
// * Get screen vibra's level
// */
//int CpProfileModel::screenVibra()
//{
//    return d_ptr->screenVibra();
//}
//
///*
// * Set screen vibra's level, 
// * the value of the level should be between 0-3
// */
//void CpProfileModel::setScreenVibra(int volume)
//{
//    d_ptr->setScreenVibra(volume);
//}

// End of file

