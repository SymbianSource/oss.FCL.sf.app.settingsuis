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

#ifndef CPPROFILEMONITOR_H_
#define CPPROFILEMONITOR_H_

#include <QObject>
#include <MProfileChangeObserver.h>

class CProfileChangeNotifyHandler;

class CpProfileMonitor: public QObject,public MProfileChangeObserver
{
    Q_OBJECT
public:
    explicit CpProfileMonitor(QObject *parent = 0);
    ~CpProfileMonitor();
signals:
    void profileActivated(int activeProfileId);
private:
    virtual void HandleActiveProfileEventL(
                TProfileEvent aProfileEvent, TInt aProfileId );

private:
    CProfileChangeNotifyHandler* mProfileNotifier;
};

#endif /* CPPROFILEMONITOR_H_ */
