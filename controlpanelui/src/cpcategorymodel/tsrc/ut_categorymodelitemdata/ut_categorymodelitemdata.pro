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

TEMPLATE = app
TARGET = ut_categorymodelitemdata
QT += testlib
CONFIG += hb qtestlib
CONFIG += symbian_test

QMAKE_EXTRA_TARGETS += test autotest

DEPENDPATH += .

INCLUDEPATH +=  ../../src \
	              ../../../inc 

LIBS += -lcpframework
LIBS += -lcpcategorymodel

# Input
HEADERS += src/ut_categorymodelitemdata.h
SOURCES += src/ut_categorymodelitemdata.cpp

symbian*: { 
    TARGET.CAPABILITY = CAP_APPLICATION
    LIBS += -lfirstpluginforcpcategorymodel
           
    deploy.path = C:
    testdll.sources += firstpluginforcpcategorymodel.dll
    testdll.path = /sys/bin
    
    testqtplugin.sources += ../testpluginsforcpcategorymodel/firstpluginforcpcategorymodel/qmakepluginstubs/firstpluginforcpcategorymodel.qtplugin
    testqtplugin.path = /resource/qt/plugins/controlpanel
    
    testqtcfgfile.sources += data/pluginfileformodelitemdata.cpcfg
    testqtcfgfile.path = /resource/qt/plugins/controlpanel/config                  
    
    DEPLOYMENT += testdll  \
                  testqtplugin \
                  testqtcfgfile
}