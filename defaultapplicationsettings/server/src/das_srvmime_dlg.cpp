/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the Services & MIME view class
*
*/



// System includes
#include <e32base.h>
#include <AknIconArray.h> 
#include <gulicon.h>
#include <AknsUtils.h>
#include <avkon.mbg>

#include <featmgr.h>
#include <hlplch.h>

#include <aknlists.h>
#include <barsread.h>
#include <e32def.h>
#include <eikclbd.h>
#include <eikmenub.h>

#include <defaultappserver.rsg>
#include <StringLoader.h>
#include <uikon.hrh>
#include <aknPopup.h>
#include <akntitle.h>
#include <aknsfld.h>
#include <featmgr.h>

#include <aknsettingitemlist.h> 

#include "das_srvmime_dlg.h"
#include "das_servmimeapps.h"
#include "das_appui.h"
#include "das_app.h"
#include "das.hrh"
#include "das.hlp.hrh"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
//    
CDefaultAppSrvMimeDlg* CDefaultAppSrvMimeDlg::NewL(CDefaultAppAppUi* aAppUi)
    {
    //we first need to create a CDefaultAppServMimeApps instance
    TInt *selected;
    if(aAppUi->iReloadDlgAdvanced)
    {
    	//some settings for the advanced view
    	selected=&(aAppUi->iSelectedItemAdvanced);
    }
    else
    {
        //some settings for the simple view
    	selected=&(aAppUi->iSelectedItemSimple);
    };
    
    //create the dialog instance (ownership of servMimeApps is passed to the dialog
    CDefaultAppSrvMimeDlg *dialog=new(ELeave)CDefaultAppSrvMimeDlg(aAppUi, selected);
    CleanupStack::PushL(dialog);
    dialog->ConstructL(R_DAS_MENUBAR); 
    CleanupStack::Pop(dialog);
    return dialog;
    }

// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//    
CDefaultAppSrvMimeDlg::CDefaultAppSrvMimeDlg(CDefaultAppAppUi* aAppUi, TInt *aSelected) : 
    CAknSelectionListDialog(*aSelected, &aAppUi->iServMimeApps->iList, NULL), 
    iDefaultAppUi(aAppUi), iSelected(aSelected), iOkToExit(EFalse)
    {
    //no implementation necessary
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
CDefaultAppSrvMimeDlg::~CDefaultAppSrvMimeDlg()
    {
    if ( iDefaultAppUi )
        {
        iDefaultAppUi->SetDialogExist( EFalse );
        }
    
    if ( iPopupList )
        {
        iPopupList->CancelPopup();
        iPopupList = NULL;
        }
    }


// ---------------------------------------------------------------------------
// From class From CAknSelectionListDialog
// This function is called when buttons/softkeys are pressed, to check if the dlg should exit.
// We make the checkings and return the result.
// ---------------------------------------------------------------------------
//   
TBool CDefaultAppSrvMimeDlg::OkToExitL(TInt aButtonId)
    {
    CAknSelectionListDialog::OkToExitL(aButtonId);
    if ( aButtonId == EAknSoftkeyOptions && iPopupList )
    	{
    	iPopupList->CancelPopup();
    	iPopupList = NULL;
    	}
    if(aButtonId == EAknSoftkeyBack || aButtonId == EAknCmdOpen || aButtonId == EAknSoftkeyOk)
        {
        ProcessCommandL(aButtonId);
        };
    if(aButtonId==EAknCmdEnd || aButtonId==EAknSoftkeyCancel)
    	{
    	iOkToExit=ETrue;
    	iDefaultAppUi->iReloadDlg=EFalse;
    	}
    return iOkToExit;
    }

// ---------------------------------------------------------------------------
// From class From CAknSelectionListDialog
// This function is called when a command is issued by the user.
// ---------------------------------------------------------------------------
//
void CDefaultAppSrvMimeDlg::ProcessCommandL(TInt aCommand)
    {
    CAknSelectionListDialog::ProcessCommandL(aCommand); //this hides the menu and takes care of find
    iEnterKeyPressed=EFalse; 
    
    
    *iSelected = ListBox()->CurrentItemIndex();
    if (FindBox() && *iSelected != -1)
	    *iSelected = STATIC_CAST(CAknFilteredTextListBoxModel*,ListBox()->Model())->Filter()->FilteredItemIndex(*iSelected);
    // ATTENTION: If *iSelected is -1 it means that the list is empty!

    
    switch(aCommand)
        {
    case EDasCmdAdvanced:
        //SwitchViewL();
        
        //make the dialog reload        
        iDefaultAppUi->iReloadDlgAdvanced=ETrue;
        iOkToExit=ETrue;
        //force our exit
        TryExitL(0);
        
        break;
    case EAknCmdHelp:
        HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), iDefaultAppUi->AppHelpContextL());
        break;
    case EDasCmdResetAll:
    case EDasCmdResetSelected:
        if(aCommand==EDasCmdResetSelected)
        {
        	if(*iSelected!=-1)
        	    iDefaultAppUi->iServMimeApps->RestoreFactorySettingsL(*iSelected);
        }   
        else
            iDefaultAppUi->iServMimeApps->RestoreFactorySettingsL(-1);
            
        //common code:
        //reload the dialog
        if(!iDefaultAppUi->iServMimeApps->iSimplifiedView)
            iDefaultAppUi->iReloadDlgAdvanced=ETrue;
        iOkToExit=ETrue;
        //delete the iServMimeApps so that it will be reloaded next time
        delete iDefaultAppUi->iServMimeApps;
        iDefaultAppUi->iServMimeApps=NULL;
        //force our exit
        TryExitL(0);
        break;
    case EAknCmdExit:
        //exit the server
        iOkToExit=ETrue;
        iDefaultAppUi->iReloadDlg=EFalse;
        //force our exit
        TryExitL(0);
        break;
    case EAknSoftkeyBack:
        //we have 2 cases here: 
        //if we are in the advanced view, we switch back to the simple view.
        //if we are in the simple view, then we exit the server
        if(iDefaultAppUi->iServMimeApps->iAppUid == KUidGS && !iDefaultAppUi->iServMimeApps->iSimplifiedView)
            {
            //switch back to simple view
            //make the dialog reload
            iDefaultAppUi->iReloadDlgAdvanced=EFalse;
            iOkToExit=ETrue;
            // no need to force our exit, we are comming from a TryExitL() 
            }
        else
            {
            //exit the server
            iOkToExit=ETrue;
            iDefaultAppUi->iReloadDlg=EFalse;
            //tell AppUi that the exit is due a "Back" command
            iDefaultAppUi->iExitBack=ETrue;
            }
        break;
    case EAknCmdOpen:
    case EAknSoftkeyOk:
        //we have the index of the selected item in *iSelected
        OpenMenuForSelectedServiceAndMimeL(*iSelected);
        break;    
        }
    }

