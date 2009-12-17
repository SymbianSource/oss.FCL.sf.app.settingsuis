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
* Description:  Document class of the module.
*
*/



#ifndef TOUCHSCREENCALIBDOCUMENT_H
#define TOUCHSCREENCALIBDOCUMENT_H

// SYSTEM INCLUDES
#include <AknDoc.h>

// FORWARD DECLARATIONS
class   CEikAppUi;

// CLASS DECLARATION

/**
*  CTouchScreenCalibDocument application class.
*/
class CTouchScreenCalibDocument : public CAknDocument
{
    public:

        /**
        *   C++ default constructor.
        */
        CTouchScreenCalibDocument(CEikApplication& aApp): CAknDocument(aApp) { }

        /**
        *   Two-phased constructor.
        */
        static CTouchScreenCalibDocument* NewL(CEikApplication& aApp);

        /**
        *   Destructor.
        */
        virtual ~CTouchScreenCalibDocument();

    private:
        /**
        *   EPOC default constructor.
        */
        void ConstructL();

        /**
        * This method makes an application hidden so that it is not visible
        * for example in Applications list and FastSwap window.
        * @param CApaWindowGroupName* aWgName
        * @return void        
        */
        void UpdateTaskNameL( CApaWindowGroupName* aWgName );

    private:// from CEikDocument

        /**
        * Create CTouchScreenCalibAppUi object.
        */
        CEikAppUi* CreateAppUiL();
        
    private:// data

};

#endif

//End of file
