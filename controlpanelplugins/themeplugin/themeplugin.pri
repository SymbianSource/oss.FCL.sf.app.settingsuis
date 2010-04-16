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
# Description: cpthemeplugin source files
#

# Input
HEADERS += src/cpthemepreview.h \
           src/cpthemecontrol.h \
           src/cpthemeplugin.h \
           src/cpthemelistview.h \
           src/cpthemepluginentryitemdata.h

SOURCES += src/cpthemepreview.cpp \
           src/cpthemecontrol.cpp \
	   src/cpthemeplugin.cpp \
           src/cpthemelistview.cpp \
           src/cpthemepluginentryitemdata.cpp

# Remove once this goes into Hb.
HEADERS += src/cpthemechanger.h \
           src/cpthemeclient_p.h \
           src/cpthemecommon_p.h \
           src/cpthemechanger_p.h

# Remove once this goes into Hb.
SOURCES += src/cpthemechanger.cpp \
           src/cpthemechanger_p.cpp \
           src/cpthemeclient_p.cpp

# Remove once this goes into Hb.
win32|mac {
    HEADERS += src/cpthemeclientqt_p.h
    SOURCES += src/cpthemeclientqt_p.cpp
}

# Remove once this goes into Hb.
symbian {
    HEADERS += src/cpthemeclientsymbian_p.h \
               src/cpthemesymbiancommon_p.h
    SOURCES += src/cpthemeclientsymbian_p.cpp
}
