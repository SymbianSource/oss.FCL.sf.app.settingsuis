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
#include "cpprofilemodel.h"
#include <cplogger.h>
#include <e32base.h>
#include <QString>
#include <MProfileEngineExtended2.h>
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
#include <MProfilesNamesArray.h>
#include <SettingsInternalCRKeys.h>
#include <hbglobal.h>
#include <QtCore/QStringList>
#include <MProfileExtended2.h>
#include <MProfileSetExtraTones2.h>
#include <MProfileExtraTones2.h>
#include <MProfileVibraSettings.h>
#include <MPRofileSetVibraSettings.h>
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
    : mEngine(0),
      //mProfileExt(0),
      //mToneSettings(0),
      //mFeedbackSettings(0),
      //mSetFeedbackSettings(0),
      //mVibraCenRep(0),
      mEditingProfileId(0),
      mInitErrFlag(false),
      mOffLineCenRep(0),
      q_ptr(0)
{
    
}

void CpProfileModelPrivate::initialize(CpProfileModel *parent)
{
    q_ptr = parent;
    CPFW_LOG("CpProfileModelPrivate::CpProfileModelPrivate(), START.");
    TRAPD( err,
            mEngine = CreateProfileEngineExtended2L();
            //int proId = mEngine->ActiveProfileId();
            //UpdateProfileSettingsL( proId );
            //mVibraCenRep = CRepository::NewL( KCRUidVibraCtrl );
            mOffLineCenRep = CRepository::NewL( KCRUidCommunicationSettings );
    );
    mInitErrFlag = err;
    CPFW_LOG("CpProfileModelPrivate::CpProfileModelPrivate(), END, err is." + QString(err) );
}    
void CpProfileModelPrivate::UpdateProfileSettingsL(int profileId)
{
    CPFW_LOG("CpProfileModelPrivate::UpdateProfileSettingsL(), START. profile id is: " + QString(profileId));
    /*if ( profileId <= EProfileWrapperErrorId || profileId >= EProfileWrapperOverflowId ){
        CPFW_LOG("CpProfileModelPrivate::UpdateProfileSettingsL(), End with invalid profile id.");
        //User::Leave(KErrNotFound);
        return;
    }
*/
/*    mProfileExt = mEngine->Profile2L( profileId ); 
    CPFW_LOG("CpProfileModelPrivate::UpdateProfileSettingsL(), Succeded in getting mProfileExt. ");

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

    mEditingProfileId = profileId;
    CPFW_LOG("CpProfileModelPrivate::UpdateProfileSettingsL(), END.");*/
}

