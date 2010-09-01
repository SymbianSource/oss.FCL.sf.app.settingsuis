/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Includes some common defines used in the TouchScreenCalib application
*
*/


#ifndef TOUCHSCREENCALIBDEFINES_H
#define TOUCHSCREENCALIBDEFINES_H

//CONSTANTS
_LIT( KTouchScreenCalibAppName, "TouchScreenCalib" ); 

// MACROS

#define PANIC(aPanic) User::Panic( KTouchScreenCalibAppName, aPanic )

#define TRACE_ADDPREFIX(aText) (TPtrC((const TText *)L"TouchScreenCalibApp: \"" L##aText L"\""))

#ifdef _DEBUG
#define TRACES(aMsg) RDebug::Print( TRACE_ADDPREFIX(aMsg) )
#define TRACES1(aFormat,aP1) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1) )
#define TRACES2(aFormat,aP1,aP2) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1),(aP2) )
#define TRACES3(aFormat,aP1,aP2,aP3) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1),(aP2),(aP3) )
#else
#define TRACES(aMsg)
#define TRACES1(aFormat,aP1)
#define TRACES2(aFormat,aP1,aP2)
#define TRACES3(aFormat,aP1,aP2,aP3)
#endif
#endif      // TOUCHSCREENCALIBDEFINES_H
            
// End of File
