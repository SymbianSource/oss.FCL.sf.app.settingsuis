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
 *     The header file for mde tone fetcher.
 *     
 */
#ifndef TONESELECTIONENGINEPRIVATE_H
#define TONESELECTIONENGINEPRIVATE_H

#include "toneselectionengine_p.h"
#include <mdesession.h>
#include <mdequery.h>
#include <mdelogiccondition.h>
#include <mdeconstants.h>
#include <mdeobjectquery.h>
#include <mdccommon.h>
#include <mdeitem.h>
#include <mdeobject.h>
#include <e32base.h>
#include <QObject>
#include <QStringList>
// FORWARD DECLARATIONS
class ToneFetcherEngine;
// CONSTANTS
_LIT( KMimeMp3, "mp3" );

// CLASS DECLARATION
/**
 *  This class is used for quering tones from mde.
 *
 */
class ToneSelectionEnginePrivate : public QObject,
                            public CBase, 
                            public MMdESessionObserver,
                            public MMdEQueryObserver,
                            public MMdEObjectObserver,
                            public MMdEObjectPresentObserver                            
{
    Q_OBJECT
public:

    enum TStorageType
        {
        EPhoneMemory = 0, ERomStorage, EMassStorage, EMemoryCard
        };
    
    enum TQueryAttribute
        {
        EAttrMediaType = 20, // integer
        EAttrFileSize, // integer
        EAttrStorageType, // integer
        EAttrMediaFileId, // integer
        EAttrFileName, // string
        EAttrFullName, // string
        EAttrSongName, // string
        EAttrArtist, // string
        EAttrAlbum, // string
        EAttrGenre, // string
        EAttrComposer
        // string
        };

public:
    ToneSelectionEnginePrivate(  ToneFetcherEngine *engine );
    virtual ~ToneSelectionEnginePrivate();
    
signals:
    void mdeSessionOpened();
    void mdeSessionError( int error );
    void queryComplete( QStringList nameList, QStringList uriList );
    void queryError( int error );
    void notifyObjectChanged();


public:
    static CMdEPropertyDef& PropertyDefL(CMdESession* aSession, TInt aAttr);
    void QueryTones();   
    void SetAttr( int attr, int value );

private:   
    void ExcludeMusicPropertiesL(CMdELogicCondition& aCondition);    
    void LeaveIfSessionClosedL();
    CMdEPropertyDef& PropertyDefL(TInt aAttr);

private:
    // from MMdESessionObserver
    void HandleSessionOpened(CMdESession& aSession, TInt aError);
    void HandleSessionError(CMdESession& aSession, TInt aError);

private:
    // from MMdEQueryObserver (mdequery.h)
    void HandleQueryNewResults(CMdEQuery& aQuery, TInt aFirstNewItemIndex,
            TInt aNewItemCount);
    void HandleQueryCompleted(CMdEQuery& aQuery, TInt aError);    
private:
    // from MMdEObjectObserver
    void HandleObjectNotification(CMdESession& aSession,
            TObserverNotificationType aType,
            const RArray<TItemId>& aObjectIdArray);

private:
    // from MMdEObjectPresentObserver
    void HandleObjectPresentNotification(CMdESession& aSession,
            TBool aPresent, const RArray<TItemId>& aObjectIdArray);
    void AddObjectObserverL();
private:

    ToneFetcherEngine *mServiceEngine;

    // session to metadata engine
    CMdESession* iSession;

    // metadata query
    CMdEObjectQuery* iQuery;

    // used for saving the quering result.
    //both name and uri.
    QStringList iNameList;
    QStringList iUriList;

    // is metadata session open
    TBool iSessionOpen;

    // max audio file file size
    TInt iMaxFileSize;
    
    // query error
    TInt iQueryError;
};
#endif /* TONESELECTIONENGINE_H_ */

