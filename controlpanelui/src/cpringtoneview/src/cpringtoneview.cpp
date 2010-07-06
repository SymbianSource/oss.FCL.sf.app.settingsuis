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
#include "cpringtoneview.h"
#include <HbListWidget>
#include <QGraphicsLinearLayout>
#include <HbLabel>
#include <QList>
#include <QPair>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStandardItem>
#include <xqaiwrequest.h>
#include <cplogger.h>
#include <hbstyleloader.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbdataform.h>
#include <cpsettingformentryitemdata.h>

CpRingToneView::CpRingToneView( QGraphicsItem *parent ):
                              CpBaseSettingView(0, parent),
                              mToneTypeList( new HbListWidget(this) ),
                              mReq(0)
{
    HbStyleLoader::registerFilePath(":/widgetml/cpdataformlistentryviewitem.css");
    HbStyleLoader::registerFilePath(":/widgetml/cpdataformlistentryviewitem_color.css");
    HbStyleLoader::registerFilePath(":/widgetml/cpdataformlistentryviewitem.widgetml");
    
    HbDataForm *form = qobject_cast<HbDataForm*> ( widget() );
    form->setHeading("txt_cp_subhead_select_tone_type");   
    
    HbDataFormModel *model = new HbDataFormModel();  
    QList< QPair<QString,QString> > tonesTypeList;
    tonesTypeList << qMakePair( QString("qtg_large_tone_off"), hbTrId("txt_cp_list_no_tone") )
                 << qMakePair( QString("qtg_large_tone"), hbTrId("txt_cp_list_tone") )
                 << qMakePair( QString("qtg_large_music"), hbTrId("txt_cp_list_music") )                 
                 << qMakePair( QString("qtg_large_ovistore"), hbTrId("txt_cp_list_get_more_tones") );
       
    for (int i = 0; i < tonesTypeList.count(); ++i) {
        HbDataFormModelItem *itemData = new HbDataFormModelItem();
        itemData->setType ( static_cast<HbDataFormModelItem::DataItemType> (CpSettingFormEntryItemData::ListEntryItem) );
        itemData->setLabel(tonesTypeList.at(i).second);
        itemData->setIcon(tonesTypeList.at(i).first);
        model->appendDataFormItem(itemData, model->invisibleRootItem());
    }
    connect(form, SIGNAL(activated(QModelIndex)), this, SLOT(itemActivated(QModelIndex)));
    form->setModel(model);
}
CpRingToneView::~CpRingToneView()
{
    if(mReq)    delete mReq;
}
void CpRingToneView::itemActivated( const QModelIndex &index )
{
    int nRow = index.row();

    switch(nRow) {
        case 0:         //no tone, set default no sound
                emit selOK(QString(""));
                emit aboutToClose();
                break;
        case 1:         //tone
                launchMediaFetcher( "com.nokia.symbian.IToneFetch", "fetch()" );
                break;
        case 2:         //music
                launchMediaFetcher("com.nokia.symbian.IMusicFetch", "fetch()" );
                break;
        case 3:         //get more tones
		default:
		        break;
	 }
}
void CpRingToneView::handleOk(const QVariant &result)
{
    CPFW_LOG( "CpPersonalizationEntryItemData::handleOk" );
    if (!result.canConvert<QString>() || result.toString().length() == 0 )  //error result
    {
        return;
    }
    hide();
    emit selOK( result.value<QString>() );
    emit aboutToClose();
}


void CpRingToneView::handleError(int errorCode, const QString& errorMessage)
{
    emit(selError( errorCode, errorMessage ));
}

void CpRingToneView::launchMediaFetcher( const QString &strService, const QString &strItface )
{
    CPFW_LOG("CpRingToneView::launchMediaFetcher, START");
    if(mReq)
    {
        delete mReq;
        mReq = 0;
    }
        //launch media fetcher
    mReq = mAppMgr.create(strService, strItface, true);
    if (!mReq)
    {
      CPFW_LOG("CpRingToneView::launchMediaFetcher, Mediafetcher start failed");
      return;
    }
    else
    {
        connect(mReq, SIGNAL( requestOk( const QVariant&)), SLOT( handleOk(const QVariant&)) );
        connect(mReq, SIGNAL( requestError( int,const QString&)), SLOT(handleError(int,const QString&)) );
    }
    
    QList<QVariant> args;
    args << QVariant(QString("<app_name>"));
    mReq->setArguments(args);
    // Make the request
    if (!mReq->send())
    {
        CPFW_LOG("CpRingToneView::launchMediaFetcher, Mediafetcher calling failed");
    }
    CPFW_LOG("CpRingToneView::launchMediaFetcher, END");
}

