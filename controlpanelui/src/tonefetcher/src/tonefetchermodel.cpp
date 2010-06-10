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
 *     The source file for tone list model
 */
#include "tonefetchermodel.h"

ToneFetcherModel::ToneFetcherModel(QObject *parent) 
    : QStandardItemModel(parent)
{
}

ToneFetcherModel::~ToneFetcherModel()
{
}

QVariant ToneFetcherModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::UserRole) {
        return  mUserDataLst.at(index.row());
    } else {
        return QStandardItemModel::data(index, role);
    }    
}

void ToneFetcherModel::insertInOrder(QStandardItem *fileName, QStandardItem *filePath, int role)
{
    QString name = fileName->text();
    QString path = filePath->text();
    int index = this->insertIndex(0, rowCount(), name);
    
    mUserDataLst.insert(index, path);
    QStandardItemModel::insertRow(index, fileName);
}

QString ToneFetcherModel::path(const QModelIndex &index) const
{
    QString str =  data(index, Qt::UserRole).toString();
    return str;
}

int ToneFetcherModel::insertIndex(int low, int high,  QString value)
{
    if (low == high) {
        return low;   
    }
    int middle = (low + high - 1)/2;
    QModelIndex lowItemIndex = ((QStandardItemModel *)this)->index(low, 0);
    QModelIndex highItemIndex = ((QStandardItemModel *)this)->index(high - 1, 0);
    QModelIndex middleItemIndex = (( QStandardItemModel *)this)->index(middle, 0);
    QString lowString = data(lowItemIndex).toString();
    QString highString = data(highItemIndex).toString();
    QString middleString = data(middleItemIndex).toString();
    
    if (value >= highString) {
        return high;
    }
    if (value < lowString) {
        return low;
    }
    high = high - 1;
    while (low < high) {
        middle = (low + high)/2;
        middleItemIndex = ((QStandardItemModel *)this)->index(middle, 0);
        middleString = data(middleItemIndex).toString();
        if (value >= middleString) {
            low = middle + 1;
        } else {
            high = middle;
        }        
    }
    return low;
}

void ToneFetcherModel::refresh()
{
    emit layoutChanged();
}

void ToneFetcherModel::toBeFreshed()
{
    emit layoutAboutToBeChanged();
}

void ToneFetcherModel::clearAll()
{
    mUserDataLst.clear();
    QStandardItemModel::clear();
}
//End of File
