
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

#include "cpvolumecontroller.h"
#include <cplogger.h>
#include <cpitemdatahelper.h>
#include <hbdataformmodelitem.h>
#include "cpvolumegroupitemdata.h"
#ifdef Q_OS_SYMBIAN
	#include <cpprofilemodel.h>
#endif
//#include <hbslider.h>
//#include <hbpushbutton.h>

CpVolumeController::CpVolumeController(CpProfileModel *profileModel,
                                       const QList<HbDataFormModelItem *> &itemList,
                                       CpItemDataHelper &itemDataHelper): 
                                           mProfileModel(profileModel),
                                           mItemList(itemList)
															 
{
	//itemDataHelper.addConnection(mMasterVolumeItem,SIGNAL(beepActivated()),this,SLOT(onBeepActivated()));
	//itemDataHelper.addConnection(mMasterVolumeItem,SIGNAL(silentActivated()),this,SLOT(onSilentActivated()));
	//itemDataHelper.addConnection(mMasterVolumeItem,SIGNAL(normalValueChanged(int)),this,SLOT(onNormalValueChanged(int)));
	//updateMasterVolumeValue();
    itemDataHelper.addConnection(
                mItemList.at(CpVolumeGroupItemData::EVolumeSilenceItem), SIGNAL(toggled(bool)),
                this, SLOT(silenceModeChange(bool))
            );
    itemDataHelper.addConnection(
                mItemList.at(CpVolumeGroupItemData::EVolumeMasterVolumeItem), SIGNAL(valueChanged(int)),
                this, SLOT(masterVolumeChange(int))
            );
    itemDataHelper.addConnection(
                mItemList.at(CpVolumeGroupItemData::EVolumeMasterVibraItem), SIGNAL(stateChanged(int)),
                this, SLOT(masterVibraChange(int))
            );
    update();
}

CpVolumeController::~CpVolumeController()
{

}

/*void CpMasterVolumeValueController::onBeepActivated()
{
#ifdef Q_OS_SYMBIAN
	mProfileModel->activateBeep();
#endif
}*/

void CpVolumeController::silenceModeChange(bool isSilence)
{
#ifdef Q_OS_SYMBIAN
	mProfileModel->setSilenceMode(isSilence);
	HbDataFormModelItem *masterVolume = mItemList.at(CpVolumeGroupItemData::EVolumeMasterVolumeItem);
    masterVolume->setEnabled(!isSilence);
#endif
}

void CpVolumeController::masterVolumeChange(int value)
{
#ifdef Q_OS_SYMBIAN
	mProfileModel->setMasterVolume( volumeLevelToInt( (CpVolumeController::VolumeLevel)value ) );
	HbDataFormModelItem *masterVolume = mItemList.at(CpVolumeGroupItemData::EVolumeMasterVolumeItem);
	masterVolume->setContentWidgetData("value",value);
#endif
}

void CpVolumeController::masterVibraChange(int state)
{
#ifdef Q_OS_SYMBIAN
    mProfileModel->setMasterVibra(state);
#endif
}

void CpVolumeController::update()
{
#ifdef Q_OS_SYMBIAN
	bool isSilenceMode = mProfileModel->isSilenceMode();
	HbDataFormModelItem *silenceIndicator = mItemList.at(CpVolumeGroupItemData::EVolumeSilenceItem);
	silenceIndicator->setContentWidgetData("checked",isSilenceMode);
	HbDataFormModelItem *masterVolume = mItemList.at(CpVolumeGroupItemData::EVolumeMasterVolumeItem);
	if (masterVolume) {
	    CPFW_LOG("::updateMasterVolumeValue(), Start using profile model.");
	    QMap<QString, QVariant> iconMaps;
	    if (isSilenceMode) {
	        CPFW_LOG("::updateMasterVolumeValue(), Got silent state.");
	        iconMaps.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_muted.svg"));
	    }
	    else {
	        iconMaps.insert(QString("IconElement"), QVariant(":/icon/hb_vol_slider_unmuted.svg"));
	        CPFW_LOG("::updateMasterVolumeValue(), Got ring volume.");	        
	    }
	    masterVolume->setContentWidgetData("elementIcons", iconMaps);
	    masterVolume->setEnabled(!isSilenceMode);
	    //masterVolume->setContentWidgetData("enabled",!isSilenceMode);
	    masterVolume->setContentWidgetData("value",intToVolumeLevel(mProfileModel->masterVolume()));
	}
	HbDataFormModelItem *masterVibra = mItemList.at(CpVolumeGroupItemData::EVolumeMasterVibraItem);
	if (masterVibra) {
        masterVibra->setContentWidgetData("checkState",(mProfileModel->masterVibra()?2:0));
	} 
	
#endif
}

int CpVolumeController::volumeLevelToInt( CpVolumeController::VolumeLevel volumeLevel )
{
    switch( volumeLevel ){
        case VolumenLevelSoft:
            return 1;
        case VolumeLevelMed:
            return 5;
        case VolumeLevelLoud:
            return 10;
    }
}
CpVolumeController::VolumeLevel CpVolumeController::intToVolumeLevel( int value )
{
    if( value < 4 ) {
        return VolumenLevelSoft;
    } else if( value < 8 )
        return VolumeLevelMed;
    else{
        return VolumeLevelLoud;
    }
}
