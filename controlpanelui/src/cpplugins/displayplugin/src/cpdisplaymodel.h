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

#ifndef CPDISPLAYMODEL_H
#define CPDISPLAYMODEL_H

class CpDisplayModelPrivate;

class CpDisplayModel
    {
public:
    CpDisplayModel();
    ~CpDisplayModel();

public:
    bool isKeyguardSupported();
    int keyguard();
    void setKeyguard(int value);
    bool isBacklightSupported();
    int backlight();
    void setBacklight(int value);
    bool isBrightnessSupported();
    int brightness();
    void setBrightness(int value);

private:
    CpDisplayModelPrivate* ptr;
    };

#endif
