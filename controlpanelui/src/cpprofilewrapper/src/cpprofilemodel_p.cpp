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

#include "cpprofilemodel_p.h"
#include <e32base.h>
#include <QString>
#include <MProfileEngineExtended.h>
#include <MProfileExtended.h>
#include <MProfileName.h>
#include <MProfileTones.h>
#include <MProfileSetTones.h>
#include <MProfileExtraTones.h>
#include <MProfileSetExtraTones.h>
#include <MProfileExtraSettings.h>
#include <MProfileSetExtraSettings.h>
#include <MProfileFeedbackSettings.h>
#include <MProfileSetFeedbackSettings.h>

#include <TProfileToneSettings.h>
#include <hwrmvibrasdkcrkeys.h>
#include <centralrepository.h>


QString stringFromDescriptor(const TDesC& dsp)
{
    return QString::fromUtf16(dsp.Ptr(), dsp.Length());
}

HBufC* descriptorFromString(const QString& str)
{
    TPtrC ptr(reinterpret_cast<const TUint16*>(str.utf16()));
    return ptr.Alloc();
}

CpProfileModelPrivate::CpProfileModelPrivate()
{
    TRAPD( err, 
        mEngine = CreateProfileEngineExtendedL();
        UpdateProfileL();
        mVibraCenRep = CRepository::NewL( KCRUidVibraCtrl );
    );
	mInitErrFlag = err;
}

void CpProfileModelPrivate::UpdateProfileL()
{
    mProfileExt = mEngine->ProfileL( mEngine->ActiveProfileId() );

    // General tones
    TProfileToneSettings& toneSettings = mProfileExt->ProfileSetTones().SetToneSettings();
    mToneSettings = &toneSettings;

    // Feedback settings, used to get screen tone
    const MProfileFeedbackSettings& feedback = 
                            mProfileExt->ProfileExtraSettings().ProfileFeedbackSettings();
    mFeedbackSettings = &feedback;

    // Feedback settings, used to set screen tone
    MProfileSetFeedbackSettings& setFeedback = 
                            mProfileExt->ProfileSetExtraSettings().ProfileSetFeedbackSettings();
    mSetFeedbackSettings = &setFeedback;
}

CpProfileModelPrivate::~CpProfileModelPrivate()
{
    delete mVibraCenRep;
    mVibraCenRep = NULL;
}

/*
 * Get the result of the initiation
 */
int CpProfileModelPrivate::initiationFlag()
{
    return mInitErrFlag;
}

/*
 * Get profile name with its id
 */
QString CpProfileModelPrivate::profileName(int profileId)
{
    if ( profileId > 2 || profileId < 0 ){
        return "";
    }

    MProfileName* name = 0;
    TRAPD( err, 
        *name = mEngine->ProfileL(profileId)->ProfileName();
    );
    
    if (name){
        return stringFromDescriptor( name->Name() );
    } else {
        return "";
    }
    
}

/*
 * Activate a profile with its id, return the operation code.
 */
int CpProfileModelPrivate::activateProfile(int profileId)
{
    if ( profileId > 2 || profileId < 0 ){
        return -1;
    }

    TRAPD( err, 
        mEngine->SetActiveProfileL( profileId );
        UpdateProfileL();
    );
    return err;
}

/*
 * Get active profile's id
 */
int CpProfileModelPrivate::activeProfileId()
{
    return mEngine->ActiveProfileId();
}

/*
 * Get path and file name of ring tone file
 */
QString CpProfileModelPrivate::ringTone()
{
    return stringFromDescriptor( mProfileExt->ProfileTones().RingingTone1() );
}

/*
 * Set path and file to ring tone
 */
int CpProfileModelPrivate::setRingTone(const QString& filePath)
{
    TRAPD(err, mProfileExt->ProfileSetTones().SetRingingTone1L( *descriptorFromString(filePath) ));
    commitChange();
	return err;
}

/*
 * Get path and file name of message tone file
 */
QString CpProfileModelPrivate::messageTone()
{
    return stringFromDescriptor( mProfileExt->ProfileTones().MessageAlertTone() );
}

/*
 * Set path and file to message tone
 */
int CpProfileModelPrivate::setMessageTone(const QString& filePath)
{
    TRAPD(err, mProfileExt->ProfileSetTones().SetMessageAlertToneL( *descriptorFromString(filePath) ));
    commitChange();
	return err;
}

/*
 * Get path and file name of email tone file
 */
