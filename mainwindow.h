#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include "graph.h"
#include "pagerank.h"
#include "planarity.h"
#include "nodectrl.h"
#include "propertyeditor.h"
#include "relarankgraph/scene.h"
#include "relarankgraph/view.h"
class MainCtrl;
class PropertyEditor;
class QSplitter;

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:		// methods

    MainWindow(QWidget * parent = 0);

protected:			// methods

    void closeEvent(QCloseEvent * event);

private slots:
    void displayAbout();

public slots:
    void openfile();

private:			// methods

    void readSettings();

    void writeSettings();

    void printsta(MainCtrl * mainCtrl, Planarity * planar);
    void clearsta();
    std::vector < graph_node > node;
    std::vector < graph_edge > edge;
    std::vector < double > pagerank;
    std::vector < std::pair<int, int> > location;
    std::vector < NodeCtrl * >nodectrl;
    relarank::Scene * relarankScene;
    relarank::View * relarankView;
    PropertyEditor *propertyEditor;

private:			// members

    MainCtrl * m_mainCtrl;

    QSplitter *m_mainSplitter;

};

#endif // MAINWINDOW_H
