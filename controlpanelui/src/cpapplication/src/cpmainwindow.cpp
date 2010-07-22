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

#include "cpmainwindow.h"
#include "cpmainview.h"
#include <cpplugininterface.h>
#include <cpcategorysettingformitemdata.h>
#include <hbapplication.h>
#include <cpevent.h>

//CpMainWindow implementation
CpMainWindow::CpMainWindow(QWidget *parent /*= 0*/, Hb::WindowFlags windowFlags /*= Hb::WindowFlagNone*/)
: HbMainWindow(parent,windowFlags)
{
}

CpMainWindow::~CpMainWindow()
{
    
}

bool CpMainWindow::event(QEvent *e)
{
    if (e->type() == (QEvent::Type)(CpCreatePluginItemDataEvent::CreatePluginItemData)) {
        CpCreatePluginItemDataEvent *event = static_cast<CpCreatePluginItemDataEvent*>(e);
        ::createCpPluginItemData(event);
        e->accept();
    }
    return HbMainWindow::event(e);
}

//End of File