QString CpProfileModelPrivate::emailTone()
{
    return stringFromDescriptor( mProfileExt->ProfileExtraTones().EmailAlertTone() );
}

/*
 * Set path and file to email tone
 */
int CpProfileModelPrivate::setEmailTone(const QString& filePath)
{
    TRAPD(err, mProfileExt->ProfileSetExtraTones().SetEmailAlertToneL( *descriptorFromString(filePath) ));
    commitChange();
	return err;
}

/*
 * Get path and file name of calendar event tone file
 */
QString CpProfileModelPrivate::calendarTone()
{
    return "";
}

/*
 * Set path and file to calendar event tone
 */
void CpProfileModelPrivate::setCalendarTone(const QString& filePath)
{
    Q_UNUSED(filePath);
}

/*
 * Get path and file name of clock alarm tone file
 */
QString CpProfileModelPrivate::alarmTone()
{
    return "";
}

/*
 * Set path and file to clock alarm tone
 */
void CpProfileModelPrivate::setAlarmTone(const QString& filePath)
{
    Q_UNUSED(filePath);
}

/*
 * Get the value of master volume
 */
int CpProfileModelPrivate::ringVolume()
{
    return mToneSettings->iRingingVolume;
}

/*
 * Set master volume, the value should be between 1-10
 */
void CpProfileModelPrivate::setRingVolume(int volume)
{
    if (volume < 0 || volume >10)
        {
        return;
        }

    mToneSettings->iRingingType = EProfileRingingTypeRinging;
    mToneSettings->iRingingVolume = volume;
    commitChange();
}

/*
 * Activate master volume to beep
 */
void CpProfileModelPrivate::activateBeep()
{
    mToneSettings->iRingingType = EProfileRingingTypeBeepOnce;
    commitChange();
}

/*
 * Get beep status in master volume
 */
bool CpProfileModelPrivate::isBeep()
{
    return (EProfileRingingTypeBeepOnce == mToneSettings->iRingingType) ? true : false;
}

/*
 * Activate master volume to silent
 */
void CpProfileModelPrivate::activateSilent()
{
    mToneSettings->iRingingType = EProfileRingingTypeSilent;
    commitChange();
}

/*
 * Get silent status in master volume
 */
bool CpProfileModelPrivate::isSilent()
{
    return (EProfileRingingTypeSilent == mToneSettings->iRingingType) ? true : false;
}

/*
 * Get master vibra's status
 */
bool CpProfileModelPrivate::vibraStatus()
{
    return mToneSettings->iVibratingAlert;
}

/*
 * Set master vibra's status
 */
void CpProfileModelPrivate::setVibraStatus(bool status)
{
    mVibraCenRep->Set(KVibraCtrlProfileVibraEnabled, status );
    mToneSettings->iVibratingAlert = status;
    commitChange();
}

/*
 * Get keypad' volume
 */
int CpProfileModelPrivate::keyVolume()
{
    return mToneSettings->iKeypadVolume;
}

/*
 * Set keypad's volume, 
 * the value of the volume should be between 0-3
 */
void CpProfileModelPrivate::setKeyVolume(int volume)
{
    if (volume < 0 || volume > 3)
    {
        return;
    }

    mToneSettings->iKeypadVolume = (TProfileKeypadVolume)volume;
    commitChange();
}

/*
 * Get screen tone's volume
 */
int CpProfileModelPrivate::screenVolume()
{
    return mFeedbackSettings->AudioFeedback();
}

/*
 * Set screen tone's volume, 
 * the value of the volume should be between 0-3
 */
void CpProfileModelPrivate::setScreenVolume(int volume)
{
    if (volume < 0 || volume > 3)
    {
        return;
    }

    mSetFeedbackSettings->SetAudioFeedback( (TProfileAudioFeedback)volume );
    commitChange();
}

/*
 * Get screen vibra's level
 */
int CpProfileModelPrivate::screenVibra()
{
    return mFeedbackSettings->TactileFeedback();
}

/*
 * Set screen vibra's level, 
 * the value of the level should be between 0-3
 */
void CpProfileModelPrivate::setScreenVibra(int volume)
{
    if (volume < 0 || volume > 3)
    {
        return;
    }

    mSetFeedbackSettings->SetTactileFeedback( (TProfileTactileFeedback)volume );
    commitChange();
}

/*
 * Commit changes when change settings value in profile.
 */
int CpProfileModelPrivate::commitChange()
{
    TRAPD( err, mEngine->CommitChangeL(*mProfileExt) );
    return err;
}

// End of file

