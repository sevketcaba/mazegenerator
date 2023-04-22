#include "cubemodel.h"

#include <QColor>
#include <QDebug>
#include <QIcon>

CubeModel::CubeModel(QObject *parent) :
    QAbstractItemModel(parent)
  , IDataStructure()
{
    reset(10);
}

CubeModel::~CubeModel()
{

}

QModelIndex CubeModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int idx = row;
    return createIndex(row, column, &mData[idx]);
}

QModelIndex CubeModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

QVariant CubeModel::data(const QModelIndex &idx, int role) const
{
    auto index = idx.row();

    if(index < 0 || index >= mData.size())
        return  QVariant();

    const auto &dataObj = mData.at(index);

    if(role == RightWallRole)
        return dataObj.rightWall;
    if(role == BottomWallRole)
        return dataObj.bottomWall;
    if(role == VisitedRole)
        return dataObj.visited;
    if(role == CurrentRole)
        return dataObj.current;
    if(role == ColumnRole)
        return dataObj.column;
    if(role == RowRole)
        return dataObj.row;

    return QVariant();
}



bool CubeModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
    auto index = idx.row();

    if(index < 0 || index >= mData.size())
        return false;

    bool dc = true;

    if(role == RightWallRole)
    {
        mData[index].rightWall = value.toBool();
    }
    else if(role == BottomWallRole)
    {
        mData[index].bottomWall = value.toBool();
    }
    else if(role == VisitedRole)
    {
        mData[index].visited = value.toBool();
    }
    else if(role == CurrentRole)
    {
        mData[index].current = value.toBool();
    }
    else
        dc = false;

    if(dc)
        emit dataChanged(idx, idx, QList<int>() << role);

    return dc;
}

int CubeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mSize * mSize;
}


int CubeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mSize;
}

QHash<int, QByteArray> CubeModel::roleNames() const
{
    QHash<int, QByteArray> rn;
    rn[RightWallRole] = "right";
    rn[BottomWallRole] = "bottom";
    rn[VisitedRole] = "visited";
    rn[CurrentRole] = "current";
    rn[ColumnRole] = "column";
    rn[RowRole] = "row";
    return rn;
}

void CubeModel::reset(int size)
{
    beginResetModel();
    mData.clear();

    mSize = size;

    for(int j=0;j<mSize;++j) {
        for(int i=0;i<mSize;++i) {
            mData.push_back(DataObject(i, j));
        }
    }

    endResetModel();

    emit sizeChanged();
}

Qt::ItemFlags CubeModel::flags(const QModelIndex &idx) const
{
    return QAbstractItemModel::flags(idx);
}
