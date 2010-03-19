#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: 
#

TEMPLATE = lib
TARGET = cpdisplayplugin

CONFIG += hb plugin


include ( ../cpplugincommon.pri )
include ( displayplugin.pri )

symbian { 
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
	                 $$MOC_DIR
	HEADERS += src/cpdisplaymodel_p.h
    SOURCES += src/cpdisplaymodel_p.cpp

    LIBS += -lcentralrepository \
            -lfeatmgr
    TARGET.UID3 = 0X20028735
    TARGET.CAPABILITY = All -TCB 
    TARGET.EPOCALLOWDLLDATA = 1
}
