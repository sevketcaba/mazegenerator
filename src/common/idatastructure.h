#ifndef IDATASTRUCTURE_H
#define IDATASTRUCTURE_H

#include <QObject>
#include <QList>
#include <QVariant>


class IDataStructure //: public QObject
{
//    Q_OBJECT
//    Q_PROPERTY(int cols READ cols NOTIFY sizeChanged)
//    Q_PROPERTY(int rows READ rows NOTIFY sizeChanged)
//    Q_PROPERTY(int count READ count NOTIFY sizeChanged)
public:
    enum Roles
    {
        RightWallRole = Qt::UserRole + 1,
        BottomWallRole,
        VisitedRole,
        CurrentRole
    };

    IDataStructure() :
        mSize(0)
    {

    }
    virtual ~IDataStructure() {}

    int cols() const { return mSize; }
    int rows() const { return mSize; }
    int count() const { return static_cast<int>(mData.size()); }

    virtual QVariant data(int c, int r, int role) const  = 0;
    virtual bool setData(int c, int r, const QVariant &value, int role)  = 0;
    virtual void reset(int size) = 0;

protected:
    struct DataObject
    {
    public:
        int column;
        int row;
        bool rightWall;
        bool bottomWall;
        bool visited;
        bool current;

        DataObject(int c, int r) :
            column(c)
          , row(r)
          , rightWall(true)
          , bottomWall(true)
          , visited(false)
          , current(false)
        {

        }
    };

    QList<DataObject> mData;
    int mSize;
};


#endif // IDATASTRUCTURE_H
