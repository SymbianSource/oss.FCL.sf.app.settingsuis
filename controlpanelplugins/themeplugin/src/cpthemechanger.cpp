/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of the HbThemeChanger class
 *
*/


#include "cpthemechanger.h"
#include "cpthemechanger_p.h"

/*!
  @alpha
  @hbcore
  \class CpThemeChanger

  \brief CpThemeChanger provides an interface for changing the current
  theme and enumerating the available themes (e.g., for the UI to provide 
  a list of themes to show).

  This API is only for use with the control panel and its theme
  changing plugin.
*/

/*!
  Constructor.
*/
CpThemeChanger::CpThemeChanger(QObject* p) :
    QObject(p),
    d_ptr(new CpThemeChangerPrivate(this))
{
}

/*!
    Provides a list of themes as a const QAbstractItemModel*.
*/
QAbstractItemModel& CpThemeChanger::model()
{
    Q_D(CpThemeChanger);

    return d->model;
}

/*! 
  Provide a list of currently available themes.
*/
const QList<CpThemeChanger::ThemeInfo> CpThemeChanger::themes() const
{
    Q_D(const CpThemeChanger);

    return d->themes();
}

/*!
  Creates a connection to the theme server for the purpose of 
  changing the theme.
 */
bool CpThemeChanger::connectToServer()
{
    Q_D(CpThemeChanger);

    return d->connectToServer();
}

/*!
  Indicates if the client is connected to the theme server.
*/
bool CpThemeChanger::isConnected() const
{
    Q_D(const CpThemeChanger);

    return d->isConnected();
}


/*!
    Returns a ThemeInfo struct containing the current theme name and
    a repersentative HbIcon.

    If no repersentative icon exists, the HbIcon returned will be
    uninitialized.
*/
const CpThemeChanger::ThemeInfo& CpThemeChanger::currentTheme() const
{
    Q_D(const CpThemeChanger);

    return d->currentTheme();
}

int CpThemeChanger::indexOf(const ThemeInfo& theme) const
{
    Q_D(const CpThemeChanger);
    return d->indexOf(theme);
}

/*!
 Change a theme. Returns true on success, false otherwise.
 */
bool CpThemeChanger::changeTheme(const QString& newtheme)
{
    Q_D(CpThemeChanger);

    return d->changeTheme(newtheme);
}

/*!
 Destructor
 */
CpThemeChanger::~CpThemeChanger()
{
    delete d_ptr;
    d_ptr = 0;
}

#include "moc_cpthemechanger.cpp"

// End of file

