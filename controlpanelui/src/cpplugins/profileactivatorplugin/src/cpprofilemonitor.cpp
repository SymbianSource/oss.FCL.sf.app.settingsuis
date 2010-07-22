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

#include "cpprofilemonitor.h"

#include <CProfileChangeNotifyHandler.h>

CpProfileMonitor::CpProfileMonitor(QObject *parent)
    :QObject(parent),mProfileNotifier(0)
{
    mProfileNotifier = CProfileChangeNotifyHandler::NewL(this);    
}

CpProfileMonitor::~CpProfileMonitor()
{    
    delete mProfileNotifier;        
}

void CpProfileMonitor::HandleActiveProfileEventL(TProfileEvent aProfileEvent, TInt aProfileId)
{
    if (EProfileNewActiveProfile == aProfileEvent) {
        emit profileActivated(aProfileId);
    }
}
