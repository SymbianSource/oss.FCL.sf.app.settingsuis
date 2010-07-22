/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  
 *   
 */

/*!
    \class CpThemeControl
    \brief CpThemeControl creates and controls two views for Theme Changer plugin and handles
	user interaction to preview and change the themes.

	It creates a list view of the themes.  When a list item is selected, it creates a preview
	of the theme icon using a CpThemePreview class.  

	This class also connects to the theme server using the HbThemeChanger and sets the theme
	based on user interaction with the views.  

 */

#include <QString>
#include <QModelIndex>
#include <QTranslator>
#include <QSortFilterProxyModel>
#include <QThread>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>

#include <hbmainwindow.h>
#include <hbinstance.h>
#include "cpthemechanger.h"

#include "cpthemecontrol.h"
#include "cpthemelistview.h"
#include "cpthemepreview.h"
#include "cpthemeinfo.h"
#include "cpthemelistmodel.h"

#include <hbdialog.h>
#include <hblabel.h>


/*!
	Helper function to fetch the main window.
*/

static HbMainWindow *mainWindow() 
{
    QList< HbMainWindow* > mainWindows = hbInstance->allMainWindows();
    if (!mainWindows.isEmpty()) {
        return mainWindows.front();
    }
    return 0;
}

/*!
	constructor.
*/
CpThemeControl::CpThemeControl(): mThemeListView(0), 
    mThemePreview(0), 
    mThemeChanger(0),
    mListModel(0),
    mThemeChangeFinished(false),
    mWaitDialog(0)
{
    mThemeChanger = new CpThemeChanger();
       
    QTranslator *translator = new QTranslator(this);
    QString lang = QLocale::system().name();
    QString path = "Z:/resource/qt/translations/";
    translator->load("control_panel_" + lang, path);
    qApp->installTranslator(translator);
    
    connect(mThemeChanger,SIGNAL(themeChangeFinished()), this, SLOT(themeChangeFinished()));
   
}


/*!
	destorys the list view, preview and theme changer objects.
*/
CpThemeControl::~CpThemeControl()
{
    delete mThemeListView;
    mThemeListView = 0;

    delete mThemeChanger;
    mThemeChanger = 0;

    delete mThemePreview;
    mThemePreview = 0;
    
    delete mWaitDialog;
    mWaitDialog = 0;
}

/*!
	Creates the theme list view.  Gets the themes, creates a model based on
	theme names, icons, and icon paths and sets the list model.
*/
void CpThemeControl::createThemeList()
{
   
    mThemeListView = new CpThemeListView();
    
    if(!mListModel) {
        mListModel = new CpThemeListModel(this);
    }
    
    // Set the model for theme list.
    mThemeListView->setModel(mListModel);
    mThemeListView->themeList()->setSelectionMode(HbAbstractItemView::SingleSelection);
    
    setActiveThemeIndex();
    
    
    //connect to signal for selecting a list item.
    connect(mThemeListView,SIGNAL(newThemeSelected(const QModelIndex&)),
            this,SLOT(newThemeSelected(const QModelIndex&)));

	//handle signal for list view closing. (e.g Back softkey pressed)
    connect(mThemeListView,SIGNAL(aboutToClose()),
            this,SLOT(themeListClosed()));
}

/*!
	returns the instance of themelist view.  Used by control panel to set
	the view.  
*/
CpBaseSettingView* CpThemeControl::themeListView()
{
    //If the view was removed before by control panel app, create it again.
    if(!mThemeListView) {
        createThemeList();
    }

    return mThemeListView;
}

/*!
        returns the name of the current theme.
*/
QString CpThemeControl::currentThemeName() const
{
    QString name = "";
    if(mThemeChanger->currentTheme()) {
        name = mThemeChanger->currentTheme()->name();
    }
    return name;
}

/*!
        returns the repersenatative icon of the current theme.
*/
HbIcon CpThemeControl::currentThemeIcon() const
{
    HbIcon icon;
    if(mThemeChanger->currentTheme()) {
        icon = mThemeChanger->currentTheme()->icon();
    }
    return icon;
}

