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


// Includes
#include <aknViewAppUi.h>
#include <StringLoader.h>
#include <bautils.h>

#include <gsdasplugin_rsc.rsg> // Plugin's own  UI resources
#include <gsdasplugin.mbg>

#include "gs_das.h"

#include <das_client.h>


// Constants

#ifdef __SCALABLE_ICONS
  // svg file
  _LIT( KGSDasPluginIconFileName, "\\resource\\apps\\GSDasPlugin.mif");
#else
  // bitmap
  _LIT( KGSDasPluginIconFileName, "\\resource\\apps\\GSDasPlugin.mbm");
#endif // __SCALABLE_ICONS


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CDefaultAppGSPlugin::CDefaultAppGSPlugin()
// Constructor
//
// ---------------------------------------------------------------------------
//
CDefaultAppGSPlugin::CDefaultAppGSPlugin( )
    : iResources( *iCoeEnv ), iClient(NULL)
    {
    
    }

// ---------------------------------------------------------------------------
// CDefaultAppGSPlugin::~CDefaultAppGSPlugin()
// Destructor
//
// ---------------------------------------------------------------------------
//
CDefaultAppGSPlugin::~CDefaultAppGSPlugin()
    {
    iResources.Close();
    if(iClient)
        delete iClient;
    if( iEmptyContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iEmptyContainer );
        delete iEmptyContainer;
        };
    }

// ---------------------------------------------------------------------------
// CDefaultAppGSPlugin::ConstructL()
// Symbian OS two-phased constructor
// 
// ---------------------------------------------------------------------------
//
void CDefaultAppGSPlugin::ConstructL()
    {
    // Find the resource file:
    TParse parse;
    parse.Set( KDefaultAppGSPluginResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( CEikonEnv::Static()->FsSession(), fileName );

    // Open resource file:
    iResources.OpenL( fileName );
    
    //We can call this after opening the resource file ...
    BaseConstructL( R_GS_DEFAULTAPP_VIEW );
    }

// ---------------------------------------------------------------------------
// CDefaultAppGSPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CDefaultAppGSPlugin* CDefaultAppGSPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CDefaultAppGSPlugin* self = new( ELeave ) CDefaultAppGSPlugin( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ========================= From CAknView ==================


// ---------------------------------------------------------------------------
// From class CAknView.
// Returns the UID of the plugin (see base class)
// ---------------------------------------------------------------------------
//
TUid CDefaultAppGSPlugin::Id() const
    {
    return KDefaultAppGSPluginUid;
    }


// ---------------------------------------------------------------------------
// From class CAknView.
// Activates the plugin (see base class)
// See das_servmimeapps.h for possible values of the flags (useful for R&D)
// -0x00010000 : 
// ---------------------------------------------------------------------------
//
void CDefaultAppGSPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                  TUid /*aCustomMessageId*/,
                                  const TDesC8& /*aCustomMessage*/ )
    {
    iPrevViewId = aPrevViewId;
    
    TInt flags(0); //See das_servmimeapps.h for possible values of the flags (useful for R&D)
    
    if( !iClient )
        {
        iClient=CDefaultAppClient::NewL(this);
        }
    
    iClient->ChangeDefaultsL( flags );
    
    //construct an empty control, so we avoid the screen flickering
    if( !iEmptyContainer )
        {
        iEmptyContainer = CCEmptyContainer::NewL( ClientRect(),NULL );
        iEmptyContainer->SetMopParent(this);
        AppUi()->AddToStackL(*this, iEmptyContainer);
        };
    }


// ---------------------------------------------------------------------------
// From class CAknView.
// Deactivates the Plugin (see base class)
// ---------------------------------------------------------------------------
//
void CDefaultAppGSPlugin::DoDeactivate()
    {
    if(iEmptyContainer)
        {
    	AppUi()->RemoveFromViewStack(*this, iEmptyContainer);
    	delete iEmptyContainer;
    	iEmptyContainer=NULL;
        };
    }


// ---------------------------------------------------------------------------
// From class CAknView.
// Handles a menu command (see base class)
// ---------------------------------------------------------------------------
//
void CDefaultAppGSPlugin::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            AppUi()->ActivateLocalViewL( iPrevViewId.iViewUid );
            break;
        case EAknCmdExit:
        case EEikCmdExit:
            AppUi()->RunAppShutter();
            break;
        default:
            AppUi()->HandleCommandL( aCommand ); //also handles EAknCmdExit 
            break;
        }
    }
// ========================= From CGSPluginInterface ==================



// ---------------------------------------------------------------------------
// From class CGSPluginInterface.
// Returns the Plugin Caption (see base class)
// ---------------------------------------------------------------------------
//
void CDefaultAppGSPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_DEFAULTAPP_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }

// ---------------------------------------------------------------------------
// From class CGSPluginInterface.
// Return the icon, if has one.
// ---------------------------------------------------------------------------
//
CGulIcon* CDefaultAppGSPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;    

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpDevDef,
        KGSDasPluginIconFileName,
        EMbmGsdaspluginQgn_prop_cp_dev_def,
        EMbmGsdaspluginQgn_prop_cp_dev_def_mask );    
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        };

    return icon;
    }

// ========================= From MAknServerAppExitObserver ==================

// ---------------------------------------------------------------------------
// From class MAknServerAppExitObserver.
// Handles server exits
// ---------------------------------------------------------------------------
//
void CDefaultAppGSPlugin::HandleServerAppExit( TInt aReason)
    {
    delete iClient;
    iClient=NULL;
    TRAP_IGNORE(HandleCommandL(aReason));//do not care why it leaved
    }

// End of file
