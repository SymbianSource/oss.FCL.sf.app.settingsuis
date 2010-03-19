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
#ifndef	CPDISPLAYVIEW_H
#define	CPDISPLAYVIEW_H

#include <cpbasesettingview.h>

class HbDataFormModelItem;
class HbDataFormModel;
class CpSettingFormItemData;
class CpDisplayModel;

class CpDisplayView : public CpBaseSettingView
{
    Q_OBJECT
public:
    explicit CpDisplayView(QGraphicsItem *parent = 0);
    virtual ~CpDisplayView();

private:
    void makeScreenItem(HbDataFormModel* model);
    void makeBacklightItem(HbDataFormModel* model);
    void makeBrightnessItem(HbDataFormModel* model);
    
public:
    bool isHidden();

private slots:
    //need handling your member's value change
    void ScreenValueChanged(int value);
    void DisplayValueChanged(int value);
    void BrightValueChanged(int value);

private:
    CpSettingFormItemData *mScreenRadioButton;
    CpSettingFormItemData *mDisplayRadioButton;
	CpSettingFormItemData *mBrightSliderItem;
	CpDisplayModel* mModel;
	bool mIsHidden;
	
};
#endif//	CPDISPLAYVIEW_H
