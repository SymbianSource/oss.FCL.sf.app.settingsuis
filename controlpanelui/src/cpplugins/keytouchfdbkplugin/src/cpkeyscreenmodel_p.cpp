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

#include "cpkeyscreenmodel_p.h"
#include "cpkeyscreenconstants.h"
#include <qglobal.h>
#include <hbcommoncrkeys.h>
#include <e32base.h>
#include <hal.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>     // KCRUidSecuritySettings
#include <hwrmlightdomaincrkeys.h>      // KCRUidLightSettings
#include <featmgr.h>

CpKeyScreenModelPrivate::CpKeyScreenModelPrivate()
{
    TRAP_IGNORE(
    mLightCenRep = CRepository::NewL( KCRUidLightSettings );
    mSecurityCenRep = CRepository::NewL( KCRUidSecuritySettings );
    mRotateSensor = CRepository::NewL(KHbSensorCenrepUid););
}

CpKeyScreenModelPrivate::~CpKeyScreenModelPrivate()
{
    delete mLightCenRep;
    delete mSecurityCenRep;
    delete mRotateSensor;
}

bool CpKeyScreenModelPrivate::isKeyguardSupported()
{
    return true;
}

int CpKeyScreenModelPrivate::keyguard()
{
    int period = KCpKeyscreenLockDefault30s;
    mSecurityCenRep->Get( KSettingsAutomaticKeyguardTime, period );
    return period;
}

void CpKeyScreenModelPrivate::setKeyguard(int value)
{
    if ( (KCpKeyscreenLock15s == value) || (KCpKeyscreenLock30s == value) 
            || (KCpKeyscreenLock45s == value) || (KCpKeyscreenLock60s == value) 
            || (KCpKeyscreenLock120s == value) ){
        mSecurityCenRep->Set( KSettingsAutomaticKeyguardTime, value );
        mLightCenRep->Set( KDisplayLightsTimeout, value );
    }
}

bool CpKeyScreenModelPrivate::isRotateSupported()
{
    return true;
}

bool CpKeyScreenModelPrivate::rotate()
{
    int rotate = 0;
    mRotateSensor->Get(KHbSensorCenrepKey, rotate);
    return rotate;
}

void CpKeyScreenModelPrivate::setRotate(bool value)
{
    mRotateSensor->Set(KHbSensorCenrepKey, value);
}

bool CpKeyScreenModelPrivate::isBrightnessSupported()
{
    if ( FeatureManager::FeatureSupported( KFeatureIdBrightnessControl ) ){
        return true;
    } else {
        return false;
    }
}

int CpKeyScreenModelPrivate::brightness()
{
    int time = 0;
    mLightCenRep->Get( KLightIntensity, time );
    return time/20;
}

void CpKeyScreenModelPrivate::setBrightness(int value)
{
    mLightCenRep->Set( KLightIntensity, value * 20 );
}

bool CpKeyScreenModelPrivate::isCallibrationSupported()
{
    return true;
}

// End of the file
