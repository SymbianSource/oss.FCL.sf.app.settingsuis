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
#include <hbdataform.h>
#include <cpitemdatahelper.h>
#include <cplogger.h>

CpMainView::CpMainView(QGraphicsItem *parent /*= 0*/)
: CpBaseSettingView(0,parent), mMainModel(0), mItemDataHelper(0)
{
	if (HbDataForm *form = settingForm()) {
        mItemDataHelper = new CpItemDataHelper(form);
		mMainModel = new CpMainModel;
        mMainModel->initialize(*mItemDataHelper);
		form->setModel(mMainModel);
	}	
	setTitle( QObject::tr("Control Panel") );	//should use qtTrId("txt_cp_title_control_panel")
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

// End of File
