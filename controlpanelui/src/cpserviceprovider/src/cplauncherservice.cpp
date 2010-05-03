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

#include "cplauncherservice.h"
#include <cpservicemainwindow.h>
#include <cplauncherinterface.h>
#include <cppluginloader.h>
#include <cpbasesettingview.h>
#include "cpsplogger.h"

CpLauncherService::CpLauncherService(HbMainWindow *mainWindow /* = 0*/)
: XQServiceProvider("com.nokia.services.cpserviceprovider.Launcher",mainWindow),
  mMainWindow(mainWindow),
  mAsyncRequestIndex(-1),
  mReturnValue(false)
{
    CPSP_LOG("CpLauncherService Constructing...");
    publishAll();
}

CpLauncherService::~CpLauncherService()
{
    CPSP_LOG("CpLauncherService Destructing...");
}

void CpLauncherService::complete()
{
    CPSP_LOG( QString("CpLauncherService::complete() mAsyncRequestIndex = %1, mReturnValue = %2").arg(
            mAsyncRequestIndex).arg(mReturnValue.toBool()) );
    completeRequest(mAsyncRequestIndex, mReturnValue);
}


bool CpLauncherService::launchSettingView(const QString &pluginFile,const QVariant &hint)
{
    CPSP_LOG("Entering CpLauncherService::launchSettingView");
   
    mAsyncRequestIndex = setCurrentRequestAsync();
    mReturnValue.setValue(false);
       
    if (mMainWindow) {
        mMainWindow->show();
        CpLauncherInterface *plugin = CpPluginLoader::loadCpLauncherInterface(pluginFile);
        if (plugin) {
            CpBaseSettingView *view = plugin->createSettingView(hint);
            if (view) {
                connect(view, SIGNAL(aboutToClose()), mMainWindow, SLOT(quit()));
                mMainWindow->addView(view);
                mMainWindow->setCurrentView(view);
                
                mReturnValue.setValue(true);
            }
            else {
                CPSP_LOG("Create setting view failed.");
            }
        }
        else {
            CPSP_LOG(QLatin1String("Load plugin interface(CpLauncherInterface) failed: ") + pluginFile);
        }
        
        if (!mReturnValue.toBool()) {
            qobject_cast<CpServiceMainWindow*>(mMainWindow)->quit();
        }
    }
       
    CPSP_LOG("Leaving CpLauncherService::launchSettingView");
    
    return mReturnValue.toBool();
}

//End of File
