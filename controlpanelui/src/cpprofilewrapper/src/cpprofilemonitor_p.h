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
#ifndef CPPROFILEMONITORPRIVATE_H
#define CPPROFILEMONITORPRIVATE_H

#include <MProfileChangeObserver.h>
#include <qglobal.h>

class CProfileChangeNotifyHandler;
class CpProfileMonitor;

class CpProfileMonitorPrivate:public MProfileChangeObserver
{
    Q_DECLARE_PUBLIC(CpProfileMonitor)
public:
    CpProfileMonitorPrivate();
    ~CpProfileMonitorPrivate();
    void initialize(CpProfileMonitor *parent);
    
#ifdef Q_OS_SYMBIAN
private:
    void HandleActiveProfileEventL(TProfileEvent aProfileEvent, TInt aProfileId);
private:
    CProfileChangeNotifyHandler *mProfileNotifier;
#endif
private:
    CpProfileMonitor *q_ptr;
};

#endif
