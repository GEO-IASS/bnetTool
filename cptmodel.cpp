/**
 * \author trungvv1
 *
 * \date 4/1/2015
 * \class CptModel
 *
 * \brief write something about your class
 *
 *
 */

#include "cptmodel.h"
#include <QColor>

CptModel::CptModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    values = nullptr;
    initValues(1, 2, 0);
}

QVariant CptModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    if (index.isValid())
    {
        if (role == Qt::DisplayRole) {
            return values[row][col];
        } else if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        } else if (role == Qt::BackgroundRole && col < nparents) {
            return QColor(180,255,255);
        }
    }
    return QVariant();
}

bool CptModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (value.toString().isEmpty()) {
            return false;
        }
        double newValue = value.toDouble();
        if (newValue == values[index.row()][index.column()] || index.column() < nparents) {
            return false;
        }
        values[index.row()][index.column()] = newValue;
        if (ncolumns-nparents == 2)
        {
            values[index.row()][ncolumns-1-(index.column()-nparents)] = 1 - newValue;
        }
        emit dataChanged(index, index);
        return true;

//        double sumProb = 0;
//        for (int i = nparents; i < index.column(); ++i) {
//            sumProb += values[index.row()][i];
//        }
//        double oldValue = values[index.row()][index.column()];
//        if (sumProb + newValue <= 1) {
//            values[index.row()][index.column()] = newValue;
//            if (index.column() == ncolumns-1) {
//                values[index.row()][index.column()-1] -= (newValue - oldValue);
//            } else {
//                values[index.row()][index.column()+1] -= (newValue - oldValue);
//            }
//            emit dataChanged(index, index);
//            return true;
//        } else if (index.column() == ncolumns-1) {
//            if ( values[index.row()][index.column()-1] > (newValue-oldValue) ) {
//                values[index.row()][index.column()] = newValue;
//                values[index.row()][index.column()-1] -= (newValue - oldValue);
//            }
//            emit dataChanged(index, index);
//            return true;
//        }
    }
    return false;
}

QVariant CptModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
        {
            if(section >=0 && section < headers.count())
                return headers[section];
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    } else if (role == Qt::BackgroundRole) {
        if (orientation == Qt::Horizontal) {
            if (section < nparents) {
                return QColor(120,210,210);
            } else {
                return QColor(150,150,250);
            }
        }
    }
    return QVariant();
}

int CptModel::rowCount(const QModelIndex &parent) const
{
    return nrows;
}

int CptModel::columnCount(const QModelIndex &parent) const
{
    return ncolumns;
}

bool CptModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    endInsertRows();
    return true;
}

bool CptModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    endRemoveRows();
    return true;
}

Qt::ItemFlags CptModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void CptModel::initValues(int nrows, int ncolumns, int nparents)
{
    beginResetModel();
    /// delete old data
    clear();
    ///

    this->nrows = nrows;
    this->ncolumns = ncolumns;
    this->nparents = nparents;
    values = new double*[nrows];
    for (int i = 0; i < nrows; ++i) {
        values[i] = new double[ncolumns];
        for (int j = 0; j < ncolumns; ++j) {
            values[i][j] = 0;
//            values[i][j] = 1.0 / ncolumns;
        }
//        values[i][0] = 1;
    }    
    endResetModel();
}

void CptModel::setValue(int row, int column, double value)
{
    values[row][column] = value;
}

double CptModel::getValue(int row, int column)
{
    if (0 <= row && row < nrows && 0 <= column && column < ncolumns)
        return values[row][column];
    else
        return 0;
}

void CptModel::setHeader(int column, QString value)
{
    headers.insert(column, value);
}

void CptModel::clear()
{
    if (values) {
        for (int i = 0; i < this->nrows; ++i) {
            delete values[i];
            values[i] = nullptr;
        }
        delete values;
        values = nullptr;
    }
    headers.clear();
    nrows = ncolumns = nparents = 0;
}
