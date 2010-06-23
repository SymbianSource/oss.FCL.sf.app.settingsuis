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
 *     The source file for mde tone fetcher.
 *     
 */
#include "toneselectionengine_p.h"
#include <XQConversions>
#include <pathinfo.h>
#include <bautils.h>
#include "tonepreviewprivate.h"
#include "tonefetcherengine.h"
#include <centralrepository.h>
#include <ProfileEngineDomainCRKeys.h>
#include <cplogger.h>


CTimeOutTimer* CTimeOutTimer::NewL(ToneSelectionEnginePrivate& aObserver)
    {
    CTimeOutTimer* self = CTimeOutTimer::NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
    }

 

CTimeOutTimer* CTimeOutTimer::NewLC(ToneSelectionEnginePrivate& aObserver)
    {
    CTimeOutTimer* self = new (ELeave) CTimeOutTimer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    } 

 

CTimeOutTimer::CTimeOutTimer(ToneSelectionEnginePrivate& aObserver)
    : CTimer(EPriorityStandard),
      iObserver( aObserver )
    { 

    }

 

CTimeOutTimer::~CTimeOutTimer()
    {
    Cancel();
    }

 

void CTimeOutTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);
    }

 

void CTimeOutTimer::RunL()
    {
    iObserver.ChangeObject();
    }

ToneSelectionEnginePrivate::ToneSelectionEnginePrivate( ToneFetcherEngine *engine ) : mServiceEngine( engine )

    {
    iSession = CMdESession::NewL( *this );
    
    iTimer = CTimeOutTimer::NewLC( *this );
    iContinue = EFalse;
    iTimerStarted = EFalse;
    iFreshing = EFalse;
    CleanupStack::Pop();
    
    
    }

ToneSelectionEnginePrivate::~ToneSelectionEnginePrivate()
    {
    delete iQuery;
    delete iSession;
    }

void ToneSelectionEnginePrivate::HandleSessionOpened( CMdESession& /*aSession*/, TInt aError )
    {
    if ( aError != KErrNone )
        {
        delete iSession;
        iSession = 0;
        iSessionOpen = EFalse;
        emit mdeSessionError( aError );
        }
    else
        {
        iSessionOpen = ETrue;
        TRAP_IGNORE( AddObjectObserverL() );
        emit mdeSessionOpened();
        }
    }



void ToneSelectionEnginePrivate::HandleSessionError( CMdESession& /*aSession*/, TInt aError )
    {
    if ( aError == KErrNone )
        {
        return;
        }
        
    delete iSession;
    iSession = 0;
    iSessionOpen = EFalse;
    emit mdeSessionError( aError );            
    }


void ToneSelectionEnginePrivate::HandleQueryNewResults( CMdEQuery& /*aQuery*/, 
                                               TInt /*aFirstNewItemIndex*/,
                                               TInt /*aNewItemCount*/ )
    {
    }

void ToneSelectionEnginePrivate::HandleObjectNotification( CMdESession& /*aSession*/, 
                                        TObserverNotificationType aType,
                                        const RArray<TItemId>& aObjectIdArray )
    {    
    if ( aObjectIdArray.Count() > 0 && ( aType == ENotifyAdd || aType == ENotifyModify || aType == ENotifyRemove ) )
        {
        CPFW_LOG("ToneSelectionEnginePrivate::HandleObjectNotification count = " + QVariant(aObjectIdArray.Count()).toString() + " type = " + QVariant(aType).toString());
        const TInt KOneSecond = 1000*1000;
        if ( !iFreshing )
            {
            emit notifyRefreshStart();
            iFreshing = ETrue;
            }
        if ( !iTimerStarted ) 
            {            
            iTimer->After( 5 * KOneSecond );
            iTimerStarted = ETrue;
            }        
        iContinue = ETrue;        
        }
    }

void ToneSelectionEnginePrivate::AddObjectObserverL()
    {
    if ( iSessionOpen )
        {
        TUint32 notificationType = ENotifyAdd | ENotifyModify | ENotifyRemove;
        CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
        iSession->AddObjectObserverL( *this, 0, notificationType, &defNS );
        
        iSession->AddObjectPresentObserverL( *this );
        }
    }

void ToneSelectionEnginePrivate::HandleObjectPresentNotification( CMdESession& /*aSession*/, 
                         TBool /*aPresent*/, const RArray<TItemId>& aObjectIdArray )
    {
    if( aObjectIdArray.Count() > 0 )
        {
        const TInt KOneSecond = 1000*1000;
        if ( !iFreshing )
            {
            emit notifyRefreshStart();
            iFreshing = ETrue;
            }
        if ( !iTimerStarted ) 
            {            
            iTimer->After( 5 * KOneSecond );
            iTimerStarted = ETrue;
            }        
        iContinue = ETrue;       
        }    
    }

