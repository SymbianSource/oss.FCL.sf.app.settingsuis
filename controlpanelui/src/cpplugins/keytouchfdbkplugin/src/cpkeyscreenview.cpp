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
#include <hbslider.h>


CpKeyScreenView::CpKeyScreenView(QGraphicsItem *parent) :
    CpBaseSettingView(0,parent),
    mScreenComboButton(0),
    mRotateCheckbox(0),
	mBrightSliderItem(0),
	mCallibItem(0),
	mModel(0)
{   
    HbDataForm *form = qobject_cast<HbDataForm*> ( widget() );
    if (form) {
        // Valid range is:
        // 15 secs, 30 secs, 45 secs, 1 min, 2 mins
        mScreenLockValues.insert(KCpKeyscreenLock15s,hbTrId("txt_cp_setlabel_keys_screen_val_15_seconds"));
        mScreenLockValues.insert(KCpKeyscreenLock30s,hbTrId("txt_cp_setlabel_keys_screen_val_30_seconds"));
        mScreenLockValues.insert(KCpKeyscreenLock45s,hbTrId("txt_cp_setlabel_keys_screen_val_45_seconds"));
        mScreenLockValues.insert(KCpKeyscreenLock60s,hbTrId("txt_cp_setlabel_keys_screen_val_1_minute"));
        mScreenLockValues.insert(KCpKeyscreenLock120s,hbTrId("txt_cp_setlabel_keys_screen_val_2_minutes"));
        
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

       /* if ( mModel->isCallibrationSupported() ) {
            makeCallibrationItem(*model);
        }*/

        form->setModel(model);
    }
}

void CpKeyScreenView::makeScreenItem(HbDataFormModel& model)
{
    mScreenComboButton = new CpSettingFormItemData(HbDataFormModelItem::ComboBoxItem,
            hbTrId("txt_cp_setlabel_keys_screen_locked_after"));    
    
    qobject_cast<HbDataForm*> ( widget() )->addConnection(
            mScreenComboButton,SIGNAL(currentIndexChanged(QString)),
            this,SLOT(screenValueChanged(QString)));
    
    model.appendDataFormItem(mScreenComboButton, model.invisibleRootItem());

    int period = mModel->keyguard();
    
    int selectedIndex(-1);    
    
    QMap<int,QString>::iterator it = mScreenLockValues.find(period);
    if (it == mScreenLockValues.end()) {
        mModel->setKeyguard(KCpKeyscreenLockDefault30s); // Set keyguard and backlight period to default
        selectedIndex = 1;
    }
    else {
        for (it = mScreenLockValues.begin();it != mScreenLockValues.end();++it) {
            selectedIndex++;
            if (it.key() == period) {
                break;
            }
        }
    }
    
    QStringList items = mScreenLockValues.values();
    mScreenComboButton->setContentWidgetData( QString("items"), items );
    mScreenComboButton->setContentWidgetData( QString("currentIndex"), selectedIndex);
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

    QList<QVariant> sliderElements;
    sliderElements << QVariant(HbSlider::IncreaseElement) << QVariant(HbSlider::TrackElement)
                   << QVariant(HbSlider::DecreaseElement);
    mBrightSliderItem->setContentWidgetData("sliderElements",sliderElements);
    mBrightSliderItem->setContentWidgetData( QString("value"), mModel->brightness() );
    mBrightSliderItem->setContentWidgetData( QString("minimum"), 1 );
    mBrightSliderItem->setContentWidgetData( QString("maximum"), 5 );
    QMap< QString, QVariant > iconElements;
    iconElements.insert(QString("IncreaseElement") , QVariant(":/icon/hb_vol_slider_increment.svg"));
    iconElements.insert(QString("DecreaseElement"), QVariant(":/icon/hb_vol_slider_decrement.svg") );
    mBrightSliderItem->setContentWidgetData( QString( "elementIcons" ), iconElements );
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

void CpKeyScreenView::screenValueChanged(const QString &value)
{    
    for (QMap<int,QString>::iterator it = mScreenLockValues.begin();
         it != mScreenLockValues.end();++it) {
        if (it.value() == value) {
            mModel->setKeyguard(it.key());
            break;
        }
    }
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


