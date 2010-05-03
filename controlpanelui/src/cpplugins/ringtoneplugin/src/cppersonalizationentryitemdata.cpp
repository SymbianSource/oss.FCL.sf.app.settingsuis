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
#include "cppersonalizationentryitemdata.h"
#include <xqaiwrequest.h>
#include <cpitemdatahelper.h>
#include <cpprofilemodel.h>
#include <cplogger.h>
#include <qdir.h>

CpPersonalizationEntryItemData::CpPersonalizationEntryItemData(CpItemDataHelper &itemDataHelper,
                const QString &text,
                const QString &description,
                const HbIcon &icon,
                const HbDataFormModelItem *parent)
                :CpSettingFormEntryItemData(itemDataHelper,text,description,
                                            icon,parent),mReq(0),mProfileModel(0)
{
    CPFW_LOG("CpPersonalizationEntryItemData::CpPersonalizationEntryItemData(), START");
    mProfileModel = new CpProfileModel();
    if(mProfileModel)
    {
        setDescription( mProfileModel->ringTone().section(QDir::separator (),-1) );
    }
    else
    {
        CPFW_LOG("CpPersonalizationEntryItemData::mProfileModel:NULL!");
    }
}
CpPersonalizationEntryItemData::~CpPersonalizationEntryItemData()
{
    if( mProfileModel )
    {
        delete mProfileModel;
        mProfileModel = NULL;
    }
}
void CpPersonalizationEntryItemData::onLaunchView()
{
    CPFW_LOG("CpPersonalizationEntryItemData::onLaunchView, START");

    //launch media fetcher
    if (!mReq)
    {
        mReq = mAppMgr.create("com.nokia.services.media.Music", "fetch(QString)", true);

        if (!mReq)
        {
          CPFW_LOG("CpPersonalizationEntryItemData::onLaunchView, Mediafetcher start failed");
          return;
        }
        else
        {
            connect(mReq, SIGNAL(requestOk(const QVariant&)), SLOT(handleOk(const QVariant&)));
            connect(mReq, SIGNAL(requestError(int,const QString&)), SLOT(handleError(int,const QString&)));
        }
    }

    // Set arguments for request (music fetcher application title)
    QList<QVariant> args;
    args << QVariant( text() );
    mReq->setArguments(args);

    // Make the request
    if (!mReq->send())
    {
        CPFW_LOG("CpPersonalizationEntryItemData::onLaunchView, Mediafetcher calling failed");
        //report error
        return;
    }
    CPFW_LOG("CpPersonalizationEntryItemData::onLaunchView, successful END");

}
void CpPersonalizationEntryItemData::handleOk(const QVariant &result)
{
    CPFW_LOG("CpPersonalizationEntryItemData::handleOk");
    if (!result.canConvert<QString>())
    {
        setDescription( "Corrupt result" );
    }
    else
    {
        QString strRet = result.value<QString>();
        if(strRet.length())
        {
            setDescription( strRet.section(QDir::separator (),-1) );
        }
        else
        {
            setDescription( "No tone" );
        }
        mProfileModel->setRingTone( strRet );
    }
}
void CpPersonalizationEntryItemData::handleError(int errorCode, const QString& errorMessage)
{
    CPFW_LOG("CpPersonalizationEntryItemData::handleError");
    //handling error return
    Q_UNUSED(errorCode);
    Q_UNUSED(errorMessage);
    setDescription("Error media fetcher");
}

CpBaseSettingView *CpPersonalizationEntryItemData::createSettingView() const
{
    return NULL;
}