void ToneSelectionEnginePrivate::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError )
    {
    iNameList.clear();
    iUriList.clear();
    if ( aError == KErrCancel )
        {      
        emit queryError( aError );
        return;
        }
    else
        {
        CMdEObjectQuery* query = static_cast<CMdEObjectQuery*> (&aQuery);
        TInt count = query->Count();
        for (TInt i = 0; i < count; ++i)
            {
            CMdEObject* object =
                    (CMdEObject*) query->TakeOwnershipOfResult(i);
            CleanupStack::PushL(object);
            CMdEPropertyDef& propDef = 
                        ToneSelectionEnginePrivate::PropertyDefL( iSession, ToneSelectionEnginePrivate::EAttrSongName  );
                
            CMdEProperty* property = 0;
            TInt err = object->Property( propDef, property, 0 );
            if ( err != KErrNotFound && property )
                {
                QString songName( XQConversions::s60DescToQString( property->TextValueL() ) );
                QString uriValue( XQConversions::s60DescToQString( object->Uri() ) );
                iNameList.append( songName );
                iUriList.append( uriValue );
                }
            CleanupStack::PopAndDestroy(object);
            }
        emit queryComplete(iNameList, iUriList );        
        }
    }

void ToneSelectionEnginePrivate::QueryTones()
    {
    LeaveIfSessionClosedL();
        
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& musicObjectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
  
    delete iQuery;
    iQuery = 0;
    iQuery = iSession->NewObjectQueryL( defNS, musicObjectDef, this );    
    
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    ExcludeMusicPropertiesL( conditions );

    iQuery->FindL();
    }

void ToneSelectionEnginePrivate::LeaveIfSessionClosedL()
    {
    if ( !iSession || !iSessionOpen )
        {
        User::Leave( KErrDisconnected );
        }
    }

CMdEPropertyDef& ToneSelectionEnginePrivate::PropertyDefL( TInt aAttr )
    {
    return PropertyDefL( iSession, aAttr );
    }

CMdEPropertyDef& ToneSelectionEnginePrivate::PropertyDefL( CMdESession* aSession, TInt aAttr )
    {
    CMdENamespaceDef& defNS = aSession->GetDefaultNamespaceDefL();
    
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
   
    if ( aAttr == EAttrFileSize )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
        }
    else if ( aAttr == EAttrMediaType )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
        }
    else if ( aAttr == EAttrSongName || aAttr == EAttrFileName )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
        }
    else if ( aAttr == EAttrArtist )
        {
        return objectDef.GetPropertyDefL( MdeConstants::MediaObject::KArtistProperty );
        }
    else if ( aAttr == EAttrAlbum )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Audio::KAlbumProperty );
        }
    else if ( aAttr == EAttrGenre )
        {
        return objectDef.GetPropertyDefL( MdeConstants::MediaObject::KGenreProperty );
        }
    else if ( aAttr == EAttrComposer )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Audio::KComposerProperty );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    
    return objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
    }

void ToneSelectionEnginePrivate::ExcludeMusicPropertiesL( CMdELogicCondition& aCondition )
    {
    TInt sizeLimitKB = 0;
    CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
    CleanupStack::PushL( cenrep );
    User::LeaveIfError( cenrep->Get( KProEngRingingToneMaxSize, sizeLimitKB ) );
    CleanupStack::PopAndDestroy(); // cenrep

    SetAttr( ToneFetcherEngine::EAttrFileSize, sizeLimitKB );
    CMdEPropertyDef& sizeTypeDef = PropertyDefL( EAttrFileSize );
    CMdEPropertyDef& mimeTypeDef = PropertyDefL( EAttrMediaType );
    CMdEPropertyDef& artistTypeDef = PropertyDefL( EAttrArtist );
    CMdEPropertyDef& albumTypeDef = PropertyDefL( EAttrAlbum );
    CMdEPropertyDef& genreTypeDef = PropertyDefL( EAttrGenre );
    CMdEPropertyDef& composerTypeDef = PropertyDefL( EAttrComposer );
    
    CMdELogicCondition& condition = 
                        aCondition.AddLogicConditionL( ELogicConditionOperatorAnd );
    condition.AddPropertyConditionL( sizeTypeDef, TMdEIntRange(0, iMaxFileSize, EMdERangeTypeBetween) );
    condition.AddPropertyConditionL( mimeTypeDef, 
            ETextPropertyConditionCompareContains, KMimeMp3 );
    condition.AddPropertyConditionL( artistTypeDef );
    condition.AddPropertyConditionL( albumTypeDef );
    condition.AddPropertyConditionL( genreTypeDef );
    condition.AddPropertyConditionL( composerTypeDef );
    
    condition.SetNegate( ETrue );
    }

void ToneSelectionEnginePrivate::SetAttr( int attr, int value )
{
    switch ( attr )
        {
        case ToneFetcherEngine::EAttrFileSize:
            {
            iMaxFileSize = value;
            break;
            }            
        default:
            {
            break;
            }
        }
}

void ToneSelectionEnginePrivate::ChangeObject()
    {    
    if ( iTimerStarted )
        {
        emit notifyObjectChanged();
        iTimerStarted = EFalse;
        }
    
    if ( iContinue  )
        {
        iContinue = EFalse;
        iTimer->After( 5000*1000 );
        iTimerStarted = ETrue;
        }
    else 
        {
        if ( iFreshing )
            {
            emit notifyRefreshFinish();
            iFreshing = EFalse;
            }        
        }
    }
// End of File

