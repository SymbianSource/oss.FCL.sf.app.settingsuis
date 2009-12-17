/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Private Publish&Subscribe definitions of the
*                Security UIs subsystem
*
*/


#ifndef TOUCHSCPRIVATEPSKEYS_H
#define TOUCHSCPRIVATEPSKEYS_H

// INCLUDES

//CONSTANTS

// =============================================================================
// Touch Screen Calibration API
// =============================================================================

/**
* Touchscreen calibration P&S UID
*/
const TUid KPSUidTouchScreenCalibration = { 0x102828BC };

/**
* Wait a touchscreen calibration
*/
const TUint32 KPSTouchScreenCalibration = 0x00000000;
enum TPSTouchScreenCalibration
    {
    ETouchScreenCalibrationWait,
    ETouchScreenCalibrationOk    
    };
    
#endif//TOUCHSCPRIVATEPSKEYS_H
