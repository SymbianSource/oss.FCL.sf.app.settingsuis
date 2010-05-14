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


#include "cpkeyscreenmodel.h"
#include <qglobal.h>

#ifdef Q_OS_SYMBIAN
#include "cpkeyscreenmodel_p.h"
#endif


CpKeyScreenModel::CpKeyScreenModel()
{
#ifdef Q_OS_SYMBIAN
    ptr = new CpKeyScreenModelPrivate();
#endif
}

CpKeyScreenModel::~CpKeyScreenModel()
{
#ifdef Q_OS_SYMBIAN
    delete ptr;
    ptr = 0;
#endif
}

bool CpKeyScreenModel::isKeyguardSupported()
{
#ifdef Q_OS_SYMBIAN
    return ptr->isKeyguardSupported();
#else
    return 0;
#endif
}

int CpKeyScreenModel::keyguard()
{
#ifdef Q_OS_SYMBIAN
    return ptr->keyguard();
#else
    return 0;
#endif
}

void CpKeyScreenModel::setKeyguard(int value)
{
#ifdef Q_OS_SYMBIAN
    ptr->setKeyguard(value);
#else
    Q_UNUSED(value);
#endif
}

bool CpKeyScreenModel::isRotateSupported()
{
#ifdef Q_OS_SYMBIAN
    return ptr->isRotateSupported();
#else
    return false;
#endif
}

bool CpKeyScreenModel::rotate()
{
#ifdef Q_OS_SYMBIAN
    return ptr->rotate();
#else
    return 0;
#endif
}

void CpKeyScreenModel::setRotate(bool value)
{
#ifdef Q_OS_SYMBIAN
    ptr->setRotate(value);
#else
    Q_UNUSED(value);
#endif
}

bool CpKeyScreenModel::isBrightnessSupported()
{
#ifdef Q_OS_SYMBIAN
    return ptr->isBrightnessSupported();
#else
    return false;
#endif
}

int CpKeyScreenModel::brightness()
{
#ifdef Q_OS_SYMBIAN
    return ptr->brightness();
#else
    return 0;
#endif    
}

void CpKeyScreenModel::setBrightness(int value)
{
#ifdef Q_OS_SYMBIAN
    ptr->setBrightness(value);
#else
    Q_UNUSED(value);
#endif
}

bool CpKeyScreenModel::isCallibrationSupported()
{
#ifdef Q_OS_SYMBIAN
    return ptr->isCallibrationSupported();
#else
    return true;
#endif
}

// End of the file
