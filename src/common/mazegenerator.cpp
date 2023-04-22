#include "mazegenerator.h"

#include <QRandomGenerator>

MazeGenerator::MazeGenerator(QObject *parent)
    : QObject(parent)
    , mModel(nullptr)
    , mCurrent(-1)
    , mGenState(Idle)
{
    QRandomGenerator::securelySeeded();

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(step()));
    mTimer.setInterval(50);
}

InstanceTable *MazeGenerator::instanceTable()
{
    return dynamic_cast<InstanceTable*>(mModel);
}

CubeModel *MazeGenerator::cubeModel()
{
    return dynamic_cast<CubeModel*>(mModel);
}

int MazeGenerator::interval() const
{
    return mTimer.interval();
}

void MazeGenerator::init(int size)
{
    Q_ASSERT(size > 0);
    Q_ASSERT(mModel != nullptr);

    mStack.clear();
    mCurrent = -1;
    mModel->reset(size);

    setGenState(Ready);
}

void MazeGenerator::start()
{
    if(mGenState == Ready) {
        mCurrent = 0;
        mModel->setData(0, 0, true, IDataStructure::CurrentRole);
        mModel->setData(0, 0, true, IDataStructure::VisitedRole);
    }
    if(mGenState == Ready || mGenState == Stopped)
    {
        mTimer.start();
        setGenState(Solving);
    }
}

void MazeGenerator::stop()
{
    mTimer.stop();
    setGenState(Stopped);
}

void MazeGenerator::setModel(InstanceTable *model)
{
    if(mModel == model)
        return;
    mModel = model;
    emit modelChanged();

    if(mModel)
        init(mModel->cols());
}

void MazeGenerator::setModel(CubeModel *model)
{
    if(mModel == model)
        return;
    mModel = model;
    emit modelChanged();
    if(mModel)
        init(mModel->cols());
}

void MazeGenerator::setInterval(int i)
{
    if(mTimer.interval() == i)
        return;
    mTimer.setInterval(i);
    emit intervalChanged();
}

void MazeGenerator::step()
{
    if(!mModel)
        return;

    Pos currentPos = pos(mCurrent);

    // STEP 1 chose a random neighbour

    Pos nextPos = getNext(mCurrent);
    if(nextPos.valid()) {
        // STEP 2 push current into stack
        mStack.push(mCurrent);

        // STEP 3 remove the walls between those two
        int cDiff = nextPos.c - currentPos.c;
        if(cDiff > 0) {
            mModel->setData(currentPos.c, currentPos.r, false, IDataStructure::RightWallRole);
        } else if(cDiff < 0) {
            mModel->setData(nextPos.c, nextPos.r, false, IDataStructure::RightWallRole);
        }
        int rDiff = nextPos.r - currentPos.r;
        if(rDiff > 0) {
            mModel->setData(currentPos.c, currentPos.r, false, IDataStructure::BottomWallRole);
        } else if(rDiff < 0) {
            mModel->setData(nextPos.c, nextPos.r, false, IDataStructure::BottomWallRole);
        }

        // STEP 4 make the choosen neighbour the current and mark as visited
        mModel->setData(currentPos.c, currentPos.r, false, IDataStructure::CurrentRole);
        mModel->setData(nextPos.c, nextPos.r, true, IDataStructure::CurrentRole);
        mCurrent = index(nextPos.c, nextPos.r);

        // mark next as visited
        mModel->setData(nextPos.c, nextPos.r, true, IDataStructure::VisitedRole);
    }
    else if(!mStack.isEmpty()) {
        // get the last inserted  as current
        mCurrent = mStack.pop();
        Pos np = pos(mCurrent);
        // unmark the last current
        mModel->setData(currentPos.c, currentPos.r, false, IDataStructure::CurrentRole);
        // mark the new as current
        mModel->setData(np.c, np.r, true, IDataStructure::CurrentRole);
    }
    else {
        mModel->setData(currentPos.c, currentPos.r, false, IDataStructure::CurrentRole);
        mCurrent = -1;
        mTimer.stop();

        setGenState(Solved);
    }
}

void MazeGenerator::setGenState(GenState st)
{
    if(mGenState == st)
        return;
    mGenState = st;
    emit genStateChanged();
}

MazeGenerator::Pos MazeGenerator::pos(int index) const {
    return Pos(index%mModel->cols(),index/mModel->cols());
}

int MazeGenerator::index(int c, int r) const
{
    return c + r * mModel->cols();
}

MazeGenerator::Pos MazeGenerator::getNext(int idx) const
{
    auto p = pos(idx);

    int rc = mModel->rows();
    QList<MazeGenerator::Pos> n;
    // check top
    if(p.r > 0 && mModel->data(p.c, p.r-1, IDataStructure::VisitedRole).toBool() == false)
        n << Pos(p.c,p.r-1);
    // check bottom
    if(p.r < rc - 1 && mModel->data(p.c, p.r+1, IDataStructure::VisitedRole).toBool() == false)
        n << Pos(p.c,p.r+1);
    // check left
    if(p.c > 0 && mModel->data(p.c-1, p.r, IDataStructure::VisitedRole).toBool() == false)
        n << Pos(p.c-1,p.r);
    // check right
    if(p.c < mModel->cols() - 1 && mModel->data(p.c+1, p.r, IDataStructure::VisitedRole).toBool() == false)
        n << Pos(p.c+1,p.r);

    if(n.isEmpty())
        return Pos();

    int rnd = QRandomGenerator::global()->bounded(0,static_cast<int>(n.size()));
    return n[rnd];


}
