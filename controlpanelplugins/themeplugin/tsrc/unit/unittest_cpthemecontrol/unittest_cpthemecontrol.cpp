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
* Contributors: XENT-MV
*
* Description:  unit tests for the CpThemeControl class from themeplugin
*
*/

#include <QtTest/QtTest>
#include <QModelIndex>
#include <QSignalSpy>

#include "cpthemechanger.h"
#include "cpthemecontrol.h"
#include "cpthemechanger.h"

class TestCpThemeControl : public QObject
{
    Q_OBJECT

private slots:
    void init();

    void testConstructor();
    void testThemeListView();
    void testCurrentThemeName();
    void testCurrentThemeIcon();
    void testNewThemeSelected();
    void testPreviewClosed();
    void testThemeApplied();
    void testUpdateThemeList();

    void cleanup();

private:
    CpThemeChanger* mThemeChanger;
};

// setup and cleanup
void TestCpThemeControl::init()
{
    mThemeChanger = new CpThemeChanger();
}

void TestCpThemeControl::cleanup()
{
    delete mThemeChanger;
}

// verify that the constructor works and that the
// defaults are sane.
void TestCpThemeControl::testConstructor()
{
    CpThemeControl * control = new CpThemeControl();

    QVERIFY(control !=0 );
    QVERIFY(!control->currentThemeName().isEmpty());
    QVERIFY(!control->currentThemeIcon().isEmpty());

    delete control;
}

// verify that the themeListView doesn't return NULL (or crash)
void TestCpThemeControl::testThemeListView()
{
    CpThemeControl control;

    QVERIFY(control.themeListView() != 0);
}

// test that we get back a non-empty QString
void TestCpThemeControl::testCurrentThemeName()
{
    CpThemeControl control;

    QVERIFY((control.currentThemeName() == mThemeChanger->currentTheme())
        || (control.currentThemeName() == QString("hbdefault")));
}

// test that we get a non-empty string for current theme icon
void TestCpThemeControl::testCurrentThemeIcon()
{
    CpThemeControl control;

    QVERIFY(!control.currentThemeIcon().isEmpty());
}

// NULL test there's no way to externally get the model the QModelIndex is based on
void TestCpThemeControl::testNewThemeSelected()
{
    CpThemeControl control;

    control.newThemeSelected(QModelIndex());

}

// NULL test - basically verifies that it doesn't burst into flames.
void TestCpThemeControl::testPreviewClosed()
{
    CpThemeControl control;

    control.previewClosed();

}
    
void TestCpThemeControl::testThemeApplied()
{
    CpThemeControl control;
    QList<CpThemeChanger::ThemeInfo> themeList = mThemeChanger->themes();
    QSignalSpy spy(&control, SIGNAL(themeUpdated(const QString&, const QString&)));
    int expectedSignalCount = 0;

    for (int i = 0; i < themeList.size(); ++i) {
        CpThemeChanger::ThemeInfo info = themeList.at(i);
        QString name = info.themeName;

        if (control.currentThemeName() == mThemeChanger->currentTheme()) {
            ++expectedSignalCount;
        }

        control.themeApplied(name);
        QCOMPARE(control.currentThemeName(), mThemeChanger->currentTheme());

        QCOMPARE(spy.count(),expectedSignalCount);
    }
}
    
// NULL test - basically verifies that it doesn't burst into flames.
void TestCpThemeControl::testUpdateThemeList()
{
    CpThemeControl control;

    control.updateThemeList();
}



QTEST_MAIN(TestCpThemeControl)
#include "unittest_cpthemecontrol.moc"
