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

#ifndef CPKEYSCREENMODEL_H
#define CPKEYSCREENMODEL_H

class CpKeyScreenModelPrivate;

class CpKeyScreenModel
    {
public:
    CpKeyScreenModel();
    ~CpKeyScreenModel();

public:
    bool isKeyguardSupported();
    int keyguard();
    void setKeyguard(int value);
    bool isRotateSupported();
    bool rotate();
    void setRotate(bool value);
    bool isBrightnessSupported();
    int brightness();
    void setBrightness(int value);
    bool isCallibrationSupported();

private:
    CpKeyScreenModelPrivate* ptr;
    };

#endif
