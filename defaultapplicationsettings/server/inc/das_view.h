/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Standard View
*
*/


#ifndef C_DEFAULTAPPVIEW_H
#define C_DEFAULTAPPVIEW_H

#include <coecntrl.h> // CCoeControl


// FORWARD DECLARATIONS
class CDefaultAppAppUi;


class CDefaultAppView : public CCoeControl
    {
    public: // New methods

        /**
        * NewL.
        * Two-phased constructor.
        * Create a CDefaultAppView object, which will draw itself to aRect.
        * @param aRect The rectangle this view will be drawn to.
        * @return a pointer to the created instance of CDefaultAppView.
        */
        static CDefaultAppView* NewL( const TRect& aRect );

        /**
        * NewLC.
        * Two-phased constructor.
        * Create a CDefaultAppView object, which will draw itself
        * to aRect.
        * @param aRect Rectangle this view will be drawn to.
        * @return A pointer to the created instance of CDefaultAppView.
        */
        static CDefaultAppView* NewLC( const TRect& aRect );

        /**
        * CDefaultAppView
        * Virtual Destructor.
        */
        virtual ~CDefaultAppView();

    public:  // Functions from base classes

        /**
        * From CCoeControl, Draw
        * Draw this CDefaultAppView to the screen.
        * @param aRect the rectangle of this view that needs updating
        */
        void Draw( const TRect& aRect ) const;

        /**
        * From CoeControl, SizeChanged.
        * Called by framework when the view size is changed.
        */
        virtual void SizeChanged();

    private: // Constructors

        /**
        * ConstructL
        * @param aRect The rectangle this view will be drawn to.
        */
        void ConstructL(const TRect& aRect);

        /**
        * C++ default constructor.
        */
        CDefaultAppView();

    };

#endif // C_DEFAULTAPPVIEW_H

// end of file

    