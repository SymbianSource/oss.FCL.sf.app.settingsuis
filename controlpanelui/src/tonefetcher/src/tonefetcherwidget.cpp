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
 *     The source file for tone fetcher widget.
 *     
 */
#include "tonefetcherwidget.h"
#include "hbabstractviewitem.h"
#include "hbstyle.h"
#include "hbabstractitemview.h"
#include <hblistview.h>
#include <hbmenu.h>
#include <hbaction.h>
#include <QModelIndex>
#include <QGraphicsLinearLayout>
#include <QDirModel>
#include <QTime>
#include <QFileInfo>
#include <QString>
#include <QStandardItemModel>
#include <XQUtils>
#include <hblabel.h>
#include "tonefetcherview.h"
#include "tonefetchermodel.h"
#include <hbmessagebox.h>
#include <hbprogressdialog.h>

ToneFetcherWidget::ToneFetcherWidget( ToneFetcherView *serviceView  ) 
    : HbWidget(this),
      mLabel(0),
      mListView(0),
      mLayout(0),
      mToneModel(0),
      mServiceView(serviceView),         
      mServiceEngine(0),
      mWaitNote(0)
      
{
    mSelected = false;
    init();
    connect(mServiceEngine, SIGNAL(mdeSessionOpened()), 
            this, SLOT(mdeSessionOpened()));
    connect(mServiceEngine, SIGNAL(mdeSessionError(int)),
            this, SLOT(mdeSessionError(int)));
    connect(mServiceEngine, SIGNAL(queryComplete(QStringList, QStringList)), 
            this, SLOT(queryComplete(QStringList, QStringList)));
    connect(mServiceEngine, SIGNAL(queryError(int)), 
            this, SLOT(queryError(int)));
    connect(mServiceEngine, 
           SIGNAL(notifyPreviewEvent(ToneServiceEngine::TPreviewEvent, int)), 
           this, SLOT(previewEvent(ToneServiceEngine::TPreviewEvent, int)));
    connect( mServiceEngine, SIGNAL(notifyObjectChanged()),
            this, SLOT(onObjectChanged()));
    connect( mServiceEngine, SIGNAL(notifyRefreshStart()),
                this, SLOT(refreshStart()));
    connect( mServiceEngine, SIGNAL(notifyRefreshFinish()),
                this, SLOT(refreshFinish()));
}

ToneFetcherWidget::~ToneFetcherWidget()
{
    delete mToneModel;
    mToneModel = 0;
    delete mWaitNote;
    mWaitNote = 0;
}

void ToneFetcherWidget::on_list_activated(const QModelIndex &index)
{  
    QModelIndexList modelIndexList = mListView->selectionModel()->selectedIndexes();
    
    //stop previewing when clicking another item.
    if (mServiceEngine->IsPlaying()) {
        mServiceEngine->stop();
    }
    /*
     * when one item is selected, reselecting it will deselect it. selecting another 
     * will also deselect it, while the other is selected.
     */
    if (mSelected){
        if(mOldSeletedItem != index) {
            mListView->selectionModel()->select(index, QItemSelectionModel::Select);
            mOldSeletedItem = index;
            emit triggerToolBar(true);
        } else {
            mListView->selectionModel()->select(index, QItemSelectionModel::Deselect);
            mSelected = false;
            emit triggerToolBar(false);
        }        
        return;
    }
    if (modelIndexList.count() > 0) {
        for (QModelIndexList::const_iterator it = modelIndexList.begin(); it != modelIndexList.end(); ++it) {
            if ((*it) == index) {
                mSelected = true;
                mOldSeletedItem = index;
                emit triggerToolBar(true);
            }            
        }
        
    }   
    
}

void ToneFetcherWidget::init()
{
    mLayout = new QGraphicsLinearLayout(this);
    mLayout->setOrientation(Qt::Vertical);
    setLayout(mLayout);

    mLabel = new HbLabel(this);
    mLabel->setPlainText(hbTrId("Select tone"));
    mLayout->addItem(mLabel);
    
    mListView = new HbListView(this);
    mListView->setObjectName("list");
    mLayout->addItem(mListView);
    mListView->setSelectionMode(HbAbstractItemView::SingleSelection);
     
    mServiceEngine = new ToneFetcherEngine(this);     
    mToneModel = new ToneFetcherModel(this);
    addRomFiles();
    
    connect(mListView, SIGNAL(activated(QModelIndex)),
        this, SLOT(on_list_activated(QModelIndex )));
    if( !mWaitNote ){
        mWaitNote = new HbProgressDialog( HbProgressDialog::WaitDialog );
        mWaitNote->setText( hbTrId( "Refreshing..." ) );
        QAction *action = mWaitNote->actions().at(0);//disable Cancel buttion.
        action->setEnabled(false);
    }
}

