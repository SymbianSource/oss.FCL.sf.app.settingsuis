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



#ifndef C_DEFAULTAPPSERVMIMEAPPS_H
#define C_DEFAULTAPPSERVMIMEAPPS_H

class CApaMaskedBitmap;
class CDefaultAppServMimeApps;
class CServicesDB;
class RApaLsSession;

/**  The UID of the General Settings Application */
const TUid KUidGS = { 0x100058EC }; //we define this here because we use it in several places

/**
 *  Helper class: stores an Application name, its UID and some other info (how many 
 *  service&MIMEs pair it supports, is it platform application or not)
 *  This helper class is used to sort the list of Applications that support a Service&MIME pair
 *
 *  @since S60 v5.0
 */
class CAppHelper : public CBase
    {
public:
    /**  flags used with the app helper. */
    enum 
        {
        EFlagPlatformApp  =0x0001,
        EFlagNameNotOwned =0x0002
        };
public:

    /**
     * Destructor.
     */
    ~CAppHelper();
    
    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CAppHelper* NewLC(const TDesC& aName, const TInt aUid);
    
    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CAppHelper* NewLC(const CAppHelper& aApp);
    
    /**
     * This function is used for ordering an array of CAppHelper objects
     *
     * @since S60 v5.0
     * @param a1 CAppHelper object
     * @param a2 CAppHelper object
     */
    static TInt OrderApplications(const CAppHelper& a1, const CAppHelper& a2);
private:

    /**
     * C++ constructor
     */
    CAppHelper(const TInt aUid, const TInt aScore, const TInt aFlags=0);
public:
    
    /**
     * the App name
     * Owned or Not Owned, depending on flags (iFlag&EFlagNameNotOwned).
     */
    HBufC* iName;
    
    /**
     * the UID of the application
     */
    TInt iUid;
    
    /**
     * the application score (how many Services & MIMEs it supports)
     */
    TInt iScore;
    
    /**
     * Object flags
     */
    TInt iFlags;
    };
    
/**
 *  Helper class: stores a Service & Mime Pair
 *
 *  This class stores data specific to a Service & MIME pair:
 *  -the Service + MIME string
 *  -the Service Uid
 *  -the MIME
 *  -a list of applications (CAppHelper) that support this Service & MIME pair
 *  -the list index and Uid of the default application for this Service & MIME pair
 *
 *  @since S60 v5.0
 */
class CServiceMime : public CBase
    {
    friend class CDefaultAppServMimeApps;
public:

    /**
     * C++ constructor
     */
    CServiceMime();
    
    /**
     * Destructor.
     */
    ~CServiceMime();
    
    /**
     * This function is used for ordering an array of CServiceMime objects
     *
     * @since S60 v5.0
     * @param a1 CServiceMime object
     * @param a2 CServiceMime object
     */
    static TInt OrderServiceMimes(const CServiceMime& a1, const CServiceMime& a2);
    
private:
    /**
     * String that contains the localized Service + MIME name
     * Owned.
     */
    HBufC *iServiceMime;
    
    /**
     * The Uid of the service
     */
    TUid  iServiceUid;
    
    /**
     * the MIME string
     * Owned.
     */
    HBufC8 *iMime;
    
    /**
     * list of applications that support this Service & MIME pair
     * Owned (members of the array).
     */
    RPointerArray<CAppHelper> iApplications;
    
    /**
     * the UID of the default application for this Service & MIME
     */
    TUid iDefaultAppUid;
    
    /**
     * the list index for the default application
     * this has the following special values:
     *  -1 : the default is (yet) unknown
     *  -2 : there is a single app in the list, its priority is System 
     *       (this means: do not add other apps unless they have system priority)
     *  -3 : there are more than one applications in the list, ALL have the System priority 
     *       (this also means: do not add other apps unless they have system priority)
     */
    TInt iDefaultApp;
    
    /**
     * indicates if the Service & MIME pair instance is used by a task or not 
     */
    TBool iUsedByTasks;
    };
    
    
