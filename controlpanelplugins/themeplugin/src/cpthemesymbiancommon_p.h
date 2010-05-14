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

#ifndef HBTHEMESYMBIANCOMMON_P_H
#define HBTHEMESYMBIANCOMMON_P_H

#include <e32base.h>
#include "cpthemecommon_p.h"
struct TIconParams
{
    TBuf<256> fileName;
    TReal width;
    TReal height;
    TUint8 aspectRatioMode;
    TUint8 mode;
    TUint8 options;
    TBool mirrored;
    TUint32 rgba;
    TBool colorflag;
 };

struct TMultiIconSymbParams
{
    TBuf<256> multiPartIconId;
    TBuf<256> multiPartIconList[9];
    TRect sources[9];
    TRect targets[9];
    TSize pixmapSizes[9];
    QSizeF size;
    TInt aspectRatioMode;
    TInt mode;
    TInt options;
    TBool mirrored;
    TInt rgba;
    TBool colorflag;
 };

// server name
_LIT(KThemeServerName,"hbthemeserver");
const TUid KServerUid3={0x20022E82};

// A version must be specifyed when creating a session with the server


const TUint KThemeServerMajorVersionNumber=0;
const TUint KThemeServerMinorVersionNumber=1;
const TUint KThemeServerBuildVersionNumber=1;

enum TThemeServerLeave
{
    ENonNumericString = 99
};

#endif // HBTHEMESYMBIANCOMMON_P_H

