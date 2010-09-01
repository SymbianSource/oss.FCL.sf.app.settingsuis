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
* Description:  This class implements the AppUi
*
*/



#ifndef C_DEFAULTAPPAPPUI_H
#define C_DEFAULTAPPAPPUI_H


#include <aknappui.h>
#include <eikmenub.h>

class CDefaultAppSrvMimeDlg;
class CDefaultAppView;
class CDefaultAppServMimeApps;



/**
 *  This class implements the AppUi.
 *
 *  @since Series 60 v5.0
 */
class CDefaultAppAppUi : public CAknAppUi
    {
public:
    
    /**
     * Destructor.
     */
    virtual ~CDefaultAppAppUi();

    /**
     * C++ default constructor.
     */
    CDefaultAppAppUi();

    /**
     * Symbian OS default constructor.
     */
    void ConstructL();
    
    /**
     * This function completes the construction of the AppUi. This function is called after the client
     * has connected to this server instance, and all the necessary data for this construction step is 
     * available.
     *
     * @since S60 v5.0
     * @param aClientUid The Uid of the client
     * @param aServiceFlags various flags that may alter what data is displayed to the user
     */
    void ConstructSrvMimeDlgL(TUid aClientUid, TInt aServiceFlags);
    
    /* 
     * This functino is used to record the exist of the dialog; 
     */
    void SetDialogExist( TBool aExist );
    
public: // from CAknAppUi
    
    /**
     * From CAknAppUi.
     * Handles commands from the menu (see base class)
     *
     * @since S60 v5.0
     * @param aCommand the command being handled
     */
    void HandleCommandL(TInt aCommand);

    /**
     * From CAknAppUi.
     * Handles layout changes (e.g. from portrait to landscape)
     *
     * @since S60 v5.0
     * @param aType The type of resources that have changed
     */
    void HandleResourceChangeL(TInt aType);
    
     /**
     * Handles Window Server events (we are interested in catching the end key event 
     * and exiting the application
     *
     * @since S60 v5.0
     * @param aEvent The event that occurred
     * @param aDestination The control associated with the event
     */
    void HandleWsEventL(const TWsEvent & aEvent, CCoeControl * aDestination);


private:
    
    /**
     * Imports and displays the icon of the client application.
     *
     * @since S60 v5.0
     */
    void SetClientIconL(TUid aClietAppUid);

public: 
    
    /**
     * Pointer to the View
     * Own.
     */
    CDefaultAppView *iView;
    
    /**
     * Pointer to the dialog
     * Own.
     */
    CDefaultAppSrvMimeDlg *iDlg;
    
    /**
     * Pointer to the "engine". The engine is a place where application data is retrieved and stored.
     * Own.
     */
    CDefaultAppServMimeApps *iServMimeApps;
    
    /**
     * variable used to remember the list selection position when reloading the dialog
     * for the simplified view
     * Own.
     */
    TInt iSelectedItemSimple;
    
    /**
     * variable used to remember the list selection position when reloading the dialog
     * for the advanced view
     * Own.
     */
    TInt iSelectedItemAdvanced;
    
    /**
     * variable used to remember dialog type (simple/advanced) when reloading the dialog
     * Own.
     */
    TBool iReloadDlgAdvanced;
    
    /**
     * variable used to specify if the dialog is reloaded
     * Own.
     */
    TBool iReloadDlg;
    
    /**
     * specifies if the dialog is dismissed or reloaded
     * Own.
     */
    TBool iExitBack;
    
    TUid  iClientUid;
    
private:
    // This is used to record the exist of the dialogue.
    TBool iDialogExist;
    };
#endif // C_DEFAULTAPPAPPUI_H

//end of file
