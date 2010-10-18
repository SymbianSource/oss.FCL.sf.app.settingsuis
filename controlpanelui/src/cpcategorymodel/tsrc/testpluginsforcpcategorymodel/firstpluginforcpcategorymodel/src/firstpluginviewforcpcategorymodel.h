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

#ifndef	FIRSTPLUGINVIEWFORCPCATEGORYMODEL_H
#define	FIRSTPLUGINVIEWFORCPCATEGORYMODEL_H

#include <cpbasesettingview.h>
#include <QGraphicsItem>

class HbDataFormModelItem;
class CpSettingFormItemData;

class FirstPluginViewForCategoryModel : public CpBaseSettingView
{
    Q_OBJECT
public:
    explicit FirstPluginViewForCategoryModel(QGraphicsItem *parent = 0);
    virtual ~FirstPluginViewForCategoryModel();
public:
    void testClose();
private slots:   
    void sliderValueChanged(int value);
    
private:
    HbDataFormModelItem   *mGroupItem;
    CpSettingFormItemData *mSliderItem;
};
#endif
