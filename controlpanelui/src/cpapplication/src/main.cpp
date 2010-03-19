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

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <QTranslator>
#include <cplogger.h>
#include "cpmainview.h"

int main(int argc, char **argv)
{
    HbApplication app(argc, argv);
    
    //used by QSettings
    QCoreApplication::setOrganizationName("nokia");
    QCoreApplication::setOrganizationDomain("Orbit");
    QCoreApplication::setApplicationName("ControlPanel");

    Logger::instance(CPFW_LOGGER_NAME)->configure(
        CP_LOGGER_CONFIG_PATH,QSettings::IniFormat);

    CPFW_LOG("Entering ControlPanel.exe...");
    
    QTranslator translator;
    if (translator.load("controlpanel_" + QLocale::system().name()))
    {
        qApp->installTranslator(&translator);
    }

    HbMainWindow mainWindow;
    CpMainView *mainView = new CpMainView();
#ifdef WIN32
	mainWindow.setOrientation(Qt::Vertical);
#endif
    QObject::connect(mainView,SIGNAL(aboutToClose()),&app, SLOT(quit()));  
    mainWindow.addView(mainView);
    mainWindow.show();
    
    int ret = app.exec();

    CPFW_LOG("Exiting ControlPanel.exe.");

    Logger::closeAll();

    return ret;
}

//
