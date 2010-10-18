/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#ifndef CPAPPLICATION_H
#define CPAPPLICATION_H

#include <hbapplication.h>

class CpApplication : public HbApplication
{
    Q_OBJECT
public:
    CpApplication(int &argc, char *argv[]);
    virtual ~CpApplication();
    void handleAppReady();
    
signals:
    void applicationReady();
};

#endif /* CPAPPLICATION_H */
