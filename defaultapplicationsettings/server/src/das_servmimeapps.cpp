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
* Description:  Keeps info about Services, MIMEs and Applications
*
*/


// System includes
#include <e32def.h> // STATIC_CAST
#include <defaultappserver.rsg> // R_DAS_SERVICESMIMES_LISTBOX
#include <StringLoader.h> // StringLoader
#include <barsread.h> //TResourceRead
#include <eikenv.h> //CEikonEnv
#include <apgcli.h>
#include <apgicnfl.h> // CApaMaskedBitmap

#include <serviceregistry.h>

#include "das_servmimeapps.h"
#include "das_app.h" // KUidDefaultAppServer
#include <services_db.h>

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apmstd.h>
#else
#include <apmstd.h>
#include <apmfndr.h> // KDataTypePrioritySystem
#endif

const TInt KStringMargin = 10; //10 is a sufficiently large margin

// ======== MEMBER FUNCTIONS ======== CAppHelper

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
// 
CAppHelper* CAppHelper::NewLC(const TDesC& aName, const TInt aUid)
    {
    CAppHelper* self = new (ELeave) CAppHelper(aUid,0);
    CleanupStack::PushL(self);
    //construct iName, copy it
    self->iName = HBufC::NewL(aName.Size());
    TPtr* ptr=new (ELeave) TPtr(self->iName->Des());
    ptr->Copy(aName);
    delete ptr;
    
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
//     
CAppHelper* CAppHelper::NewLC(const CAppHelper& aApp)
    {
    CAppHelper* self = new (ELeave) CAppHelper(aApp.iUid,1,aApp.iFlags);
    CleanupStack::PushL(self);
    //construct iName, get pointer and flag it
    self->iName = aApp.iName;
    self->iFlags |= EFlagNameNotOwned;
    return self;
    }
 
// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//     
CAppHelper::CAppHelper(const TInt aUid, const TInt aScore, const TInt aFlags):
    iName(NULL), iUid(aUid), iScore(aScore), iFlags(aFlags)
    {
    //no implementation needed
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
CAppHelper::~CAppHelper(void)
    {
    if(!( iFlags&EFlagNameNotOwned ))
        delete iName;
    iName = NULL;
    }
// ---------------------------------------------------------------------------
// This function is used to order Applications
// ---------------------------------------------------------------------------
//  
TInt CAppHelper::OrderApplications(const CAppHelper& a1, const CAppHelper& a2)
    {
    //if a1<a2 return -1, otherwise 0 or 1
    //platform applications are always first (smaller)
    if(a1.iFlags&EFlagPlatformApp && !(a2.iFlags&EFlagPlatformApp))
        return -1; //a1 is platform app, a2 is not
    if(!(a1.iFlags&EFlagPlatformApp) && a2.iFlags&EFlagPlatformApp)
        return 1; //a1 is not platform app, a2 is
    
    //look into names names
    if(*a1.iName < *a2.iName) return -1;
    if(*a1.iName > *a2.iName) return 1;
    //if we are here, strings were equal
    return 0;
    }

    
// ======== MEMBER FUNCTIONS ======== CServiceMime

// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//     
CServiceMime::CServiceMime() : iServiceMime(NULL), iMime(NULL)
    {
    iDefaultAppUid=TUid::Uid(0);
    iDefaultApp=-1;//currently we do not know the default index
    iUsedByTasks=EFalse;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CServiceMime::~CServiceMime()
    {
    if (iServiceMime) delete iServiceMime;
    if (iMime) delete iMime;
    for (TInt i=0 ; i<iApplications.Count() ; i++)
        delete iApplications[i];
    iApplications.Close();
    }
    
// ---------------------------------------------------------------------------
// This function is used to order Services & MIMEs
// ---------------------------------------------------------------------------
//  
TInt CServiceMime::OrderServiceMimes(const CServiceMime& a1, const CServiceMime& a2)
    {
    //if a1<a2 return -1, otherwise 0 or 1
    //we sort by the MIME, then by the service
    if(*a1.iMime<*a2.iMime)return -1;
    if(*a1.iMime>*a2.iMime)return 1;
    //if we are here, MIMEs are the same
    if(*a1.iServiceMime<*a2.iServiceMime)return -1;
    if(*a1.iServiceMime>*a2.iServiceMime)return 1;
    //if we are here, strings were equal
    return 0;
    }

// ======== MEMBER FUNCTIONS ======== CMediaTask

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
// 
CMediaTask* CMediaTask::NewLC( TResourceReader& aReader )
    {
    CMediaTask* self = new (ELeave) CMediaTask();
    CleanupStack::PushL(self);
    self->ConstructL(aReader);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
CMediaTask::~CMediaTask()
    {
    
    if(iTaskList)delete iTaskList;
    if(iTaskTitle)delete iTaskTitle;
    
    if(iMimeLabel)delete iMimeLabel;
    
    iMimes.ResetAndDestroy();
    iServicesUids.Close();
    
    iSMs.Close();//elements not owned
    
    iApplications.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
// 
CMediaTask::CMediaTask() : iMimeLabel(NULL), iDefaultApp(-1)
    {
    }

// ---------------------------------------------------------------------------
// The construction means reading the task details from the resource file
// ---------------------------------------------------------------------------
//    
void CMediaTask::ConstructL( TResourceReader& aReader )
    {
    TInt i,count;
    //read the task name
    iTaskList = aReader.ReadHBufCL();
    if( !iTaskList )User::Leave(KErrArgument);
    
    //read the task title
    iTaskTitle = aReader.ReadHBufCL();
    if( !iTaskTitle )User::Leave(KErrArgument);
    
    //read the mime label
    iMimeLabel = aReader.ReadHBufC8L();
    if( !iMimeLabel )User::Leave(KErrArgument);
    
    //read the service mimes entries
    count = aReader.ReadInt16();
    for (i=0; i<count; i++)
        {
        //read the service uid
        TInt uid=aReader.ReadInt32();
        if(uid == 0)User::Leave(KErrArgument);
        //read the mime
        HBufC8* mime=aReader.ReadHBufC8L();
        if( !mime )User::Leave(KErrArgument);
        //append both things
        iServicesUids.Append(uid);
        iMimes.Append(mime);//takes ownership
        }
    }
    
    
// ======== MEMBER FUNCTIONS ======== CDefaultAppServMimeApps

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
// 
CDefaultAppServMimeApps* CDefaultAppServMimeApps::NewL(const TUid& aAppUid, TInt aServiceFlags)
    {
    CDefaultAppServMimeApps* self = CDefaultAppServMimeApps::NewLC(aAppUid, aServiceFlags);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian 2-phased constructor
// ---------------------------------------------------------------------------
// 
CDefaultAppServMimeApps* CDefaultAppServMimeApps::NewLC(const TUid& aAppUid, TInt aServiceFlags)
    {
    CDefaultAppServMimeApps* self = new (ELeave) CDefaultAppServMimeApps();
    CleanupStack::PushL(self);
    self->ConstructL(aAppUid, aServiceFlags);
    return self;
    }

// ---------------------------------------------------------------------------
// This function builds the data structures for the dialog:
// It reads the considered services from the resource files, then it builds the
// task list and the associated list of Services& MIMEs and applications.
// ---------------------------------------------------------------------------
//
void CDefaultAppServMimeApps::ConstructL(TUid aAppUid, TInt aServiceFlags)
    {
    //set the client app uid
    /* Uncomment this for testing the General Settings (Control Pannel) specific view from another application (R&D purpose)
     * 
    TBool flagGsClient = aServiceFlags & EFlagGsClient;
    if(flagGsClient)aAppUid=KUidGS; //we mimic the GS Client. 
    */
    iAppUid = aAppUid;
    
    //for all the available services, launch the AddMIMEsForServiceL function
    TResourceReader reader;
    CEikonEnv::Static()->CreateResourceReaderLC( reader, R_DA_SERVICE_MIME );
    CServicesDB* sdb = CServicesDB::NewLC(&reader);
    TInt i;
    TUid uid;
    for(i=0; i<sdb->Count(); i++)
        {
        uid = sdb->ServiceUidL(i);
        AddMIMEsForServiceL(uid,i,aServiceFlags,sdb);
        };
    CleanupStack::PopAndDestroy(sdb);
    CleanupStack::PopAndDestroy( ); // reader
    //change MIMEs & sort
    BeautifyAndSortServMimeApps();
    }
    
// ---------------------------------------------------------------------------
// this is by far the most complicated function in the entire subsystem
// some more comments are needed, but after this function will be split in 2
// ---------------------------------------------------------------------------
//
void CDefaultAppServMimeApps::AddMIMEsForServiceL(TUid aServiceUid, TInt aServiceIndex, TInt aServiceFlags, CServicesDB* aServicesDb)
    {
    CServiceMime *sm = NULL;
    TUid uid;
    
    RApaLsSession ls;
    TApaAppInfo info;
    CApaAppServiceInfoArray* serv=NULL;
    CServiceRegistry *sr=NULL;
    TInt i,j,k;
    TInt it,jt;//indexes for iterating inside tasks
    TInt lowerMarker, upperMarker;
    //flags
    TBool flagShowAll = EFalse;
    /*
     * Uncomment the line below to show all the Services & MIMEs in views, and not only those for which default app can be changed.
     * This can be used for R&D, to see all the MIMEs&Services in the system.
    flagShowAll = aServiceFlags & EFlagShowAllServicesAndMimes;
     */
    iFlagNoObserver = aServiceFlags & EFlagNoObserver;
    
    //check who is connecting, so that we can set the view
    if(iAppUid == KUidGS)
        {
        iSimplifiedView=ETrue; //at least in the beginning. 
                               //For this function, this is also a flag that says if we have GS as client or not
        GetTaskListL(R_DA_TASKS);
        }
    else iSimplifiedView=EFalse;
        
    //connect
    User::LeaveIfError( ls.Connect() );
    CleanupClosePushL( ls );
    
    //Get all the apps and MIMEs for the current service
    ls.GetAppInfo(info,iAppUid);
    serv=ls.GetServiceImplementationsLC(aServiceUid);
    lowerMarker = upperMarker = iServMimes.Count();
    //create entries for the MIMEs supported by the client application
    for (i=0; i<serv->Array().Count(); i++)
        {
        uid=serv->Array()[i].Uid();
        if( iSimplifiedView || uid == iAppUid) //this means that for app clients, we first skip all the other applications
            {
            //if iSimplifiedView is true, then we have GS as a client    
            for(j=0; j<serv->Array()[i].DataTypes().Count(); j++)
                {
                TBool createSM=ETrue;
                if(iSimplifiedView)
                    {
                    //check if we already have an entry for this service & MIME
                    for(k=lowerMarker; k<upperMarker; k++)
                        if(iServMimes[k]->iMime->Des() == serv->Array()[i].DataTypes()[j].iDataType.Des8())
                            {
                            //we found it!
                            sm=iServMimes[k];
                            createSM=EFalse;
                            break;//we found the sm, no need to search for it any more
                            }
                    //if we don't find it, we will create an entry ...
                    }
            
                if(createSM)
                    {
                    //we have to create sm, we did not found it previously (or we were not looking for it)
                    sm=new (ELeave) CServiceMime;
                    CleanupStack::PushL(sm);
                    sm->iMime = HBufC8::NewL(serv->Array()[i].DataTypes()[j].iDataType.Des8().Size());
                    *(sm->iMime) = serv->Array()[i].DataTypes()[j].iDataType.Des8();
                    //transform the MIME from audio/mpeg to audio mpeg
                    HBufC *transformedMime=HBufC::NewLC(serv->Array()[i].DataTypes()[j].iDataType.Des().Size());
                    TPtr *ptr=new (ELeave) TPtr(transformedMime->Des());
                    CleanupStack::PushL(ptr);
                    ptr->Copy(serv->Array()[i].DataTypes()[j].iDataType.Des());
                    TInt location=ptr->Locate('/');
                    if(location>0)ptr->Replace(location,1,_L(" "));
                    //transforming done
                    //sm->iServiceMime = aServicesDb->ServiceStringLC(aServiceIndex, serv->Array()[i].DataTypes()[j].iDataType.Des());
                    sm->iServiceMime = aServicesDb->ServiceStringLC(aServiceIndex, transformedMime->Des());
                    CleanupStack::Pop(sm->iServiceMime);
                    CleanupStack::PopAndDestroy(ptr);
                    CleanupStack::PopAndDestroy(transformedMime);
                    //StringLoader::LoadL( R_DA_SERVICE_OPEN, serv->Array()[i].DataTypes()[j].iDataType.Des());
                    sm->iServiceUid=aServiceUid;
                    }
            
                //insert the client application
                InsertApplicationL(*sm, uid, serv->Array()[i].DataTypes()[j].iPriority, &ls);
            
                //get the Uid of the default application for this service and MIME
                ls.AppForDataType(serv->Array()[i].DataTypes()[j].iDataType,sm->iDefaultAppUid);
                
                if(createSM)
                    {
                    //sm was created this iteration, ad it.
                    iServMimes.AppendL(sm);//takes ownership of sm
                    upperMarker++;
                    CleanupStack::Pop(sm);
                
                    }
                }//for
                /*
                 * If a service that has no MIME will be considered, some code must be added (probably in this place)
                 */
            }//if (
        
        }
    __ASSERT_DEBUG(upperMarker == iServMimes.Count(), User::Panic( _L("upperMarker bad value"), 1));
    
    if(sr)
        {
        //we do not need it any more
        delete sr;
        sr=NULL;
        }
    if(lowerMarker == upperMarker)
        {
        //current app does not support any MIME for the current service, clean and get out of here
        CleanupStack::PopAndDestroy(serv);
        CleanupStack::PopAndDestroy();  // closes RApaLsSession
        return;
        }
        
    if(!iSimplifiedView)
        {
        //if we are here, we have MIMEs for the current service
        //iterate once more and add applications for MIMEs already in the list
        for (i=0; i<serv->Array().Count(); i++)
            {
            uid=serv->Array()[i].Uid();
            if( uid == iAppUid) continue; //we don't add our client application once more in the list (it is already there)
            for(j=0; j<serv->Array()[i].DataTypes().Count(); j++)
                for(k=lowerMarker; k<upperMarker; k++)
                    if(iServMimes[k]->iMime->Des() == serv->Array()[i].DataTypes()[j].iDataType.Des8())
                        {
                        InsertApplicationL(*(iServMimes[k]), serv->Array()[i].Uid(), serv->Array()[i].DataTypes()[j].iPriority, &ls);
                        }//if same MIME
            }
        }
        
    //before deleting some of the entries ...    
    if(iSimplifiedView)
        {
        //check the service & MIMEs against the task list...
        //first, check the service against task services
        for(it=0; it<iTasks.Count(); it++)
            for(jt=0; jt<iTasks[it]->iServicesUids.Count(); jt++)
                if(aServiceUid.iUid == iTasks[it]->iServicesUids[jt])
                    {
                    //lets check if the MIME matches too...
                    for(k=lowerMarker; k<upperMarker; k++)
                        if(*iServMimes[k]->iMime == *iTasks[it]->iMimes[jt])
                            {
                            //this entry matches an entry in the task list
                            //add the applications to the list
                            PopulateTaskWithApplicationsL(*iTasks[it],iServMimes[k]);
                            //link the sm, if it has more than 1 application
                            break;//sm found, no need to search for it further
                            }
                    }
        }
    
    
    
    //iterate from the newly added entries and set the index
    for(k=lowerMarker; k<upperMarker; k++)
        {
        TBool smSetOrRemoved = EFalse;
        //check for single applications
        if( iServMimes[k]->iApplications.Count() <2 && !flagShowAll)
            {
            //this entry has a single element (that should not be shown)
            //we either delete it, or move it
            if(iServMimes[k]->iUsedByTasks)
                {
                //the sm is used by tasks, we move it
                iTaskServMimes.AppendL(iServMimes[k]);//takes ownership of sm
                }
            else
                {
                //the sm is not used by tasks, we delete it.
                delete iServMimes[k];
                }
            
            //we remove the entry from the iServMimes (so that it is not shown)
            iServMimes.Remove(k);
            k--;
            upperMarker--;
            smSetOrRemoved=ETrue;
            }
        
        //set the index
        if(!smSetOrRemoved)
            {
            for(i=0 ; i< iServMimes[k]->iApplications.Count() ; i++)
                {
                if(iServMimes[k]->iApplications[i]->iUid == iServMimes[k]->iDefaultAppUid.iUid)
                    iServMimes[k]->iDefaultApp=i;
                }
            }
        };
    
    if(iSimplifiedView)
    for(it=0; it<iTasks.Count(); it++)
        if(iTasks[it]->iDefaultAppUid.iUid == 0 && iTasks[it]->iApplications.Count()>0)
            {
            //there is no uid, look for the platform App and make it the default
            for(jt=0; jt<iTasks[it]->iApplications.Count(); jt++)
                if(iTasks[it]->iApplications[jt]->iFlags & CAppHelper::EFlagPlatformApp)
                    {
                    iTasks[it]->iDefaultAppUid.iUid = iTasks[it]->iApplications[jt]->iUid;
                    iTasks[it]->iDefaultApp=jt;
                    break;//app found, no need to search for it any longer
                    };
                
            //if no app found, mark the first one as default
            if(iTasks[it]->iDefaultAppUid.iUid == 0)
                {
                iTasks[it]->iDefaultAppUid.iUid = iTasks[it]->iApplications[0]->iUid;
                iTasks[it]->iDefaultApp = 0;//the index
                };
            }
        else
            for(jt=0; jt<iTasks[it]->iApplications.Count(); jt++)
                if(iTasks[it]->iApplications[jt]->iUid == iTasks[it]->iDefaultAppUid.iUid)
                    {
                    iTasks[it]->iDefaultApp=jt;
                    break;//app found, no need to search for it any longer
                    }
    
    //done, destroy serv
    CleanupStack::PopAndDestroy(serv);
    CleanupStack::PopAndDestroy( );  // closes RApaLsSession
    }
    
// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//     
CDefaultAppServMimeApps::CDefaultAppServMimeApps(): iList(4)
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
CDefaultAppServMimeApps::~CDefaultAppServMimeApps()
    {
    iServMimes.ResetAndDestroy();
    iTasks.ResetAndDestroy();
    iTaskServMimes.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// This function sorts the Services & MIMEs and their applications. 
// It also sorts the Serivces & MIMEs associated with tasks
// ---------------------------------------------------------------------------
//
void CDefaultAppServMimeApps::BeautifyAndSortServMimeApps(void)
    {
    //first, go through the Services & MIMEs localized names and sort them
    TLinearOrder<CServiceMime> order(&CServiceMime::OrderServiceMimes);
    iServMimes.Sort(order);
    
    //go through applications and sort them
    TInt i,j;
    for(i=0; i<iServMimes.Count(); i++)
        {
        TLinearOrder<CAppHelper> order2(&CAppHelper::OrderApplications);
        iServMimes[i]->iApplications.Sort(order2);
        //get our app index 
        for(j=0; j<iServMimes[i]->iApplications.Count(); j++)
            if(iServMimes[i]->iApplications[j]->iUid == iServMimes[i]->iDefaultAppUid.iUid)
                iServMimes[i]->iDefaultApp = j;
        };
    //if we have tasks, we sort their apps too
    for(i=0; i<iTasks.Count(); i++)
        {
        TLinearOrder<CAppHelper> order2(&CAppHelper::OrderApplications);
        iTasks[i]->iApplications.Sort(order2);
        //get our app index 
        for(j=0; j<iTasks[i]->iApplications.Count(); j++)
            if(iTasks[i]->iApplications[j]->iUid == iTasks[i]->iDefaultAppUid.iUid)
                iTasks[i]->iDefaultApp = j;
        };
    }


// ---------------------------------------------------------------------------
// This function reads from the resource file the list of tasks (and associated data). 
// This function is used during construction
// ---------------------------------------------------------------------------
//    
void CDefaultAppServMimeApps::GetTaskListL( TInt aResourceId )
    {
    TResourceReader reader;
    TInt i;
    CServiceRegistry* sr=CServiceRegistry::NewL();
    CleanupStack::PushL(sr);

    // Read tasks
    
    CEikonEnv::Static()->CreateResourceReaderLC( reader, aResourceId );

    TInt count = reader.ReadInt16();
    for ( i = 0; i < count; i++ )
        {
        CMediaTask* task=CMediaTask::NewLC(reader);
        // Read the default application for this task ...
        sr->GetDefault(KOpenServiceUid, *task->iMimeLabel, task->iDefaultAppUid);
        
        //add the task to the list
        iTasks.Append(task);
        CleanupStack::Pop(task);
        }

    CleanupStack::PopAndDestroy(); // reader
    CleanupStack::PopAndDestroy(sr);
    }
    
// ---------------------------------------------------------------------------
// This function adds a new Application to a list of an Service & Mime object.
// ---------------------------------------------------------------------------
//    
void CDefaultAppServMimeApps::InsertApplicationL(CServiceMime& aServMime, const TUid& aAppUid, const TDataTypePriority& aPrio, const RApaLsSession *aLs)
    {
    TBool addIt = ETrue;

    if(aPrio == KDataTypePrioritySystem)
        {
        // the application to be inserted has system priority
        if(aServMime.iDefaultApp == -1)
            {
            //we have non-system applications in the list
            //delete them ...
            aServMime.iApplications.ResetAndDestroy();
                          
            //... and make the default -2 ... (it means single app with system priority)
            aServMime.iDefaultApp = -2;
            //... and add our application (after exiting from the "if")
            }
        else
            {
            //do not delete the applications, but just add our application
            //...and make the default -3 (it means multiple apps with system priority)
            aServMime.iDefaultApp = -3;
            //... and add our application (after exiting from the "if")
            }
        }
    else
        {
        // the application to be inserted does NOT have system priority
        if(aServMime.iDefaultApp < -1)
            {
            // there are only System apps in the list, do not add ours
            addIt = EFalse;
            }
        // else there are normal applications in the list, insert ours, as well
        }
    //add the application to the list
    if(addIt)
        {
        TApaAppInfo info;
        aLs->GetAppInfo(info,aAppUid);
        //find the best name for the application
        CAppHelper *app = NULL;
        if(info.iCaption.Size() != 0)
            {
            //this is for now, the preferred name
            app = CAppHelper::NewLC(info.iCaption, aAppUid.iUid);
            }
        else
            {
            //fall-back name
            app = CAppHelper::NewLC(info.iShortCaption, aAppUid.iUid);
            };
        //is the app platform application??
        if (info.iFullName.Left(1) == _L("Z") || 
            info.iFullName.Left(1) == _L("z")) 
            app->iFlags |= CAppHelper::EFlagPlatformApp;
        //append the name
        aServMime.iApplications.AppendL(app);
        CleanupStack::Pop(app);
        }
    }

// ---------------------------------------------------------------------------
// This function takes a task and a Service & MIME object. It checks all the applications that support the given
// Service & MIME object (from its list). If an application is not in the task's application list, this function adds 
// it there, with a score of 1. If the application is already in the list, then its score is incremented.
// The Service & MIME object is also marked as being used by a task.
// ---------------------------------------------------------------------------
//    
void CDefaultAppServMimeApps::PopulateTaskWithApplicationsL(CMediaTask& aTask,CServiceMime* aServMime)
    {
    TInt i,j;
    TBool found;
    //add the applications to the list
    for(i=0; i<aServMime->iApplications.Count(); i++)
        {
        found=EFalse;
        for(j=0; j<aTask.iApplications.Count(); j++)
            if(aServMime->iApplications[i]->iUid == aTask.iApplications[j]->iUid)
                {
                //we found this application...
                found=ETrue;
                //... increase its score
                aTask.iApplications[j]->iScore++;
                //... and get to the next application
                break; //application found, no need to search for it any more
                }
        //if the application was not found, we have to add it!
        if(!found)
            {
            CAppHelper *app = CAppHelper::NewLC(*aServMime->iApplications[i]);
            aTask.iApplications.AppendL(app);
            CleanupStack::Pop(app);
            }
        }
    //mark the sm
    aServMime->iUsedByTasks=ETrue;
    //link the sm
    aTask.iSMs.Append(aServMime);
    }

// ---------------------------------------------------------------------------
// This function fills with entries a data structure used by a List Box to display Services & MIMEs or Tasks
// ---------------------------------------------------------------------------
//
void CDefaultAppServMimeApps::GetServicesAndMimesListL(CDesCArray& aServicesAndMimesArray)
    {
    TInt i;
    aServicesAndMimesArray.Reset();
    if(iSimplifiedView)
        {
        for ( i=0 ; i<iTasks.Count() ; i++ )
            {
            TDesC* string = GetMediaTaskStringLC(*iTasks[i], ETrue);
            aServicesAndMimesArray.AppendL (*string);
            CleanupStack::PopAndDestroy(string);
            }
        }
    else
        {
        for ( i=0 ; i<iServMimes.Count() ; i++ )
            {
            TDesC* string = GetServiceAndMimeStringLC(*iServMimes[i], ETrue);
            aServicesAndMimesArray.AppendL (*string);
            CleanupStack::PopAndDestroy(string);
            }
        }
    
    }
    
// ---------------------------------------------------------------------------
// This function fills a Popup-list data structure with applications specific to the selected Service & MIME or Task
// ---------------------------------------------------------------------------
//    
void CDefaultAppServMimeApps::GetApplicationsListL(TInt aIndex, CDesCArray& aApplicationsArray, HBufC*& aTitle)
    {
    // Strings will be of the format "1\tApplication"
    _LIT (KStringAppsDefault, "1\t");
    _LIT (KStringAppsNonDefault, "0\t");
    
    TInt bufLen=20;
    HBufC *string=HBufC::NewLC(bufLen);
    HBufC *newString=NULL;
    TInt i,len;
    RPointerArray<CAppHelper> *apps=NULL;
    
    //get the proper list
    if(iSimplifiedView)
        apps = &(iTasks[aIndex]->iApplications);
    else
        apps = &(iServMimes[aIndex]->iApplications);
    
    for ( i=0 ; i<apps->Count() ; i++ )
        {
        len=(*apps)[i]->iName->Size() + KStringMargin ;
        if(len>bufLen)
            {
            newString=string->ReAllocL(len);
            if(newString != string)
                {
                CleanupStack::Pop(string);//already destroyed
                string=newString;
                CleanupStack::PushL(string);
                }
            bufLen=len;
            newString=NULL;
            }
        //copy the application into the string buffer
        TPtr ptr=string->Des();
        TBool isDefault=EFalse;
        
        if( iSimplifiedView && iTasks[aIndex]->iDefaultApp==i) isDefault=ETrue;
        if( !iSimplifiedView && iServMimes[aIndex]->iDefaultApp==i) isDefault=ETrue;
        
        if( isDefault ) ptr.Copy(KStringAppsDefault);
        else ptr.Copy(KStringAppsNonDefault);
        ptr.Append(*(*apps)[i]->iName);
        
        aApplicationsArray.AppendL (ptr);
        }
    
    CleanupStack::PopAndDestroy(string); //data in ptr is out of scope
        
    //get the title now
    if ( iSimplifiedView )
        aTitle = GetMediaTaskStringLC(*iTasks[aIndex], EFalse);
    else
        aTitle = GetServiceAndMimeStringLC(*iServMimes[aIndex], EFalse);
    CleanupStack::Pop(aTitle);
    }

// ---------------------------------------------------------------------------
// This function sets a new default, for a Service & MIME pair or for a Task.
// The function also updates the list of Services & MIMEs (or Tasks), to display the new default application
// ---------------------------------------------------------------------------
//    
void CDefaultAppServMimeApps::UpdateDefaultL(TInt aServiceAndMimeIndex, TInt aDefaultAppIndex, CDesCArray *aServicesAndMimesArray)
    {
    //check for correct parameters
    if (aServiceAndMimeIndex <0 || aDefaultAppIndex <0) User::Leave(KErrArgument);
    if(iSimplifiedView)
        {
        if( aServiceAndMimeIndex >= iTasks.Count() ||
            aDefaultAppIndex >= iTasks[aServiceAndMimeIndex]->iApplications.Count())
            User::Leave(KErrArgument);
        }
    else
        {
        if( aServiceAndMimeIndex >= iServMimes.Count() ||
            aDefaultAppIndex >= iServMimes[aServiceAndMimeIndex]->iApplications.Count())
            User::Leave(KErrArgument);
        }
    
    //if we are here, parameters are within their range
    TBool doUpdate=ETrue;
    TUid defaultAppUid;
    CServiceRegistry *sr=CServiceRegistry::NewL();
    CleanupStack::PushL(sr);
        
    //update the default in the Service Registry
    if(iSimplifiedView)
        {
        TInt i,j;
        TUid serviceUid;
        //set the default for the generic MIME (and our server application)
        TDataType dt(*iTasks[aServiceAndMimeIndex]->iMimeLabel);
        defaultAppUid=TUid::Uid(iTasks[aServiceAndMimeIndex]->iApplications[aDefaultAppIndex]->iUid);
        if(sr->SetDefault(KOpenServiceUid, dt,defaultAppUid))
            {
            //if we are here, SetDefault returned an error.
            //so we do not update the default...
            doUpdate=EFalse;
            }
        //set the selected default for all the Services & MIME that it supports
        if(doUpdate)
            {
            for(i=0; i<iTasks[aServiceAndMimeIndex]->iSMs.Count(); i++)
                for(j=0; j<iTasks[aServiceAndMimeIndex]->iSMs[i]->iApplications.Count(); j++)
                    if(defaultAppUid.iUid == iTasks[aServiceAndMimeIndex]->iSMs[i]->iApplications[j]->iUid )
                        {
                        //the selected application supports this Service & MIME pair.
                        //make the app default for the pair.
                        dt=*iTasks[aServiceAndMimeIndex]->iSMs[i]->iMime;
                        serviceUid=iTasks[aServiceAndMimeIndex]->iSMs[i]->iServiceUid;
                        sr->SetDefault(serviceUid, dt,defaultAppUid);
                        //update the sm so that it reflects the new default
                        iTasks[aServiceAndMimeIndex]->iSMs[i]->iDefaultAppUid=defaultAppUid;
                        iTasks[aServiceAndMimeIndex]->iSMs[i]->iDefaultApp=j;
                        break; //application found in sm's list, do not need to search for it any more
                        }
            //update the default entries
            iTasks[aServiceAndMimeIndex]->iDefaultApp=aDefaultAppIndex;
            iTasks[aServiceAndMimeIndex]->iDefaultAppUid=defaultAppUid;
            }
            
        }
    else
        {
        TDataType dt(*iServMimes[aServiceAndMimeIndex]->iMime);
        defaultAppUid=TUid::Uid(iServMimes[aServiceAndMimeIndex]->iApplications[aDefaultAppIndex]->iUid);
        if(sr->SetDefault(iServMimes[aServiceAndMimeIndex]->iServiceUid, dt,defaultAppUid))
            {
            //if we are here, SetDefault returned an error.
            //so we do not update the default...
            doUpdate=EFalse;
            }
        //update the default entries
        if(doUpdate)
            {
            iServMimes[aServiceAndMimeIndex]->iDefaultApp=aDefaultAppIndex;
            iServMimes[aServiceAndMimeIndex]->iDefaultAppUid=defaultAppUid;
            }
        }
    CleanupStack::PopAndDestroy(sr);
        
    //check if setting the default failed
    if(!doUpdate)
    {
    	//### if updating the default failed, here would be the place to put an error note to the user
    	return; // or leave
    };
        
    //update the item in the list
    if (aServicesAndMimesArray)
        {
        //get the string
        TDesC* string;
        if(iSimplifiedView)
            string = GetMediaTaskStringLC(*iTasks[aServiceAndMimeIndex], ETrue);
        else
            string = GetServiceAndMimeStringLC(*iServMimes[aServiceAndMimeIndex], ETrue);
        aServicesAndMimesArray->Delete(aServiceAndMimeIndex);
        aServicesAndMimesArray->InsertL(aServiceAndMimeIndex,*string);
        CleanupStack::PopAndDestroy(string);
        }
    }

// ---------------------------------------------------------------------------
// This function creates a string that will become en element of a list box. To create the string, the function
// concatenates several sub-strings.
// ---------------------------------------------------------------------------
//    
HBufC* CDefaultAppServMimeApps::GetServiceAndMimeStringLC(CServiceMime& aServMime, TBool aInsertDefaultApp) const
    {
    HBufC *string=NULL;
    TPtr *ptr=NULL;
    TInt len;
    _LIT(KTab,"\t");
    
    if(aInsertDefaultApp && aServMime.iDefaultApp >= 0) //it may be that we do not have a default ...
        len=aServMime.iApplications[aServMime.iDefaultApp]->iName->Size();
    else
        len=0;
    len+= aServMime.iServiceMime->Size();
    len+= KStringMargin ; 
    
    string=HBufC::NewLC(len);
        
    //build the string, add the tabs before and after
    ptr=new (ELeave) TPtr(string->Des());
    CleanupStack::PushL(ptr);
    
    ptr->Copy(*aServMime.iServiceMime);

    if(aInsertDefaultApp)
        {
        ptr->Insert(0,KTab);
        ptr->Append(KTab);

        //add default app
        if(aServMime.iDefaultApp >= 0) //it may be that we do not have a default ...
            ptr->Append(*aServMime.iApplications[aServMime.iDefaultApp]->iName);
        }

    CleanupStack::PopAndDestroy(ptr);

    return string; //pass ownership, string also on the stack
        
    }

// ---------------------------------------------------------------------------
// This function creates a string that will become en element of a list box. To create the string, the function
// concatenates several sub-strings.
// ---------------------------------------------------------------------------
//    
HBufC* CDefaultAppServMimeApps::GetMediaTaskStringLC(CMediaTask& aMediaTask, TBool aInsertDefaultApp) const
    {
    TPtr *ptr=NULL;
    TInt len = KStringMargin;
    _LIT(KTab,"\t");
    HBufC *taskName=NULL; //not owned, not deleted at the end of function
    
    //compute the string length
    if(aInsertDefaultApp)
        {
        //we insert the task list name
        taskName = aMediaTask.iTaskList;
        //we also insert the length od the default app (if we have it)
        if(aMediaTask.iDefaultApp >= 0)//it may be that we do not have a default ...
            len += aMediaTask.iApplications[aMediaTask.iDefaultApp]->iName->Size();
        }
    else
        {
        //we should insert the task title, if we have it
        taskName = aMediaTask.iTaskTitle;
        //taskName = aMediaTask.iTaskList;
        };
    len += taskName->Size();
    
    //allocate the string
    HBufC *string=HBufC::NewLC(len);
    
    //add the title
    ptr=new (ELeave) TPtr(string->Des());
    CleanupStack::PushL(ptr);
    ptr->Copy(*taskName);

    //add other stuff
    if(aInsertDefaultApp)
        {
        ptr->Insert(0,KTab);
        ptr->Append(KTab);
        //add default app
        if(aMediaTask.iDefaultApp >= 0) //it may be that we do not have a default ...
            ptr->Append(*aMediaTask.iApplications[aMediaTask.iDefaultApp]->iName);
        }
    
    CleanupStack::PopAndDestroy(ptr);

    return string; //pass ownership, string also on the stack
    }

// ---------------------------------------------------------------------------
// This function resets (removes) the defaults associated with a certain task, 
// or it can remove all the defaults
// ---------------------------------------------------------------------------
//  
TInt CDefaultAppServMimeApps::RestoreFactorySettingsL(TInt aCathegory)
    {
    TInt i, j;
    CServiceRegistry *sr=NULL;
    
    if(aCathegory == -1)
        {
        sr=CServiceRegistry::NewL();
        
        if ( iSimplifiedView )
        	{
            for(i=0; i<iTasks.Count(); i++)
        	    {
                for(j=0; j<iTasks[i]->iMimes.Count(); j++)
            	    {
            	    sr->RemoveEntry(TUid::Uid(iTasks[i]->iServicesUids[j]), TDataType(*iTasks[i]->iMimes[j]));
            	    }
                //remove the entry that identifies the default app for the task
                sr->RemoveEntry(KOpenServiceUid, TDataType(*iTasks[i]->iMimeLabel));
        	    }
        	}
        else
        	{
        	//we reset the defaults for all the services & MIME
        	for(i=0; i<iServMimes.Count(); i++)
        	    sr->RemoveEntry(iServMimes[i]->iServiceUid, TDataType(*iServMimes[i]->iMime));
        	//remove the entry that identifies the default app for the task
        	for(i=0; i<iTasks.Count(); i++)
        	    sr->RemoveEntry(KOpenServiceUid, TDataType(*iTasks[i]->iMimeLabel));     	
        	}
        }
    else if(iSimplifiedView && aCathegory >= 0 && aCathegory < iTasks.Count())
        {
        //we are in simplified view, restore defaults for a single task
        sr=CServiceRegistry::NewL();
        for(i=0; i<iTasks[aCathegory]->iMimes.Count(); i++)
                sr->RemoveEntry(TUid::Uid(iTasks[aCathegory]->iServicesUids[i]), TDataType(*iTasks[aCathegory]->iMimes[i]));
            //remove the entry that identifies the default app for the task
            sr->RemoveEntry(KOpenServiceUid, TDataType(*iTasks[aCathegory]->iMimeLabel));
        }
    else if(!iSimplifiedView && aCathegory >= 0 && aCathegory < iServMimes.Count())
        {
        //we are in the advanced view, restore defaults for a single Service & MIME pair
        sr=CServiceRegistry::NewL();
        sr->RemoveEntry(iServMimes[aCathegory]->iServiceUid, TDataType(*iServMimes[aCathegory]->iMime));
        }
    else return -1; //wrong aCathegory range
    
    if(sr) delete sr;
    return 0;
    }