// ---------------------------------------------------------------------------
// From class From CAknSelectionListDialog
// Called during the construction of the dialog. We build the list of elements here.
// ---------------------------------------------------------------------------
//    
void CDefaultAppSrvMimeDlg::PreLayoutDynInitL()
    {
    CAknSelectionListDialog::PreLayoutDynInitL();
    
    CEikSettingsListBox* listbox=(CEikSettingsListBox*)ListBox();
    // Ownership retained by us
    listbox->CreateScrollBarFrameL( ETrue );
    listbox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto );
    listbox->UpdateScrollBarsL();   

    //enable marquee
    EnableMarqueeL();   
        
    //restore the selected item    
    if(*iSelected >= 0)
        {
        listbox->SetCurrentItemIndex(*iSelected);
        };
        
    //change the title
    CEikStatusPane *statusPane=iDefaultAppUi->StatusPane();
    if(statusPane && statusPane->PaneCapabilities(TUid::Uid(EEikStatusPaneUidTitle)).IsPresent())
        {
        CAknTitlePane *titlePane=(CAknTitlePane*)(statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)));
        HBufC* titleString;
        //load the correct title string
        if(iDefaultAppUi->iReloadDlgAdvanced)
            {
                if(iDefaultAppUi->iClientUid == KUidGS)
                    //User activated advanced view in Control Panel
                    titleString=StringLoader::LoadLC(R_DA_TITLE_ADVANCED);
                else //Client is some application other than Control Panel; load app specific view.
                    titleString=StringLoader::LoadLC(R_DA_TITLE_APP);
            }
        else //Simple view in Control Panel
            titleString=StringLoader::LoadLC(R_DA_TITLE_MAIN);
        titlePane->SetTextL(*titleString);
        CleanupStack::PopAndDestroy(titleString);
        };
    
    ListBox()->AddItemChangeObserverL( this );
    }
    
