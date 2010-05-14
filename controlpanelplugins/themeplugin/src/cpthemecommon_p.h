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

#ifndef HBTHEMECOMMON_P_H
#define HBTHEMECOMMON_P_H

#include <QImage>
#include <QSize>
#include <QStringList>
#include <QColor>

#define THEME_SERVER_NAME "hbthemeserver"
#define HB_THEME_SHARED_PIXMAP_CHUNK "themeserver_chunk"
#define ORGANIZATION "Nokia"
#define THEME_COMPONENT "ThemeFramework"

// To enable/disable debug messages for theme server functionality
#undef THEME_SERVER_TRACES

// To enable fute testing for cache
//#define HB_ICON_CACHE_DEBUG
struct MultiPartSizeData
{
    // Indexing order is:

    // 'NinePieces'             -> tl, t, tr, l, c, r, bl, b, br
    // 'ThreePiecesHorizontal'  -> l, c, r
    // 'ThreePiecesVertical'    -> t, c, b
    QString multiPartIconId;
    QRect sources[9];       // rects used from the rasterized frame part pixmaps
    QRect targets[9];       // frame part target rects inside the bounding rectangle of the frame
    QSize pixmapSizes[9];   // frame part pixmaps are rasterized to these sizes
};


struct HbMultiIconParams
{
    QString multiPartIconId;
    QStringList multiPartIconList;
    MultiPartSizeData multiPartIconData;
    QSizeF size;
    int aspectRatioMode;
    int mode;
    int options;
    bool mirrored;
    QColor color;
    int rgba;
    bool colorflag;
};


enum IconFormatType {
    INVALID_FORMAT = -1,
    NVG,
    PIC,
    SVG,
    BLOB,
    OTHER_SUPPORTED_FORMATS
};

struct HbSharedPixmapInfo
{
    int offset;
    int width;
    int height;
    int defaultWidth;
    int defaultHeight;
    QImage::Format format;
};

struct HbSharedNVGInfo
{		
    int offset;
    int dataSize;
    int width;
    int height;
    int defaultWidth;
    int defaultHeight;
    
};

struct HbSharedPICInfo
{
    int offset;
    int dataSize;
    int width;
    int height;
    int defaultWidth;
    int defaultHeight;
};

struct HbSharedBLOBInfo
{
    int offset;
    int dataSize;
};

struct HbSharedIconInfo
{
    IconFormatType type;
    
    union 
    {
    	HbSharedPixmapInfo pixmapData;
    	HbSharedNVGInfo nvgData;
    	HbSharedPICInfo picData;
        HbSharedBLOBInfo blobData;
    };	
      
};

struct HbSharedStyleSheetInfo
{
    int offset;
    int refCount;
    HbSharedStyleSheetInfo(): 
        offset(-1),
        refCount(0)    
    {}
};

struct HbSharedEffectInfo
{
    int offset;
    HbSharedEffectInfo(): offset(-1){}
};

struct HbSharedWMLInfo
{
    int offset;
    HbSharedWMLInfo() : offset(-1) {}
};

struct HbDeviceProfileInfo
{
    int offset;
    HbDeviceProfileInfo() : offset(-1) {}
};

// Function codes (opcodes) used in message passing between client and server
enum ThemeServerRequest
    {
     EIconLookup = 1,
     EIconDefaultSize,
     EStyleSheetLookup,
     EThemeSelection,
     EMultiPieceIcon,
     EWidgetMLLookup,
     EDeviceProfileOffset,
 #ifdef HB_ICON_CACHE_DEBUG
     EIconCleanUp,
     ECacheIconCount,
     ERasterMemLimit,
     EVectorMemLimit,
     EFreeRasterMem,
     EFreeVectorMem,
     ELastAddedItemMem,
     ELastAddedItemRefCount,
     ELastRemovedItemMem,
     ELastRemovedItemRefCount,
     EEnableCache,
     ECacheHit,
     ECacheMiss,
     ECleanRasterLRUList,
     ECleanVectorLRUList,
     EServerHeap,
     EGpuLruCount,
     ECpuLruCount,
     EServerStat,
     EServerHeapMarkStart,
     EServerHeapMarkEnd,
     EServerAllocFail,
     EServerAllocReset,
 #endif
     EThemeContentUpdate,
	 EEffectLookupFilePath,
	 EEffectAdd,
     EUnloadIcon,
     EThemeServerStop
    };

#endif /* HBTHEMECOMMON_P_H */
