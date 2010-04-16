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


#include "cpdisplaymodel.h"

#ifdef Q_OS_SYMBIAN
#include "cpdisplaymodel_p.h"
#endif

#include <qglobal.h>

CpDisplayModel::CpDisplayModel()
{
#ifdef Q_OS_SYMBIAN
    ptr = new CpDisplayModelPrivate();
#endif
}

CpDisplayModel::~CpDisplayModel()
{
#ifdef Q_OS_SYMBIAN
    delete ptr;
    ptr = 0;
#endif
}

bool CpDisplayModel::isKeyguardSupported()
{
#ifdef Q_OS_SYMBIAN
    return ptr->isKeyguardSupported();
#else
    return 0;
#endif
}

int CpDisplayModel::keyguard()
{
#ifdef Q_OS_SYMBIAN
    return ptr->keyguard();
#else
    return 0;
#endif
}

void CpDisplayModel::setKeyguard(int value)
{
#ifdef Q_OS_SYMBIAN
    ptr->setKeyguard(value);
#else
    Q_UNUSED(value);
#endif
}

bool CpDisplayModel::isBacklightSupported()
{
#ifdef Q_OS_SYMBIAN
    return ptr->isBacklightSupported();
#else
	return false;
#endif
}

int CpDisplayModel::backlight()
{
#ifdef Q_OS_SYMBIAN
    return ptr->backlight();
#else
    return 0;
#endif
}

void CpDisplayModel::setBacklight(int value)
{
#ifdef Q_OS_SYMBIAN
    ptr->setBacklight(value);
#else
    Q_UNUSED(value);
#endif
}

bool CpDisplayModel::isBrightnessSupported()
{
#ifdef Q_OS_SYMBIAN
    return ptr->isBrightnessSupported();
#else
	return false;
#endif
}

int CpDisplayModel::brightness()
{
#ifdef Q_OS_SYMBIAN
    return ptr->brightness();
#else
    return 0;
#endif    
}

void CpDisplayModel::setBrightness(int value)
{
#ifdef Q_OS_SYMBIAN
    ptr->setBrightness(value);
#else
    Q_UNUSED(value);
#endif
}

// End of the file