void ToneFetcherWidget::mdeSessionOpened()
{
    mServiceEngine->getTone();
}

void ToneFetcherWidget::queryComplete(const QStringList &nameList, const QStringList &uriList)
{
    QStandardItem *fileName = 0;
    QStandardItem *filePath = 0; 
    for (int i = 0; i < nameList.size(); ++i) { 
        QString tr1 = nameList.at(i);
        tr1 = uriList.at(i);
        fileName = new QStandardItem(nameList.at(i));
        filePath = new QStandardItem(uriList.at(i));
        mToneModel->insertInOrder(fileName, filePath);       
    }   
    mLabel->setPlainText(QString::number(mSimpleSoundList.size() + mDigitalSoundList.size() + nameList.size()) + " tones");
    mListView->setModel(mToneModel);
    mToneModel->refresh();
}

void ToneFetcherWidget::queryError(int error)
{
    Q_UNUSED(error);
        
}

void ToneFetcherWidget::mdeSessionError(int error)
{
    Q_UNUSED(error);
}

QString ToneFetcherWidget::getCurrentItemPath() 
{
    QModelIndexList modelIndexList = mListView->selectionModel()->selectedIndexes();
    if (modelIndexList.count() > 0) {
        QModelIndex index = modelIndexList.at(0);
        return mToneModel->data(index, Qt::UserRole).toString();
    }
    return QString();
}

void ToneFetcherWidget::playOrPause() 
{
    if(mServiceEngine->IsPlaying()) {
        mServiceEngine->stop();
    } else {    
    mServiceEngine->preview(getCurrentItemPath());
    }
    
}

void ToneFetcherWidget::previewEvent(ToneFetcherEngine::TPreviewEvent event, int errorId) 
{
    Q_UNUSED(errorId);
    if (event == ToneFetcherEngine::EAudioPreviewComplete) {
        //reserved
    } else {
        HbMessageBox::information(QString(hbTrId("Preview Error")));
    }
}

void ToneFetcherWidget::onObjectChanged()
{
    if (mServiceEngine->IsPlaying()) {
        mServiceEngine->stop();      
    }
    emit triggerToolBar(false);
    mToneModel->toBeFreshed();
    mToneModel->clearAll();
    mDigitalSoundList.clear();
    mSimpleSoundList.clear();
    addRomFiles();
    mServiceEngine->getTone();    
}

void ToneFetcherWidget::addRomFiles() 
{
    QStandardItem *fileName = 0;
    QStandardItem *filePath = 0;
    QDir digitalSoundPath(XQUtils::romRootPath() + XQUtils::digitalSoundsPath());
    mDigitalSoundList = digitalSoundPath.entryInfoList();  
           
    QDir simpleSoundPath(XQUtils::romRootPath() + XQUtils::simpleSoundsPath());
    mSimpleSoundList = simpleSoundPath.entryInfoList();

    for (int i = 0; i < mDigitalSoundList.size(); ++i) {
        QFileInfo fileInfo = mDigitalSoundList.at(i);        
        fileName = new QStandardItem(fileInfo.fileName());
        filePath = new QStandardItem(fileInfo.absoluteFilePath());
        mToneModel->insertInOrder(fileName, filePath);
    }
      
    for (int i = 0; i < mSimpleSoundList.size(); ++i) {
        QFileInfo fileInfo = mSimpleSoundList.at(i);       
        fileName = new QStandardItem(fileInfo.fileName());
        filePath = new QStandardItem(fileInfo.absoluteFilePath());
        mToneModel->insertInOrder(fileName, filePath);  
    }
}

void ToneFetcherWidget::refreshFinish()
{
    if (mWaitNote) {
        mWaitNote->close();
    }
}

void ToneFetcherWidget::refreshStart()
{
    if (mWaitNote) {
        mWaitNote->open();
    }
}
//End of File
