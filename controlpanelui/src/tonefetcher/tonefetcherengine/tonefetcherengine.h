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
 *     The header file for tone fetcher engine.
 *     
 */

#ifndef TONEFETCHERENGINE_H
#define TONEFETCHERENGINE_H
#include <QObject>
#include "toneselectionengine_p.h"
#include <QStringList>
class TonePreviewPrivate;
class ToneSelectionEnginePrivate;

// CLASS DECLARATION
/**
 *  This class is used for interacting with platform based codes
 *  including fetching tones from MDE (Metadata Engine) and previewing tones
 *  using platform-dependant interface.
 *
 */
class  ToneFetcherEngine : public QObject
{
    Q_OBJECT
    
public:
    enum TMediaFileListAttribute {
        // max media file size
        EAttrFileSize = 0,
        // ringing volume (TProfileRingingVolume from Profile.hrh)
        EAttrVolume,
        // ringing type,  (TProfileRingingType from Profile.hrh)
        EAttrRingingType,
        // vibra on/off   (Boolean) 
        EAttrVibra,
        // 3D effect      (TProfile3DToneEffect from ProfileInternal.hrh)
        EAttr3DEffect,
        // 3D echo        (TProfile3DToneEcho from ProfileInternal.hrh)
        EAttr3DEcho,
        // excluded mime type text
        EAttrExcludeMimeType,
        // for file protection checking
        EAttrAutomatedType,
        // media file dialog title
        EAttrTitle,
        // excluded folder (see enum TFolderType)
        EAttrExcludeFolder
    };
    
    enum State {
    SessionConnected = 0,
    SessionError    
    };
    
    enum TPreviewEvent {         
        EAudioPreviewComplete, 
        EPreviewError
    };
    explicit ToneFetcherEngine( QObject* parent = 0 );
    ~ToneFetcherEngine();
    /*
     * search the tone using MDE, not including rom files.
     */
    void getTone();
    
    /*
     * preview the tone
     * @param file the absolute path of the file.
     */
    void preview(const QString &file);
    
    bool IsPlaying();
    
    /*
     * stop previewing
     */
    void stop();
signals:
    void mdeSessionOpened();
    void mdeSessionError(int error);
    void queryComplete(const QStringList &nameList, const QStringList &uriList);
    void queryError(int error);
    void notifyPreviewEvent(ToneFetcherEngine::TPreviewEvent event, int errorId);
    void notifyObjectChanged();
private:
    ToneSelectionEnginePrivate *d;
    TonePreviewPrivate *mAudioPlayer;
};
#endif
