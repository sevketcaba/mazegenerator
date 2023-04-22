#ifndef CUBEMODEL_H
#define CUBEMODEL_H

#include <QAbstractItemModel>
#include <QQuick3DInstancing>

#include "../common/idatastructure.h"

class CubeModel : public QAbstractItemModel, public IDataStructure
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int cols READ cols NOTIFY sizeChanged)
    Q_PROPERTY(int rows READ rows NOTIFY sizeChanged)
    Q_PROPERTY(int count READ count NOTIFY sizeChanged)
public:
    explicit CubeModel(QObject *parent = nullptr);
    virtual ~CubeModel() override;

    enum ExtraRoles {
        ColumnRole = IDataStructure::CurrentRole + 1,
        RowRole
    };


    Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QModelIndex parent(const QModelIndex &child) const override;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &idx) const override;

    Q_INVOKABLE QVariant data(const QModelIndex &idx, int role) const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE QHash<int,QByteArray> roleNames() const override;

    // custom
    Q_INVOKABLE QVariant data(int c, int r, int role) const override {
        QModelIndex idx = index(c + r * mSize, 0, QModelIndex());
        return data(idx, role);
    }
    Q_INVOKABLE bool setData(int c, int r, const QVariant &v, int role) override {
        QModelIndex idx = index(c + r * mSize, 0, QModelIndex());
        return setData(idx, v, role);
    }

    void reset(int size) override;
signals:
    void sizeChanged();
};

#endif // CUBEMODEL_H

