QT += quick quick3d

CONFIG += qmltypes
QML_IMPORT_NAME = tr.gen.caba.qmltypes
QML_IMPORT_MAJOR_VERSION = 1

INCLUDEPATH += common
INCLUDEPATH += modelview
INCLUDEPATH += instancerendering

HEADERS += \
common/idatastructure.h \
common/mazegenerator.h \
modelview/cubemodel.h \
instancerendering/instancetable.h

SOURCES += \
instancerendering/instancetable.cpp \
modelview/cubemodel.cpp \
common/mazegenerator.cpp \
main.cpp

resources.files = main.qml \
common/ControlArea.qml \
modelview/DelegateBlock.qml \
modelview/cubeMaterial.frag \
modelview/cubeMaterial.vert \
instancerendering/InstanceBlock.qml \
instancerendering/cubeMaterial.frag \
instancerendering/cubeMaterial.vert

resources.prefix = /$${TARGET}
RESOURCES += resources

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



