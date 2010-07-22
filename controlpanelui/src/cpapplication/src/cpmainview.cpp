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

#include "cpmainview.h"
#include "cpmainmodel.h"
#include <QList>
#include <hbmainwindow.h>
#include <hbdataform.h>
#include <cpitemdatahelper.h>
#include <cplogger.h>

//CpMainView implementation
CpMainView::CpMainView(HbMainWindow *mainWindow/*= 0*/)
: CpBaseSettingView(0,0), 
  mMainModel(0), 
  mItemDataHelper(0), 
  mMainWindow(mainWindow)
{ 
    //delay loading
    connect(mMainWindow,SIGNAL(viewReady()),this,SLOT(initializeMainModel()));
}

CpMainView::~CpMainView()
{
    delete mMainModel;
    delete mItemDataHelper;
}

bool CpMainView::event(QEvent *e)
{
    if (e->type() == QEvent::Show || e->type() == QEvent::ShowToParent) {
        CPPERF_LOG("CpMainView shown.");
    }
    return CpBaseSettingView::event(e);
}

void CpMainView::initializeMainModel()
{
    if (HbDataForm *form = qobject_cast<HbDataForm *>(widget())) {
        if (!mMainModel) {  
            mItemDataHelper = new CpItemDataHelper(form);
            mMainModel = new CpMainModel;
            mMainModel->initialize(*mItemDataHelper);
            form->setModel(mMainModel);
            
            connect(form,SIGNAL(activated(QModelIndex)),this,SLOT(onDataFormItemActivated(QModelIndex)));
        }
    }   
}
// End of File
