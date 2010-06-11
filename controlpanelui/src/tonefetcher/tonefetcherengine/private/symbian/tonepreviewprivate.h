/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 * 
 * Contributors:
 * 
 * Description:
 *     The header file for tone previewing.
 *     
 */

#ifndef TONEPREVIEWPRIVATE_H
#define TONEPREVIEWPRIVATE_H
#include <e32base.h>
#include <DrmAudioSamplePlayer.h>
#include <mdaaudiotoneplayer.h>
#include <videoplayer.h>
#include <centralrepository.h>
#include <apgcli.h>  // for RApaLsSession
#include <e32property.h>
#include <QObject>
#include "tonefetcherengine.h"
#include <coemain.h>

class C3DRingingToneInterface;
class RWindow;

/**
* CMFPreviewHandlerBase
*
* Base class for CMFAudioPreviewHandler.
 */
class CMFPreviewHandlerBase : public QObject, public CBase
                        
{
Q_OBJECT
public: 

   enum TMediaFileType
       {
       EMediaFileTypeAudio = 0,
       EMediaFileTypeVideo
       };
    enum TPlayerStatus
        {
        EPlayerNotCreated,
        EPlayerInitializing,
        EPlayerReady,
        EPlayerPlaying,
        EPlayerInitialized,
        EPlayerPlayingWith3DEffect
        };

    // these must match with the ones in Profile Engine
    enum TRingingTypes
        {
        ERingingTypeRinging = 0,
        ERingingTypeAscending,
        ERingingTypeRingOnce,
        ERingingTypeBeepOnce,
        ERingingTypeSilent
        };

    enum TFLAllowScreenSaver
        {
        EFLScreenSaverAllowed = 0, EFLScreenSaverNotAllowed
        };

protected:

    virtual ~CMFPreviewHandlerBase();

protected:

    /**
     * C++ default constructor.
     */
    CMFPreviewHandlerBase( QObject *parent );

    /**
     * By default Symbian OS constructor is private.
     */
    void ConstructL();

public:    
    void SetAttr(const QString &file);    
    TInt Attr(TInt aAttr);    
    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual TBool IsPlaying() = 0;

protected:
    TInt ConvertVolume(TInt aVolume);
    void ReadActiveProfileL();
    TInt GetDataType(const TDesC& aFileName, TDataType& aDataType);
    void ReadDefaultToneL();   
    

    void DisableBackLight();

    static TInt DoResetInactivityTimer(TAny* aObject);
protected:
    TInt RingingVolume();
    TInt RingingType();
    TInt Vibra();
    TInt Echo3D();
    TInt Effect3D();

protected:
    static TInt ConvertVolume(TInt aVolume, TInt aMaxVolume);    

protected:
    TInt iRingingVolume;
    TInt iRingingType;
    TInt iVibra;
    TInt i3DEffect;
    TInt i3DEcho;
    TInt iMediaType;
    TInt iFileSize;
    HBufC* iFullName;

    TBool iActiveProfileRead;
    TInt iActiveProfileRingingVolume;
    TInt iActiveProfileRingingType;
    TInt iActiveProfileVibra;
    TInt iActiveProfile3DEffect;
    TInt iActiveProfile3DEcho;
   

    TInt iPlayerStatus;

    // handle to window
    RWindow* iWindow; // does not own

    // for getting file MIME types
    RApaLsSession iApaSession;

    // for setting screensaver on/off
    RProperty iProperty;

    // default ringing tone
    TFileName iDefaultTone;

    // file server session
    //RFs iFsSession;

    /**
     * Timer for resetting the user inactivity timeout
     */
    CPeriodic* iBacklightTimer;
};

// CLASS DECLARATION
/**
 *  This class is used for previewing the tones.
 *
 */

class TonePreviewPrivate : public CMFPreviewHandlerBase,
                           public MDrmAudioPlayerCallback,
                           public MMdaAudioToneObserver,
                           public MCoeForegroundObserver

{
Q_OBJECT
public:
    TonePreviewPrivate( QObject *parent );
    virtual ~TonePreviewPrivate();   
signals:
    void notifyPreviewEvent( ToneFetcherEngine::TPreviewEvent event, int errorId );
    
public:
    void Play();
    void Stop();
    TBool IsPlaying();

private:
    void Cancel();
    void SetAudioRingingType( TInt aRingingType );
    void SetToneRingingType( TInt aRingingType );
    TInt ConvertVolume( TInt aVolume );
        
        
private:
    // from MMdaAudioToneObserver
    virtual void MatoPrepareComplete( TInt aError );
    virtual void MatoPlayComplete(TInt aError);

private:
    // from MDrmAudioPlayerCallback
    void MdapcInitComplete(TInt aError,
            const TTimeIntervalMicroSeconds& aDuration);
    void MdapcPlayComplete(TInt aError);
    // from MCoeForegroundObserver
    void HandleLosingForeground();
    void HandleGainingForeground();

private:
    // audio player
    CDrmPlayerUtility* iAudioPlayer;

    /// Audioplayer status
    TPlayerStatus iAudioPlayerStatus;

    /// toneplayer
    CMdaAudioToneUtility* iTonePlayer;

    /// Toneplayer status
    TPlayerStatus iTonePlayerStatus;
    
    // plugin for playing 3D effects
    C3DRingingToneInterface* i3dRingingTonePlugin;

};
    
#endif /* TONEPREVIEWPRIVATE_H_ */
