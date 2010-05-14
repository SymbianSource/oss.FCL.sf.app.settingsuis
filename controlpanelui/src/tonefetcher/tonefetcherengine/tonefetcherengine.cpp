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
 *     The source file for tone fetcher engine.
 *     
 */
#include "tonefetcherengine.h"
#include "toneselectionengine_p.h"
#include "tonepreviewprivate.h"

ToneFetcherEngine::ToneFetcherEngine(QObject* parent) : QObject(parent)
{
    d = new ToneSelectionEnginePrivate(this);
    Q_ASSERT(d);
    mAudioPlayer = new TonePreviewPrivate( this );
    Q_ASSERT(mAudioPlayer);
    connect(d, SIGNAL(mdeSessionOpened()), 
            this, SIGNAL(mdeSessionOpened()));
    connect(d, SIGNAL(mdeSessionError(int)),
            this, SIGNAL(mdeSessionError(int)));
    connect(d, SIGNAL(queryComplete(QStringList, QStringList)),
            this, SIGNAL(queryComplete(QStringList, QStringList)));
    connect(d, SIGNAL(queryError(int)),
            this, SIGNAL(queryError(int)));
    connect(d, SIGNAL(notifyObjectChanged()),
            this, SIGNAL(notifyObjectChanged()));
    connect(mAudioPlayer, SIGNAL(notifyPreviewEvent(ToneServiceEngine::TPreviewEvent, int)),
            this, SIGNAL(notifyPreviewEvent(ToneServiceEngine::TPreviewEvent, int)));
}

ToneFetcherEngine::~ToneFetcherEngine()
{
    delete d;
}

void ToneFetcherEngine::getTone()
{
    d->QueryTones();
}

void ToneFetcherEngine::preview(const QString &file )
{
    mAudioPlayer->SetAttr(file);
    mAudioPlayer->Play();
}

bool ToneFetcherEngine::IsPlaying()
{
    mAudioPlayer->IsPlaying();
}
//End of File
