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

#include <QString>
#include <QGraphicsPixmapItem>
#include <QGraphicsLinearLayout>

#include <hbaction>
#include <hbtoolbar>
#include <hbicon.h>
#include <hbaction.h>
#include <hblabel.h>
#include <hbiconitem.h>

#include "cpthemepreview.h"

/*!
    \class CpThemePreview
    \brief CpThemePreview shows a preview of a selected theme with a heading displaying the name of the theme as well as
           a toolbar with Select and Cancel buttons.  This view is used for the user to either select the theme and apply
           the theme change or press Cancel and go back to theme list view.
*/

/*!
    constructor.
*/
CpThemePreview::CpThemePreview(const CpThemeChanger::ThemeInfo& theme, QGraphicsItem *parent) :
     HbView(parent), 
     mTheme(theme),
     mSoftKeyBackAction(0)
{
    
    //Create the layout and add heading and and preview icon to the layout.
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);

    
    //setup the heading.
    //TODO: translation of string  hbTrId("txt_cp_title_preview_1")
    
    QString themeHeading = tr("Preview: ") + mTheme.name;
    HbLabel* label = new HbLabel(themeHeading, this);
    label->setFontSpec(HbFontSpec(HbFontSpec::Primary));

    layout->addItem(label);
    
    //Create the toolbar and "Select" and "Cancel" actions.
    HbToolBar* mToolBar = new HbToolBar(this);

    HbAction* selectAction = new HbAction(tr("Select"));
    
    //Add Action to the toolbar and show toolbar
    mToolBar->addAction( selectAction );

    HbAction* cancelAction = new HbAction(tr("Cancel"));
    mToolBar->addAction( cancelAction );

    QObject::connect( selectAction, SIGNAL(triggered()), 
                      this, SLOT(themeSelected()));

    QObject::connect( cancelAction, SIGNAL(triggered()), 
                      this, SIGNAL(aboutToClose()));

    //layout->addItem(&HbIconItem(mTheme.icon, this ));
    HbIconItem* layoutItem = new HbIconItem(mTheme.icon, this);
    layout->addItem(layoutItem);
    setToolBar(mToolBar);
    setLayout(layout);

    //Setup the Back button action and set softkey. Back button 
    //takes the user to the theme list view.
    mSoftKeyBackAction = new HbAction(Hb::BackAction, this);
    QObject::connect(mSoftKeyBackAction, SIGNAL(triggered()), 
            this, SIGNAL(aboutToClose()) );

    this->setNavigationAction(mSoftKeyBackAction);
}

/*!
    destructor.
*/
CpThemePreview::~CpThemePreview()
{
}

/*!
  sets the theme to \a theme.
*/
void CpThemePreview::setThemeInfo(const CpThemeChanger::ThemeInfo& theme)
{
    mTheme = theme;
}

/*!
    returns the themeName.
*/
const QString& CpThemePreview::themeName() const
{
    return mTheme.name;
}

/*!
    returns the repersentative themeIcon of the current theme.
*/
const HbIcon& CpThemePreview::themeIcon() const
{
    return mTheme.icon;
}

/*!
    Slot to handle when the user selects a theme.  
*/
void CpThemePreview::themeSelected()
{
    emit applyTheme(mTheme.name);
}

