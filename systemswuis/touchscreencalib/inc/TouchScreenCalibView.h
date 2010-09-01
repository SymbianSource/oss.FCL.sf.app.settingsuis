/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
*     This class is the container class of the CTouchScreenCalibView.
*     Is used to show tap target animation.
*
*/



#ifndef TOUCHSCREENCALIBVIEW_H
#define TOUCHSCREENCALIBVIEW_H

// SYSTEM INCLUDES
#include <coecntrl.h>
#include <data_caging_path_literals.hrh>

#include <touchfeedback.h>
// USER INCLUDES
#include "TouchScreenCalibAppUi.h"

// CONSTANTS

// FORWARD DECLARATIONS
class TCalibAnimImageRect
    {
public:
    TRect iAnimImageRect1;
    TRect iAnimImageRect2;
    TRect iAnimImageRect3;
    TRect iAnimImageRect4;
    };

class CTouchScreenCalibAppUi;
class CAknBitmapAnimation;

// CLASS DECLARATION

/**
*  This class takes care of showing welcome animatio to the user.
*/
class CTouchScreenCalibView :  public CCoeControl , public MCoeControlObserver 
    {
    public:  // Constructors and destructor
        
        /**
        *  C++ default constructor.
        */
        CTouchScreenCalibView( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi );

        /**
        * Two-phased constructor.
        */
        static CTouchScreenCalibView* NewL( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi );

        /**
        *  Destructor
        */
        ~CTouchScreenCalibView();      

        /**
        *  This handles the key events in this control.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

        /**
        * Update UI
        */
        void UpdateL( TAknLayoutText aTextLayout );
        
        /**
        * Prepare to draw background for ending phase
        */
        void DrawEndingBackground();
        
        /**
        * Prepare to draw background for notes
        */
        void EndTargetAnimation();
    
        /**
        *  This makes the animation module to stop showing animation.
        */
        void EndAnimation();

        /**
        * 
        */
        void SetTextL(TAknLayoutText aTextLayout);

        /**
        * 
        */
        void SetWinPriority(TInt aPriority);

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        CTouchScreenCalibView();

        /**
        *  Is called when size is changed.
        */
        void SizeChanged();

        /**
        *  Returns the count of the components in the container.
        */
        TInt CountComponentControls() const;

        /**
        *  Returns the component specified by aIndex parameter.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

        /**
        *  Handles the event of the control.
        */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

        /**
        *  Handles key events.
        *  @param  aKeyEvent        Event to be handled
        *  @param  aType            Type of the key event 
        *  @return TKeyResponse
        */
        virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

        /**
        * Handles pointer events
        */
        virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);

        /**
        * Checks tapped point's validity
        * @param TPoint             co-ordinates
        * @since S60 3.2
        */
        TBool Validate(TPoint aPos);

        /**
        * Draw white background
        * @since S60 3.2
        */
        void DrawBackground() const;

        /**
        * Draw texts
        * @since S60 3.2
        */
        void DrawText() const;

        /**
        * Draw inactive tap points
        * @since S60 3.2
        */
        void ShowImage( TRect aRect ) const;

    private: // Functions from base classes

        /**
        * From CCoeControl
        */
        void Draw( const TRect& aRect ) const;
    
    protected: // Data

        //Used for showing animation    
        CAknBitmapAnimation *iAnim; //owns

        CFbsBitmap *iBitmap;
        CFbsBitmap *iBitmapMask;

        CTouchScreenCalibAppUi* iTouchScreenCalibAppUi; //uses

        //used for telling when the animation is showing
        TBool iAnimationShowing;

        //used for telling if animation is cancelled by user.
        TBool iAnimationCancelled;

        const CFont* iFont; // not owned

        CArrayPtr<HBufC>* iText;

        
        TCalibAnimImageRect iAnimImageRect;

        TPoint iTextTopLeft;
        TSize iTextSize;
        TInt iTextBaselineOffset;
        TRgb iTextColor;
        CGraphicsContext::TTextAlign iTextAlignment;
        TBool iCalibrationCompleted;
        MTouchFeedback* iTouchFeedback;
    };

#endif      // TOUCHSCREENCALIBVIEW_H
            
// End of File
