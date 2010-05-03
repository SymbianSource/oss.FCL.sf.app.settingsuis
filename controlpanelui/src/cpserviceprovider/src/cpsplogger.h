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

#ifndef CPSPLOGGER_H
#define CPSPLOGGER_H

#include <QLatin1String>
#include <logger.h>

#define CPSP_LOGGER_NAME     QLatin1String("CpServiceProvider")

#if defined (Q_OS_SYMBIAN)
    #define CPSP_LOGGER_CONFIG_PATH QLatin1String("C:/data/.config/cpserviceproviderlog.conf")
#elif defined (Q_WS_WIN)
    #ifdef _DEBUG
        #define CPSP_LOGGER_CONFIG_PATH QLatin1String("C:/controlpanel/debug/bin/cpserviceproviderlog.conf")
    #else 
        #define CPSP_LOGGER_CONFIG_PATH QLatin1String("C:/controlpanel/release/bin/cpserviceproviderlog.conf")
    #endif
#endif

#define CPSP_LOG(str) Logger::instance(CPSP_LOGGER_NAME)->log(str);

#endif
