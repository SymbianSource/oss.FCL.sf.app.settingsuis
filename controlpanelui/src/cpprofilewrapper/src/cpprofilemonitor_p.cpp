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

#include "cpprofilemonitor_p.h"
#include "cpprofilemonitor.h"

#include <CProfileChangeNotifyHandler.h>

/*
 * Construtor
 */
CpProfileMonitorPrivate::CpProfileMonitorPrivate():
                                mProfileNotifier(0)
{
    
}

/*
 * Desconstructor
 */
CpProfileMonitorPrivate::~CpProfileMonitorPrivate()
{
    delete mProfileNotifier;
}

/*
 * Initializing for the monitoring profile event
 */
void CpProfileMonitorPrivate::initialize(CpProfileMonitor *parent)
{
    q_ptr = parent;
    TRAP_IGNORE(mProfileNotifier = CProfileChangeNotifyHandler::NewL(this));   
}

/*
 * From MProfileChangeObserver, monitor the profile event
 */
void CpProfileMonitorPrivate::HandleActiveProfileEventL(TProfileEvent aProfileEvent, TInt aProfileId)
{
    switch (aProfileEvent) {
        case EProfileNewActiveProfile:
        {
            q_ptr->profileActivated(aProfileId);
            break;
        }
        case EProfileActiveProfileModified:
        {
            q_ptr->activeProfileModified(aProfileId);
            break;
        }
        default:
            break;
    }
}