CpProfileModelPrivate::~CpProfileModelPrivate()
{
    //delete mEngine;
    if (mEngine!=0) {
        mEngine->Release();
    }
    //delete mVibraCenRep;
    //mVibraCenRep = 0;    
    delete mOffLineCenRep;
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
QString CpProfileModelPrivate::profileName(int profileId) const
{
    CPFW_LOG("CpProfileModelPrivate::profileName(), START.");

//    MProfileName* name = 0;
//    TRAPD( err, *name = mEngine->ProfileL(profileId)->ProfileName(); );
//    
//    if ( !err && name){
//        CPFW_LOG("CpProfileModelPrivate::profileName(), have name, END.");
//        return stringFromDescriptor( name->Name() );
//    } else {
//        CPFW_LOG("CpProfileModelPrivate::profileName(), no name, END.");
//        return "";
//    }
    // should return qt localized profile name  
    // engine return symbian localized name, that is a wrong way to got it
    switch (profileId) {
    case EProfileWrapperGeneralId:
        return hbTrId("txt_cp_setlabel_active_profile_val_general");
    case EProfileWrapperMeetingId:
        return hbTrId("txt_cp_setlabel_active_profile_val_meeting");
    default:
        return QString("");
    }
}
QStringList CpProfileModelPrivate::profileNames() const
{
    //hard code, until engine part support qt localized name
    QStringList nameList;
    nameList<<hbTrId("txt_cp_setlabel_active_profile_val_general")
            <<hbTrId("txt_cp_setlabel_active_profile_val_meeting");
    return nameList;
}


/*
 * Activate a profile with its id, return the operation code.
 */
int CpProfileModelPrivate::activateProfile(int profileId)
{
    CPFW_LOG("CpProfileModelPrivate::activateProfile(), START.");

    TRAPD( err, 
        mEngine->SetActiveProfileL( profileId );
        //UpdateProfileSettingsL( profileId );
    );
    CPFW_LOG("CpProfileModelPrivate::activateProfile(), END.");
    return err;
}

/*
 * Get active profile's id
 */
int CpProfileModelPrivate::activeProfileId()
{
    return mEngine->ActiveProfileId();
}

int CpProfileModelPrivate::profileSettings(int profileId,
        CpProfileSettings& profileSettings)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
//            CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return err;
    } 
    CleanupReleasePushL(*profileExtend);
    const MProfileTones &setTones = profileExtend->ProfileTones();
    const TProfileToneSettings &toneSettings = setTones.ToneSettings();
    const MProfileExtraTones2 &extTones = profileExtend->ProfileExtraTones2();
    const MProfileVibraSettings &vibraSettings =
            profileExtend->ProfileVibraSettings();
    const MProfileExtraSettings &extraSettings =
            profileExtend->ProfileExtraSettings();
    const MProfileFeedbackSettings &feedbackSettings =
            extraSettings.ProfileFeedbackSettings();
    
    
    profileSettings.mRingTone = stringFromDescriptor(setTones.RingingTone1());
    profileSettings.mMessageTone = stringFromDescriptor(setTones.MessageAlertTone());
    profileSettings.mEmailTone = stringFromDescriptor(extTones.EmailAlertTone());
    profileSettings.mReminderTone = stringFromDescriptor(extTones.ReminderTone());
    profileSettings.mNotificationTone = toneSettings.iWarningAndGameTones;
    profileSettings.mKeyTouchScreenTone = toneSettings.iKeypadVolume;
    
    profileSettings.mRingAlertVibra = vibraSettings.RingAlertVibra();
    profileSettings.mMessageVibra = vibraSettings.MessageAlertVibra();
    profileSettings.mEmailVibra = vibraSettings.EmailAlertVibra();
    profileSettings.mReminderAlertVibra = vibraSettings.ReminderAlarmVibra();
    profileSettings.mNotificationVibra = vibraSettings.InformationVibra();
    profileSettings.mKeyTouchScreenVibra = feedbackSettings.TactileFeedback();
    
    CleanupStack::PopAndDestroy(profileExtend);
    return err;    
}
int CpProfileModelPrivate::setProfileSettings(int profileId, CpProfileSettings& profileSettings)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            
            )
    if (err!=KErrNone) {
        return err;
    }
    CleanupReleasePushL(*profileExtend);
    MProfileSetTones &setTones = profileExtend->ProfileSetTones();
    TProfileToneSettings &toneSettings = setTones.SetToneSettings();
    MProfileSetExtraTones2 &setExtTones = profileExtend->ProfileSetExtraTones2();
    MProfileSetVibraSettings &setVibraSettings = profileExtend->ProfileSetVibraSettings();
    MProfileSetExtraSettings &extraSettings = profileExtend->ProfileSetExtraSettings();
    MProfileSetFeedbackSettings &setFeedbackSettings = extraSettings.ProfileSetFeedbackSettings();

    TRAP_IGNORE(
            setTones.SetRingingTone1L( *descriptorFromString(profileSettings.mRingTone) );
            setTones.SetMessageAlertToneL( *descriptorFromString(profileSettings.mMessageTone) );
            setExtTones.SetEmailAlertToneL( *descriptorFromString(profileSettings.mEmailTone) );
            setExtTones.SetReminderToneL( *descriptorFromString(profileSettings.mReminderTone) );
            )

    toneSettings.iWarningAndGameTones = profileSettings.mNotificationTone;
    toneSettings.iKeypadVolume = static_cast<TProfileKeypadVolume>(profileSettings.mKeyTouchScreenTone);
    setVibraSettings.SetRingAlertVibra( profileSettings.mRingAlertVibra );
    setVibraSettings.SetMessageAlertVibra ( profileSettings.mMessageVibra );
    setVibraSettings.SetEmailAlertVibra ( profileSettings.mEmailVibra );
    setVibraSettings.SetReminderAlarmVibra ( profileSettings.mReminderAlertVibra );
    setVibraSettings.SetInformationVibra ( profileSettings.mNotificationVibra );
    setFeedbackSettings.SetTactileFeedback(static_cast<TProfileTactileFeedback>(profileSettings.mKeyTouchScreenVibra));    

    TRAPD(err2, 
            mEngine->CommitChangeL(*profileExtend);
            CleanupStack::PopAndDestroy(profileExtend);
             )
    return err2;
}