// ---------------------------------------------------------------------------
// From class CAknDialog.
// Called when the menu is initialized.
// ---------------------------------------------------------------------------
//
void CDefaultAppSrvMimeDlg::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    if (aResourceId != R_DAS_MENU) return;
    __ASSERT_ALWAYS(iDefaultAppUi->iServMimeApps,User::Leave(KErrGeneral));
    
    // for !iSimplifiedView dim the EDasCmdAdvanced
    if(!iDefaultAppUi->iServMimeApps->iSimplifiedView)
        aMenuPane->SetItemDimmed(EDasCmdAdvanced, ETrue);    
    
    // if the Client is not observing when we are exiting, then dim the Exit
    if(iDefaultAppUi->iServMimeApps->iFlagNoObserver)
        aMenuPane->SetItemDimmed(EAknCmdExit, ETrue);
    
    // Help should be displayed only if the feature is supported
    // according to Feature Manager
    if (!FeatureManager::FeatureSupported(KFeatureIdHelp))
        {
        aMenuPane->SetItemDimmed(EAknCmdHelp, ETrue);
        }
		//if the findbox already exists, dim the find menu item.
    if ( FindBox()->IsVisible() )
        {
        aMenuPane->SetItemDimmed(EAknCmdFindPopupActivated, ETrue);
        }
    
    CTextListBoxModel* modelSm = ( ( CEikSettingsListBox* )ListBox() )->Model();  // not taking ownership
    TInt itemCount = modelSm->NumberOfItems();
		//if the listbox is empty, dim the change menu item.
    if (itemCount == 0)
        {
        aMenuPane->SetItemDimmed(EAknCmdOpen, ETrue);
        }
    
    }
    
// ---------------------------------------------------------------------------
// From class CCoeControl.
// Called to get the help context.
// ---------------------------------------------------------------------------
//
void CDefaultAppSrvMimeDlg::GetHelpContext(TCoeHelpContext& aContext) const
    {
    aContext.iMajor = KUidDefaultAppServer;
    
    //check if we have proper values
    if(!iDefaultAppUi->iServMimeApps)
        aContext.iContext = KDA_HLP_MAIN;
    else
        {
        //check for GS client
        if (iDefaultAppUi->iServMimeApps->iAppUid == KUidGS)
            {
            //the client is GS. Check for simplified or advanced view
            if (iDefaultAppUi->iServMimeApps->iSimplifiedView)
                aContext.iContext = KDA_HLP_MAIN;
            else
                aContext.iContext = KDA_HLP_ADVANCED;
            }
        else
            {
            //this is not GS client
            aContext.iContext = KDA_HLP_APPLICATION;
            };
        };
    }

// ---------------------------------------------------------------------------
// From class CCoeControl.
// Called to get the help context.
// ---------------------------------------------------------------------------
//    
void CDefaultAppSrvMimeDlg::SizeChanged()
    {
    CAknSelectionListDialog::SizeChanged(); 
    }
   
    
// ---------------------------------------------------------------------------
// Enables marquee for the list elements.
// ---------------------------------------------------------------------------
//  
void CDefaultAppSrvMimeDlg::EnableMarqueeL()
    {
    CFormattedCellListBoxData *tmp=((CEikSettingsListBox*)(ListBox()))->ItemDrawer()->FormattedCellData();
    
    tmp->SetMarqueeParams(KMaxTInt, 6, 2000000, 10000); // Magic: loop forever
    tmp->EnableMarqueeL(ETrue);
    }


// ---------------------------------------------------------------------------
// Loads icons from a file and sets them in the drawer for iSavedGamesList
// ---------------------------------------------------------------------------
//  
CArrayPtr<CGulIcon>* CDefaultAppSrvMimeDlg::GetPopupListIconsL() const
    {
    // CGulIcon class packages two bitmaps: icon image and its mask
    // CAknIconArray inherits from CArrayPtrFlat
    CArrayPtr<CGulIcon>* iconList = new (ELeave) 
                                 CAknIconArray(2);
    CleanupStack::PushL(iconList);
    
    TFileName iconFile( AknIconUtils::AvkonIconFileName() );
    MAknsSkinInstance* skinInstance;
    CFbsBitmap* newIconBmp = NULL;
    CFbsBitmap* newIconBmpMask = NULL;
    CGulIcon* newIcon = NULL;
    
    skinInstance = AknsUtils::SkinInstance();
              
    //creating blank icon
    AknsUtils::CreateIconLC(skinInstance,KAknsIIDQgnPropEmpty,
                           newIconBmp,newIconBmpMask,iconFile,
                           EMbmAvkonQgn_prop_empty,
                           EMbmAvkonQgn_prop_empty_mask);
    
    newIcon = CGulIcon::NewL(newIconBmp,newIconBmpMask);
    
    CleanupStack::Pop(2);//newIconBmp & newIconBmpMask
    CleanupStack::PushL(newIcon);
    iconList->AppendL(newIcon);
    
    CleanupStack::Pop(newIcon);     
              
    //creating TICK mark icon
    AknsUtils::CreateIconLC(skinInstance,KAknsIIDQgnPropSubCurrent,
                           newIconBmp,newIconBmpMask,iconFile,
                           EMbmAvkonQgn_prop_sub_current,
                           EMbmAvkonQgn_prop_sub_current_mask);
    
    newIcon = CGulIcon::NewL(newIconBmp,newIconBmpMask);
    
    CleanupStack::Pop(2);//newIconBmp & newIconBmpMask
    CleanupStack::PushL(newIcon);
    iconList->AppendL(newIcon);

    CleanupStack::Pop(newIcon);
    
    CleanupStack::Pop(iconList);

    newIconBmp = NULL;
    newIconBmpMask = NULL;
    newIcon = NULL;

    return iconList;
    }
        
    
