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
# Description: cpserviceprovider project - qmake settings
#

TEMPLATE=app
TARGET=cpserviceprovider

CONFIG += hb service
symbian:TARGET.UID3 = 0x2002873F

include( ../common.pri )
include( cpserviceprovider.pri )

LIBS+=-lxqservice -lxqserviceutil -lcpframework

SERVICE.FILE = service_conf.xml
SERVICE.OPTIONS = embeddable
SERVICE.OPTIONS += hidden embeddable

libFiles.sources = xqservice.dll
libFiles.path = "!:\sys\bin"
DEPLOYMENT += libFiles