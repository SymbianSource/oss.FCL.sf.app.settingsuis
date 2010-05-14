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
* Description:  main.cpp
*
*/
#include <hbapplication.h>
#include <hbstyleloader.h>
#include <QDir>
#include <cpbasepath.h>
#include "cpservicemainwindow.h"
#include "cpsplogger.h"

int main(int argc, char **argv)
{
    HbApplication app(argc,argv ); 
    
    Logger::instance(CPSP_LOGGER_NAME)->configure(
            CPSP_LOGGER_CONFIG_PATH,QSettings::IniFormat);
    CPSP_LOG("Entering CpServiceProvider.exe...");
    
    HbStyleLoader::registerFilePath(CP_RESOURCE_PATH + QDir::separator() + WIDGETML_SUB_PATH);
    
    CpServiceMainWindow wnd;
    wnd.show();
    
    int ret = app.exec();
    
    CPSP_LOG("Exiting CpServiceProvider.exe.");

    Logger::closeAll();
    
    return ret;     
}

//End of File
