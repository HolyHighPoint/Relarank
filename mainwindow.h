#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include "graph.h"
#include "nodectrl.h"
#include "propertyeditor.h"
#include "relarankgraph/scene.h"
#include "relarankgraph/view.h"
class MainCtrl;
class PropertyEditor;
class QSplitter;

/*class RenderThread : public QThread
{
    Q_OBJECT

signals:
    void notify(int);

public:
    RenderThread(std::vector<graph_node> &_node,
                 std::vector<graph_edge> &_edge, std::vector<NodeCtrl*> &_nodectrl,
                 MainCtrl* &_mainCtrl, QObject *parent = 0)
        : node(_node), edge(_edge), nodectrl(_nodectrl), mainCtrl(_mainCtrl), QThread(parent)
    {
    }

protected:
    void run();
private:
    std::vector<graph_node> &node;
    std::vector<graph_edge> &edge;
    std::vector<NodeCtrl*> &nodectrl;
    MainCtrl* &mainCtrl;
};*/

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

    void printsta(MainCtrl * mainCtrl);
    void clearsta();
    std::vector < graph_node > node;
    std::vector < graph_edge > edge;
    std::vector < NodeCtrl * >nodectrl;
    relarank::Scene * relarankScene;
    relarank::View * relarankView;
    PropertyEditor *propertyEditor;

private:			// members

    MainCtrl * m_mainCtrl;

    QSplitter *m_mainSplitter;

};

#endif // MAINWINDOW_H
