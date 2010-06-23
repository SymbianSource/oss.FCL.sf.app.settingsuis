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
 *     The header file for tone fetcher widget.
 *     
 */
#ifndef TONEFETCHERWIDGET_H
#define TONEFETCHERWIDGET_H

#include <hbwidget.h>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include "tonefetcherengine.h"

class QDirModel;
class HbListView;
class QGraphicsLinearLayout;
class QStandardItemModel;
class HbAbstractViewItem;
class ToneFetcherView;
class ToneFetcherModel;
class HbLabel;
class HbProgressDialog;

class ToneFetcherWidget : public HbWidget
{
    Q_OBJECT
    
public:
    explicit ToneFetcherWidget(ToneFetcherView *serviceView);
    ~ToneFetcherWidget();
    QString getCurrentItemPath();
    void playOrPause();
    
signals:
    void itemClicked(const QString &item);
    void triggerToolBar(bool enable);
    
private slots:
    void on_list_activated(const QModelIndex &index);    
    void mdeSessionOpened();
    void mdeSessionError(int error);
    void queryComplete(const QStringList &nameList, const QStringList &uriList);
    void queryError(int error );
    void previewEvent(ToneFetcherEngine::TPreviewEvent event, int errorId);
    void onObjectChanged();
    void refreshFinish();
    void refreshStart();
    
private:
    void init();
    void addRomFiles();
    
private:
    HbLabel *mLabel;
    HbListView *mListView;
    QGraphicsLinearLayout *mLayout;
    ToneFetcherModel *mToneModel;   
    
    ToneFetcherView *mServiceView;
    ToneFetcherEngine *mServiceEngine;
    
    QFileInfoList mSimpleSoundList;
    QFileInfoList mDigitalSoundList;    
    bool mSelected;
    QModelIndex mOldSeletedItem;
    HbProgressDialog *mWaitNote;
};
#endif /* TONEFETCHERWIDGET_H_ */
