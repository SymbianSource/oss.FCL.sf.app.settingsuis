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
#include <QTranslator>
#include <QLocale>
#include "cpservicemainwindow.h"
#include "cpsplogger.h"

int main(int argc, char **argv)
{
    HbApplication app(argc,argv ); 
    
    Logger::instance(CPSP_LOGGER_NAME)->configure(
            CPSP_LOGGER_CONFIG_PATH,QSettings::IniFormat);
    CPSP_LOG("Entering CpServiceProvider.exe...");
    
    QTranslator translator;
    if (translator.load("control_panel_" + QLocale::system().name(),"Z:/resource/qt/translations"))
    {
        qApp->installTranslator(&translator);
    }
    
    HbStyleLoader::registerFilePath(":/widgetml/cpdataformlistentryviewitem.css");
    HbStyleLoader::registerFilePath(":/widgetml/cpdataformlistentryviewitem_color.css");
    HbStyleLoader::registerFilePath(":/widgetml/cpdataformlistentryviewitem.widgetml");
    
    CpServiceMainWindow wnd;
    wnd.show();
    
    int ret = app.exec();
    
    CPSP_LOG("Exiting CpServiceProvider.exe.");

    Logger::closeAll();
    
    return ret;     
}

//End of File
