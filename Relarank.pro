QT          += core gui widgets
CONFIG      += c++11 static
DEFINES     *= QT_USE_QSTRINGBUILDER

TARGET = Relarank
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    collapsible.cpp \
    mainctrl.cpp \
    nodectrl.cpp \
    nodeproperties.cpp \
    propertyeditor.cpp \
    relarankgraph/baseedge.cpp \
    relarankgraph/bezieredge.cpp \
    relarankgraph/drawedge.cpp \
    relarankgraph/edgearrow.cpp \
    relarankgraph/edgegroup.cpp \
    relarankgraph/edgegroupinterface.cpp \
    relarankgraph/edgegrouppair.cpp \
    relarankgraph/edgelabel.cpp \
    relarankgraph/labeltextfactory.cpp \
    relarankgraph/node.cpp \
    relarankgraph/nodehandle.cpp \
    relarankgraph/nodelabel.cpp \
    relarankgraph/perimeter.cpp \
    relarankgraph/plug.cpp \
    relarankgraph/plugarranger.cpp \
    relarankgraph/plugedge.cpp \
    relarankgraph/plughandle.cpp \
    relarankgraph/pluglabel.cpp \
    relarankgraph/scene.cpp \
    relarankgraph/scenehandle.cpp \
    relarankgraph/straightdoubleedge.cpp \
    relarankgraph/straightedge.cpp \
    relarankgraph/view.cpp \
    pagerank.cpp \
    planarity.cpp \
    ranklist.cpp

HEADERS  += mainwindow.h \
    collapsible.h \
    mainctrl.h \
    nodectrl.h \
    nodeproperties.h \
    propertyeditor.h \
    relarankgraph/baseedge.h \
    relarankgraph/bezieredge.h \
    relarankgraph/drawedge.h \
    relarankgraph/edgearrow.h \
    relarankgraph/edgegroup.h \
    relarankgraph/edgegroupinterface.h \
    relarankgraph/edgegrouppair.h \
    relarankgraph/edgelabel.h \
    relarankgraph/labeltextfactory.h \
    relarankgraph/node.h \
    relarankgraph/nodehandle.h \
    relarankgraph/nodelabel.h \
    relarankgraph/perimeter.h \
    relarankgraph/plug.h \
    relarankgraph/plugarranger.h \
    relarankgraph/plugedge.h \
    relarankgraph/plughandle.h \
    relarankgraph/pluglabel.h \
    relarankgraph/scene.h \
    relarankgraph/scenehandle.h \
    relarankgraph/straightdoubleedge.h \
    relarankgraph/straightedge.h \
    relarankgraph/utils.h \
    relarankgraph/view.h \
    graph.h \
    pagerank.h \
    planarity.h \
    ranklist.h

RESOURCES += \
    res/icons.qrc

RC_FILE = res/Qt_ico.rc
