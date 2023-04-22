#include "instancetable.h"

#include <QColor>
#include <QDebug>
#include <QIcon>

InstanceTable::InstanceTable(QQuick3DObject *parent) :
    QQuick3DInstancing(parent)
  , IDataStructure()
  , mDirty(true)
  , mBoxSize(100)
{
    reset(10);
}

InstanceTable::~InstanceTable()
{

}

QVariant InstanceTable::data(int c, int r, int role) const
{
    Q_UNUSED(c)
    auto index = c + r * mSize;

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

    return QVariant();
}

double InstanceTable::boxSize() const
{
    return mBoxSize;
}

bool InstanceTable::setData(int c, int r, const QVariant &value, int role)
{
    int index = c + r * mSize;

    if(index < 0 || index >= mData.size())
        return false;

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
        return false;

    mDirty = true;
    markDirty();

    return true;
}

void InstanceTable::reset(int size)
{
    Q_ASSERT(size > 0);
    mData.clear();

    mSize = size;

    for(int j=0;j<mSize;++j) {
        for(int i=0;i<mSize;++i) {
            mData.push_back(DataObject(i, j));
        }
    }

    emit sizeChanged();

    mDirty = true;
    markDirty();
}

void InstanceTable::setBoxSize(double s)
{
    if(qFuzzyCompare(s, mBoxSize))
        return;
    mBoxSize = s;
    emit boxSizeChanged();

    mDirty = true;
    markDirty();
}

QByteArray InstanceTable::getInstanceBuffer(int *instanceCount)
{
    if(mDirty)
    {
        mRawData.resize(0);
        int index = 0;
        for(int j=0;j<mSize;++j) {

            for(int i=0;i<mSize;++i) {
                const auto &d = mData[index];
                QColor color = QColor::fromRgba(qRgba(255,255,255,255));

                QVector4D customData(0,0,0,0);
                // we have for float fields
                // but need to pass 6 booleans
                // current
                // visited
                // top
                // right
                // bottom
                // left

                // x will contain if it's current and/or visited
                // if visitedCurrentData < 0.1 -> neither VISITED nor CURRENT
                // else if visitedCurrentData > 0.2 -> both VISITED and CURRENT
                // else if visitedCurrentData > 0.1 -> CURRENT
                // else if visitedCurrentData > 0 -> VISITED
                float visitedCurrentData = 0.0;
                if(d.visited)
                    visitedCurrentData += 0.11f;
                if(d.current)
                    visitedCurrentData += 0.21f;
                customData.setX(visitedCurrentData);

                // y will contain if it's top most left or top

                // if leftTopData < 0.1 -> neither LEFT nor TOP
                // else if leftTopData > 0.3 -> both LEFT and TOP
                // else if leftTopData > 0.1 -> TOP
                // else if leftTopData > 0 -> LEFT
                float leftTopData = 0.f;
                if(i == 0) // left
                    leftTopData += 0.11f;
                if(j == 0) // top
                    leftTopData += 0.21f;
                customData.setY(leftTopData);

                // Z will contain if right wall is visible
                customData.setZ(d.rightWall ? 1.0f : 0);
                // W will contain if bottom wall is visible
                customData.setW(d.bottomWall ? 1.0f : 0);

                float x = ((i - (mSize/2.0)) * mBoxSize) + (mBoxSize / 2.0);
                float y = ((j - (mSize/2.0)) * mBoxSize) + (mBoxSize / 2.0);
                float z = 0;

                QVector3D pos(x,y,z);

                auto entry = calculateTableEntry(pos,
                                                 {1,1,.5},
                                                 {},
                                                 color,
                                                 customData
                                                 );
                mRawData.append(reinterpret_cast<const char *>(&entry), sizeof(entry));
                ++index;
            }
        }
    }

    if(instanceCount)
        *instanceCount = static_cast<int>(mData.size());
    return mRawData;
}
