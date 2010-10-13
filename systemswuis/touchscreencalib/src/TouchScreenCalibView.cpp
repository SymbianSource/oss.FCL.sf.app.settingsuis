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


// SYSTEM INCLUDES
#include <aknappui.h>
#include <aknnotewrappers.h>
#include <barsread.h>
#include <AknBitmapAnimation.h>
#include <ConeResLoader.h>
#include <AknBidiTextUtils.h>
#include <TouchScreenCalib_anim.rsg>
#include <TouchScreenCalib.rsg>
#include <touchscreencalib.mbg>

// USER INCLUDES
#include "TouchScreenCalibView.h"
#include "TouchScreenCalibDefines.h"
#include "TouchScreenCalibAppUi.h"

// CONSTANTS
_LIT( KTargetAnimationResource, "z:TouchScreenCalib_anim.rsc" );

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::ConstructL()
    {
    TRACES("CTouchScreenCalibView::ConstructL()");

    CreateWindowL();

    iAnimImageRect.iAnimImageRect1 = iTouchScreenCalibAppUi->GetAnimImageRect(1);
    iAnimImageRect.iAnimImageRect2 = iTouchScreenCalibAppUi->GetAnimImageRect(2);
    iAnimImageRect.iAnimImageRect3 = iTouchScreenCalibAppUi->GetAnimImageRect(3);
    iAnimImageRect.iAnimImageRect4 = iTouchScreenCalibAppUi->GetAnimImageRect(4);

    // Parse filename of bitmaps
    _LIT( KDirAndFile, "z:TouchScreenCalib.mif" );
    TParse* fp1 = new(ELeave) TParse(); 
    fp1->Set(KDirAndFile, &KDC_APP_BITMAP_DIR, NULL);
    TFileName fileName( fp1->FullName() );
    TRACES1("CTouchScreenCalibImage::CreateIconL(): Image: %S", &(fp1->FullName()) );
    delete fp1;

    AknIconUtils::CreateIconL( iBitmap, 
                               iBitmapMask, 
                               fileName, 
                               EMbmTouchscreencalibQgn_graf_screencalib,
                               EMbmTouchscreencalibQgn_graf_screencalib_mask);

    iAvkonAppUi->StatusPane()->MakeVisible(EFalse);

    TRACES("CTouchScreenCalibView::ConstructL(): Animation loading started");
    iAnim = CAknBitmapAnimation::NewL();
    iAnim->SetContainerWindowL( *this );
    iAnim->SetScaleModeForAnimationFrames(EAspectRatioPreservedAndUnusedSpaceRemoved);
    TResourceReader rr;
    RConeResourceLoader loader( *iCoeEnv );

    TParse* fp = new(ELeave) TParse(); 
    fp->Set(KTargetAnimationResource, &KDC_APP_RESOURCE_DIR, NULL);
    TRACES1("CTouchScreenCalibView::ConstructL(): Animated target resource path: %S", &fp->FullName());
    TFileName name( fp->FullName() );
    delete fp;
    TInt fileError = loader.Open( name );
    if ( fileError == KErrNone )
        {
        CleanupClosePushL( loader );
        iCoeEnv->CreateResourceReaderLC(rr, R_SHUTDOWN_ANIM);
        TRAPD(err, iAnim->ConstructFromResourceL( rr ));
        if( err == KErrNone )
            {
            TResourceReader timeReader;
            iCoeEnv->CreateResourceReaderLC(timeReader, R_ANIM_DURATION);
            CleanupStack::PopAndDestroy(); // pop timeReader
            iAnim->SetPosition(TPoint(0,0));
            iAnim->SetSize(TSize(0,0));
            TRACES("CTouchScreenCalibView::ConstructL(): animation succesfully loaded");
            }
        else
            {
            TRACES("CTouchScreenCalibView::ConstructL(): animation loading failed");
            }
        CleanupStack::PopAndDestroy(); //pop rr
        TRACES("CTouchScreenCalibView::ConstructL(): animation loading ended");
        CleanupStack::PopAndDestroy(); //pop loader
        }
    else
        {
        TRACES("CTouchScreenCalibView::ConstructL(): resource file loading failed");
        }

    SetRect(iAvkonAppUi->ApplicationRect());
    ActivateL();

    iFont = AknLayoutUtils::FontFromId(EAknLogicalFontSecondaryFont);
    iText = new( ELeave ) CArrayPtrFlat<HBufC>( 20 );

    TApaTask self(iCoeEnv->WsSession());
    self.SetWgId(iCoeEnv->RootWin().Identifier());
    self.BringToForeground(); 

    if (iTouchScreenCalibAppUi->FirstBoot())
        {
        Window().SetOrdinalPosition( 0, ECoeWinPriorityAlwaysAtFront + 10000 );
        }
    else
        {
        Window().SetOrdinalPosition( 0, ECoeWinPriorityHigh + 1 );
        }
    iTouchFeedback = MTouchFeedback::Instance();

    TRACES("CTouchScreenCalibView::ConstructL(): ConstructL ended");
    }