QString CpProfileModelPrivate::ringTone() const
{
    MProfileExtended2 *profileExtend = 0;
    
    TRAPD(err,
            profileExtend = mEngine->Profile2L(EProfileWrapperGeneralId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err!=KErrNone) {
        return QString("");
    }
    CleanupReleasePushL(*profileExtend);
   
    QString ringTone = stringFromDescriptor(profileExtend->ProfileTones().RingingTone1());
    CleanupStack::PopAndDestroy(profileExtend);
    return ringTone;
    CPFW_LOG("CpProfileModelPrivate::ringTone(), END with invalid mProfileExt. ");
    
}

void CpProfileModelPrivate::setRingTone(const QString& filePath)
{
    TInt ids[] = {EProfileGeneralId,EProfileMeetingId};

    for (TInt i = 0; i < sizeof(ids)/sizeof(ids[0]); ++i)
    {
    MProfileExtended2 *profileExtend = 0;
        TRAP_IGNORE(
                profileExtend = mEngine->Profile2L(ids[i]);
                )
        CleanupReleasePushL(*profileExtend);
        MProfileSetTones &setTones = profileExtend->ProfileSetTones();
        setTones.SetRingingTone1L( *descriptorFromString(filePath) );
        mEngine ->CommitChangeL(*profileExtend);
        CleanupStack::PopAndDestroy(profileExtend);
    }   
}

int CpProfileModelPrivate::masterVolume() const
{
    //NEED L FUNCTION?
    TInt masterVolume = mEngine->MasterVolumeL();
    return masterVolume;
}

void CpProfileModelPrivate::setMasterVolume(int volume)
{
    //NEED L FUNCTION ?
    TRAP_IGNORE(mEngine->SetMasterVolumeL( volume );)
}

bool CpProfileModelPrivate::masterVibra() const
{
    bool masterVibra = mEngine->MasterVibraL();
    return masterVibra; 
}
void CpProfileModelPrivate::setMasterVibra(bool isVibra)
{
    TRAP_IGNORE(mEngine->SetMasterVibraL( isVibra );)    
}

bool CpProfileModelPrivate::isSilenceMode() const
{
    bool isSlience = mEngine->SilenceModeL();
    return isSlience;
}
void CpProfileModelPrivate::setSilenceMode(bool isSilence)
{
    TRAP_IGNORE(mEngine->SetSilenceModeL( isSilence );)    
}

bool CpProfileModelPrivate::isOffLineMode() const    
{
    int offLineMode = 0;
    User::LeaveIfError(mOffLineCenRep->Get( KSettingsAirplaneMode, offLineMode ));
    
    return offLineMode;
}
void CpProfileModelPrivate::setOffLineMode(bool isOffLine)
{
    User::LeaveIfError(mOffLineCenRep->Set(KSettingsAirplaneMode, isOffLine));
}

QString CpProfileModelPrivate::ringTone(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return QString("");
    }
    CleanupReleasePushL(*profileExtend);
    const MProfileTones &setTones = profileExtend->ProfileTones();
    
    
    QString ringTone = stringFromDescriptor(setTones.RingingTone1());
    CleanupStack::PopAndDestroy(profileExtend);
    return ringTone;
}

void CpProfileModelPrivate::setRingTone(int profileId, const QString& filePath)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetTones &setTones = profileExtend->ProfileSetTones();

        TRAP_IGNORE(
                setTones.SetRingingTone1L( *descriptorFromString(filePath) );
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }            
}

QString CpProfileModelPrivate::messageTone(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return QString("");
    }
    CleanupReleasePushL(*profileExtend);
    const MProfileTones &setTones = profileExtend->ProfileTones();
    
    QString messageTone = stringFromDescriptor(setTones.MessageAlertTone());
       
    CleanupStack::PopAndDestroy(profileExtend);
    return messageTone;    

}

