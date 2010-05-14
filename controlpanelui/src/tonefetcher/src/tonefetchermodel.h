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
 *     The header file for tone list model
 *     
 */

#ifndef TONEFETCHERMODEL_H
#define TONEFETCHERMODEL_H

#include <QStandardItemModel>
#include <QStringList>

// CLASS DECLARATION
/**
 *  This class is used for storing tone list items.
 *  inherited from QStandardItemModel so that many existing functions could be used.
 *
 */
class ToneFetcherModel : public QStandardItemModel
{
public:
    explicit ToneFetcherModel( QObject *parent );
    virtual ~ToneFetcherModel();
    //from QAbstractItemModel
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void insertInOrder(QStandardItem *fileName, QStandardItem *filePath, int role = Qt::DisplayRole);    
    QString path(const QModelIndex &index) const;
    void refresh();
    void clearAll();
private:
    /*
     * binary search (ascendant) for the correct index to insert.
     * @param low the start of search
     * @param high the end of search.
     * @return the correct index
     */
    int insertIndex(int low, int high, QString variant);
    
    /*
     * save the absolute path of the tone.
     */
    QStringList mUserDataLst;   
    
};

#endif /* TONEFETCHERMODEL_H_ */