/*!
	Slot called when a list item of the theme list is selected.
*/
void CpThemeControl::newThemeSelected(const QModelIndex& index)
{
    if(!index.isValid()) {
        return;
    }
    
    CpThemeInfo themeInfo;
    QVariant data;

    //reset the current index to active theme, so that the selection remains on current
    //theme even though another list item is selected.
    setActiveThemeIndex();
    
    // Figure out whether this is a URI and appropriately delegate
    data = index.data(CpThemeListModel::ItemTypeRole);
    if(data.isValid() && data.canConvert<CpThemeInfo::ThemeListItemType>()) {

        CpThemeInfo::ThemeListItemType type = data.value<CpThemeInfo::ThemeListItemType>();

        switch (type) {
            case CpThemeInfo::ThemeListItemType_URL:
                //get the URL
                data = index.data(CpThemeListModel::ItemDataRole);
                if(data.isValid()) {
                    QString url = data.toString();
                    // Launch the URL in the browser and 
                    // continue to Preview if not successful
                    if (QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode))) {
                        return;
                    }
                }
                break;
    
            case CpThemeInfo::ThemeListItemType_APP:
                break;

            default:
                // do nothing
                qt_noop();
        }
    }
    
    //get the theme name.
    data = index.data(Qt::DisplayRole);
    if(data.isValid()) {
        themeInfo.setName(data.toString());
    }
    
    //get theme icon.
    data = index.data(Qt::DecorationRole);
    if(data.isValid()) {
        themeInfo.setIcon(data.value<HbIcon>());
    }
    
    data = index.data(CpThemeListModel::PortraitPreviewRole);
    if(data.isValid()) {
        themeInfo.setPortraitPreviewIcon(data.value<HbIcon>());
    }
    
    data = index.data(CpThemeListModel::LandscapePreviewRole);
    if(data.isValid()) {
        themeInfo.setLandscapePreviewIcon(data.value<HbIcon>());
    }
    
   //Set up the theme preview and set it to
    //the current view of main window.
    HbMainWindow*  mWindow = ::mainWindow();
   
    if(!mThemePreview){
        mThemePreview = new CpThemePreview(themeInfo);
        mWindow->addView(mThemePreview);
        
        connect(mThemePreview,SIGNAL(aboutToClose()),
            this, SLOT(previewClosed()));

        connect(mThemePreview, SIGNAL(applyTheme(const QString&)),
                this, SLOT(themeApplied(const QString&)));
    } else {
        mThemePreview->setThemeInfo(themeInfo);
    }
    mThemePreview->setTitle(hbTrId("txt_cp_title_control_panel"));

    mWindow->setCurrentView(mThemePreview);

}

/*!
	Slot called when a Select key is pressed in theme preview view.
*/
void CpThemeControl::themeApplied(const QString& theme)
{
    QThread::currentThread()->setPriority(QThread::HighPriority);  
    
    if(mThemeChanger->changeTheme(theme)) {
    
        //Start a timer. If theme change takes more than 1 seconds,
        //we will show a dialog (mWaitDialog) until theme change
        //is done (themeChangeFinished is called).
        QTimer::singleShot(1000, this, SLOT(themeWaitTimeout()));
        
        mThemeChangeFinished = false;
    } else {
        //theme change failed, go back to control panel.
        previewClosed();
        triggerThemeListClose();
    }
   
}

/*!
	Slot called when the theme preview view is closed.
*/
void CpThemeControl::previewClosed()
{
    //The theme preview closed, go back
    //to theme list view.
    HbMainWindow*  mainWindow = ::mainWindow();
	mainWindow->removeView(mThemePreview);
    mThemePreview->deleteLater();
    mThemePreview = 0;
  
    //reset the current index to active theme, so that the selection remains on current
    //theme even though another list item is selected.
    setActiveThemeIndex();
	mainWindow->setCurrentView(mThemeListView);   
}

/*!
    Slot for when the theme list view is closed. Ownership of the theme list was given to
    control panel, so the class won't delete it.
    
*/
void CpThemeControl::themeListClosed()
{
    mThemeListView = 0;

    delete mThemePreview;
    mThemePreview = 0;
}

/*!
    asks the theme list view to close.  
*/
void CpThemeControl::triggerThemeListClose()
{
    mThemeListView->closeView();
}

void CpThemeControl::themeChangeTimeout()
{
    //Theme change is finished and idle timer has timed out,
    //so revert back the application priority to normal
    //and go back to control panel view.
    if(mWaitDialog && mWaitDialog->isVisible()) {
        mWaitDialog->hide();
    }
    
    previewClosed();
    //ask the themelistview to close.  Control Panel will
    //take care of removing it from window.
    triggerThemeListClose();
    
    QThread::currentThread()->setPriority(QThread::NormalPriority); 
}

void CpThemeControl::themeWaitTimeout()
{
    //If after this timeOut, theme change is still in progress,
    //show a processing dialog.
    if(!mThemeChangeFinished)
    {
        if(!mWaitDialog) {
            mWaitDialog = new HbDialog();
            mWaitDialog->setDismissPolicy(HbPopup::NoDismiss);
            mWaitDialog->setModal(false);
            mWaitDialog->setTimeout(HbPopup::NoTimeout);
            //TODO: need localized text for Hb Dialog
            // Create and set HbLabel as content widget.
            HbLabel *label = new HbLabel("Processing ...");
            label->setAlignment(Qt::AlignCenter);
            mWaitDialog->setContentWidget(label);
        }
       // as we do not need any signals, calling show() instead of open()
       mWaitDialog->show();
    }
}

void CpThemeControl::themeChangeFinished()
{
    //Theme change is done. Start an idle timer to let the UI
    //finish remaining tasks.
    QTimer::singleShot(0, this, SLOT(themeChangeTimeout()));
    mThemeChangeFinished = true;
    
    if(mThemeChanger->currentTheme()) {
        emit themeUpdated(mThemeChanger->currentTheme()->name(), mThemeChanger->currentTheme()->icon());
    }
    
}

/*!
 * Private function that sets the current index of theme list view to indicate
 * the active theme.
 */
void CpThemeControl::setActiveThemeIndex()
{
    //Get the index of current theme.
    CpThemeListModel* themeListModel = dynamic_cast<CpThemeListModel*>(mListModel);
    const CpThemeInfo* currentTheme = mThemeChanger->currentTheme();
    if(themeListModel && currentTheme) {
        QModelIndex sourceIndex = mListModel->index(themeListModel->indexOf(*currentTheme),0);
        //set current index.
        mThemeListView->themeList()->setCurrentIndex(sourceIndex, QItemSelectionModel::SelectCurrent);
    }
}
    