void CpProfileModelPrivate::setMessageTone(int profileId, const QString& filePath)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetTones &setTones = profileExtend->ProfileSetTones();

        TRAP_IGNORE(
                setTones.SetMessageAlertToneL( *descriptorFromString(filePath) );
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }            

}

QString CpProfileModelPrivate::emailTone(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return QString("");
    }
    CleanupReleasePushL(*profileExtend);
    const MProfileExtraTones2 &extTones = profileExtend->ProfileExtraTones2(); 
    
    QString emailTone = stringFromDescriptor(extTones.EmailAlertTone());
    
    CleanupStack::PopAndDestroy(profileExtend);
    return emailTone;    
    
}

void CpProfileModelPrivate::setEmailTone(int profileId, const QString& filePath)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetExtraTones2 &setExtTones = profileExtend->ProfileSetExtraTones2();

        TRAP_IGNORE(
                setExtTones.SetEmailAlertToneL( *descriptorFromString(filePath) );
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }            
}

QString CpProfileModelPrivate::reminderTone(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return QString("");
    } 
    CleanupReleasePushL(*profileExtend);
    const MProfileExtraTones2 &extTones = profileExtend->ProfileExtraTones2();
    
    QString reminderTone = stringFromDescriptor(extTones.ReminderTone());
    
    CleanupStack::PopAndDestroy(profileExtend);
    return reminderTone;    
    
}

void CpProfileModelPrivate::setReminderTone(int profileId, const QString& filePath)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetExtraTones2 &setExtTones = profileExtend->ProfileSetExtraTones2();

        TRAP_IGNORE(
                setExtTones.SetReminderToneL( *descriptorFromString(filePath) );
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }            
}

bool CpProfileModelPrivate::notificationTone(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return false; // ??????
    } 
    CleanupReleasePushL(*profileExtend);
    const MProfileTones &setTones = profileExtend->ProfileTones();
    const TProfileToneSettings &toneSettings = setTones.ToneSettings();
    
    bool notificationTone = toneSettings.iWarningAndGameTones;
    
    CleanupStack::PopAndDestroy(profileExtend);
    return notificationTone;       
}

void CpProfileModelPrivate::setNotificationTone(int profileId, bool isActive)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetTones &setTones = profileExtend->ProfileSetTones();
        TProfileToneSettings &toneSettings = setTones.SetToneSettings();


        toneSettings.iWarningAndGameTones = isActive;
                
        TRAP_IGNORE( 
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }        
}

int CpProfileModelPrivate::keyTouchScreenTone(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return -1;
    } 
    CleanupReleasePushL(*profileExtend);
    const MProfileTones &setTones = profileExtend->ProfileTones();
    const TProfileToneSettings &toneSettings = setTones.ToneSettings();
    
    int keyTouchScreenTone = toneSettings.iKeypadVolume;
    
    CleanupStack::PopAndDestroy(profileExtend);
    return keyTouchScreenTone;    
    
}

void CpProfileModelPrivate::setKeyTouchScreenTone(int profileId, int level)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetTones &setTones = profileExtend->ProfileSetTones();
        TProfileToneSettings &toneSettings = setTones.SetToneSettings();

        toneSettings.iKeypadVolume = static_cast<TProfileKeypadVolume>(level);
                
        TRAP_IGNORE( 
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }
    
}

bool CpProfileModelPrivate::ringAlertVibra(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return false;
    } 
    CleanupReleasePushL(*profileExtend);
    const MProfileVibraSettings &vibraSettings =
            profileExtend->ProfileVibraSettings();
    
    
    bool ringAlertVibra = vibraSettings.RingAlertVibra();
    CleanupStack::PopAndDestroy(profileExtend);
    return ringAlertVibra;    
    
}

void CpProfileModelPrivate::setRingAlertVibra(int profileId, bool isActive)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetVibraSettings &setVibraSettings = profileExtend->ProfileSetVibraSettings();
        
        setVibraSettings.SetRingAlertVibra(isActive);
                
        TRAP_IGNORE( 
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }
}

