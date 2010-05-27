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
#include <QMap>
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
#include <settingsinternalcrkeys.h>
#include <hbglobal.h>
#include <QtCore/QStringList>
#include <MProfileExtended2.h>
#include <MProfileSetExtraTones2.h>
#include <MProfileExtraTones2.h>
#include <MProfileVibraSettings.h>
#include <MProfileSetVibraSettings.h>
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
      mInitErrFlag(0),
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
            mOffLineCenRep = CRepository::NewL( KCRUidCommunicationSettings );
            
            /*
             * Currently, engine part will return all previous version of profile
             * so some invalid profile will be added in the new list, to avoid this 
             * use hard code to get the right list of profile. 
             */
            /*MProfilesNamesArray* nameList = mEngine->ProfilesNamesArrayLC();
            int profileCount = nameList->MdcaCount();
            for (int i = 0; i<profileCount; i++) {
                MProfileName *profileName = nameList->ProfileName(i);
                mProfileList.insert(profileName->Id(), mEngine->Profile2L(profileName->Id())); 
            }
            CleanupStack::PopAndDestroy(*nameList);*/
            mProfileList.insert(0, mEngine->Profile2L(0)); // general id 
            mProfileList.insert(2, mEngine->Profile2L(2));
    );
    
    // currently, reserve a error code for deal with low memory ... leave
    // as known, qt in symbian exception safety's development is ongoing. 
    // we will follow the official way to deal with symbian leave or exception
    mInitErrFlag = err;
    CPFW_LOG("CpProfileModelPrivate::CpProfileModelPrivate(), END, err is." + QString(err) );
}    

CpProfileModelPrivate::~CpProfileModelPrivate()
{
    if (mEngine!=0) {
        mEngine->Release();
    }
    foreach(MProfileExtended2* profile, mProfileList)
    {   
        if (profile!=0 ) {
        profile->Release();
        }
    }
    mProfileList.clear();
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
    // so hard code here, wait for engine's correcting.
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
int CpProfileModelPrivate::activeProfileId() const
{
    return mEngine->ActiveProfileId();
}

void CpProfileModelPrivate::profileSettings(int profileId,
        CpProfileSettings& profileSettings)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
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
						
}
int CpProfileModelPrivate::setProfileSettings(int profileId, CpProfileSettings& profileSettings)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    MProfileSetTones &setTones = profileExtend->ProfileSetTones();
    TProfileToneSettings &toneSettings = setTones.SetToneSettings();
    MProfileSetExtraTones2 &setExtTones =
            profileExtend->ProfileSetExtraTones2();
    MProfileSetVibraSettings &setVibraSettings =
            profileExtend->ProfileSetVibraSettings();
    MProfileSetExtraSettings &extraSettings =
            profileExtend->ProfileSetExtraSettings();
    MProfileSetFeedbackSettings &setFeedbackSettings =
            extraSettings.ProfileSetFeedbackSettings();
    // ignore here, wait for the exception deal framework of qt-symbian
    TRAP_IGNORE(
            setTones.SetRingingTone1L(*descriptorFromString(
                    profileSettings.mRingTone));
            setTones.SetMessageAlertToneL(*descriptorFromString(
                    profileSettings.mMessageTone));
            setExtTones.SetEmailAlertToneL(*descriptorFromString(
                    profileSettings.mEmailTone));
            setExtTones.SetReminderToneL(*descriptorFromString(
                    profileSettings.mReminderTone));
            )
    toneSettings.iWarningAndGameTones
            = profileSettings.mNotificationTone;
    toneSettings.iKeypadVolume
            = static_cast<TProfileKeypadVolume> (profileSettings.mKeyTouchScreenTone);
    setVibraSettings.SetRingAlertVibra(profileSettings.mRingAlertVibra);
    setVibraSettings.SetMessageAlertVibra(profileSettings.mMessageVibra);
    setVibraSettings.SetEmailAlertVibra(profileSettings.mEmailVibra);
    setVibraSettings.SetReminderAlarmVibra(
            profileSettings.mReminderAlertVibra);
    setVibraSettings.SetInformationVibra(
            profileSettings.mNotificationVibra);
    setFeedbackSettings.SetTactileFeedback(
            static_cast<TProfileTactileFeedback> (profileSettings.mKeyTouchScreenVibra));

    TRAPD(err,
            mEngine->CommitChangeL(*profileExtend);                
         )
    return err;
}

