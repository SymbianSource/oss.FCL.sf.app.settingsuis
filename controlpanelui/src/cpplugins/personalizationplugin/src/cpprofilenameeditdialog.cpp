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

#include "cpprofilenameeditdialog.h"

#include <hblineedit.h>
#include <hblabel.h>
#include <hbaction.h>

#include <QGraphicsLinearLayout>
#include <QScopedPointer>

CpProfileNameEditDialog::CpProfileNameEditDialog( QGraphicsItem *parent )
    :HbDialog( parent )
{
    init();
}

CpProfileNameEditDialog::~CpProfileNameEditDialog()
{
}

void CpProfileNameEditDialog::init()
{
    setDismissPolicy( HbPopup::NoDismiss );
    setHeadingWidget( new HbLabel( hbTrId( "txt_cp_title_edit_name" ), this ));
    
    QGraphicsLinearLayout *vLayout = new QGraphicsLinearLayout();
    vLayout->setOrientation( Qt::Vertical );
    HbWidget *contentWidget = new HbWidget( this );
    mTextEdit = new HbLineEdit( this );
    mTextEdit->setFontSpec( HbFontSpec( HbFontSpec::Primary ) );
    vLayout->addItem( mTextEdit );
    contentWidget->setLayout( vLayout );
    setContentWidget( contentWidget );
    
    connect( mTextEdit, SIGNAL( contentsChanged() ), this, SLOT( checkPrimaryAction() ) );
    
    addAction( new HbAction( hbTrId( "txt_common_button_ok" ), this ));
    addAction( new HbAction( hbTrId( "txt_common_button_cancel" ), this ) );
    
    setTimeout( NoTimeout );
}

void CpProfileNameEditDialog::setLineEditText( const QString &text )
{
    mTextEdit->setText( text );
    mTextEdit->setSelection( 0, text.length() );
}

QString CpProfileNameEditDialog::getLineEditText()
{
    QString text = mTextEdit->text();
    return text;
}

bool CpProfileNameEditDialog::launchProfileNameEditDialog( QString &profileName )
{
    CpProfileNameEditDialog *profileEditNameDialog = new CpProfileNameEditDialog();
    
    profileEditNameDialog->setLineEditText( profileName );
    profileEditNameDialog->checkPrimaryAction();
    profileEditNameDialog->show();
    return false;
}

void CpProfileNameEditDialog::checkPrimaryAction()
{
    HbAction *const primaryAction = qobject_cast<HbAction *>
                                                (actions().at(0));
    if (primaryAction) {
        if ( !mTextEdit->text().isEmpty() ) {
            primaryAction->setEnabled(true);        
        } else {        
            primaryAction->setEnabled(false);        
        }
    }
}