bool CpProfileModelPrivate::messageVibra(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return false;
    } 
    CleanupReleasePushL(*profileExtend);
    const MProfileVibraSettings &vibraSettings =
            profileExtend->ProfileVibraSettings();
    
    
    bool messageVibra = vibraSettings.MessageAlertVibra();
    CleanupStack::PopAndDestroy(profileExtend);
    return messageVibra;    
    
}

void CpProfileModelPrivate::setMessageVibra(int profileId, bool isActive)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);    
        MProfileSetVibraSettings &setVibraSettings = profileExtend->ProfileSetVibraSettings();
        
        setVibraSettings.SetMessageAlertVibra(isActive);
                
        TRAP_IGNORE( 
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }
}

bool CpProfileModelPrivate::emailVibra(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return false;
    } 
    CleanupReleasePushL(*profileExtend);
    const MProfileVibraSettings &vibraSettings =
            profileExtend->ProfileVibraSettings();
    
    
    bool emailVibra = vibraSettings.EmailAlertVibra();
    
    CleanupStack::PopAndDestroy(profileExtend);
    return emailVibra;    
    
}

void CpProfileModelPrivate::setEmailVibra(int profileId, bool isActive)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetVibraSettings &setVibraSettings = profileExtend->ProfileSetVibraSettings();
        
        setVibraSettings.SetEmailAlertVibra ( isActive );
        
        TRAP_IGNORE( 
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }
}

bool CpProfileModelPrivate::reminderVibra(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return false;
    }
    CleanupReleasePushL(*profileExtend);
    const MProfileVibraSettings &vibraSettings =
            profileExtend->ProfileVibraSettings();
    
    
    bool reminderAlarmVibra = vibraSettings.ReminderAlarmVibra();
    
    CleanupStack::PopAndDestroy(profileExtend);
    return reminderAlarmVibra;    
    
}

void CpProfileModelPrivate::setReminderVibra(int profileId, bool isActive) 
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetVibraSettings &setVibraSettings = profileExtend->ProfileSetVibraSettings();
        
        setVibraSettings.SetReminderAlarmVibra ( isActive );
        
        TRAP_IGNORE( 
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }
}

bool CpProfileModelPrivate::notificationVibra(int profileId) const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return false;
    }
    CleanupReleasePushL(*profileExtend);
    const MProfileVibraSettings &vibraSettings =
            profileExtend->ProfileVibraSettings();
    
    
    bool notificationVibra = vibraSettings.InformationVibra();
    
    CleanupStack::PopAndDestroy(profileExtend);
    return notificationVibra;    
    
}

void CpProfileModelPrivate::setNotificationVibra(int profileId, bool isActive)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetVibraSettings &setVibraSettings = profileExtend->ProfileSetVibraSettings();
        
        setVibraSettings.SetInformationVibra( isActive );
        
        TRAP_IGNORE( 
                mEngine->CommitChangeL(*profileExtend);
                 )
        CleanupStack::PopAndDestroy(profileExtend);
    }                         
}

int CpProfileModelPrivate::keyTouchScreenVibra(int profileId)const
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD(err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err != KErrNone) {
        return -1;
    }
    CleanupReleasePushL(*profileExtend);
    const MProfileExtraSettings &extraSettings =
            profileExtend->ProfileExtraSettings();
    const MProfileFeedbackSettings &feedbackSettings =
            extraSettings.ProfileFeedbackSettings();
    
    
    bool keyTouchScreenVibra = feedbackSettings.TactileFeedback();
    
    CleanupStack::PopAndDestroy(profileExtend);
    return keyTouchScreenVibra;    
    
}

