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
#include <QDir>
#include <hbmainwindow.h>
#include <hbstyleloader.h>
#include <cpbasepath.h>
#include "mainview.h"

int main(int argc, char **argv)
{
    HbApplication app(argc, argv);
    
    HbStyleLoader::registerFilePath(CP_RESOURCE_PATH + QDir::separator() + WIDGETML_SUB_PATH);
    
    HbMainWindow mainWindow;
    MainView *mainView = new MainView();
    mainWindow.addView(mainView);
    mainWindow.show();
    
    return app.exec();
}

//End of File

