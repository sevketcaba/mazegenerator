#ifndef INSTANCETABLE_H
#define INSTANCETABLE_H

#include "../common/idatastructure.h"

#include <QQuick3DInstancing>

class InstanceTable : public QQuick3DInstancing, public IDataStructure
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int cols READ cols NOTIFY sizeChanged)
    Q_PROPERTY(int rows READ rows NOTIFY sizeChanged)
    Q_PROPERTY(int count READ count NOTIFY sizeChanged)

    Q_PROPERTY(double boxSize READ boxSize WRITE setBoxSize NOTIFY boxSizeChanged)
public:
    explicit InstanceTable(QQuick3DObject *parent = nullptr);
    ~InstanceTable() override;

    QVariant data(int c, int r, int role) const override;

    double boxSize() const;
public slots:
    bool setData(int c, int r, const QVariant &value, int role) override;
    void reset(int size) override;

    void setBoxSize(double s);
signals:
    void sizeChanged();
    void boxSizeChanged();
protected:
    QByteArray getInstanceBuffer(int *instanceCount) override;

    QByteArray mRawData;
    bool mDirty;
    double mBoxSize;
};

#endif // INSTANCETABLE_H

