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

#include "cpservicemainwindow.h"
#include <QCoreApplication>
#include <hbview.h>
#include "cplauncherservice.h"
#include "cpsplogger.h"

CpServiceMainWindow::CpServiceMainWindow(QWidget *parent /* = 0*/)
: HbMainWindow(parent), mLauncherService(0)
{
    CPSP_LOG("CpServiceMainWindow Constructing...");
    mLauncherService = new CpLauncherService(this);
}

CpServiceMainWindow::~CpServiceMainWindow()
{
    CPSP_LOG("CpServiceMainWindow Destructing...");
}

void CpServiceMainWindow::quit()
{
    CPSP_LOG("CpServiceMainWindow::quit()");
 
    connect(mLauncherService, SIGNAL(returnValueDelivered()), qApp, SLOT(quit()));
    mLauncherService->complete();
    qApp->quit();
    
    /*
    hide();
    mLauncherService->complete();
    HbView *currView = currentView();
    if (currView) {
        removeView(currView);
        currView->deleteLater();
    }
    */
}

//End of File