QString CpProfileModelPrivate::ringTone() const
{
    MProfileExtended2 *profileExtend = mProfileList.value(EProfileWrapperGeneralId);
    QString ringTone = stringFromDescriptor(
    profileExtend->ProfileTones().RingingTone1());
    return ringTone;
}

void CpProfileModelPrivate::setRingTone(const QString& filePath)
{
    int ids[] = {EProfileWrapperGeneralId,EProfileWrapperMeetingId};
    
    for (TInt i = 0; i < sizeof(ids)/sizeof(ids[0]); ++i) {        
        MProfileExtended2 *profileExtend = mProfileList.value(ids[i]);
        
        MProfileSetTones &setTones = profileExtend->ProfileSetTones();
        TRAP_IGNORE(
                setTones.SetRingingTone1L( *descriptorFromString(filePath) );
                mEngine ->CommitChangeL(*profileExtend);
                )
        // ERROR SHOULD BE DEAL HERE OR RETURN A ERROR CODE
    }     
}

int CpProfileModelPrivate::masterVolume() const
{
    int masterVolume = 0;
    TRAP_IGNORE(masterVolume = mEngine->MasterVolumeL();)
    return masterVolume;
}

void CpProfileModelPrivate::setMasterVolume(int volume)
{
    TRAP_IGNORE(mEngine->SetMasterVolumeL( volume );)
}

bool CpProfileModelPrivate::masterVibra() const
{
    bool masterVibra = false;
    TRAP_IGNORE(masterVibra = mEngine->MasterVibraL();)
    return masterVibra; 
}
void CpProfileModelPrivate::setMasterVibra(bool isVibra)
{
    TRAP_IGNORE(mEngine->SetMasterVibraL( isVibra );)    
}

bool CpProfileModelPrivate::silenceMode() const
{
    bool isSlience = false;
    TRAP_IGNORE(isSlience = mEngine->SilenceModeL();)
    return isSlience;
}
void CpProfileModelPrivate::setSilenceMode(bool isSilence)
{
    TRAP_IGNORE(mEngine->SetSilenceModeL( isSilence );)    
}

bool CpProfileModelPrivate::offLineMode() const    
{
    int offLineMode = 0;
    // What we should do if we can't set offline mode
    mOffLineCenRep->Get( KSettingsAirplaneMode, offLineMode );
    
    return offLineMode;
}
void CpProfileModelPrivate::setOffLineMode(bool isOffLine)
{
    mOffLineCenRep->Set(KSettingsAirplaneMode, isOffLine);
}

QString CpProfileModelPrivate::ringTone(int profileId) const
{
    MProfileExtended2 *profileExtend =  mProfileList.value(profileId);
    const MProfileTones &setTones = profileExtend->ProfileTones();

    QString ringTone = stringFromDescriptor(setTones.RingingTone1());
    return ringTone;
}

void CpProfileModelPrivate::setRingTone(int profileId, const QString& filePath)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    MProfileSetTones &setTones = profileExtend->ProfileSetTones();
    TRAP_IGNORE(
                    setTones.SetRingingTone1L(*descriptorFromString(filePath));
                    mEngine->CommitChangeL(*profileExtend);                    
                )            
}

QString CpProfileModelPrivate::messageTone(int profileId) const
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    
    const MProfileTones &setTones = profileExtend->ProfileTones();

    QString messageTone = stringFromDescriptor(setTones.MessageAlertTone());

    return messageTone;    
}

