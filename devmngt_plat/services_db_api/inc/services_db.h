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
* Description:  Implementation of class CServicesDB
*
*/



#ifndef SERVICES_DB_H
#define SERVICES_DB_H

#include <e32def.h>
#include <e32base.h>

class TResourceReader;
 
/**
 *  API for retrieving various localized strings associated with services.
 *
 *  This class receives a resource ID during construction (resource type is DAS_SERVICES)
 *  and retrieves various info defined in this resource. 
 *
 *  @lib defaultappclient.dll
 *  @since S60 v5.0
 */
class CServicesDB : public CBase
    {

public:
    /**
     * Symbian OS two-phased constructor
     * @return
     */
    IMPORT_C static CServicesDB* NewL( TResourceReader* aResReader );
    
    /**
     * Symbian OS two-phased constructor
     * @return
     */
    IMPORT_C static CServicesDB* NewLC( TResourceReader* aResReader );

    /**
     * Destructor.
     */
    IMPORT_C ~CServicesDB();
    
public:

    /**
     * Returns the number of available services 
     * (the number of services read from the resource during construction)
     *
     * @since S60 v5.0
     * @return number of available services/elements
     */
    IMPORT_C TInt Count() const;
    
    /**
     * Function to return the Uid of a service (by index).
     *
     * @since S60 v5.0
     * @return the uid of the service (it leaves if aIndex is not a valid index)
     */
    IMPORT_C TUid ServiceUidL(TInt aIndex) const;
    
    /**
     * Function to return the localized name associated with a service. If the returned 
     * strings accepts a parameter, this can be specified as aParam
     *
     * @since S60 v5.0
     * @param aIndex the index of the service
     * @param aParam parameter for the localized string
     * @return the localized name (it leaves if aIndex is not a valid index)
     */
    IMPORT_C HBufC* ServiceStringLC(TInt aIndex, const TDes& aParam) const;
    
    /**
     * Function to return the localized name associated with a service. If the returned 
     * strings accepts a parameter, this can be specified as aParam
     *
     * @since S60 v5.0
     * @param aServiceUid the Uid of the service
     * @param aParam parameter for the localized string
     * @return the localized name (NULL, if the service is not found)
     */
    IMPORT_C HBufC* ServiceStringLC(TUid aServiceUid, const TDes& aParam) const;
    
    /**
     * Function to return the generic name of a service. 
     *
     * @since S60 v5.0
     * @param aIndex the index of the service
     * @return the generic name of the service (it leaves if aIndex is not a valid index)
     */
    IMPORT_C HBufC* ServiceNameLC(TInt aIndex) const;
    
    /**
     * Function to return the generic name of a service. 
     *
     * @since S60 v5.0
     * @param aServiceUid the Uid of the service
     * @return the generic name of the service (NULL, if the service is not found)
     */
    IMPORT_C HBufC* ServiceNameLC(TUid aServiceUid) const;
    
private:
    
    /**
     * C++ default constructor.
     */
    CServicesDB();
    
    /**
     * Symbian constructor.
     */
    void ConstructL( TResourceReader* aResReader );
        
private:
    /**
     * array of service Uids
     */
    RArray<TInt> iServiceUids;
    
    /**
     * array of service names
     */
    RPointerArray<HBufC> iServiceNames;
    
    /**
     * array of service localized names
     */
    RPointerArray<HBufC> iServiceLoc;
    };

#endif // SERVICES_DB_H
