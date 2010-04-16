/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
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



#include <QGraphicsLinearLayout>
#include <QModelIndex>

#include <hbview>
#include <hblabel.h>
#include <hblistview.h>

#include "cpthemelistview.h"

/*!
    \class CpThemeListView
    \brief CpThemeListView displays a heading (Select theme) and a list of themes with
    corresponding icons.

    Note: This class is a subclass of CpBaseSettingView for compatibility with Control Panel
          framework.  However, it does not use HbDataForm as its widget and as its parent does, so
          it uses SetWidget to set the listview to its widget instead of an HbDataForm.
 */

/*!
    constructor.  Creates the heading label and the list and adds it to layout.
*/
CpThemeListView::CpThemeListView(QGraphicsItem *parent) : CpBaseSettingView(0, parent),
    mThemeList(new HbListView(this))
{

    //Create a layout with a heading "Select theme" at top and the list below it.
    HbWidget* contentWidget = new HbWidget(this);
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    	
   
    
    //setup the heading.
    HbLabel* label = new HbLabel(hbTrId("txt_cp_title_select_theme"), contentWidget);
    label->setFontSpec(HbFontSpec(HbFontSpec::Primary));
    layout->addItem(label);

    connect(mThemeList, SIGNAL(activated(const QModelIndex&)),
            this, SIGNAL(newThemeSelected(const QModelIndex&)));
    
    //add the list to layout.
    layout->addItem(mThemeList);

    contentWidget->setLayout(layout);
   
    setWidget(contentWidget);
}

/*!
    destructor.
*/
CpThemeListView::~CpThemeListView()
{
}

/*!
    returns the listview instance (list of themes).
*/
HbListView* CpThemeListView::themeList() const
{
    return mThemeList;
}

/*!
    Sets the model of its listView.
*/
void CpThemeListView::setModel(QAbstractItemModel* model)
{
    mThemeList->setModel(model);
}

/*!
    sets the widget. Reimplementation from HbView. 
*/
void CpThemeListView::setWidget(QGraphicsWidget *widget)
{
    HbView::setWidget(widget);
}

/*!
    emits aboutToClose() signal.
*/
void CpThemeListView::closeView()
{
    emit aboutToClose();
}


