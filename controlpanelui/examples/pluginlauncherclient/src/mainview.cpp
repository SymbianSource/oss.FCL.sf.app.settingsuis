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
#include "mainview.h"
#include <hbmenu.h>
#include <hbaction.h>
#include <qcoreapplication.h>
#include <cppluginlauncher.h>
#include <QStringList>
#include <cpbasesettingview.h>
#include <hbmessagebox.h>
#include <xqaiwrequest.h>
#include <XQServiceRequest.h>

MainView::MainView(QGraphicsItem *parent/* = 0*/)
: HbView(parent)
{
	init();
}

MainView::~MainView()
{

}

void MainView::init()
{
  setTitle(tr("CpPlugin Launcher"));  
  
  HbMenu *menu = new HbMenu();
  setMenu(menu);
  
  HbAction *action = menu->addAction(tr("Launch View(in process)"));
  connect(action, SIGNAL(triggered()), this, SLOT(launchInProcessProfileView()));

  action = menu->addAction(tr("Launch View(QtHighway)"));
  connect(action, SIGNAL(triggered()), this, SLOT(launchQtHighwayProfileView()));
}

void MainView::launchInProcessProfileView()
{
    CpBaseSettingView *settingView = CpPluginLauncher::launchSettingView("cppersonalizationplugin.dll","profile_view");
    if (settingView) {
        connect(settingView,SIGNAL(returnValueDelivered(QVariant)),this,SLOT(handleReturnValue(QVariant)));
    }
}

void MainView::launchQtHighwayProfileView()
{
    XQAiwRequest *request = mAppMgr.create("com.nokia.symbian.ICpPluginLauncher", "launchSettingView(QString,QVariant)", true);

    if (!request)
    {
        return;
    }
    else
    {
        connect(request, SIGNAL(requestOk(QVariant)), SLOT(handleReturnValue(QVariant)));
        connect(request, SIGNAL(requestError(int,QString)), SLOT(handleError(int,QString)));
    }


    // Set arguments for request 
    QList<QVariant> args;
    args << QVariant( "cppersonalizationplugin.dll" );
    args << QVariant ( "profile_view" );
    request->setArguments(args);

    // Make the request
    if (!request->send())
    {
        //report error     
    }
    
    delete request;
}

void MainView::handleReturnValue(const QVariant &returnValue)
{
    HbMessageBox::information( QString("Return value:") + returnValue.toString());
}

void MainView::handleError(int errorCode,const QString &errorMessage)
{
    HbMessageBox::information( QString("handle error:") + errorMessage);
}

//End of File
