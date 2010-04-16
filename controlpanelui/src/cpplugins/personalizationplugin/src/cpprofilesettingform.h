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

#ifndef CPPROFILESETTINGFORM_H
#define CPPROFILESETTINGFORM_H

#include <hbdataform.h>

class HbDataFormModel;
class HbDataFormModelItem;

class CpProfileSettingForm : public HbDataForm
{
    Q_OBJECT
public:
    CpProfileSettingForm();
    virtual ~CpProfileSettingForm();
private:
    void initModel();
    void initVolumeGroup(HbDataFormModelItem *parent);
    void initVibraGroup(HbDataFormModelItem *parent);
 //   void initRingToneGroup(HbDataFormModelItem *parent);
 //   void initMessageToneGroup(HbDataFormModelItem *parent);
 //   void initAlertToneGroup(HbDataFormModelItem *parent);
 //   void initKeyAndScreenToneGroup(HbDataFormModelItem *parent);
private:
    HbDataFormModel *mModel;
};


#endif //CPPROFILESETTINGFORM_H
