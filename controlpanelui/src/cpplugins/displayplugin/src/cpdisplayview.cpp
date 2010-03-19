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

#include "cpdisplayview.h"
#include "cpdisplaymodel.h"
#include <hbdataform.h>
#include <qstringlist>
#include <QDebug>
#include <hbdataformmodel.h>
#include <cpsettingformitemdata.h>
#include <hbmessagebox.h>


CpDisplayView::CpDisplayView(QGraphicsItem *parent) :
    CpBaseSettingView(0,parent),
    mScreenRadioButton(0),
    mDisplayRadioButton(0),
	mBrightSliderItem(0),
	mModel(0)
{
    HbDataForm *form = settingForm();
    if (form) {
        form->setHeading(tr("Display"));
        mModel = new CpDisplayModel();
        HbDataFormModel *model = new HbDataFormModel;

        bool screen = false;
		bool backlight = false;
		bool brightness = false;
        if ( mModel->isKeyguardSupported() ) {
            makeScreenItem(model);
            screen = true;
        }

        if ( mModel->isBacklightSupported() ) {
            makeBacklightItem(model);
            backlight = true;
        }

        if ( mModel->isBrightnessSupported() ) {
            makeBrightnessItem(model);
            brightness = true;
        }

        if ( screen || backlight || brightness ){
            mIsHidden = false;
        } else {
            mIsHidden = true;
        }

		settingForm()->setModel(model);
    }
}

bool CpDisplayView::isHidden()
{
    return mIsHidden;
}

void CpDisplayView::makeScreenItem(HbDataFormModel* model)
{
    mScreenRadioButton = new CpSettingFormItemData(HbDataFormModelItem::RadioButtonListItem,
            QString("Keys & screen locked after"));
    settingForm()->addConnection(mScreenRadioButton,SIGNAL(itemSelected(int)),this,SLOT(ScreenValueChanged(int)));
    model->appendDataFormItem(mScreenRadioButton, model->invisibleRootItem());

    QStringList sList;
    sList<< "0 seconds"<< "5 seconds"<< "10 seconds"<< "15 seconds"<< "20 seconds"<< "25 seconds";
    mScreenRadioButton->setContentWidgetData( QString("items"), sList );
    mScreenRadioButton->setContentWidgetData( QString("selected"), mModel->keyguard() );
}

void CpDisplayView::makeBacklightItem(HbDataFormModel* model)
{
    mDisplayRadioButton = new CpSettingFormItemData(HbDataFormModelItem::RadioButtonListItem,
            QString("Display dimmed after"));
    settingForm()->addConnection(mDisplayRadioButton,SIGNAL(itemSelected(int)),this,SLOT(DisplayValueChanged(int)));
    model->appendDataFormItem(mDisplayRadioButton, model->invisibleRootItem());

    QStringList sList;
    sList<< "0 seconds"<< "5 seconds"<< "10 seconds"<< "15 seconds"<< "20 seconds"<< "25 seconds";
    mDisplayRadioButton->setContentWidgetData( QString("items"), sList );
    mDisplayRadioButton->setContentWidgetData( QString("selected"), mModel->backlight() );
}

void CpDisplayView::makeBrightnessItem(HbDataFormModel* model)
{
    mBrightSliderItem = new CpSettingFormItemData(HbDataFormModelItem::SliderItem,
            QString("Brightness"));
    settingForm()->addConnection(mBrightSliderItem,SIGNAL(valueChanged(int)),this,SLOT(BrightValueChanged(int)));
    model->appendDataFormItem(mBrightSliderItem, model->invisibleRootItem());

    mBrightSliderItem->setContentWidgetData( QString("value"), mModel->brightness() );
    mBrightSliderItem->setContentWidgetData( QString("minimum"), 0 );
    mBrightSliderItem->setContentWidgetData( QString("maximum"), 100 );
}

CpDisplayView::~CpDisplayView()
{
    delete mModel;
	mModel = 0;
}

void CpDisplayView::ScreenValueChanged(int value)
{
    mModel->setKeyguard(value);
}

void CpDisplayView::DisplayValueChanged(int value)
{
    mModel->setBacklight(value);
}

void CpDisplayView::BrightValueChanged(int value)
{
	mModel->setBrightness(value);
}


