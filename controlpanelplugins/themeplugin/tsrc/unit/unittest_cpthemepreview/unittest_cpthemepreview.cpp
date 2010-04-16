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
* Description:  unit tests for the CpThemePreview class from themeplugin
*
*/

#include <QtTest/QtTest>

#include "cpthemepreview.h"
#include "cpthemechanger.h"

class TestCpThemePreview : public QObject
{
    Q_OBJECT

private slots:
    void init();

    void testConstructor();
    void testConstructor2();
    void testSetThemeName();
    void testSetPreviewIcon();
    void testThemeSelected();

    void cleanup();

private:
    CpThemeChanger *mThemeChanger;
};

void TestCpThemePreview::init()
{
    mThemeChanger = new CpThemeChanger();
}

void TestCpThemePreview::cleanup()
{
    delete mThemeChanger;
}

void TestCpThemePreview::testConstructor()
{
    CpThemePreview *obj = new CpThemePreview();

    QVERIFY( obj != 0 );

    delete obj;
}

void TestCpThemePreview::testConstructor2()
{
    QList<CpThemeChanger::ThemeInfo> themeInfo = mThemeChanger->themes();

    for (int i = 0; i < themeInfo.size(); ++i) {
        QString name = themeInfo.at(i).themeName;
        QString icon = themeInfo.at(i).iconPath;

        CpThemePreview *obj = new CpThemePreview(name, icon);
        QVERIFY (obj != 0 );
        QCOMPARE(obj->themeName(), name);

        delete obj;
    }

    // try with gibberish for the name and/or icon
    QString name = QString("alzihgiaureh");
    QString icon = QString("ahilwvihln");
    CpThemePreview *obj = new CpThemePreview(name);
    QVERIFY(obj != 0);
    delete obj;

    obj = new CpThemePreview(name,icon);
    QVERIFY(obj != 0);
    delete obj;
}

void TestCpThemePreview::testSetThemeName()
{
    CpThemePreview *obj = new CpThemePreview();
    QList<CpThemeChanger::ThemeInfo> themeInfo = mThemeChanger->themes();

    for (int i = 0; i < themeInfo.size(); ++i) {
        QString name = themeInfo.at(i).themeName;

        obj->setThemeName(name);
        QCOMPARE(obj->themeName(), name);
    }

    delete obj;
}

// NULL TEST
void TestCpThemePreview::testSetPreviewIcon()
{
    CpThemePreview *obj = new CpThemePreview();
    QList<CpThemeChanger::ThemeInfo> themeInfo = mThemeChanger->themes();

    for (int i = 0; i < themeInfo.size(); ++i) {
        QString name = themeInfo.at(i).themeName;

        obj->setThemeName(name);
        QCOMPARE(obj->themeName(), name);
    }

    delete obj;
}

void TestCpThemePreview::testThemeSelected()
{
    CpThemePreview *obj = new CpThemePreview();
    QSignalSpy spy(obj,SIGNAL(applyTheme(const QString&)));

    obj->themeSelected();

    QCOMPARE(spy.count(), 1);

    delete obj;
}


QTEST_MAIN(TestCpThemePreview)
#include "unittest_cpthemepreview.moc"