/**
 *  Helper class: stores a Task
 *
 *  This class stores data specific to a Task:
 *  -the task caption/title (list layout and title layout)
 *  -the lists of Service (UIDs) & MIMEs associated with this task
 *  -the MIME label 
 *  -a list of Services & MIMEs objects (and their associated application list) that correspond to the list 
 *   of Services & MIMEs associated with the task
 *  -a list of applications (CAppHelper) that are associated with this task
 *  -the UID and list index of the default application for the task, in the list(s) above.
 *
 *  How this works:
 *  The Service & MIME pairs associated with the task are those pairs for which changing the default application for the
 *  task will have an impact: the new default becomes the default application for all these pairs, if applicable.
 *  The list of candidate default applications is build from the Service & MIME pairs associated with the task: any
 *  application that can handle at least one pair, is added to the list. The score corresponds to the number of pairs an 
 *  application is able to handle (the better the score, the more suitable the application is for the task).
 *  When a new application is made default, it is also made default for Open + the MIME label. This way we can retrieve
 *  the default application for the task, to display it to the user.
 *  The list of Services & MIME objects are instances of CServiceMime that correspond to the list of Services & MIMEs that
 *  are associated with the task (the associated Services & MIMEs is a "theoretical" list, while the CServiceMime list
 *  is a list of Services & MIMEs that were found in the system. The Services & MIMEs in this object list is a subset of
 *  the associated Services & MIMEs. The reunion of all Applications lists associated with each CServiceMime object in the
 *  list is going to be same as the list of Applications that are associated with the Task.
 *  When a new default application is selected for the task, this new default is saved in 2 ways:
 *  -the new default application is made default for the Open service and the MIME label. This way, we can retrieve next
 *   time the default application for the task. There is no danger to launch this default for opening the MIME label, since
 *   no application is supposed to support the MIME label.
 *  -the new default application is made default for all the Services & MIMEs associated with the task: for each instance
 *   of CServiceMime in the list, the default is looked in the application list of the object: if it is found (which means 
 *   that the new default application supports that Service & MIME), then the new task default is made default for the
 *   current Service & MIME too.
 *
 *  @since S60 v5.0
 */
class CMediaTask : public CBase
    {
    friend class CDefaultAppServMimeApps;
public:

    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CMediaTask* NewLC( TResourceReader& aReader );
    
    /**
     * Destructor.
     */
    ~CMediaTask();
private:
    
    /**
     * C++ constructor
     */
    CMediaTask();
    
    /**
     * Symbian constructor
     */
    void ConstructL( TResourceReader& aReader );
private:
    /**
     * Task caption, list layout
     * Owned.
     */
    HBufC *iTaskList;

    /**
     * Task caption, title layout
     * Owned.
     */
    HBufC *iTaskTitle;
    
    /**
     * List of MIMEs associated with the task
     * Owned (members of the array).
     */
    RPointerArray<HBufC8> iMimes;
    
    /**
     * List of services associated with the task (1 to 1 correspondence with iMimes, they form pairs together) 
     */
    RArray<TInt> iServicesUids;
    
    /**
     * the MIME label
     * Owned.
     */
    HBufC8 *iMimeLabel;
    
    /**
     * the list of real/discovered CServiceMime associated with the task
     * Not Owned (members of the array).
     */
    RPointerArray<CServiceMime> iSMs; //not owned
    
    /**
     * Application captions for the applications associated with the task
     * Owned (members of the array).
     * Application names NOT owned (names are owned by CServiceMime objects)
     */
    RPointerArray<CAppHelper> iApplications;
    
    /**
     * The UID of the default application for the task
     */
    TUid iDefaultAppUid; //the Uid for the default application
    
    /**
     * the index (in the application list) of the default application for the task
     */
    TInt iDefaultApp; //index
    
    };    


