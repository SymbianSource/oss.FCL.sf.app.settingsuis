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
* Description:  The Service & MIME list dlg
*
*/


#ifndef C_DEFAULTAPPSRVMIMEDLG_H
#define C_DEFAULTAPPSRVMIMEDLG_H


#include <badesca.h> // CDesCArray
#include <coecntrl.h> // CCoeControl
#include <eiklbo.h> // MEikListBoxObserver


#include <aknselectionlist.h> 
#include <aknsettingitemlist.h> 

// FORWARD DECLARATIONS
class CAknDoubleStyleListBox;
class CDefaultAppAppUi;
//class CDefaultAppServMimeApps;

/**
 *  This is the Dialog class of the application
 *
 *  This dialog displays a double list box and it can also display a pop-up list on top of it.
 *  The elements of the lists are created by the engine (CDefaultAppServMimeApps class)
 *
 *  @since S60 v5.0
 */
class CDefaultAppSrvMimeDlg : public CAknSelectionListDialog, 
                              public MListBoxItemChangeObserver
    {
public:
    
    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CDefaultAppSrvMimeDlg* NewL(CDefaultAppAppUi* aAppUi);
    
    /**
     * Destructor.
     */
    virtual ~CDefaultAppSrvMimeDlg();
    
public: //from CAknSelectionListDialog
    
    /**
     * From CAknSelectionListDialog.
     * This function is called when buttons/softkeys are pressed, to check if the dlg should exit.
     * We always return EFalse, since this is a modeless & nonwaiting dialog.
     *
     * @since S60 v5.0
     * @param aButtonId the list box for which the event happened
     */
    virtual TBool OkToExitL(TInt aButtonId);
    
    /**
     * From CAknSelectionListDialog.
     * This function is called when a command is issued by the user.
     *
     * @since S60 v5.0
     * @param aCommandId the list box for which the event happened
     */
    virtual void ProcessCommandL(TInt aCommand);
    
    /**
     * From CAknSelectionListDialog.
     * This function is called during the construction of the dialog.
     * We build the list of elements here.
     *
     * @since S60 v5.0
     */
    virtual void PreLayoutDynInitL();
    

public: //from CAknDialog
    
    /**
     * From CAknDialog.
     * This function is called to complete the menu construction.
     *
     * @since S60 v5.0
     * @param aResourceId the resource id
     * @param aMenuPane the menu class instance
     */
    virtual void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
    
public: //from CCoeControl

    /**
     * From CCoeControl.
     * This function is called to get the help context
     *
     * @since S60 v5.0
     * @param aContext recipient for the help context
     */
    virtual void GetHelpContext(TCoeHelpContext& aContext) const;
    
    /**
     * From CCoeControl.
     * Called by the framework when the view size is changed
     *
     * @since S60 v5.0
     */
    virtual void SizeChanged();
    
    /**
     * From CCoeControl.
     * Called by the framework to process the key event
     *
     * @since S60 v5.0
     */
    virtual TKeyResponse OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType);
    
    
public: //own functions

    /**
     * Enables marquee for the list elements.
     *
     * @since S60 v5.0
     */
    void EnableMarqueeL();
    
    
    /**
     * Part of the popup menu construction process: 
     * gets 2 icons, the selected (ticked) and empty icon
     *
     * @since S60 v5.0
     * @return an array with the 2 icons
     */
    CArrayPtr<CGulIcon>* GetPopupListIconsL() const;
    
    /**
     * Creates a pop-up list for a selected item of the list-box.
     *
     * @since S60 v5.0
     * @param aSelectedIndex the selected element (used to get the content of the pop-up list)
     */
    void OpenMenuForSelectedServiceAndMimeL(TInt aSelectedIndex);    
public:
    //from MListBoxItemChangeObserver
    void ListBoxItemsChanged(CEikListBox* aListBox);
    
private: //construction

    /**
     * C++ default constructor.
     */
    CDefaultAppSrvMimeDlg(CDefaultAppAppUi* aAppUi, TInt *aSelected);

private: //data    
    /**
     * Pointer to the AppUi
     * Not Owned.
     */
    CDefaultAppAppUi* iDefaultAppUi;
    
    /**
     * The index of the selected element in the list. This is pointer to AppUI's index
     * Not owned.
     */
    TInt *iSelected;
    
    /**
     * Flag allowing to exit the dialog
     */
    TBool iOkToExit;

    /**
     * Pointer refer to CAknPopupList
     */
    CAknPopupList* iPopupList;
    };
    

#endif // C_DEFAULTAPPSRVMIMEDLG_H

// end of file

    