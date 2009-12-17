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
* Description:  Application class of the module.
*
*/



#ifndef TOUCHSCREENCALIBAPPLICATION_H
#define TOUCHSCREENCALIBAPPLICATION_H

// SYSTEM INCLUDES
#include <aknapp.h>

// CONSTANTS
const TUid KUidTouchScreenCalib = { 0x102828BC };

// CLASS DECLARATION

/**
* CTouchScreenCalibApp application class.
*/
class CTouchScreenCalibApplication : public CAknApplication
    {
    private: // from CApaApplication
        /**
        * Create CTouchScreenCalibDocument document object.
        */
        CApaDocument* CreateDocumentL();

        /**
        * Return KUidTouchScreenCalib.
        */
        TUid AppDllUid() const;
    };

#endif // TOUCHSCREENCALIBAPPLICATION_H

// End of file

