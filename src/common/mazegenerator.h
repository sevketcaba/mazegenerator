#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <QObject>
#include <QStack>
#include <QTimer>
#include <QtQmlIntegration>

#include "../instancerendering/instancetable.h"
#include "../modelview/cubemodel.h"

class MazeGenerator : public QObject
{
public:
    enum GenState {
        Idle,
        Ready,
        Solving,
        Stopped,
        Solved
    };
private:
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(InstanceTable *instanceTable READ instanceTable WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(CubeModel *cubeModel READ cubeModel WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(GenState genState READ genState NOTIFY genStateChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)

    Q_ENUM(GenState)
public:
    explicit MazeGenerator(QObject *parent = nullptr);

    InstanceTable *instanceTable();
    CubeModel *cubeModel();

    GenState genState() const { return mGenState; }

    int interval() const;

signals:
    void modelChanged();
    void genStateChanged();
    void intervalChanged();

    void done();
public slots:
    void init(int size);
    void start();
    void stop();
    void setModel(InstanceTable *model);
    void setModel(CubeModel *model);
    void setInterval(int i);
protected slots:
    void step();
    void setGenState(GenState st);
protected:
    IDataStructure *mModel;
    int mCurrent;

    struct Pos {
        int c;
        int r;
        Pos(int cx = -1, int rx = -1) :
            c(cx)
          , r(rx)
        {

        }

        bool valid() const { return c >= 0 && r >= 0; }
    };

    Pos pos(int index) const;
    int index(int c, int r) const;
    Pos getNext(int idx) const;

    QStack<int> mStack;
    QTimer mTimer;

    GenState mGenState;
};

#endif // MAZEGENERATOR_H
