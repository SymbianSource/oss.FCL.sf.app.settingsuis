/*
* Copyright (c) 2006 - 2008 Nokia Corporation and/or its subsidiary(-ies).
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


// SYSTEM INCLUDES
#include <aknnotewrappers.h>
#include <touchscreencalib.rsg>
#include <e32property.h>
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
#include "touchscprivatepskeys.h"
#else //RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <StartupAppInternalPSKeys.h>
#endif //RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <centralrepository.h>
#include <starterdomaincrkeys.h>
#include <starterclient.h>     //used for RemoveSplashScreen

// USER INCLUDES
#include "TouchScreenCalibApplication.h"
#include "TouchScreenCalibAppUi.h"
#include "TouchScreenCalibDefines.h"
#include "TouchScreenCalibDocument.h"
#include "TouchScreenCalibView.h"
#include "TouchScreenCalibPubSubObserver.h"

// CONSTANTS

_LIT_SECURITY_POLICY_C1(KReadDeviceDataPolicy, ECapabilityReadDeviceData);
_LIT_SECURITY_POLICY_C1(KWriteDeviceDataPolicy, ECapabilityWriteDeviceData);

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------
// CTouchScreenCalibAppUi::CTouchScreenCalibAppUi()
// ----------------------------------------------------
CTouchScreenCalibAppUi::CTouchScreenCalibAppUi():
    iTouchScreenCalibPubSubObserver( NULL ),
    iCurrentCalibrationStep( 0 ),
    iExitting( EFalse ),
    iCalibrationDone( EFalse ),
    iCalibrationCancel( EFalse )
    {
    TRACES("CTouchScreenCalibAppUi::CTouchScreenCalibAppUi");
    iNativeOrientation = KUnknownOrientation;
    }

// ----------------------------------------------------
// CTouchScreenCalibAppUi::ConstructL()
// ----------------------------------------------------
void CTouchScreenCalibAppUi::ConstructL()
    {
    TRACES("CTouchScreenCalibAppUi::ConstructL()");
    TInt flags = EStandardApp|EAknEnableSkin;

    BaseConstructL(flags);

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    RProperty::Define(KPSUidTouchScreenCalibration,
#else //RD_STARTUP_ANIMATION_CUSTOMIZATION    
    RProperty::Define(KPSUidStartup,
#endif //RD_STARTUP_ANIMATION_CUSTOMIZATION
                      KPSTouchScreenCalibration,
                      RProperty::EInt,
                      KReadDeviceDataPolicy,
                      KWriteDeviceDataPolicy );
    
    // Clearing the calibration before doing the actual calibration
    TDigitizerCalibrationType caltype = EFactory;
    UserHal::RestoreXYInputCalibration(caltype);
    ResetTapPoints();
    
//    SetOrientation();

    iCalibrationPoints = CalibrationPoints();

    // Set TSC application to be system application
    CEikonEnv& eikEnv = *CEikonEnv::Static();
    eikEnv.SetSystem( ETrue );

    iTouchScreenCalibLayout.iAnimImageLayout1.LayoutRect(Rect(),AknLayoutScalable_Apps::main_touch_calib_pane_g1().LayoutLine());
    iTouchScreenCalibLayout.iAnimImageLayout2.LayoutRect(Rect(),AknLayoutScalable_Apps::main_touch_calib_pane_g2().LayoutLine());
    iTouchScreenCalibLayout.iAnimImageLayout3.LayoutRect(Rect(),AknLayoutScalable_Apps::main_touch_calib_pane_g3().LayoutLine());
    iTouchScreenCalibLayout.iAnimImageLayout4.LayoutRect(Rect(),AknLayoutScalable_Apps::main_touch_calib_pane_g4().LayoutLine());

    iTouchScreenCalibView = CTouchScreenCalibView::NewL( this );
    AddToStackL( iTouchScreenCalibView );

    iTouchScreenCalibView->SetWinPriority(0);
    // Disable priority changes of window server
    eikEnv.WsSession().ComputeMode( RWsSession::EPriorityControlDisabled );

    UpdateL();
    
    iTouchScreenCalibPubSubObserver = CTouchScreenCalibPubSubObserver::NewL( this );

    if (FirstBoot())
        {
        TRACES("CTouchScreenCalibAppUi::ConstructL(): First boot ongoing");        
        TRACES("CTouchScreenCalibAppUi::ConstructL(): Connect to Starter");
        RStarterSession startersession;
        if( startersession.Connect() == KErrNone )
            {
            TRACES("CTouchScreenCalibAppUi::ConstructL(): Connected to Starter");
            startersession.EndSplashScreen();
            TRACES("CTouchScreenCalibAppUi::ConstructL(): Splash screen removed");
            startersession.Close();
            }
        }
    NextCalibrationStepL();

    TRACES("CTouchScreenCalibAppUi::ConstructL(): End");
    }

// ----------------------------------------------------
// CTouchScreenCalibAppUi::~CTouchScreenCalibAppUi()
// ----------------------------------------------------
CTouchScreenCalibAppUi::~CTouchScreenCalibAppUi()
    {
    TRACES("CTouchScreenCalibAppUi::~CTouchScreenCalibAppUi()");
    if (iTouchScreenCalibView)
        {
        TRACES("CTouchScreenCalibAppUi::~CTouchScreenCalibAppUi(): delete iTouchScreenCalibView");
        RemoveFromStack( iTouchScreenCalibView );
        delete iTouchScreenCalibView;
        }
    if (iTouchScreenCalibPubSubObserver)
        {
        TRACES("CTouchScreenCalibAppUi::~CTouchScreenCalibAppUi(): delete iTouchScreenCalibPubSubObserver");
        delete iTouchScreenCalibPubSubObserver;
        }
    TRACES("CTouchScreenCalibAppUi::~CTouchScreenCalibAppUi(): End");
    }

// ----------------------------------------------------
// CTouchScreenCalibAppUi::NextCalibrationStepL()
// ----------------------------------------------------
void CTouchScreenCalibAppUi::NextCalibrationStepL()
    {
    TRACES("CTouchScreenCalibAppUi::NextCalibrationStepL()");
    TRACES2("CTouchScreenCalibAppUi::NextCalibrationStepL(): TapPoint1: %d %d",iTapPoints.iTl.iX, iTapPoints.iTl.iY);
    TRACES2("CTouchScreenCalibAppUi::NextCalibrationStepL(): TapPoint2: %d %d",iTapPoints.iTr.iX, iTapPoints.iTr.iY);
    TRACES2("CTouchScreenCalibAppUi::NextCalibrationStepL(): TapPoint3: %d %d",iTapPoints.iBr.iX, iTapPoints.iBr.iY);
    TRACES2("CTouchScreenCalibAppUi::NextCalibrationStepL(): TapPoint4: %d %d",iTapPoints.iBl.iX, iTapPoints.iBl.iY);
    iCurrentCalibrationStep++;
    TRACES1("CTouchScreenCalibAppUi::NextCalibrationStepL(): CurrentCalibrationStep: %d",iCurrentCalibrationStep);

    if (iCurrentCalibrationStep > 4)
        {
        TRACES("CTouchScreenCalibAppUi::NextCalibrationStepL(): Calibration succesfully completed. Save it and exit application.");
        SaveCalibration();
        iCalibrationDone = ETrue;

        iTouchScreenCalibView->EndTargetAnimation();
        iTouchScreenCalibView->SetWinPriority(ECoeWinPriorityNormal);
        TRACES("CTouchScreenCalibAppUi::NextCalibrationStepL(): Show 'Calibration Done' note");
        HBufC* noteText = CEikonEnv::Static()->AllocReadResourceLC(R_QTN_TOUCH_SCREEN_CALI_DONE );
        CAknInformationNote* note =
            new( ELeave ) CAknInformationNote( ETrue );
        note->ExecuteLD( *noteText );
        CleanupStack::PopAndDestroy( noteText );
        if ( FirstBoot() )
            {
            iTouchScreenCalibView->DrawEndingBackground();
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
            RProperty::Set( KPSUidTouchScreenCalibration, KPSTouchScreenCalibration, ETouchScreenCalibrationOk );    
#else //RD_STARTUP_ANIMATION_CUSTOMIZATION    
            RProperty::Set( KPSUidStartup, KPSTouchScreenCalibration, ETouchScreenCalibrationOk );    
#endif //RD_STARTUP_ANIMATION_CUSTOMIZATION            
            iTouchScreenCalibView->SetWinPriority(0);
            }
        else
            {
            PrepareToExit();
            }
        }
    else
        {
        TRACES("CTouchScreenCalibAppUi::NextCalibrationStepL(): Update next step.");
        UpdateL();
        }
    TRACES("CTouchScreenCalibAppUi::NextCalibrationStepL(): End");
    }

// ----------------------------------------------------
// CTouchScreenCalibAppUi::SaveCalibration()
// ----------------------------------------------------
void CTouchScreenCalibAppUi::SaveCalibration()
    {
    TRACES("CTouchScreenCalibAppUi::SaveCalibration()");

    // Touch Screen driver supports only portrait orientation. If device is now in
    // landscape mode, convert points to portrait.
    TDigitizerCalibration tapPoints = ConvertToCorrectOrientation(iTapPoints,
                                                                  KConvertSaveCase);
    UserHal::SetXYInputCalibration(tapPoints);
    TRACES("CTouchScreenCalibAppUi::SaveCalibration(): End");
    }

// ----------------------------------------------------
// CTouchScreenCalibAppUi::ResetCalibrationStepsL()
// ----------------------------------------------------
void CTouchScreenCalibAppUi::ResetCalibrationStepsL()
    {
    iCurrentCalibrationStep = 0;
    ResetTapPoints();
    NextCalibrationStepL();
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::HandleForegroundEventL(TBool aForeground)
// ---------------------------------------------------------------------------
void CTouchScreenCalibAppUi::HandleForegroundEventL( TBool aForeground )
    {
    // call super-class first
    CAknAppUi::HandleForegroundEventL( aForeground );
    TRACES1("CTouchScreenCalibAppUi::HandleForegroundEventL( %d )", aForeground);
    if ( aForeground )
        {
        // if we are coming to foreground
        UpdateL();
        }
    else
        {
        if (!FirstBoot())
            {
            // Take old calibration in use                
            TDigitizerCalibrationType caltype = ESaved;
            UserHal::RestoreXYInputCalibration(caltype);
            PrepareToExit();
            }
        }
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::HandleKeyL(const TKeyEvent& aKeyEvent, TEventCode /*aType*/ )
// ---------------------------------------------------------------------------
void CTouchScreenCalibAppUi::HandleKeyL( const TKeyEvent& aKeyEvent, TEventCode /*aType*/ )
    {
    TRACES1("CTouchScreenCalibAppUi::HandleKeyL(): %d",aKeyEvent.iCode);

    if (aKeyEvent.iCode == EKeyYes) //Send key
        {
        TRACES("CTouchScreenCalibAppUi::HandleKeyL(): Send Key");
        if (FirstBoot()) // Restart calibration from step 1
            {
            TRACES("CTouchScreenCalibAppUi::HandleKey(): Go to first calibration step");
            ResetCalibrationStepsL();
            }
        else// Show "Calibration cancelled" Information note.
            {
            TRACES("CTouchScreenCalibAppUi::HandleKeyL(): Show cancel note");
            iCalibrationCancel = ETrue;
            
            iTouchScreenCalibView->EndAnimation();          
            iTouchScreenCalibView->SetWinPriority(ECoeWinPriorityNormal);
            HBufC* noteText = CEikonEnv::Static()->AllocReadResourceLC(R_QTN_TOUCH_SCREEN_CALI_CANCEL );
            CAknInformationNote* note =
                new( ELeave ) CAknInformationNote( ETrue );
            note->ExecuteLD( *noteText );
            CleanupStack::PopAndDestroy( noteText );
            
            // Take old calibration in use
            TDigitizerCalibrationType caltype = ESaved;
            UserHal::RestoreXYInputCalibration(caltype);
            PrepareToExit();
            }
        }
    else if ( aKeyEvent.iCode == EKeyNo || aKeyEvent.iCode == EKeyApplication0 
                     || ( aKeyEvent.iCode == EKeyNull && aKeyEvent.iScanCode == EStdKeyDevice7 ))
        {
        TRACES("CTouchScreenCalibAppUi::HandleKeyL(): End Key or App Key");
        if (!FirstBoot())
            {
            // Take old calibration in use                
            TDigitizerCalibrationType caltype = ESaved;
            UserHal::RestoreXYInputCalibration(caltype);
            PrepareToExit();
            }
        }
    TRACES("CTouchScreenCalibAppUi::HandleKeyL(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::HandlePointerL()
// ---------------------------------------------------------------------------
void CTouchScreenCalibAppUi::HandlePointerL( TPoint aPos )
    {
    TRACES("CTouchScreenCalibAppUi::HandlePointerL()");

    switch(iCurrentCalibrationStep)
        {
        case 1:
            TRACES("CTouchScreenCalibAppUi::HandlePointerL(): case 1");
            iTapPoints.iTl.iX = aPos.iX;
            iTapPoints.iTl.iY = aPos.iY;
            break;
        case 2:
            TRACES("CTouchScreenCalibAppUi::HandlePointerL(): case 2");
            iTapPoints.iTr.iX = aPos.iX;
            iTapPoints.iTr.iY = aPos.iY;
            break;
        case 3:
            TRACES("CTouchScreenCalibAppUi::HandlePointerL(): case 4");
            iTapPoints.iBr.iX = aPos.iX;
            iTapPoints.iBr.iY = aPos.iY;
            break;
        case 4:
            TRACES("CTouchScreenCalibAppUi::HandlePointerL(): case 3");
            iTapPoints.iBl.iX = aPos.iX;
            iTapPoints.iBl.iY = aPos.iY;
            break;
        default:
            TRACES("CTouchScreenCalibAppUi::HandlePointerL(): case default");
            break;
        }
    TRACES("CTouchScreenCalibAppUi::HandlePointerL(): End: Continue calibration");
    NextCalibrationStepL();
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::PrepareToExit()
// ---------------------------------------------------------------------------
void CTouchScreenCalibAppUi::PrepareToExit()
    {
    TRACES("CTouchScreenCalibAppUi::PrepareToExit()");
    if (!iExitting)
        {
        iExitting = ETrue;
        CEikAppUi::PrepareToExit();
        Exit();
        }
    TRACES("CTouchScreenCalibAppUi::PrepareToExit(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::ResetTapPoints()
// ---------------------------------------------------------------------------
void CTouchScreenCalibAppUi::ResetTapPoints()
    {
    TRACES("CTouchScreenCalibAppUi::ResetTapPoints()");
    iTapPoints.iTl.iX = 0;
    iTapPoints.iTl.iY = 0;
    iTapPoints.iTr.iX = 0;
    iTapPoints.iTr.iY = 0;
    iTapPoints.iBr.iX = 0;
    iTapPoints.iBr.iY = 0;
    iTapPoints.iBl.iX = 0;
    iTapPoints.iBl.iY = 0;
    TRACES("CTouchScreenCalibAppUi::ResetTapPoints(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::UpdateL()
// ---------------------------------------------------------------------------
void CTouchScreenCalibAppUi::UpdateL()
    {
    TRACES("CTouchScreenCalibAppUi::UpdateL()");
    TRACES1("CTouchScreenCalibAppUi::UpdateL(): iCurrentCalibrationStep: %d",iCurrentCalibrationStep);

    TAknLayoutText textLayout;
    textLayout.LayoutText(TRect(), AknLayoutScalable_Apps::main_touch_calib_pane_t1().LayoutLine());

    TRACES2("CTouchScreenCalibAppUi::UpdateL(): Text rect top:    X:%d Y:%d",textLayout.TextRect().iTl.iX, textLayout.TextRect().iTl.iY);
    TRACES2("CTouchScreenCalibAppUi::UpdateL(): Text rect:bottom: X:%d Y:%d",textLayout.TextRect().iBr.iX, textLayout.TextRect().iBr.iY);

    iTouchScreenCalibView->UpdateL(textLayout);

    TRACES("CTouchScreenCalibAppUi::UpdateL(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::GetCalibrationStep()
// ---------------------------------------------------------------------------
TInt CTouchScreenCalibAppUi::GetCalibrationStep()
    {
    return iCurrentCalibrationStep;
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::GetAnimImageRect()
// ---------------------------------------------------------------------------
TRect CTouchScreenCalibAppUi::GetAnimImageRect(TInt aPointNumber)
    {
    TRACES1("CTouchScreenCalibAppUi::GetAnimImageRect(%d)", aPointNumber);
    TRect rect;
    TInt width(0);
    TInt height(0);

    // If aPointNumber is 0, return current calibration point.
    if (aPointNumber == 0)
        {
        aPointNumber = GetCalibrationStep();
        }

    switch (aPointNumber)
        {
        case 1:
            width = iTouchScreenCalibLayout.iAnimImageLayout1.Rect().iBr.iX -
                         iTouchScreenCalibLayout.iAnimImageLayout1.Rect().iTl.iX;
            height= iTouchScreenCalibLayout.iAnimImageLayout1.Rect().iBr.iY -
                         iTouchScreenCalibLayout.iAnimImageLayout1.Rect().iTl.iY;

            rect = TRect(TPoint( iCalibrationPoints.iTl.iX - (width/2),
                                 iCalibrationPoints.iTl.iY - (height/2)),
                         TPoint( iCalibrationPoints.iTl.iX + (width/2),
                                 iCalibrationPoints.iTl.iY + (height/2)));
            break;
        case 2:
            width = iTouchScreenCalibLayout.iAnimImageLayout2.Rect().iBr.iX -
                         iTouchScreenCalibLayout.iAnimImageLayout2.Rect().iTl.iX;
            height= iTouchScreenCalibLayout.iAnimImageLayout2.Rect().iBr.iY -
                         iTouchScreenCalibLayout.iAnimImageLayout2.Rect().iTl.iY;

            rect = TRect(TPoint( iCalibrationPoints.iTr.iX - (width/2),
                                 iCalibrationPoints.iTr.iY - (height/2)),
                         TPoint( iCalibrationPoints.iTr.iX + (width/2),
                                 iCalibrationPoints.iTr.iY + (height/2)));
            break;
        case 3:
            width = iTouchScreenCalibLayout.iAnimImageLayout3.Rect().iBr.iX -
                         iTouchScreenCalibLayout.iAnimImageLayout3.Rect().iTl.iX;
            height= iTouchScreenCalibLayout.iAnimImageLayout3.Rect().iBr.iY -
                         iTouchScreenCalibLayout.iAnimImageLayout3.Rect().iTl.iY;

            rect = TRect(TPoint( iCalibrationPoints.iBr.iX - (width/2),
                                 iCalibrationPoints.iBr.iY - (height/2)),
                         TPoint( iCalibrationPoints.iBr.iX + (width/2),
                                 iCalibrationPoints.iBr.iY + (height/2)));
            break;
        case 4:
            width = iTouchScreenCalibLayout.iAnimImageLayout4.Rect().iBr.iX -
                         iTouchScreenCalibLayout.iAnimImageLayout4.Rect().iTl.iX;
            height= iTouchScreenCalibLayout.iAnimImageLayout4.Rect().iBr.iY -
                         iTouchScreenCalibLayout.iAnimImageLayout4.Rect().iTl.iY;

            rect = TRect(TPoint( iCalibrationPoints.iBl.iX - (width/2),
                                 iCalibrationPoints.iBl.iY - (height/2)),
                         TPoint( iCalibrationPoints.iBl.iX + (width/2),
                                 iCalibrationPoints.iBl.iY + (height/2)));
            break;
        default:
            break;
        }
    TRACES("CTouchScreenCalibAppUi::GetAnimImageRect(): End");
    return rect;
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::HandleResourceChange(...)
// ---------------------------------------------------------------------------
void CTouchScreenCalibAppUi::HandleScreenDeviceChangedL()
    {
    TRACES("CTouchScreenCalibAppUi::HandleResourceChange()");
    CAknAppUiBase::HandleScreenDeviceChangedL();

    if (!iCalibrationDone && !iCalibrationCancel )
        {
        TRACES("CTouchScreenCalibAppUi::HandleResourceChange(): Restart calibration");
        iCalibrationPoints = CalibrationPoints();
        if (iTouchScreenCalibView)
            {
            RemoveFromStack( iTouchScreenCalibView );
            delete iTouchScreenCalibView;
            iTouchScreenCalibView = NULL;
            }
        iTouchScreenCalibView = CTouchScreenCalibView::NewL( this );
        AddToStackL( iTouchScreenCalibView );
        ResetCalibrationStepsL();
        }
    else
        {
        TRACES("CTouchScreenCalibAppUi::HandleResourceChange(): Update ending background");
        iTouchScreenCalibView->DrawEndingBackground();
        }
    TRACES("CTouchScreenCalibAppUi::HandleResourceChange(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::CalibrationPoints();
// ---------------------------------------------------------------------------
TDigitizerCalibration CTouchScreenCalibAppUi::CalibrationPoints()
    {
    TRACES("CTouchScreenCalibAppUi::CalibrationPoints()");
    TDigitizerCalibration calibrationPoints;
#if defined(__WINS__)
    // Dummy values for emulator for testing purposes
    TRect rect = iAvkonAppUi->ApplicationRect();
    TRACES2("CTouchScreenCalibAppUi::CalibrationPoints(): Window resolution: (%d. %d)",rect.iBr.iX,rect.iBr.iY);

    calibrationPoints.iTl.iX = rect.iTl.iX+50;
    calibrationPoints.iTl.iY = rect.iTl.iY+50;
    calibrationPoints.iTr.iX = rect.iBr.iX-50;
    calibrationPoints.iTr.iY = rect.iTl.iY+50;
    calibrationPoints.iBr.iX = rect.iBr.iX-50;
    calibrationPoints.iBr.iY = rect.iBr.iY-50;
    calibrationPoints.iBl.iX = rect.iTl.iX+50;
    calibrationPoints.iBl.iY = rect.iBr.iY-50;
#else
    UserHal::CalibrationPoints(calibrationPoints);
#endif

    // Orientation in driver level could be different than window orientation. Convert
    // points to correct orientation.
    if (calibrationPoints.iBr.iX > calibrationPoints.iBr.iY )
        {
        TRACES("CTouchScreenCalibAppUi::CalibrationPoints(): Native orientation: landscape");
        iNativeOrientation = KLandscapeOrientation;
        }
    else
        {
        TRACES("CTouchScreenCalibAppUi::CalibrationPoints(): Native orientation: portrait");
        iNativeOrientation = KPortraitOrientation;
        }
    calibrationPoints = ConvertToCorrectOrientation(calibrationPoints, KConvertGetCase);

    TRACES("CTouchScreenCalibAppUi::CalibrationPoints(): End");
    return calibrationPoints;
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::ConvertToCorrectOrientation(TDigitizerCalibration aPoints);
// ---------------------------------------------------------------------------
TDigitizerCalibration CTouchScreenCalibAppUi::ConvertToCorrectOrientation(TDigitizerCalibration aPoints,
                                                                          TInt aConvertCase)
    {
    TRACES("CTouchScreenCalibAppUi::ConvertToCorrectOrientation()");
    TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): X:%d Y:%d", aPoints.iTl.iX,aPoints.iTl.iY);
    TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): X:%d Y:%d", aPoints.iTr.iX,aPoints.iTr.iY);
    TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): X:%d Y:%d", aPoints.iBr.iX,aPoints.iBr.iY);
    TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): X:%d Y:%d", aPoints.iBl.iX,aPoints.iBl.iY);
    TDigitizerCalibration points;
    TPoint destReso;

    TRect rect = iAvkonAppUi->ApplicationRect();

    if (!IsCorrectOrientation(rect))
        {
        TRACES("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): Points are not for current layout.");
        if (aConvertCase == KConvertGetCase)
            {
            destReso.iX = rect.iBr.iX;
            destReso.iY = rect.iBr.iY;
            TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): Destin reso: X:%d Y:%d", destReso.iX,destReso.iY);
            points = ChangeOrientation(aPoints, destReso);
            }
        else
            {
            destReso.iX = rect.iBr.iY;
            destReso.iY = rect.iBr.iX;
            TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): Destin reso: X:%d Y:%d", destReso.iX,destReso.iY);
            points = ChangeOrientation(aPoints, destReso);
            }
        }
    else
        {
        points = aPoints;
        }

    TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): X:%d Y:%d", points.iTl.iX,points.iTl.iY);
    TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): X:%d Y:%d", points.iTr.iX,points.iTr.iY);
    TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): X:%d Y:%d", points.iBr.iX,points.iBr.iY);
    TRACES2("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): X:%d Y:%d", points.iBl.iX,points.iBl.iY);

    TRACES("CTouchScreenCalibAppUi::ConvertToCorrectOrientation(): End");
    return points;
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::IsCorrectOrientation(TRect aRect)
// ---------------------------------------------------------------------------
TBool CTouchScreenCalibAppUi::IsCorrectOrientation(TRect aRect)
    {
    TRACES("CTouchScreenCalibAppUi::IsCorrectOrientation()");
    TBool ret(ETrue);
    TBool nativePortrait(ETrue);

    if (iNativeOrientation == KLandscapeOrientation)
        {
        TRACES("CTouchScreenCalibAppUi::IsCorrectOrientation(): Native orientation: landscape");
        nativePortrait = EFalse;
        }

    TBool windowsPortrait(ETrue);

    if (aRect.iBr.iX > aRect.iBr.iY)
        {
        TRACES("CTouchScreenCalibAppUi::IsCorrectOrientation(): Window orientation: landscape");
        windowsPortrait = EFalse;
        }
    else
        {
        windowsPortrait = ETrue;
        }

    if (nativePortrait != windowsPortrait)
        {
        ret = EFalse;
        }

    TRACES1("CTouchScreenCalibAppUi::IsCorrectOrientation(): End: returns %d",ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibAppUi::ChangeOrientation(TDigitizerCalibration aPoints);
// ---------------------------------------------------------------------------
TDigitizerCalibration CTouchScreenCalibAppUi::ChangeOrientation(TDigitizerCalibration aSourcePoints,
                                                                TPoint aDestReso)
    {
    TRACES("CTouchScreenCalibAppUi::ChangeOrientation()");
    TDigitizerCalibration destPoints;

    destPoints.iTl.iX = aSourcePoints.iTr.iY;
    destPoints.iTl.iY = aDestReso.iY - aSourcePoints.iTr.iX;
    destPoints.iTr.iX = aSourcePoints.iBr.iY;
    destPoints.iTr.iY = aDestReso.iY - aSourcePoints.iBr.iX;
    destPoints.iBr.iX = aSourcePoints.iBl.iY;
    destPoints.iBr.iY = aDestReso.iY - aSourcePoints.iBl.iX;
    destPoints.iBl.iX = aSourcePoints.iTl.iY;
    destPoints.iBl.iY = aDestReso.iY - aSourcePoints.iTl.iX;


    TRACES("CTouchScreenCalibAppUi::ChangeOrientation(): End");
    return destPoints;
    }

// ---------------------------------------------------------
// CTouchScreenCalibAppUi::FirstBoot()
// ---------------------------------------------------------
TBool CTouchScreenCalibAppUi::FirstBoot()
    {
    TRACES("CTouchScreenCalibAppUi::FirstBoot()");
    TInt value( 0 );

    TRACES("CTouchScreenCalibAppUi::FirstBoot(): Read first boot info from Central Repository ");
    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartup ) );
    if ( err == KErrNone )
        {
        err = repository->Get( KStartupFirstBoot, value );
        }

    delete repository;

    if (value)
        {
        TRACES("CTouchScreenCalibAppUi::FirstBoot(): End, return EFalse");
        return EFalse;
        }

    else
        {
        TRACES("CTouchScreenCalibAppUi::FirstBoot(): End, return ETrue");
        return ETrue;
        }
    }


/*
 * This funcntion is used to change the screen orientation to the
 * same orientation with the factory given calibration data. But it 
 * is not used now. So just leave it here!
 * */


void CTouchScreenCalibAppUi::SetOrientation()
    {
#ifndef __WINS__

    TInt orientation = KUnknownOrientation;
    TInt nativeOrientation = KUnknownOrientation;
    TDigitizerCalibration Points;
    UserHal::CalibrationPoints( Points );
    if ( Points.iBr.iX  > Points.iBr.iY)
        {
        orientation = KLandscapeOrientation;
        }
    else
        {
        orientation = KPortraitOrientation;
        }
    TRect rect = iAvkonAppUi->ApplicationRect();
    if ( rect.iBr.iX > rect.iBr.iY )
        {
        nativeOrientation = KLandscapeOrientation;
        }
    else
        {
        nativeOrientation = KPortraitOrientation;
        }
    
    if (orientation != nativeOrientation )
        {
        if ( orientation == KLandscapeOrientation )
            {
            SetOrientationL( CAknAppUiBase::EAppUiOrientationLandscape );
            }
        else
            {
            SetOrientationL( CAknAppUiBase::EAppUiOrientationPortrait );
            }
        }
#endif
    }
// End of file
