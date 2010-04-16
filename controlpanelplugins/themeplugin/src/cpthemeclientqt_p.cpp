/*
* Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
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


#include "cpthemeclientqt_p.h"
#include <QLocalSocket>
#include <QProcess>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

#include <hbinstance.h>
// TODO Do we need these?
//#include <hbtheme_p.h>
//#include <hbtheme.h>
//#include <hbevent.h>
//#include "hbthemecommon_p.h"

#define WAIT_TIME_TO_CONNECT_TO_SERVER 500
#define WAIT_TIME_TO_START_SERVER 5000
//static const QString SERVERFILEPATH = QLatin1String(HB_BIN_DIR) + QDir::separator() + QLatin1String("hbthemeserver");

/**
 * Constructor
 */
CpThemeClientPrivate::CpThemeClientPrivate():clientConnected( false ),localSocket( new QLocalSocket() )
{
#ifdef THEME_SERVER_TRACES
    qDebug() << Q_FUNC_INFO ;
#endif
}

/**
 * connectToServer
 */
bool CpThemeClientPrivate::connectToServer()
{
    localSocket->connectToServer(THEME_SERVER_NAME);
    // This logic needs to be improved
    bool success = localSocket->waitForConnected( WAIT_TIME_TO_CONNECT_TO_SERVER );

#ifdef THEME_SERVER_TRACES
    qDebug() << Q_FUNC_INFO << "Socket Connect status: " << success;
#endif 

// Stub shouldn't try starting the theme server yet again.
#if 0
    if(!success) {
        QProcess *newProcess = new QProcess();
        newProcess->start(SERVERFILEPATH);
        success = newProcess->waitForStarted( WAIT_TIME_TO_START_SERVER );
#ifdef THEME_SERVER_TRACES
        qDebug() << Q_FUNC_INFO << "Server Start Status: " << success << "Error = " << newProcess->error ();
#endif

        // If server started
        if (success) {
            // ToDo: This is to wait for server to start running. Logic needs to be improved.
            newProcess->waitForFinished(3000);
#ifdef THEME_SERVER_TRACES
            qDebug() <<Q_FUNC_INFO<< " Server Start Wait is over" ;
#endif
            localSocket->connectToServer(THEME_SERVER_NAME);
            success = localSocket->waitForConnected();
#ifdef THEME_SERVER_TRACES
            qDebug() <<Q_FUNC_INFO<< "socketconnected : "<<success;
#endif
        }
    }
#endif
    if (success) {
        connect(localSocket, SIGNAL(readyRead()), this, SLOT(themeChanged()));
    }
    return clientConnected = success;
}

/**
 * isConnected
 */
bool CpThemeClientPrivate::isConnected()
{
    return clientConnected;
}

/**
 * Destructor
 */
CpThemeClientPrivate::~CpThemeClientPrivate()
{
    localSocket->disconnectFromServer();
    delete localSocket;
}

/**
 * CpThemeClientPrivate::themeChanged()
 */
void CpThemeClientPrivate::themeChanged()
{
#ifdef THEME_SERVER_TRACES
    qDebug() << Q_FUNC_INFO;
#endif

    QByteArray inputByteArray = localSocket->readAll();
    QDataStream inputDataStream(inputByteArray);
    int request;
    inputDataStream >> request;

#ifdef THEME_SERVER_TRACES
    qDebug() << Q_FUNC_INFO << "recognizer: "<<request;
#endif
 
    if(EThemeSelection==request) {
        QString themeName;
        handleThemeChangeRequest(inputDataStream);
    }
#if 0
    // TODO determine if we need this for the control panel app
    if(EThemeContentUpdate == request) {
        hbInstance->theme()->d_ptr->clearCache();
    }
#endif
}

/**
 * CpThemeClientPrivate::handleThemeChangeRequest()
 */
void CpThemeClientPrivate::handleThemeChangeRequest(QDataStream &dataStream)
{
    QString themeName;
    dataStream >> themeName;
#ifdef THEME_SERVER_TRACES
    qDebug() << Q_FUNC_INFO <<"themeName is : " <<themeName;
#endif

    if(!(hbInstance->theme()->name() == themeName)) {
#ifdef THEME_SERVER_TRACES
        qDebug() << Q_FUNC_INFO <<"themeChanged(): called";
#endif
#if 0
        // TODO determine if we need this for the control panel app
        hbInstance->theme()->d_ptr->handleThemeChange();
#endif
    }
}

/**
 * changeTheme
 */
bool CpThemeClientPrivate::changeTheme(const QString &newTheme)
{
#ifdef THEME_CHANGER_TRACES
    qDebug() <<"ThemeClientQt::changeTheme("<<newTheme<<") called";
#endif
    if( (themeName==newTheme) || (newTheme.isEmpty()) ) {
#ifdef THEME_CHANGER_TRACES
        qDebug() <<"ThemeClientQt:: return Sametheme applied";
#endif
        return false;
    }
    QByteArray outputByteArray;
    QDataStream outputDataStream(&outputByteArray, QIODevice::WriteOnly);
    ThemeServerRequest requestType = EThemeSelection;
    outputDataStream << (int)requestType;
    outputDataStream << newTheme;
    themeName = newTheme;
    int expected = outputByteArray.size();
    int count = localSocket->write(outputByteArray);
#ifdef THEME_CHANGER_TRACES
    qDebug()<<"ThemeClientQt::ThemeName written to server";
#endif
    localSocket->flush();
    return count == expected;
}






