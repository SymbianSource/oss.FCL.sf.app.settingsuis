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

#include "cpkeyscreenview.h"
#include "cpkeyscreenmodel.h"
#include "cpkeyscreenconstants.h"
#include <hbdataform.h>
#include <QStringList>
#include <QDebug>
#include <QtCore/qobject.h>
#include <hbdataformmodel.h>
#include <cpsettingformitemdata.h>
#include <hbmessagebox.h>


CpKeyScreenView::CpKeyScreenView(QGraphicsItem *parent) :
    CpBaseSettingView(0,parent),
    mScreenRadioButton(0),
    mRotateCheckbox(0),
	mBrightSliderItem(0),
	mCallibItem(0),
	mModel(0)
{
    HbDataForm *form = qobject_cast<HbDataForm*> ( widget() );
    if (form) {
        form->setHeading(hbTrId("txt_cp_subhead_keys_screen"));
        mModel = new CpKeyScreenModel();
        HbDataFormModel *model = new HbDataFormModel;

        if ( mModel->isKeyguardSupported() ) {
            makeScreenItem(*model);
        }

        if ( mModel->isRotateSupported() ) {
            makeRotateItem(*model);
        }

        makeBrightnessItem(*model);

        if ( mModel->isCallibrationSupported() ) {
            makeCallibrationItem(*model);
        }

        form->setModel(model);
    }
}

void CpKeyScreenView::makeScreenItem(HbDataFormModel& model)
{
    mScreenRadioButton = new CpSettingFormItemData(HbDataFormModelItem::RadioButtonListItem,
            hbTrId("txt_cp_setlabel_keys_screen_locked_after"));
    qobject_cast<HbDataForm*> ( widget() )->addConnection(mScreenRadioButton,SIGNAL(itemSelected(int)),this,SLOT(screenValueChanged(int)));
    model.appendDataFormItem(mScreenRadioButton, model.invisibleRootItem());

    // Valid range is:
    // 15 secs, 30 secs, 45 secs, 1 min, 2 mins
    QStringList sList;
    sList<< hbTrId("txt_cp_setlabel_keys_screen_val_15_seconds")<< hbTrId("txt_cp_setlabel_keys_screen_val_30_seconds")
         << hbTrId("txt_cp_setlabel_keys_screen_val_45_seconds")<< hbTrId("txt_cp_setlabel_keys_screen_val_1_minute")
         << hbTrId("txt_cp_setlabel_keys_screen_val_2_minutes");
    int period = mModel->keyguard();
    int selectedIndex = period/KCpKeyscreenTimeCoeff - 1;
    if ( KCpKeyscreenLock120s == period ){ // 2 minutes
        selectedIndex = EKeyScreenLockItem5;
    }
    if ( (KCpKeyscreenLock15s != period) && (KCpKeyscreenLock30s != period) && 
            (KCpKeyscreenLock45s != period) && (KCpKeyscreenLock60s != period) && 
            (KCpKeyscreenLock120s != period) ){
        mModel->setKeyguard(KCpKeyscreenLockDefault30s);  // Set keyguard and backlight period to default
        selectedIndex = EKeyScreenLockItem2;
    }
    mScreenRadioButton->setContentWidgetData( QString("items"), sList );
    mScreenRadioButton->setContentWidgetData( QString("selected"), selectedIndex);
}

void CpKeyScreenView::makeRotateItem(HbDataFormModel& model)
{
    mRotateCheckbox = new CpSettingFormItemData(HbDataFormModelItem::CheckBoxItem, QString());
    qobject_cast<HbDataForm*> ( widget() )->addConnection(mRotateCheckbox,SIGNAL(stateChanged(int)),this,SLOT(rotateValueChanged(int)));
    model.appendDataFormItem(mRotateCheckbox, model.invisibleRootItem());

    mRotateCheckbox->setContentWidgetData( QString("text"), QVariant(hbTrId("txt_cp_list_autorotate_display")) );
    mRotateCheckbox->setData(HbDataFormModelItem::DescriptionRole, QString( hbTrId("txt_cp_info_rotate_the_display_content_automatical") ));
    Qt::CheckState state;
    if ( mModel->rotate() ){
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    mRotateCheckbox->setContentWidgetData( QString("checkState"), state );
}

void CpKeyScreenView::makeBrightnessItem(HbDataFormModel& model)
{
    mBrightSliderItem = new CpSettingFormItemData(HbDataFormModelItem::SliderItem, hbTrId("txt_cp_setlabel_brightness"));
    qobject_cast<HbDataForm*> ( widget() )->addConnection(mBrightSliderItem,SIGNAL(valueChanged(int)),this,SLOT(brightValueChanged(int)));
    model.appendDataFormItem(mBrightSliderItem, model.invisibleRootItem());

    mBrightSliderItem->setContentWidgetData( QString("value"), mModel->brightness() );
    mBrightSliderItem->setContentWidgetData( QString("minimum"), 1 );
    mBrightSliderItem->setContentWidgetData( QString("maximum"), 5 );
}

void CpKeyScreenView::makeCallibrationItem(HbDataFormModel& model)
{
    mCallibItem = new CpSettingFormItemData(HbDataFormModelItem::ToggleValueItem,
                        QString());
    qobject_cast<HbDataForm*> ( widget() )->addConnection(mCallibItem,SIGNAL(pressed()),this,SLOT(launchCallib()));
    model.appendDataFormItem(mCallibItem, model.invisibleRootItem());
    mCallibItem->setContentWidgetData( QString("text"), hbTrId("txt_cp_button_touch_screen_calibration"));
}

CpKeyScreenView::~CpKeyScreenView()
{
    delete mModel;
	mModel = 0;
}

void CpKeyScreenView::screenValueChanged(int index)
{
    int period = (index + 1) * KCpKeyscreenTimeCoeff;
    if (EKeyScreenLockItem5 == index){
        period = KCpKeyscreenLock120s;
    }
    mModel->setKeyguard(period);
}

void CpKeyScreenView::rotateValueChanged(int value)
{
    mModel->setRotate( value );
}

void CpKeyScreenView::brightValueChanged(int value)
{
	mModel->setBrightness(value);
}

void CpKeyScreenView::launchCallib()
{
}


