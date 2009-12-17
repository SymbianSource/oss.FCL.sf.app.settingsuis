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
* Description:  AppUi class of the application.
*
*/


#ifndef TOUCHSCREENCALIBAPPUI_H
#define TOUCHSCREENCALIBAPPUI_H

// SYSTEM INCLUDES
#include <aknappui.h>
#include <coecntrl.h>
#include <StringLoader.h>
#include <aknPopup.h>
#include <aknlists.h>
#include <badesca.h>
#include <AknDef.h>
#include <e32hal.h>

// CONSTANTS
const TInt KConvertGetCase = 0;
const TInt KConvertSaveCase = 1;

const TInt KUnknownOrientation = 0;
const TInt KPortraitOrientation = 1;
const TInt KLandscapeOrientation = 2;

// FORWARD DECLARATIONS
class TTouchScreenCalibLayout
    {
public:
    TAknLayoutRect      iAnimImageLayout1;
    TAknLayoutRect      iAnimImageLayout2;
    TAknLayoutRect      iAnimImageLayout3;
    TAknLayoutRect      iAnimImageLayout4;
    };

class CTouchScreenCalibView;
class CTouchScreenCalibPubSubObserver;
/**
*  'AppUi' class.
*
*/
class CTouchScreenCalibAppUi : public CAknAppUi, CCoeControl
{
    public: // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CTouchScreenCalibAppUi();

        /**
        * Destructor.
        */
        ~CTouchScreenCalibAppUi();

        // from CCoeAppUiBase
        void PrepareToExit();

        /**
        * Handles key events
        * @param TKeyEvent key event
        * @param TEventCode event code
        * @since S60 3.2
        */
        void HandleKeyL( const TKeyEvent& aKeyEvent, TEventCode aType );

        /**
        * Handles pointer events
        * @param TPoint co-ordinates
        * @since S60 3.2
        */
        void HandlePointerL( TPoint aPos );

        /**
        * Check current point number
        * @since S60 3.2
        * @return TInt, point number
        */
        TInt GetCalibrationStep();

        /**
        * Get rect of current tap animation or image
        * @since S60 3.2
        * @param TInt, point number
        * @return TRect, animation or image rect.
        */
        TRect GetAnimImageRect(TInt aPointNumber = 0);

        /**
        * From CAknAppUi, called when screen layout changes
        */
        void HandleScreenDeviceChangedL();

        /**
        * Check if first boot is going on
        * @since S60 3.2
        * @return. ETrue if first boot, otherwise EFalse
        */
        TBool FirstBoot();

    protected:
        /*
         * Handles changes to the application when it
         * switches to or from the foreground.
         */
        virtual void HandleForegroundEventL(TBool aForeground);

    private:
        /**
        * EPOC default constructor.
        */
        void ConstructL();

        /**
        * Update display
        * @since S60 3.2
        */
        void UpdateL();

        /**
        * Checks layout orientation
        * @since S60 3.2
        * @return TBool. ETrue if orientation same in driver native level and window level
        */
        TBool IsCorrectOrientation(TRect aRect);

        /**
        * Convert calibration or tapped points to different layout orientation
        * @since S60 3.2
        * @param Points
        * @param Convert case.
        * @return Calibration points. KConvertGetCase or KConvertSaveCase.
        */
        TDigitizerCalibration ConvertToCorrectOrientation(TDigitizerCalibration aPoints,
                                                          TInt aConvertCase);
        /**
        * Get calibration points
        * @since S60 3.2
        * @return Calibration points
        */
        TDigitizerCalibration CalibrationPoints();

        /**
        * Change to next calibration point
        * @since S60 3.2
        */
        void NextCalibrationStepL();

        /**
        * Saves calibration
        * @since S60 3.2
        */
        void SaveCalibration();

        /**
        * Restart calibration at first point
        * @since S60 3.2
        */
        void ResetCalibrationStepsL();

        /**
        * Sets tap points to 0 values
        * @since S60 3.2
        */
        void ResetTapPoints();

        TDigitizerCalibration ChangeOrientation(TDigitizerCalibration aSourcePoints,
                                                TPoint aDestReso);
        
        
        void SetOrientation();

    private: //Data
        CTouchScreenCalibView*              iTouchScreenCalibView; //owns
        CTouchScreenCalibPubSubObserver*    iTouchScreenCalibPubSubObserver; //owns
        TDigitizerCalibration               iCalibrationPoints;
        TDigitizerCalibration               iTapPoints;
        TInt                                iCurrentCalibrationStep;
        TBool                               iExitting;
        TTouchScreenCalibLayout             iTouchScreenCalibLayout;
        TBool                               iCalibrationDone;
        TInt								iNativeOrientation;
        TBool                               iCalibrationCancel;
};

#endif // TOUCHSCREENCALIBAPPUI_H

// End of file
