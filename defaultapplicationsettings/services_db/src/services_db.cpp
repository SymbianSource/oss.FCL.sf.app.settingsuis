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
* Description:  Implements the Services DB API
*
*/


#include <e32std.h>
#include <bautils.h>
#include <StringLoader.h>

#include <services_db.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CServicesDB::NewLC()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C CServicesDB* CServicesDB::NewLC( TResourceReader* aResReader )
    {
    CServicesDB* self = new (ELeave) CServicesDB();
    CleanupStack::PushL(self);
    self->ConstructL(aResReader);
    return self;
    }

// ---------------------------------------------------------------------------
// CServicesDB::NewL()
//
//
// ---------------------------------------------------------------------------
//    
EXPORT_C CServicesDB* CServicesDB::NewL( TResourceReader* aResReader )
    {
    CServicesDB* self = CServicesDB::NewLC(aResReader);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CServicesDB::CServicesDB()
//
//
// ---------------------------------------------------------------------------
//
CServicesDB::CServicesDB()
    {
    //no implementation necessary
    }

// ---------------------------------------------------------------------------
// CServicesDB::~CServicesDB()
//
//
// ---------------------------------------------------------------------------
//    
EXPORT_C CServicesDB::~CServicesDB()
    {
    iServiceUids.Close();
    iServiceNames.ResetAndDestroy();
    iServiceLoc.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// The class is constructed by reading the data from the specified resource id into memory
// ---------------------------------------------------------------------------
//
void CServicesDB::ConstructL( TResourceReader* aResReader )
    {
    TInt i;
    TInt count = aResReader->ReadInt16();
    for ( i = 0; i < count; i++ )
        {
        //read the service uid
        TInt uid=aResReader->ReadInt32();
        if(uid == 0)User::Leave(KErrArgument);
        //read the service name
        HBufC* service=aResReader->ReadHBufCL();
        if(!service)User::Leave(KErrArgument);
        //read the localized string
        HBufC* serviceLoc=aResReader->ReadHBufCL();
        if(!serviceLoc)User::Leave(KErrArgument);
        
        //append things
        iServiceUids.Append(uid);
        iServiceNames.Append(service);//takes ownership
        iServiceLoc.Append(serviceLoc);//takes ownership
        }
    //done
    }            

// ---------------------------------------------------------------------------
// Returns the number of available services 
// (the number of services read from the resource during construction)
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CServicesDB::Count() const
    {
    return iServiceUids.Count();
    }

// ---------------------------------------------------------------------------
// Function to return the Uid of a service (by index).
// ---------------------------------------------------------------------------
//
EXPORT_C TUid CServicesDB::ServiceUidL(TInt aIndex) const
    {
    if(aIndex<0 || aIndex>=iServiceUids.Count())
        User::Leave(KErrArgument);
    return TUid::Uid(iServiceUids[aIndex]);
    }
    
    
// ---------------------------------------------------------------------------
// Function to return the localized name associated with a service. If the returned 
// strings accepts a parameter, this can be specified as aParam
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC* CServicesDB::ServiceStringLC(TInt aIndex, const TDes& aParam) const
    {
    TInt size=aParam.Size();
    HBufC *string=NULL;
    
    //check parameters
    if(aIndex<0 || aIndex>=iServiceUids.Count())
        User::Leave(KErrArgument);
    
    //get the size of the new string
    size+=iServiceLoc[aIndex]->Size();
    //allocate the string
    string = HBufC::NewLC(size);
    //create string content
    TPtr ptr=string->Des();
    if(aParam.Size())
        {
        //we have something in aParam
        //ptr.Format(*iServiceLoc[aIndex],&aParam);
        StringLoader::Format(ptr,*iServiceLoc[aIndex],-1,aParam);
        }
    else
        {
        //nothing in aParam, just copy the localized string to string
        ptr.Copy(*iServiceLoc[aIndex]);
        }
    //done, return
    return string; //string is on the stack, too
    }
    
// ---------------------------------------------------------------------------
// Function to return the localized name associated with a service. If the returned 
// strings accepts a parameter, this can be specified as aParam
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC* CServicesDB::ServiceStringLC(TUid aServiceUid, const TDes& aParam) const
    {
    HBufC* string=NULL;
    TInt i;
    
    //look for the service UIDs
    for(i=0; i<iServiceUids.Count(); i++)
        {
        if(iServiceUids[i] == aServiceUid.iUid)
            {
            string = ServiceStringLC(i,aParam);
            break;//service found
            }
        }
    if(!string)CleanupStack::PushL(string);//otherwise, string is already on the stack!
    return string;
    }
    
// ---------------------------------------------------------------------------
// Function to return the generic name of a service. 
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC* CServicesDB::ServiceNameLC(TInt aIndex) const
    {
    HBufC* string=NULL;
    TInt size=0;
     
    //check parameters
    if(aIndex<0 || aIndex>=iServiceUids.Count())
        User::Leave(KErrArgument);
    
    //get the size of the new string
    size=iServiceNames[aIndex]->Size();
    //allocate the string
    string = HBufC::NewLC(size);
    //create string content
    TPtr ptr=string->Des();
    ptr.Copy(*iServiceNames[aIndex]);
    return string;
    }

// ---------------------------------------------------------------------------
// Function to return the generic name of a service. 
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC* CServicesDB::ServiceNameLC(TUid aServiceUid) const
    {
    HBufC* string=NULL;
    TInt i;
    
    //loc for the service UIDs
    for(i=0; i<iServiceUids.Count(); i++)
        {
        if(iServiceUids[i] == aServiceUid.iUid)
            {
            string = ServiceNameLC(i);
            break;//service found
            }
        }
    if(!string)CleanupStack::PushL(string);//otherwise, string is already on the stack!
    return string;
    }