// -----------------------------------------------------------------------------
// CTouchScreenCalibView::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTouchScreenCalibView* CTouchScreenCalibView::NewL( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi )
    {
    TRACES("CTouchScreenCalibView::NewL()");
    CTouchScreenCalibView* self = new (ELeave) CTouchScreenCalibView( aTouchScreenCalibAppUi );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CTouchScreenCalibView::CTouchScreenCalibView()
// ---------------------------------------------------------
CTouchScreenCalibView::CTouchScreenCalibView( CTouchScreenCalibAppUi* aTouchScreenCalibAppUi ) :
    iTouchScreenCalibAppUi( aTouchScreenCalibAppUi ),
    iAnimationShowing( EFalse ),
    iAnimationCancelled ( EFalse ),
    iText( NULL ),
    iCalibrationCompleted( EFalse )
    {
    TRACES("CTouchScreenCalibView::CTouchScreenCalibView()");
    TRACES("CTouchScreenCalibView::CTouchScreenCalibView(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::UpdateL( TAknLayoutText aTextLayout  )
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::UpdateL( TAknLayoutText aTextLayout )
    {
    TRACES("CTouchScreenCalibView::UpdateL()");

    TRect rect = iTouchScreenCalibAppUi->GetAnimImageRect();

    SetTextL(aTextLayout);
    EndAnimation();
    DrawDeferred();

    TPoint pos = TPoint(rect.iTl.iX, rect.iTl.iY);
    TSize size = TSize(rect.iBr.iX-rect.iTl.iX, rect.iBr.iY-rect.iTl.iY);
    iAnim->SetPosition(pos);
    iAnim->SetSize(size);

    TRAPD(err, iAnim->StartAnimationL());      
    if ( err != KErrNone )
        {
        TRACES("CTouchScreenCalibView::UpdateL(): Starting animation failed");
        }
    
    TRACES("CTouchScreenCalibView::UpdateL(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::DrawEndingBackground()
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::DrawEndingBackground()
    {
    TRACES("CTouchScreenCalibView::DrawEndingBackground()");
    iCalibrationCompleted = ETrue;
    SetRect( iAvkonAppUi->ApplicationRect() );
    EndAnimation();
    DrawNow();
    TRACES("CTouchScreenCalibView::DrawEndingBackground(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::EndTargetAnimation()
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::EndTargetAnimation()
    {
    TRACES("CTouchScreenCalibView::EndTargetAnimation()");
    EndAnimation();
    DrawNow();
    TRACES("CTouchScreenCalibView::EndTargetAnimation(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::~CTouchScreenCalibView()
// ---------------------------------------------------------------------------
CTouchScreenCalibView::~CTouchScreenCalibView()
    {
    TRACES("CTouchScreenCalibView::~CTouchScreenCalibView() begin");

    if( iAnim )
        {
        if( iAnimationShowing )
            {
            iAnim->CancelAnimation();
            TRACES("CTouchScreenCalibView::~CTouchScreenCalibView() animation cancelled");
            }
        }
    delete iAnim;
    TRACES("CTouchScreenCalibView::~CTouchScreenCalibView() iAnim deleted");

    if ( iText )
        {
        iText->ResetAndDestroy();
        delete iText;
        iText = NULL;
        }

    delete iBitmap;
    delete iBitmapMask;

    TRACES("CTouchScreenCalibView::~CTouchScreenCalibView() end");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::ComponentControl(TInt aIndex)
// ---------------------------------------------------------------------------
CCoeControl* CTouchScreenCalibView::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            {
            return iAnim;
            }
        default:
            {
            return NULL;
            }
        }
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::CountComponentControls()
// ---------------------------------------------------------------------------
TInt CTouchScreenCalibView::CountComponentControls() const
    {
    return iAnim ? 1 : 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::Draw(const TRect& aRect) const
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::Draw(const TRect& ) const
    {
    TRACES("CTouchScreenCalibView::Draw()");
    DrawBackground();
    if (!iCalibrationCompleted)
        {
        DrawText();
        ShowImage(iAnimImageRect.iAnimImageRect1);
        ShowImage(iAnimImageRect.iAnimImageRect2);
        ShowImage(iAnimImageRect.iAnimImageRect3);
        ShowImage(iAnimImageRect.iAnimImageRect4);
        }
    TRACES("CTouchScreenCalibView::Draw(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::SizeChanged()
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::SizeChanged()
    {
    if( iAnim )
        {
        iAnim->SetRect(Rect());
        }
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::EndAnimation()
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::EndAnimation() 
    {
    TRACES("CTouchScreenCalibView::EndAnimation()");
    iAnim->CancelAnimation();
    TRACES("CTouchScreenCalibView::EndAnimation(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::HandleControlEventL(...)
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::HandleControlEventL(
        CCoeControl* ,
        TCoeEvent )
    {
    //pure virtual from MCoeControlObserver
    TRACES("CTouchScreenCalibView::HandleControlEventL()");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::OfferKeyEventL(...)
// ---------------------------------------------------------------------------
TKeyResponse CTouchScreenCalibView::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    TRACES("CTouchScreenCalibView::OfferKeyEventL()");
    iTouchScreenCalibAppUi->HandleKeyL( aKeyEvent, aType );
    TRACES("CTouchScreenCalibView::OfferKeyEventL(): End: return EKeyWasConsumed");
    return EKeyWasConsumed;
    }

// ----------------------------------------------------
// CTouchScreenCalibView::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
// ----------------------------------------------------
TKeyResponse CTouchScreenCalibView::HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TRACES("CTouchScreenCalibView::HandleKeyEventL()");
    iTouchScreenCalibAppUi->HandleKeyL( aKeyEvent, aType );
    TRACES("CTouchScreenCalibView::HandleKeyEventL(): End: return EKeyWasConsumed");
    return EKeyWasConsumed;
    }

// ----------------------------------------------------
// CTouchScreenCalibView::HandlePointerEventL(const TPointerEvent& aPointerEvent)
// ----------------------------------------------------
void CTouchScreenCalibView::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    TRACES("CTouchScreenCalibView::HandlePointerEventL()");
    if (aPointerEvent.iType == TPointerEvent::EButton1Up)
        {
        TRACES("CTouchScreenCalibView::HandlePointerEventL(): EButton1Up");
        TRACES1("CTouchScreenCalibView::HandlePointerEventL(): X = %d",aPointerEvent.iPosition.iX);
        TRACES1("CTouchScreenCalibView::HandlePointerEventL(): Y = %d",aPointerEvent.iPosition.iY);

        TPoint pos;

        pos.iX = aPointerEvent.iPosition.iX;
        pos.iY = aPointerEvent.iPosition.iY;
        if (Validate(pos))
            {
            iTouchFeedback->InstantFeedback( ETouchFeedbackBasic );
            iTouchScreenCalibAppUi->HandlePointerL( pos );
            }
        }
    TRACES("CTouchScreenCalibView::HandlePointerEventL(): End --------------- Waiting for next event -------------");
    }

// ----------------------------------------------------
// CTouchScreenCalibView::Validate(TPoint aPos)
// ----------------------------------------------------
TBool CTouchScreenCalibView::Validate(TPoint aPos)
    {
    TRACES("CTouchScreenCalibView::Validate()");
    TInt retval( EFalse );

    if ((aPos.iX > iTouchScreenCalibAppUi->GetAnimImageRect().iTl.iX) && 
        (aPos.iX < iTouchScreenCalibAppUi->GetAnimImageRect().iBr.iX) &&
        (aPos.iY > iTouchScreenCalibAppUi->GetAnimImageRect().iTl.iY) &&
        (aPos.iY < iTouchScreenCalibAppUi->GetAnimImageRect().iBr.iY))
        {
        retval = ETrue;
        }
    TRACES1("CTouchScreenCalibView::Validate(): End: Return %d",retval);
    return retval;
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::DrawBackground() const
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::DrawBackground() const
    {
    TRACES("CTouchScreenCalibView::DrawBackground()");
    CWindowGc& gc = SystemGc();
    TRect rect = iAvkonAppUi->ApplicationRect();
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbWhite);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(rect);
    ControlEnv()->WsSession().Flush(); // force draw of the context
    TRACES("CTouchScreenCalibView::DrawBackground(): End");
    }

// ---------------------------------------------------------------------------
// CTouchScreenCalibView::DrawText()
// ---------------------------------------------------------------------------
void CTouchScreenCalibView::DrawText() const
    {
    TRACES("CTouchScreenCalibView::DrawText()");

    CWindowGc& gc = SystemGc();

    gc.SetPenStyle(CGraphicsContext::ENullPen);

    gc.UseFont( iFont );

    gc.SetBrushStyle(CGraphicsContext::ENullBrush);

    TPoint position( 0, 0 );
    TPoint topLeft;
    position = iTextTopLeft;

    for ( TInt index = 0 ;
          index < iText->Count();
          index++, position.iY += iTextBaselineOffset )
        {
        HBufC* text = (*iText)[ index ];
        if ( text )
            {
            topLeft = TPoint( position.iX, position.iY - iTextBaselineOffset );
            gc.SetPenColor(iTextColor);
            TRACES2("CTouchScreenCalibView::DrawText(): TopLeft: %d, %d", topLeft.iX, topLeft.iY);
            gc.DrawText( *text,
                         TRect( topLeft, iTextSize ),
                         iTextBaselineOffset,
                         iTextAlignment );
            }
        }
    ControlEnv()->WsSession().Flush(); // force draw of the context
    gc.DiscardFont();
    TRACES("CTouchScreenCalibView::DrawText(): End");
    }

// -----------------------------------------------------------------------------
// CTouchScreenCalibView::SetTextL(TAknLayoutText aTextLayout)
// -----------------------------------------------------------------------------
void CTouchScreenCalibView::SetTextL(TAknLayoutText aTextLayout)
    {
    TRACES("CTouchScreenCalibView::SetTextL()");

    TInt textLineWidth;

    TRACES2("CTouchScreenCalibView::SetTextL(): Text rect top: X:%d Y:%d",aTextLayout.TextRect().iTl.iX, aTextLayout.TextRect().iTl.iY);
    TRACES2("CTouchScreenCalibView::SetTextL(): Text rect:bot: X:%d Y:%d",aTextLayout.TextRect().iBr.iX, aTextLayout.TextRect().iBr.iY);

    textLineWidth = aTextLayout.TextRect().Width();

    iTextTopLeft = TPoint(aTextLayout.TextRect().iTl.iX, aTextLayout.TextRect().iTl.iY);

    iTextBaselineOffset = iFont->HeightInPixels() * 4 / 3;

    iTextSize = TSize(textLineWidth, iTextBaselineOffset + iFont->DescentInPixels());
            
    iTextColor = aTextLayout.Color();

    iTextAlignment = CGraphicsContext::ECenter;

    HBufC* mainText1 = StringLoader::LoadLC(R_QTN_TOUCH_SCREEN_CALI_INSTR_GEN, 
                                            iTouchScreenCalibAppUi->GetCalibrationStep());

    HBufC* mainText2;
    if (iTouchScreenCalibAppUi->FirstBoot())
        {
        mainText2 = CEikonEnv::Static()->AllocReadResourceLC(R_QTN_TOUCH_SCREEN_CALI_INSTR_RES );
        }
    else
        {
        mainText2 = CEikonEnv::Static()->AllocReadResourceLC(R_QTN_TOUCH_SCREEN_CALI_INSTR_CAN );
        }

    TBuf<256> text;
    text.Copy( mainText1->Des() );
    _LIT(KLineChange, "\n");
    text.Append(KLineChange);
    text.Append(mainText2->Des());

    TRACES1("CTouchScreenCalibView::SetTextL(): text: %S",&text);

    CArrayFix<TPtrC>* wrappedArray =
        new( ELeave ) CArrayFixFlat<TPtrC>( 10 );
    CleanupStack::PushL( wrappedArray );

    HBufC* dataToDestroy =
        AknBidiTextUtils::ConvertToVisualAndWrapToArrayL(
            text, textLineWidth, *iFont, *wrappedArray
        );

    TInt numLines( wrappedArray->Count() );
    TRACES1("CTouchScreenCalibView::SetTextL(): Number of lines: %d",numLines );

    if ( iText )
        {
        iText->ResetAndDestroy();
        delete iText;
        iText = NULL;
        }
    iText = new( ELeave ) CArrayPtrFlat<HBufC>( 20 );

    for ( TInt i = 0 ; i < numLines ; i++ )
        {
        HBufC* line = (*wrappedArray)[i].AllocLC();
        TInt lineLength = line->Length();
        TRACES1("CTouchScreenCalibView::SetTextL(): lineLength: %d",lineLength);        
        if(!lineLength)
            {
            iText->AppendL( NULL );
            CleanupStack::PopAndDestroy(line);  // line
            }
        else
            {
            iText->AppendL( line );
            CleanupStack::Pop(line);  // line
            }
        }
    iText->AppendL( NULL );

    // If the last char was newline, add one extra, since
    // wrapping automatically removes it.
    if ( text[ text.Length() - 1 ] == '\n' )
        {
        iText->AppendL( NULL );
        }

    CleanupStack::PopAndDestroy(wrappedArray); // wrappedArray
    delete dataToDestroy;

    CleanupStack::PopAndDestroy(2); //mainText1, mainText2
    TRACES("CTouchScreenCalibView::SetTextL(); End");
    }

// -----------------------------------------------------------------------------
// CTouchScreenCalibView::ShowImage(TRect aRect)
// -----------------------------------------------------------------------------
void CTouchScreenCalibView::ShowImage(TRect aRect) const
    {
    TRACES("CTouchScreenCalibView::ShowImage()");
    CWindowGc& gc = SystemGc();    
    AknIconUtils::SetSize( iBitmap, aRect.Size(), EAspectRatioPreservedAndUnusedSpaceRemoved );
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbRed);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);
    gc.BitBlt( TPoint(aRect.iTl.iX, aRect.iTl.iY), iBitmap ); // CWindowGc member function
    ControlEnv()->WsSession().Flush(); // force draw of the context
    TRACES("CTouchScreenCalibView::ShowImage(): End");
    }

// -----------------------------------------------------------------------------
// CTouchScreenCalibView::SetWinPriority()
// -----------------------------------------------------------------------------
void CTouchScreenCalibView::SetWinPriority(TInt aPriority)
    {
    TRACES("CTouchScreenCalibView::SetWinPriority()");
    Window().SetOrdinalPosition( 0, aPriority );
    TRACES("CTouchScreenCalibView::SetWinPriority(): End");
    }

//  End of File
