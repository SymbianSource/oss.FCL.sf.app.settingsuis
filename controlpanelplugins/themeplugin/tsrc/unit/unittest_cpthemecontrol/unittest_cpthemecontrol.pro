#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0".
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:  XENT-MV
#
# Description:  unit tests for the cpthemecontrol class in the themeplugin.
#
#

include(../common.pri)


TEMPLATE = app
TARGET = unittest_cpthemecontrol
DEPENDPATH += .
win32::DEPENDPATH += c:/ControlPanel/debug/bin

INCLUDEPATH += . 
INCLUDEPATH += src/
INCLUDEPATH += ../../../src

LIBS += -lcpframework 
win32::LIBS += -LC:/ControlPanel/debug/bin

symbian::TARGET.UID3=0x2002DD2A


# Input

HEADERS += ../../../src/cpthemecontrol.h
HEADERS += ../../../src/cpthemechanger.h
HEADERS += ../../../src/cpthemechanger_p.h
HEADERS += ../../../src/cpthemeclient_p.h
symbian::HEADERS += ../../../src/cpthemeclientsymbian_p.h
win32::HEADERS += ../../../src/cpplugins/themeplugin/src/cpthemeclientqt_p.h
HEADERS += ../../../src/cpthemelistview.h
HEADERS += ../../../src/cpthemepreview.h
SOURCES += unittest_cpthemecontrol.cpp
SOURCES += ../../../src/cpthemecontrol.cpp
SOURCES += ../../../src/cpthemechanger.cpp
SOURCES += ../../../src/cpthemechanger_p.cpp
SOURCES += ../../../src/cpthemeclient_p.cpp
symbian::SOURCES += ../../../src/cpthemeclientsymbian_p.cpp
win32::SOURCES += ../../../src/cpplugins/themeplugin/src/cpthemeclientqt_p.cpp
SOURCES += ../../../src/cpthemelistview.cpp
SOURCES += ../../../src/cpthemepreview.cpp
