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

#ifndef CPDISPLAYMODEL_P_H
#define CPDISPLAYMODEL_P_H

class CRepository;

class CpDisplayModelPrivate
    {
public:
    CpDisplayModelPrivate();
    ~CpDisplayModelPrivate();

public:
    int keyguard();
    bool isKeyguardSupported();
    void setKeyguard(int value);
    bool isBacklightSupported();
    int backlight();
    void setBacklight(int value);
    bool isBrightnessSupported();
    int brightness();
    void setBrightness(int value);

private:
    CRepository* mLightCenRep;
    CRepository* mSecurityCenRep;
    };

#endif
