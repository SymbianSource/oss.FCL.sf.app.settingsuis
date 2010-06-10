# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description: cpthemeplugin source files
# Input
HEADERS += src/cpthemeclient_p_p.h \
    src/cpthemepreview.h \
    src/cpthemecontrol.h \
    src/cpthemeplugin.h \
    src/cpthemelistview.h \
    src/cpthemelistmodel.h \
    src/cpthemechanger.h \
    src/cpthemeclient_p.h \
    src/cpthemecommon_p.h \
    src/cpthemechanger_p.h \
    src/cpthemepluginentryitemdata.h

SOURCES += src/cpthemepreview.cpp \
    src/cpthemecontrol.cpp \
    src/cpthemeplugin.cpp \
    src/cpthemelistview.cpp \
    src/cpthemelistmodel.cpp \
    src/cpthemepluginentryitemdata.cpp \
    src/cpthemechanger.cpp \
    src/cpthemechanger_p.cpp \
    src/cpthemeclient_p.cpp
    	
win32|mac { 
    SOURCES += src/cpthemeclientqt_p.cpp
}

symbian { 
    HEADERS += src/cpthemesymbiancommon_p.h
    SOURCES += src/cpthemeclientsymbian_p.cpp
}
