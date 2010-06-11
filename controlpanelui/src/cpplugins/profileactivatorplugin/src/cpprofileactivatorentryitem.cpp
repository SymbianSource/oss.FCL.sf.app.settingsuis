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
#include "cpprofileactivatorentryitem.h"

#include "cpprofileactivatordialog.h"
#include <cpitemdatahelper.h>
#include <cpprofilemodel.h>
#include <QScopedPointer>
CpProfileActivatorEntryItem::CpProfileActivatorEntryItem(CpItemDataHelper &itemDataHelper,
                const QString &text,
                const QString &description,
                const HbIcon &icon,
                const HbDataFormModelItem *parent)
                :CpSettingFormEntryItemData(itemDataHelper,text,description,
                                            icon,parent)
{
    mProfileModel = new CpProfileModel();
    int currentId = mProfileModel->activeProfileId();
    QString currentName = mProfileModel->profileName(currentId);
    this->setDescription(currentName);
}
CpProfileActivatorEntryItem::~CpProfileActivatorEntryItem()
{
    delete mProfileModel;
}
void CpProfileActivatorEntryItem::onLaunchView()
{
    CpProfileActivatorDialog *dialog = 
        new CpProfileActivatorDialog(this, *mProfileModel);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
   // dialog->open(this, SLOT(ultimateDialogSlot(HbAction*)));
    dialog->show();
    
}
/*void CpPersonalizationEntryItemData::handleOk(const QVariant &result)
{
    if (!result.canConvert<QString>())
    {
        setDescription( "Corrupt result" );
    }
    else
    {
        setDescription( result.value<QString>() );
    }
}
void CpPersonalizationEntryItemData::handleError(int errorCode, const QString& errorMessage)
{
    //
    Q_UNUSED(errorCode);
    Q_UNUSED(errorMessage);
    setDescription("Error");
}*/

CpBaseSettingView *CpProfileActivatorEntryItem::createSettingView() const
{
        return 0;
}