/**
 *  Engine class for the application
 *
 *  This class stores a list of Services and MIMEs and it may store a list of Tasks. All data handling operations
 *  are implemented in this class.
 *  There are 2 types of clients for the Default App Server:
 *   1. General Settings Application (using a plugin). For this type of client we have to display first a list of tasks,
 *      and if the user switches to the advanced view, we have to display a list of all Services & MIMEs in the system
 *      for which the user can change the default application.
 *   2. Normal applications may also be clients for the Default APp Server. For normal applications, only a list of Services
 *      and MIMEs for which the cliet application is a default candidate is displayed. The task list is not constructed.
 *  When the class is instantiated, it looks in the system (AppArc) and it builds all the data structures it needs. After
 *  that, it can populate various lists for displaying them to the user, and can also change the default application, as 
 *  instructed by the user (AppUi calls the functions).
 *
 *  @since S60 v5.0
 */
class CDefaultAppServMimeApps : public CBase
    {
public:
    /**  flags used with the Set Default service. They influence what data is stored in the data structures. */
    enum 
        {
        EFlagNoObserver=1,
        //R&D values
        EFlagShowAllServicesAndMimes    = 0x00010000,
        EFlagGsClient                   = 0x00020000,
        };
    
    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CDefaultAppServMimeApps* NewL(const TUid& aAppUid, TInt aServiceFlags);
    
    /**
     * Symbian OS two-phased constructor
     * @return
     */
    static CDefaultAppServMimeApps* NewLC(const TUid& aAppUid, TInt aServiceFlags);

    /**
     * Destructor.
     */
    virtual ~CDefaultAppServMimeApps();

    /**
     * This function fills with entries a data structure used by a List Box to display Services & MIMEs or Tasks
     *
     * @since S60 v5.0
     * @param aServicesAndMimes the container for the list entries
     */
    void GetServicesAndMimesListL(CDesCArray& aServicesAndMimesArray);
    
    /**
     * This function fills a Popup-list data structure with applications specific to the selected Service & MIME or Task
     *
     * @since S60 v5.0
     * @param aIndex the index of the selected Service & MIME pair or Task
     * @param aApplicationsArray the container for the list entries
     * @param aTitle a container for the title of the list (also filled by the function)
     */
    void GetApplicationsListL(TInt aIndex, CDesCArray& aApplicationsArray, HBufC*& aTitle);
    
    /**
     * This function sets a new default, for a Service & MIME pair or for a Task.
     * The function also updates the list of Services & MIMEs (or Tasks), to display the new default application
     *
     * @since S60 v5.0
     * @param aServiceAndMimeIndex the index of the selected Service or Mime (or Task)
     * @param aDefaultAppIndex the index of the new default application
     * @param aServicesAndMimesArray the container for the list entries (to be updated)
     */
    void UpdateDefaultL(TInt aServiceAndMimeIndex, TInt aDefaultAppIndex, CDesCArray *aServicesAndMimesArray);
    
    /**
     * This function resets (removes) the defaults associated with a certain task, 
     * or it can remove all the defaults
     *
     * @since S60 v5.0
     * @param aCathegory specifies the task index for which the function should reset 
     *                   the default, or -1 if all defaults should be reset
     * @return 0 or error code (KErrArgument -> aCathegory has an invalid value)
     */
    TInt RestoreFactorySettingsL(TInt aCathegory);
    
private:  //construction

    /**
     * C++ constructor
     */
    CDefaultAppServMimeApps();

    /**
     * This function builds the info database behind this class.
     * For all available services in the resources it creats the corresponding
     * Services & MIMEs (or Tasks) instances.
     *
     * @since S60 v5.0
     * @param aAppUid the Uid of the client application
     * @param aServiceFlags different service flags requested by the client
     */
    void ConstructL(TUid aAppUid, TInt aServiceFlags);
    
    /**
     * This function builds part the info database behind this class.
     * It creates the Services & MIMEs (or Tasks) instances for the given service
     *
     * @since S60 v5.0
     * @param aServiceUid the Uid of the current service
     * @param aServiceIndex the index of the current service
     * @param aServiceFlags different service flags requested by the client
     * @param aServicesDb pointer to an instance that holds localized service names
     */
    void AddMIMEsForServiceL(TUid aServiceUid, TInt aServiceIndex, TInt aServiceFlags, CServicesDB* aServicesDb);
    
    /**
     * This function sorts the Services & MIMEs and their applications. 
     * It also sorts the Serivces & MIMEs associated with tasks
     *
     * @since S60 v5.0
     */
    void BeautifyAndSortServMimeApps(void);

    /**
     * This function reads from the resource file the list of tasks (and associated data). 
     * This function is used during construction
     *
     * @since S60 v5.0
     * @param aResourceId the resource id corresponding to the tasks resource
     */
    void GetTaskListL( TInt aResourceId );
    
    /**
     * This function adds a new Application to a list of an Service & Mime object.
     *
     * @since S60 v5.0
     * @param aServMime the Service & Mime object
     * @param aAppUid the uid of the inserted application
     * @param aPrio the priority of the inserted application for the Service & MIME of the host object
     * @param aLs pointer to a RApaLsSession object (so we do not need to create a new connection)
     */
    void InsertApplicationL(CServiceMime& aServMime, const TUid& aAppUid, const TDataTypePriority& aPrio, const RApaLsSession *aLs);
    
    /**
     * This function takes a task and a Service & MIME object. It checks all the applications that support the given
     * Service & MIME object (from its list). If an application is not in the task's application list, this function adds 
     * it there, with a score of 1. If the application is already in the list, then its score is incremented.
     * The Service & MIME object is also marked as being used by a task.
     *
     * @since S60 v5.0
     * @param aTask the selected task
     * @param aServMime the selected Service & MIME object
     */
    void PopulateTaskWithApplicationsL(CMediaTask& aTask, CServiceMime* aServMime);
    
    /**
     * This function creates a string that will become en element of a list box. To create the string, the function
     * concatenates several sub-strings.
     *
     * @since S60 v5.0
     * @param aServMime the selected Service & MIME object
     * @param aInsertDefaultApp if TRUE, creates an object that also contains the name of the default application for the
     *        Service & MIME pair
     * @return the created string
     */
    HBufC* GetServiceAndMimeStringLC(CServiceMime& aServMime, TBool aInsertDefaultApp) const;
    
    /**
     * This function creates a string that will become en element of a list box. To create the string, the function
     * concatenates several sub-strings.
     *
     * @since S60 v5.0
     * @param aMediaTask the selected Task object
     * @param aInsertDefaultApp if TRUE, creates an object that also contains the name of the default application for the
     *        Service & MIME pair
     * @return the created string
     */
    HBufC* GetMediaTaskStringLC(CMediaTask& aMediaTask, TBool aInsertDefaultApp) const;

private:  //data
    
    /**
     * The list of Services & MIMEs
     * Owned (members of the array).
     */
    RPointerArray<CServiceMime> iServMimes;
    
    /**
     * The list of Tasks
     * Owned (members of the array).
     */
    RPointerArray<CMediaTask> iTasks;
    
    /**
     * The list of Services & MIMEs that are used by tasks (but otherwise would have been deleted, since they are not 
     * displayed to the user, because there is no more than one default candidate for each Service & MIME in this list)
     * Owned (members of the array).
     */
    RPointerArray<CServiceMime> iTaskServMimes; //ServMimes with a single application, used in tasks
    
public:

    /**
     * the list of elements, ar required by the dialog
     */
    CDesCArraySeg iList;

    /**
     * the UID of the client application
     */
    TUid iAppUid;
    
    /**
     * TRUE if the current view is the GS simplified view (if this var is TRUE, it also implies that GS is our client)
     */
    TBool iSimplifiedView;
    
    /**
     * if TRUE, the client application does not observe our exit. In this case do not display "Exit" in the menu, since 
     * the client application will not exit when the server exits.
     */
    TBool iFlagNoObserver;
    };


#endif // C_DEFAULTAPPSRVMIMEVIEW_H

// end of file