void CpProfileModelPrivate::setKeyTouchScreenVibra(int profileId, int level)
{
    MProfileExtended2 *profileExtend = 0;
    TRAPD( err,
            profileExtend = mEngine->Profile2L(profileId);
            //CleanupReleasePushL(*profileExtend);
            )
    if (err==KErrNone) {
        CleanupReleasePushL(*profileExtend);
        MProfileSetExtraSettings &extraSettings = profileExtend->ProfileSetExtraSettings();
        MProfileSetFeedbackSettings &setFeedbackSettings = extraSettings.ProfileSetFeedbackSettings();
        setFeedbackSettings.SetTactileFeedback(static_cast<TProfileTactileFeedback>(level));    

        TRAP_IGNORE(
                    mEngine->CommitChangeL(*profileExtend);
                    )

        CleanupStack::PopAndDestroy(profileExtend);                                               
    }             
}
///*
// * Set a profile as current editing profile
// */
//int CpProfileModelPrivate::setEditingProfile(int profileId)
//{
//    CPFW_LOG("CpProfileModelPrivate::setEditingProfile(), START.");
//    TRAPD( err, UpdateProfileSettingsL( profileId ););
//    CPFW_LOG("CpProfileModelPrivate::setEditingProfile(), END.");
//    return err;
//}
//
///*
// * Get path and file name of ring tone file
// */
//QString CpProfileModelPrivate::ringTone()
//{
//    if ( mProfileExt ){
//        return stringFromDescriptor( mProfileExt->ProfileTones().RingingTone1() );
//    }
//    CPFW_LOG("CpProfileModelPrivate::ringTone(), END with invalid mProfileExt. ");
//    return "";
//}
//
///*
// * Set path and file to ring tone
// */
//int CpProfileModelPrivate::setRingTone(const QString& filePath)
//{
//    if ( mProfileExt ){
//        TRAPD(err, mProfileExt->ProfileSetTones().SetRingingTone1L( *descriptorFromString(filePath) ));
//        commitChange();
//        return err;
//    } 
//	CPFW_LOG("CpProfileModelPrivate::setRingTone(), END with invalid mProfileExt.");
//    return -1;
//}
//
///*
// * Get path and file name of message tone file
// */
//QString CpProfileModelPrivate::messageTone()
//{
//    if ( mProfileExt ){
//        return stringFromDescriptor( mProfileExt->ProfileTones().MessageAlertTone() );
//    } 
//	CPFW_LOG("CpProfileModelPrivate::messageTone(), END with invalid mProfileExt.");
//    return "";
//}
//
///*
// * Set path and file to message tone
// */
//int CpProfileModelPrivate::setMessageTone(const QString& filePath)
//{
//    if ( mProfileExt ){
//        TRAPD(err, mProfileExt->ProfileSetTones().SetMessageAlertToneL( *descriptorFromString(filePath) ));
//        commitChange();
//	    return err;
//    } 
//	CPFW_LOG("CpProfileModelPrivate::setMessageTone(), END with invalid mProfileExt.");
//    return -1;
//}
//
///*
// * Get path and file name of email tone file
// */
//QString CpProfileModelPrivate::emailTone()
//{
//    if ( mProfileExt ){
//        return stringFromDescriptor( mProfileExt->ProfileExtraTones().EmailAlertTone() );
//    }
//	CPFW_LOG("CpProfileModelPrivate::emailTone(), END with invalid mProfileExt.");
//    return "";
//}
//
///*
// * Set path and file to email tone
// */
//int CpProfileModelPrivate::setEmailTone(const QString& filePath)
//{
//    if ( mProfileExt ){
//        TRAPD(err, mProfileExt->ProfileSetExtraTones().SetEmailAlertToneL( *descriptorFromString(filePath) ));
//        commitChange();
//	    return err;
//    } 
//	CPFW_LOG("CpProfileModelPrivate::setEmailTone(), END with invalid mProfileExt.");
//    return -1;
//}
//
///*
// * Get path and file name of calendar event tone file
// */
//QString CpProfileModelPrivate::calendarTone()
//{
//    return "";
//}
//
///*
// * Set path and file to calendar event tone
// */
//void CpProfileModelPrivate::setCalendarTone(const QString& filePath)
//{
//    Q_UNUSED(filePath);
//}
//
///*
// * Get path and file name of clock alarm tone file
// */
//QString CpProfileModelPrivate::alarmTone()
//{
//    return "";
//}
//
///*
// * Set path and file to clock alarm tone
// */
//void CpProfileModelPrivate::setAlarmTone(const QString& filePath)
//{
//    Q_UNUSED(filePath);
//}
//
///*
// * Get the value of master volume
// */
//int CpProfileModelPrivate::ringVolume()
//{
//    CPFW_LOG("CpProfileModelPrivate::ringVolume(), START.");
//    if ( mToneSettings ){
//	    CPFW_LOG("CpProfileModelPrivate::ringVolume(), End.");
//        return mToneSettings->iRingingVolume;
//    } 
//	CPFW_LOG("CpProfileModelPrivate::ringVolume(), END with invalid mToneSettings.");
//    return -1;
//}
//
///*
// * Set master volume, the value should be between 1-10
// */
//void CpProfileModelPrivate::setRingVolume(int volume)
//{
//    CPFW_LOG("CpProfileModelPrivate::setRingVolume(), START.");
//    if ( !mToneSettings ) {
//        CPFW_LOG("CpProfileModelPrivate::setRingVolume(), END with invalid mToneSettings.");
//        return;
//    }
//    if ( EProfileWrapperSilentId == mEditingProfileId || volume < 0 || volume >10 )
//        {
//        return;
//        }
//
//    mToneSettings->iRingingType = EProfileRingingTypeRinging;
//    mToneSettings->iRingingVolume = volume;
//    commitChange();
//    
//    // General profile and meeting profile should have same ring tone volume
//    int profileId = activeProfileId();
//    if ( EProfileWrapperMeetingId == profileId ){
//        profileId = EProfileWrapperGeneralId;
//    } else if ( EProfileWrapperGeneralId == profileId ) {
//        profileId = EProfileWrapperMeetingId;
//    }
//    MProfileExtended* profileExt = 0;
//    TRAPD( err0, profileExt = mEngine->ProfileL( profileId ););
//    Q_UNUSED(err0);
//    TProfileToneSettings& toneSettings = profileExt->ProfileSetTones().SetToneSettings();
//    toneSettings.iRingingType = EProfileRingingTypeRinging;
//    toneSettings.iRingingVolume = volume;
//    TRAPD( err, mEngine->CommitChangeL(*profileExt) );
//    Q_UNUSED(err);
//    CPFW_LOG("CpProfileModelPrivate::setRingVolume(), END.");
//    // currently implementation: keep the two profiles same volume
//}
//
///*
// * Activate master volume to beep
// */
//void CpProfileModelPrivate::activateBeep()
//{
//    if ( mToneSettings ){
//        mToneSettings->iRingingType = EProfileRingingTypeBeepOnce;
//        commitChange();
//    } else {
//        CPFW_LOG("CpProfileModelPrivate::activateBeep(), END with invalid mToneSettings.");
//    }
//}
//
///*
// * Get beep status in master volume
// */
//bool CpProfileModelPrivate::isBeep()
//{
//    if ( mToneSettings ){
//        return (EProfileRingingTypeBeepOnce == mToneSettings->iRingingType) ? true : false;
//    }
//    CPFW_LOG("CpProfileModelPrivate::isBeep(), END with invalid mToneSettings.");
//    return false;
//}
//
///*
// * Activate master volume to silent
// */
//void CpProfileModelPrivate::activateSilent()
//{
//    if ( mToneSettings ){
//        mToneSettings->iRingingType = EProfileRingingTypeSilent;
//        commitChange();
//    } else {
//        CPFW_LOG("CpProfileModelPrivate::vibraStatus(), END with invalid mToneSettings.");
//    }
//}
//
///*
// * Get silent status in master volume
// */
//bool CpProfileModelPrivate::isSilent()
//{
//    if ( mToneSettings ){
//        return (EProfileRingingTypeSilent == mToneSettings->iRingingType) ? true : false;
//    } else {
//        CPFW_LOG("CpProfileModelPrivate::vibraStatus(), END with invalid mToneSettings.");
//        return false;
//    }
//}
//
///*
// * Get master vibra's status
// */
//bool CpProfileModelPrivate::vibraStatus()
//{
//    CPFW_LOG("CpProfileModelPrivate::vibraStatus(), Start.");
//    if ( mToneSettings ){
//	    CPFW_LOG("CpProfileModelPrivate::vibraStatus(), End.");
//        return mToneSettings->iVibratingAlert;
//    } 
//	CPFW_LOG("CpProfileModelPrivate::vibraStatus(), END with invalid mToneSettings.");
//    return false;
//}
//
///*
// * Set master vibra's status
// */
//void CpProfileModelPrivate::setVibraStatus(bool status)
//{
//    CPFW_LOG("CpProfileModelPrivate::setVibraStatus(), Start.");
//    if ( mVibraCenRep ){
//        mVibraCenRep->Set(KVibraCtrlProfileVibraEnabled, status );
//    }
//    
//    if ( mToneSettings ){
//        mToneSettings->iVibratingAlert = status;
//        commitChange();
//    }
//    CPFW_LOG("CpProfileModelPrivate::setVibraStatus(), END.");
//}
//
///*
// * Get keypad' volume
// */
//int CpProfileModelPrivate::keyVolume()
//{
//    CPFW_LOG("CpProfileModelPrivate::keyVolume(), Start.");
//    if ( mToneSettings ){
//        CPFW_LOG("CpProfileModelPrivate::keyVolume(), END.");
//        return mToneSettings->iKeypadVolume;
//    }
//    CPFW_LOG("CpProfileModelPrivate::keyVolume(), END with invalid mToneSettings.");
//    return -1;
//}
//
///*
// * Set keypad's volume, 
// * the value of the volume should be between 0-3
// */
//void CpProfileModelPrivate::setKeyVolume(int volume)
//{
//    CPFW_LOG("CpProfileModelPrivate::setKeyVolume(), Start.");
//    if (volume < 0 || volume > 3)
//    {
//        CPFW_LOG("CpProfileModelPrivate::setKeyVolume(), End with invalid volume.");
//        return;
//    }
//
//    if ( mToneSettings ){
//        mToneSettings->iKeypadVolume = (TProfileKeypadVolume)volume;
//        commitChange();
//    }
//    CPFW_LOG("CpProfileModelPrivate::setKeyVolume(), End.");
//}
//
///*
// * Get screen tone's volume
// */
//int CpProfileModelPrivate::screenVolume()
//{
//    CPFW_LOG("CpProfileModelPrivate::screenVolume(), Start.");
//    if ( mFeedbackSettings ){
//        CPFW_LOG("CpProfileModelPrivate::screenVolume(), End.");
//        return mFeedbackSettings->AudioFeedback();
//    }
//    CPFW_LOG("CpProfileModelPrivate::screenVolume(), End.");
//    return -1;
//}
//
///*
// * Set screen tone's volume, 
// * the value of the volume should be between 0-3
// */
//void CpProfileModelPrivate::setScreenVolume(int volume)
//{
//    CPFW_LOG("CpProfileModelPrivate::setScreenVolume(), Start.");
//    if (volume < 0 || volume > 3)
//    {
//        CPFW_LOG("CpProfileModelPrivate::setScreenVolume(), End with invalid volume.");
//        return;
//    }
//
//    if ( mSetFeedbackSettings ){
//        mSetFeedbackSettings->SetAudioFeedback( (TProfileAudioFeedback)volume );
//        commitChange();
//    }
//    CPFW_LOG("CpProfileModelPrivate::setScreenVolume(), End.");
//}
//
///*
// * Get screen vibra's level
// */
//int CpProfileModelPrivate::screenVibra()
//{
//    CPFW_LOG("CpProfileModelPrivate::screenVibra(), Start.");
//    if ( mFeedbackSettings ){
//        CPFW_LOG("CpProfileModelPrivate::screenVibra(), End.");
//        return mFeedbackSettings->TactileFeedback();
//    }
//    CPFW_LOG("CpProfileModelPrivate::screenVibra(), End with invalid mFeedbackSettings.");
//    return -1;
//}
//
///*
// * Set screen vibra's level, 
// * the value of the level should be between 0-3
// */
//void CpProfileModelPrivate::setScreenVibra(int volume)
//{
//    if (volume < 0 || volume > 3)
//    {
//        return;
//    }
//
//    if ( mSetFeedbackSettings ){
//        mSetFeedbackSettings->SetTactileFeedback( (TProfileTactileFeedback)volume );
//        commitChange();
//    }
//}

/*
 * Commit changes when change settings value in profile.
 */
int CpProfileModelPrivate::commitChange()
{
  /*  TRAPD( err, mEngine->CommitChangeL(*mProfileExt) );
    return err;*/
    return 0;
}

// End of file