// ---------------------------------------------------------------------------
// Pops-up a menu list with the Applications for the selected Service & MIME item
// ---------------------------------------------------------------------------
//  
void CDefaultAppSrvMimeDlg::OpenMenuForSelectedServiceAndMimeL(TInt aSelectedIndex)
    {
    if(!iDefaultAppUi->iServMimeApps)User::Leave(KErrNotFound);
    if(aSelectedIndex<0)return; //there is no selected item (list is empty)
    
    CAknSingleGraphicPopupMenuStyleListBox *selectedServiceAndMime = new (ELeave) CAknSingleGraphicPopupMenuStyleListBox;
    CleanupStack::PushL(selectedServiceAndMime);
    
    if ( iPopupList != NULL )
    	{
        iPopupList->CancelPopup();
        iPopupList = NULL;
    	}
    
    iPopupList = CAknPopupList::NewL(selectedServiceAndMime, R_AVKON_SOFTKEYS_OK_CANCEL);
    selectedServiceAndMime->ConstructL(iPopupList,EAknListBoxMenuList);
    selectedServiceAndMime->CreateScrollBarFrameL (ETrue);
    selectedServiceAndMime->ScrollBarFrame()->SetScrollBarVisibilityL( 
            CEikScrollBarFrame::EOff, 
            CEikScrollBarFrame::EAuto );

    //construct icons
    CArrayPtr<CGulIcon>* icons=GetPopupListIconsL();
    selectedServiceAndMime->ItemDrawer()->ColumnData()->SetIconArray(icons); // passing ownership of icons
    
    
    //populate the applications list
    CTextListBoxModel* modelApps = selectedServiceAndMime->Model();  // not taking ownership
    modelApps->SetOwnershipType (ELbmOwnsItemArray);
    CDesCArray* applications = STATIC_CAST(CDesCArray*, modelApps->ItemTextArray());
    HBufC* listTitle=NULL;
    iDefaultAppUi->iServMimeApps->GetApplicationsListL(aSelectedIndex, *applications, listTitle);
    
    
    //set the title of the list
    CleanupStack::PushL(listTitle);
    iPopupList->SetTitleL(*listTitle);
    CleanupStack::PopAndDestroy(listTitle);
    //
    TInt popupOk = iPopupList->ExecuteLD();
    iPopupList = NULL;
    
    if(popupOk)
        {
        TInt selected = selectedServiceAndMime->CurrentItemIndex();
        
        //update the default in the array & list
        //get the list array first
        CTextListBoxModel* modelSm = ((CEikSettingsListBox*)ListBox())->Model();  // not taking ownership
        //modelSm->SetOwnershipType (ELbmOwnsItemArray);
        CDesCArray* servicesAndMimesArray = STATIC_CAST(CDesCArray*, modelSm->ItemTextArray());
        //do the update
        iDefaultAppUi->iServMimeApps->UpdateDefaultL(aSelectedIndex,selected,servicesAndMimesArray);
        //redraw the list
        DrawNow();
        }
    CleanupStack::PopAndDestroy(selectedServiceAndMime);
    }

// ---------------------------------------------------------------------------
// OfferKeyEventL from ccoecontrol
// ---------------------------------------------------------------------------
// 
TKeyResponse CDefaultAppSrvMimeDlg::OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType)
    {
    if ( aKeyEvent.iCode == EKeyBackspace )
        {
        if ( FindBox()->TextLength() >0 )
            {
            return FindBox()->OfferKeyEventL( aKeyEvent, aType );
            }
        }
    return CAknSelectionListDialog::OfferKeyEventL( aKeyEvent ,aType );
    }

// -----------------------------------------------------------------------------
// CDefaultAppSrvMimeDlg::ListBoxItemsChanged (from MListBoxItemChangeObserver)
// 
// -----------------------------------------------------------------------------
//
void CDefaultAppSrvMimeDlg::ListBoxItemsChanged( CEikListBox* aListBox )
    {
    if ( aListBox )
        {
        CEikButtonGroupContainer& cba = ButtonGroupContainer();
        CTextListBoxModel* modelSm = ((CEikSettingsListBox*)aListBox)->Model();  // not taking ownership
        TInt itemCount = modelSm->NumberOfItems();
        if (itemCount == 0)
            {
            cba.MakeCommandVisible(EAknCmdOpen, EFalse);
            }
        else
            {
            cba.MakeCommandVisible(EAknCmdOpen, ETrue);
            }
        }
    }
   
