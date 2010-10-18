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

#include "secondpluginviewforcpcategorymodel.h"

#include <hbdataform.h>
#include <qstringlist>
#include <QDebug>
#include <hbdataformmodel.h>
#include <cpsettingformitemdata.h>
#include <hbmessagebox.h>

SecondPluginViewForCategoryModel::SecondPluginViewForCategoryModel(QGraphicsItem *parent) 
    : CpBaseSettingView(0,parent),
    mGroupItem(0),
    mSliderItem(0)
{
    HbDataForm *form = qobject_cast<HbDataForm*>(widget());
    if (form) {
        HbDataFormModel *model = new HbDataFormModel;
        
        form->setHeading(tr("View from test plugin"));
        mGroupItem = new HbDataFormModelItem(HbDataFormModelItem::GroupItem, QString("Group"));
        
        model->appendDataFormItem(mGroupItem);
        
        mSliderItem = new CpSettingFormItemData(HbDataFormModelItem::SliderItem,
                QString("New Slider"));
        mSliderItem->setContentWidgetData("iconEnabled","FALSE");
        form->addConnection(mSliderItem,SIGNAL(valueChanged(int)),this,SLOT(sliderValueChanged(int)));
        mGroupItem->appendChild(mSliderItem);      
        
        form->setModel(model);
    }
}
SecondPluginViewForCategoryModel::~SecondPluginViewForCategoryModel()
{
}

void SecondPluginViewForCategoryModel::testClose()
{
    close();
}
void SecondPluginViewForCategoryModel::sliderValueChanged(int value)
{
    //Disable the slider alert.
	HbMessageBox::information(QString("slider value changed to:%1").arg(value));
}
