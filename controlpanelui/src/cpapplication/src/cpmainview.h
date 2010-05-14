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
#ifndef CPMAINVIEW_H
#define CPMAINVIEW_H

#include <cpbasesettingview.h>

class CpMainModel;
class HbAbstractViewItem;
class HbDataFormModelItem;
class CpItemDataHelper;
class HbMainWindow;

class CpMainView : public CpBaseSettingView
{
	Q_OBJECT
public:
    explicit CpMainView(HbMainWindow *mainWindow = 0);
    virtual ~CpMainView();
    bool event(QEvent *e);
private slots:
    void initializeMainModel();
private:
    Q_DISABLE_COPY(CpMainView)
private:
	CpMainModel *mMainModel;
    CpItemDataHelper *mItemDataHelper;
    HbMainWindow *mMainWindow;
};

#endif
//End of file

