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
* Description:  Implements a (minimal) GS Plugin
*
*/



#ifndef GS_DAS_H
#define GS_DAS_H

// Includes
#include <aknview.h>
#include <AknServerApp.h>
#include <eikclb.h>
#include <ConeResLoader.h>

#include <gsplugininterface.h>
#include "gs_das_emptycontainer.h"
// Classes referenced
class CDefaultAppClient;

// Constants
/** This is the name of the plugin's resource file */
_LIT( KDefaultAppGSPluginResourceFileName, "z:gsdasplugin_rsc.rsc" );

/** This is the plugin's UID */
const TUid KDefaultAppGSPluginUid = { 0x10281BA1 };
  
// CLASS DECLARATION

/**
 *  This class implements a GS plugin.
 *
 *  This class inplements a GS plugin that acts as a client for the Default App Server, so that it can be
 *  accessed from the GS application.
 *
 *  @since S60 5.0
 */
class CDefaultAppGSPlugin : public CGSPluginInterface, public MAknServerAppExitObserver
    {
    
public: // Constructors and destructor
        
    /**
     * Symbian OS two-phased constructor
     * @return
     */

    static CDefaultAppGSPlugin* NewL( TAny* aInitParams );

    /**
     * Destructor.
     */
    ~CDefaultAppGSPlugin();

public: // From CAknView
                
    /**
     * From CAknView
     * Returns the UID of the Plugin (see base class)
     *
     * @since S60 5.0
     */
    TUid Id() const;
        
    /**
     * From CAknView
     * Activates the plugin (see base class)
     *
     * @since S60 5.0
     * @param aPrevViewId see base class
     * @param aCustomMessageId see base class
     * @param aCustomMessage see base class
     */        
    void DoActivateL( const TVwsViewId& aPrevViewId,
                      TUid aCustomMessageId,
                      const TDesC8& aCustomMessage );
        
    /**
     * From CAknView
     * Deactivates the Plugin (see base class)
     *
     * @since S60 5.0
     */
    void DoDeactivate();
    
    /**
     * From CAknView
     * Handles a menu command (see base class)
     *
     * @since S60 5.0
     * @param aCommand the command to handle
     */
    void HandleCommandL( TInt aCommand );

public: // From CGSPluginInterface
    
    /**
     * From CGSPluginInterface
     * Returns the Plugin Caption (see base class)
     *
     * @since S60 5.0
     * @param aCaption placeholder for the caption
     */   
    void GetCaptionL( TDes& aCaption ) const;
    
    /**
     * From CGSPluginInterface
     * Returns the plugin's icon (see CGSPluginInterface header file)
     *
     * @since S60 5.0
     * @param aIconType icon type
     */  
    CGulIcon* CreateIconL( const TUid aIconType );
        
private: // From MAknServerAppExitObserver
    
    /**
     * From MAknServerAppExitObserver
     * Handles server exits
     *
     * @since S60 5.0
     * @param aReason for the server exit
     */
    void HandleServerAppExit ( TInt aReason );
        
private:
        
    /**
     * C++ default constructor.
     */
    CDefaultAppGSPlugin();

    /**
     * Symbian OS default constructor.
     */
    void ConstructL();
        
private: // Data  

    /**
     * The Resource Loader
     */
    RConeResourceLoader iResources; // Resouce loader.
    
    /**
     * The id of the previous View, to be activated when user pushes Back
     */
    TVwsViewId iPrevViewId;
    
    /**
     * Pointer to the Default App Client API class
     * Own.
     */
    CDefaultAppClient* iClient;
    
    /**
     * Empty Container, used to avoid screen flickering
     * Own.
     */
    CCEmptyContainer* iEmptyContainer;
    };

#endif // GS_DAS_H
// End of File
