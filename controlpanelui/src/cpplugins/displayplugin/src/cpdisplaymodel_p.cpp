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

#include "cpdisplaymodel_p.h"
#include <qglobal.h>
#include <e32base.h>
#include <hal.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>     // KCRUidSecuritySettings
#include <hwrmlightdomaincrkeys.h>      // KCRUidLightSettings
#include <featmgr.h>

CpDisplayModelPrivate::CpDisplayModelPrivate()
{
    mLightCenRep = CRepository::NewL( KCRUidLightSettings );
    mSecurityCenRep = CRepository::NewL( KCRUidSecuritySettings );
}

CpDisplayModelPrivate::~CpDisplayModelPrivate()
{
    delete mLightCenRep;
    mLightCenRep = 0;

    delete mSecurityCenRep;
    mSecurityCenRep = 0;
}

bool CpDisplayModelPrivate::isKeyguardSupported()
{
    return true;
}

int CpDisplayModelPrivate::keyguard()
{
    int period = 0;
    
    if ( mSecurityCenRep->
         Get( KSettingsAutomaticKeyguardTime, period ) != KErrNone ){
        period = 60;
        mSecurityCenRep->Set( KSettingsAutomaticKeyguardTime, period );
    }

    return period/5;
}

void CpDisplayModelPrivate::setKeyguard(int value)
{
    value *= 5;
    mSecurityCenRep->Set( KSettingsAutomaticKeyguardTime, value );
}

bool CpDisplayModelPrivate::isBacklightSupported()
{
    return true;
}

int CpDisplayModelPrivate::backlight()
{
    int time = 0;
    mLightCenRep->Get( KDisplayLightsTimeout, time );
    return time/5;
}

void CpDisplayModelPrivate::setBacklight(int value)
{
    value *=5;
    mLightCenRep->Set( KDisplayLightsTimeout, value );
}

bool CpDisplayModelPrivate::isBrightnessSupported()
{
    if ( FeatureManager::FeatureSupported( KFeatureIdBrightnessControl ) ){
        return true;
    } else {
        return false;
    }
}

int CpDisplayModelPrivate::brightness()
{
    int time = 0;
    mLightCenRep->Get( KLightIntensity, time );
    return time;
}

void CpDisplayModelPrivate::setBrightness(int value)
{
    mLightCenRep->Set( KLightIntensity, value );
}

// End of the file