void CpProfileModelPrivate::setMessageTone(int profileId, const QString& filePath)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);                
    MProfileSetTones &setTones =
            profileExtend->ProfileSetTones();
    TRAP_IGNORE(
        setTones.SetMessageAlertToneL(*descriptorFromString(filePath));
        mEngine->CommitChangeL(*profileExtend);
            )                
}

QString CpProfileModelPrivate::emailTone(int profileId) const
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);           
    const MProfileExtraTones2 &extTones =
            profileExtend->ProfileExtraTones2();

    QString emailTone = stringFromDescriptor(extTones.EmailAlertTone());    
    return emailTone;    
}

void CpProfileModelPrivate::setEmailTone(int profileId, const QString& filePath)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    MProfileSetExtraTones2 &setExtTones =
            profileExtend->ProfileSetExtraTones2();
    TRAP_IGNORE (
        setExtTones.SetEmailAlertToneL(*descriptorFromString(filePath));
        mEngine->CommitChangeL(*profileExtend);
    )
}

QString CpProfileModelPrivate::reminderTone(int profileId) const
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    const MProfileExtraTones2 &extTones = profileExtend->ProfileExtraTones2();
    
    QString reminderTone = stringFromDescriptor(extTones.ReminderTone());
    return reminderTone;        
}

void CpProfileModelPrivate::setReminderTone(int profileId, const QString& filePath)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    MProfileSetExtraTones2 &setExtTones = profileExtend->ProfileSetExtraTones2();
                
    TRAP_IGNORE(
            setExtTones.SetReminderToneL( *descriptorFromString(filePath) );
            mEngine->CommitChangeL(*profileExtend);
    )            
}

bool CpProfileModelPrivate::notificationTone(int profileId) const
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    const MProfileTones &setTones = profileExtend->ProfileTones();
    const TProfileToneSettings &toneSettings = setTones.ToneSettings();

    bool notificationTone = toneSettings.iWarningAndGameTones;
    return notificationTone;    
}

void CpProfileModelPrivate::setNotificationTone(int profileId, bool isActive)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    MProfileSetTones &setTones = profileExtend->ProfileSetTones();
    TProfileToneSettings &toneSettings = setTones.SetToneSettings();

    toneSettings.iWarningAndGameTones = isActive;
    TRAP_IGNORE(
            mEngine->CommitChangeL(*profileExtend);
            ) 
}

int CpProfileModelPrivate::keyTouchScreenTone(int profileId) const
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    
    const MProfileTones &setTones = profileExtend->ProfileTones();
    const TProfileToneSettings &toneSettings = setTones.ToneSettings();

    int keyTouchScreenTone = toneSettings.iKeypadVolume;
    return keyTouchScreenTone;        
}

void CpProfileModelPrivate::setKeyTouchScreenTone(int profileId, int level)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);                    
    MProfileSetTones &setTones =
            profileExtend->ProfileSetTones();
    TProfileToneSettings &toneSettings =
            setTones.SetToneSettings();

    toneSettings.iKeypadVolume
            = static_cast<TProfileKeypadVolume> (level);
    TRAP_IGNORE (
            mEngine->CommitChangeL(*profileExtend);
    )    
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
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    
    const MProfileExtraSettings &extraSettings =
            profileExtend->ProfileExtraSettings();
    const MProfileFeedbackSettings &feedbackSettings =
            extraSettings.ProfileFeedbackSettings();
    
    int keyTouchScreenVibra = feedbackSettings.TactileFeedback();
    return keyTouchScreenVibra;        
}

void CpProfileModelPrivate::setKeyTouchScreenVibra(int profileId, int level)
{
    MProfileExtended2 *profileExtend = mProfileList.value(profileId);
    MProfileSetExtraSettings &extraSettings =
            profileExtend->ProfileSetExtraSettings();
    MProfileSetFeedbackSettings &setFeedbackSettings =
            extraSettings.ProfileSetFeedbackSettings();
    setFeedbackSettings.SetTactileFeedback(
            static_cast<TProfileTactileFeedback> (level));

    TRAP_IGNORE(
            mEngine->CommitChangeL(*profileExtend);
    )                
}

// End of file


