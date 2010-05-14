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
#include <cpmainwindow.h>
#include <cpmainview.h>
#include <hbstyleloader.h>
#include <QTranslator>
#include <QLocale>
#include <QLatin1String>
#include <QDir>
#include <cplogger.h>
#include <cpbasepath.h>

int main(int argc, char **argv)
{
    HbApplication app(argc, argv);
    
    //used by QSettings
    QCoreApplication::setOrganizationName("nokia");
    QCoreApplication::setOrganizationDomain("Orbit");
    QCoreApplication::setApplicationName("ControlPanel");

    Logger::instance(CPFW_LOGGER_NAME)->configure(
        CP_LOGGER_CONFIG_PATH,QSettings::IniFormat);
    Logger::instance(CPPERF_LOGGER_NAME)->configure(
        CP_LOGGER_CONFIG_PATH,QSettings::IniFormat);

    CPFW_LOG("Entering ControlPanel.exe...");
    CPPERF_LOG("Entering ControlPanel.exe...");
    
    QTranslator translator;
    if (translator.load("control_panel_" + QLocale::system().name(),"Z:/resource/qt/translations"))
    {
        qApp->installTranslator(&translator);
    }
    
    HbStyleLoader::registerFilePath(CP_RESOURCE_PATH + QDir::separator() + WIDGETML_SUB_PATH);

    CpMainWindow mainWindow;
    
    CpMainView *mainView = new CpMainView(&mainWindow);
    QObject::connect(mainView,SIGNAL(aboutToClose()),qApp,SLOT(quit()));
    mainWindow.addView(mainView);
    mainWindow.setCurrentView(mainView);
    
    mainWindow.show();
    
    int ret = app.exec();
    
    CPFW_LOG("Exiting ControlPanel.exe.");

    Logger::closeAll();

    return ret;
}

// End of File
